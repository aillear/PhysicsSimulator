#pragma once
#include <__format/format_functions.h>
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
    
    template<typename... Args> void Log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
        Log(level, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Debug(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Info(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Warning(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::WARNING, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void Error(std::format_string<Args...> fmt, Args&&... args) {
        Log(LogLevel::ERROR, std::format(fmt, std::forward<Args>(args)...));
    }
    
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

#define F_LOG_DEBUG(...) \
    Logger::Instance().Debug(__VA_ARGS__)
#define F_LOG_INFO(...) \
    Logger::Instance().Info(__VA_ARGS__)
#define F_LOG_WARNING(...) \
    Logger::Instance().Warning(__VA_ARGS__)
#define F_LOG_ERROR(...) \
    Logger::Instance().Error(__VA_ARGS__)