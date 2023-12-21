#include "Logger.h"
#include <filesystem>
#include "util/utils.h"

// 构造函数
Logger::Logger(const std::string& logFileName) : logFileName(logFileName) {
    logFile.open(logFileName, std::ios::app);  // 以追加方式打开文件
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open log file " << logFileName << std::endl;
    }
}

// 析构函数
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();  // 关闭文件
    }
}
// 格式化字符串并记录日志
void Logger::_log(LogLevel level, const char* file, int line, char* message) {
    if (logFile.is_open()) {
        // 获取当前时间
        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // 输出时间、文件名、行号、日志级别和日志信息
        logFile << "[" << timestamp << "] ";
        logFile << "[" << getLogLevelString(level) << "] ";
        logFile << "[" << file << ":" << line << "] ";
        logFile << message << std::endl;
        logFile.flush();
    }
}

// 获取日志级别的字符串表示
std::string Logger::getLogLevelString(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_INFO: return "INFO";
    case LOG_LEVEL_DEBUG: return "DEBUG";
    case LOG_LEVEL_ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

// 定义全局日志对象
Logger gLogger(utils::getFileAbsolutePath(LOG_FILE_NAME).c_str());

void LOG0(LogLevel level, const char* file, int line, const char* format, ...)
{
    // 使用可变参数列表进行格式化字符串
    va_list args;
    va_start(args, format);
    char buffer[256];  // 假设每条日志不超过256个字符
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::filesystem::path p(file);
    gLogger._log(level, p.filename().string().c_str(), line, buffer);
}
