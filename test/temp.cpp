#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <memory>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    // 获取单例实例（饿汉式）
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // 初始化日志系统
    void init(const std::string& filename = "app.log", 
             LogLevel level = LogLevel::INFO,
             bool consoleOutput = true) {
        std::lock_guard<std::mutex> lock(mutex_);
        logFile_.open(filename, std::ios::app);
        currentLevel_ = level;
        consoleOutput_ = consoleOutput;
    }

    // 设置日志级别
    void setLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        currentLevel_ = level;
    }

    // 核心日志方法
    void log(LogLevel level, const std::string& message) {
        if (level < currentLevel_) return;

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::lock_guard<std::mutex> lock(mutex_);

        // 格式化时间戳
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
           << "." << std::setw(3) << std::setfill('0') 
           << (std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()).count() % 1000);

        // 构建完整日志条目
        std::string entry = "[" + ss.str() + "] "
            + levelToString(level) + ": " + message;

        // 输出到控制台
        if (consoleOutput_) {
            std::cout << entry << std::endl;
        }

        // 输出到文件
        if (logFile_.is_open()) {
            logFile_ << entry << std::endl;
            logFile_.flush();  // 确保及时写入
        }
    }

    // 快捷方法
    void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
    void info(const std::string& msg)  { log(LogLevel::INFO, msg); }
    void warning(const std::string& msg) { log(LogLevel::WARNING, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

private:
    Logger() = default;  // 私有构造函数
    ~Logger() {
        if (logFile_.is_open()) {
            logFile_.close();
        }
    }

    // 禁用拷贝和赋值
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string levelToString(LogLevel level) {
        switch(level) {
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            default:                return "UNKNOWN";
        }
    }

    std::ofstream logFile_;
    LogLevel currentLevel_ = LogLevel::INFO;
    bool consoleOutput_ = true;
    std::mutex mutex_;
};

// 使用宏简化调用
#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg)  Logger::getInstance().info(msg)
#define LOG_WARN(msg)  Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)