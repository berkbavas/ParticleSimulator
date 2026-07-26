#pragma once

#include <assert.h>
#include <atomic>
#include <chrono>
#include <format>
#include <iostream>
#include <string>

#include <QDebug>

namespace ParticleSimulator
{
    enum class LogLevel
    {
        ALL = -1,
        TRACE = 0,
        DEBUG = 1,
        INFO = 2,
        WARNING = 3,
        FATAL = 4,
        NONE = 5
    };

    class Logger
    {
      public:
        Logger() = delete;

        static void Log(LogLevel LogLevel, const std::string& Message);
        static void SetLogLevel(LogLevel LogLevel);
        static bool IsLogEnabledFor(LogLevel LogLevel);
        static LogLevel GetLogLevel();
        static std::string GetTimeString();
        static void QtMessageOutputCallback(QtMsgType Type, const QMessageLogContext& Context, const QString& Message);
        static std::string GetLogLevelString(LogLevel Level);

      private:
        static LogLevel mLogLevel;
        static std::atomic_uint32_t mLastThreadId;
    };
}

#define LOG_PRIVATE(LEVEL, FORMAT, ...) \
    do \
    { \
        if (ParticleSimulator::Logger::IsLogEnabledFor(LEVEL)) \
        { \
            ParticleSimulator::Logger::Log(LEVEL, std::format(FORMAT, __VA_ARGS__)); \
        } \
    } while (false)

#define LOG_TRACE(FORMAT, ...) LOG_PRIVATE(ParticleSimulator::LogLevel::TRACE, FORMAT, __VA_ARGS__)
#define LOG_DEBUG(FORMAT, ...) LOG_PRIVATE(ParticleSimulator::LogLevel::DEBUG, FORMAT, __VA_ARGS__)
#define LOG_INFO(FORMAT, ...) LOG_PRIVATE(ParticleSimulator::LogLevel::INFO, FORMAT, __VA_ARGS__)
#define LOG_WARN(FORMAT, ...) LOG_PRIVATE(ParticleSimulator::LogLevel::WARNING, FORMAT, __VA_ARGS__)
#define LOG_FATAL(FORMAT, ...) LOG_PRIVATE(ParticleSimulator::LogLevel::FATAL, FORMAT, __VA_ARGS__)

#define PS_ASSERT(EXPRESSION) assert(EXPRESSION)

#define PS_EXIT_FAILURE(FORMAT, ...) \
    do \
    { \
        LOG_FATAL(FORMAT, __VA_ARGS__); \
        std::exit(EXIT_FAILURE); \
\
    } while (false)