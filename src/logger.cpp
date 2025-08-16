#include "logger.h"
#include <stdexcept>
#include <chrono>
#include <iomanip>


Logger::Logger(const std::string& filename, LogLevel defaultLogLevel): currentLevel_(defaultLogLevel) {
    logFile_.open(filename, std::ios::app);
    if (!logFile_.is_open()) throw std::runtime_error("Error opening the logging file: " + filename);
}

Logger::~Logger() {
    // по сути поток это RAII-Обьект и сам будет корректно закрываться но для полноты можно проверить 
    if (logFile_.is_open()) logFile_.close();
}

bool Logger::log(std::string_view message, LogLevel level) {
    if (static_cast<int>(level) < static_cast<int>(currentLevel_.load(std::memory_order_acquire))) return false;
    std::lock_guard<std::mutex> lock {logMutex_};
    if (static_cast<int>(level) < static_cast<int>(currentLevel_.load(std::memory_order_relaxed))) return false;

    if (!logFile_.is_open()) return false;

    logFile_ << "[" << getCurrentTime() << "]\t" 
             << "[" << logLevelToStr(level) << "]\t";
    logFile_.write(message.data(), message.size());
    logFile_ << std::endl;

    return true;
}

void Logger::setLogLevel(LogLevel new_level){
    currentLevel_.store(new_level, std::memory_order_release);
}

LogLevel Logger::getLogLevel()  const{
    return currentLevel_.load(std::memory_order_acquire);
}

std::string Logger::logLevelToStr(LogLevel level) const {
    switch (level)
    {
    case LogLevel::Info: return "INFO";
        break;
    case LogLevel::Warning: return "WARNING";
        break;
    case LogLevel::Error: return "ERROR";
        break;
    default: return "UNKNOWN";
    }
}

LogLevel toLogLevel(int value) {
    switch (value) {
        case 0: return LogLevel::Info;
            break;
        case 1: return LogLevel::Warning;
            break;
        case 2: return LogLevel::Error;
            break;
        default: throw std::invalid_argument("Invalid argument. Use 0-2");
    }
}

std::string Logger::getCurrentTime() const {
    auto in_time { std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
    
    std::tm buf;
    #ifdef _WIN32
        localtime_s(&buf, &in_time);
    #else
        localtime_r(&in_time, &buf);
    #endif

    std::ostringstream ss;
    ss << std::put_time(&buf, "%Y-%M-%D %H:%M:%S");

    return ss.str();
}


