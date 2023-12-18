#ifndef __KL_LOGGER_H__
#define __KL_LOGGER_H__

#include "pch.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdarg>

// 定义日志级别
enum LogLevel {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_ERROR
};

// 定义日志类
class Logger {
public:
    // 构造函数
    Logger(const std::string& logFileName);

    // 析构函数
    ~Logger();

    // 格式化字符串并记录日志
    void log(LogLevel level, const char* file, int line, char* message);

private:
    // 获取日志级别的字符串表示
    std::string getLogLevelString(LogLevel level);

    std::ofstream logFile;  // 日志文件流
    std::string logFileName;  // 日志文件名
};


void LOG0(LogLevel level, const char* file, int line, const char* format, ...);

#define LOG_INFO(format, ...)  LOG0(LOG_LEVEL_INFO , __FILE__, __LINE__, format,  __VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG0(LOG_LEVEL_DEBUG, __FILE__, __LINE__, format,  __VA_ARGS__)
#define LOG_ERROR(format, ...) LOG0(LOG_LEVEL_ERROR, __FILE__, __LINE__, format,  __VA_ARGS__)

#endif  // __KL_LOGGER_H__