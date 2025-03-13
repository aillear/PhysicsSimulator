#pragma once
#include <fstream>
#include <mutex>
#include <string>
class Logger {
  public:
    enum LogLevel {
        DEBUG = 0,
        INFO,
        WARNING,
        ERROR
    };

    static Logger& Instance();

    // LogLevel : set original log level;
    // logName  : set name of log file;
    // instantFlush : if it's true, the log will be instantly writen to log file;
    void Init(LogLevel level = LogLevel::INFO,
              const std::string& logName = "app.log", bool instantFlush = false);

    void SetLogLevel(LogLevel level);

    // LogLevel: when the this log's level is less than current log level, the log will be ignored.
    void Log(LogLevel level, const std::string& msg);
    
    void Debug(const std::string& msg);
    void Info(const std::string& msg);
    void Warning(const std::string& msg);
    void Error(const std::string& msg);

  private:
    Logger() = default;
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string LevelToStr(LogLevel level);
    std::ofstream logFile_;
    LogLevel currentLevel_;
    std::mutex mutex_;
    bool instantFlush_;
};

//////////////////////////////////////////////////////////////////
// 日志宏定义（供外部调用）
//////////////////////////////////////////////////////////////////

#define LOG_DEBUG(msg) \
    Logger::Instance().Debug(msg)
#define LOG_INFO(msg)  \
    Logger::Instance().Info(msg)
#define LOG_WARN(msg)  \
    Logger::Instance().Warning(msg)
#define LOG_ERROR(msg) \
    Logger::Instance().Error(msg)
