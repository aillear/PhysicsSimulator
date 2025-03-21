#include "logger.h"
#include <chrono>
#include <format>
#include <fstream>
#include <ostream>
#include <utility>

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::Init(LogLevel level, const std::string& logName, bool instantFlush) {
    mutex_.lock();
    logFile_.open(logName, std::ios::app);
    if (!logFile_.is_open()) {
        return;
    }
    currentLevel_ = level;
    instantFlush_ = instantFlush;
    mutex_.unlock();
    Info("program start");
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
    
    Info("exit.");
    logFile_ << std::endl;
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