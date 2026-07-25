
#include "Logger.h"

void ParticleSimulator::Logger::Log(LogLevel LogLevel, const std::string& Message)
{
    thread_local const auto TL_THREAD_ID = mLastThreadId.fetch_add(1);

    const auto Log = std::format("{:.12} [{:}] [ {:5} ] {:}", GetTimeString(), TL_THREAD_ID, GetLogLevelString(LogLevel), Message);

    switch (LogLevel)
    {
    case LogLevel::TRACE:
    case LogLevel::DEBUG:

    case LogLevel::INFO:
        std::cout << Log << std::endl;
        break;
    case LogLevel::WARNING:
    case LogLevel::FATAL:
        std::cerr << Log << std::endl;
        break;
    default:
        break;
    }
}

void ParticleSimulator::Logger::SetLogLevel(LogLevel LogLevel)
{
    mLogLevel = LogLevel;
}

ParticleSimulator::LogLevel ParticleSimulator::Logger::GetLogLevel()
{
    return mLogLevel;
}

std::string ParticleSimulator::Logger::GetTimeString()
{
    const auto Zone = std::chrono::current_zone();
    const auto Now = std::chrono::system_clock::now();
    const auto Local = std::chrono::zoned_time(Zone, Now);

    return std::format("{:%T}", Local);
}

void ParticleSimulator::Logger::QtMessageOutputCallback(QtMsgType Type, const QMessageLogContext& Context, const QString& Message)
{
    switch (Type)
    {
    case QtDebugMsg:
        LOG_DEBUG("{}", Message.toStdString());
        break;
    case QtInfoMsg:
        LOG_INFO("{}", Message.toStdString());
        break;
    case QtWarningMsg:
        LOG_WARN("{}", Message.toStdString());
        break;
    case QtCriticalMsg:
        LOG_FATAL("{}", Message.toStdString());
        break;
    case QtFatalMsg:
        LOG_FATAL("{}", Message.toStdString());
        break;
    }
}

bool ParticleSimulator::Logger::IsLogEnabledFor(LogLevel LogLevel)
{
    return mLogLevel <= LogLevel;
}

ParticleSimulator::LogLevel ParticleSimulator::Logger::mLogLevel = ParticleSimulator::LogLevel::ALL;

std::atomic_uint32_t ParticleSimulator::Logger::mLastThreadId = 0;