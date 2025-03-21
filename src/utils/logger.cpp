#include "logger.h"
#include "pathMgr.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::Init(LogLevel level, const std::string& logName, bool instantFlush) {
    mutex_.lock();

    auto path = GET_PATH("logs", logName);
    auto logDir = path.parent_path();
    bool logDirCreated = false;

    // check is log dir exist, if not, create log dir;
    if (!fs::exists(logDir)) {
        logDirCreated = fs::create_directories(logDir);
        if (!logDirCreated) {
            std::cerr << "Fail to create log directory!" << logDir << std::endl;
            mutex_.unlock();
            return;
        }
    }

    // try to open log
    logFile_.open(path, std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Fail to open log file!" << std::endl;
        mutex_.unlock();
        return;
    }

    currentLevel_ = level;
    instantFlush_ = instantFlush;
    mutex_.unlock();
    if (logDirCreated) Warning("Log directory has been created automatically: path=`{}`", logDir.string());
    Info("Logger Initialized: level=`{}`, file=`{}`.", LevelToStr(level), path.string());
}

void Logger::SetLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    currentLevel_ = level;
}

void Logger::Log(LogLevel level, const std::string& msg) {
    if (level < currentLevel_) return;
    

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::lock_guard<std::mutex> lock(mutex_);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
           << "." << std::setw(3) << std::setfill('0') 
           << (std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count() % 1000);
    
    std::string entry = "[" + ss.str() + "] " + LevelToStr(level) + ": " + msg;

    if (logFile_.is_open()) {
        logFile_ << entry << "\n";
        if (instantFlush_) logFile_.flush();
    }
}

void Logger::Debug(const std::string& msg) {
    Log(LogLevel::DEBUG, msg);
}

void Logger::Info(const std::string& msg) {
    Log(LogLevel::INFO, msg);
}

void Logger::Warning(const std::string& msg) {
    Log(LogLevel::WARNING, msg);
}

void Logger::Error(const std::string& msg) {
    Log(LogLevel::ERROR, msg);
}

Logger::~Logger() {
    if (!logFile_.is_open()) return;
    
    Info("Program exited.");
    logFile_ << '\n' << std::endl;
    logFile_.close();
}

std::string Logger::LevelToStr(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default:    return "UNKNOWED";
    }
}