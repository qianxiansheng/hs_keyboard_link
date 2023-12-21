#include "Logger.h"
#include <filesystem>
#include "util/utils.h"

// ���캯��
Logger::Logger(const std::string& logFileName) : logFileName(logFileName) {
    logFile.open(logFileName, std::ios::app);  // ��׷�ӷ�ʽ���ļ�
    if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open log file " << logFileName << std::endl;
    }
}

// ��������
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();  // �ر��ļ�
    }
}
// ��ʽ���ַ�������¼��־
void Logger::_log(LogLevel level, const char* file, int line, char* message) {
    if (logFile.is_open()) {
        // ��ȡ��ǰʱ��
        std::time_t now = std::time(nullptr);
        char timestamp[20];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // ���ʱ�䡢�ļ������кš���־�������־��Ϣ
        logFile << "[" << timestamp << "] ";
        logFile << "[" << getLogLevelString(level) << "] ";
        logFile << "[" << file << ":" << line << "] ";
        logFile << message << std::endl;
        logFile.flush();
    }
}

// ��ȡ��־������ַ�����ʾ
std::string Logger::getLogLevelString(LogLevel level) {
    switch (level) {
    case LOG_LEVEL_INFO: return "INFO";
    case LOG_LEVEL_DEBUG: return "DEBUG";
    case LOG_LEVEL_ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}

// ����ȫ����־����
Logger gLogger(utils::getFileAbsolutePath(LOG_FILE_NAME).c_str());

void LOG0(LogLevel level, const char* file, int line, const char* format, ...)
{
    // ʹ�ÿɱ�����б���и�ʽ���ַ���
    va_list args;
    va_start(args, format);
    char buffer[256];  // ����ÿ����־������256���ַ�
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::filesystem::path p(file);
    gLogger._log(level, p.filename().string().c_str(), line, buffer);
}
