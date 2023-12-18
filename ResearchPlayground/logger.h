#ifndef __KL_LOGGER_H__
#define __KL_LOGGER_H__

#include "pch.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cstdarg>

// ������־����
enum LogLevel {
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_ERROR
};

// ������־��
class Logger {
public:
    // ���캯��
    Logger(const std::string& logFileName);

    // ��������
    ~Logger();

    // ��ʽ���ַ�������¼��־
    void log(LogLevel level, const char* file, int line, char* message);

private:
    // ��ȡ��־������ַ�����ʾ
    std::string getLogLevelString(LogLevel level);

    std::ofstream logFile;  // ��־�ļ���
    std::string logFileName;  // ��־�ļ���
};


void LOG0(LogLevel level, const char* file, int line, const char* format, ...);

#define LOG_INFO(format, ...)  LOG0(LOG_LEVEL_INFO , __FILE__, __LINE__, format,  __VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG0(LOG_LEVEL_DEBUG, __FILE__, __LINE__, format,  __VA_ARGS__)
#define LOG_ERROR(format, ...) LOG0(LOG_LEVEL_ERROR, __FILE__, __LINE__, format,  __VA_ARGS__)

#endif  // __KL_LOGGER_H__