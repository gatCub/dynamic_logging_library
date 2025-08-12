#include "logger.h"

Logger::Logger(const std::string& filename, LogLevel defaultLogLevel): currentLevel_(defaultLogLevel) {
    logFile_.open(filename, std::ios::app);
    if (!logFile_.is_open()) throw std::runtime_error("Ошибка открытия файла логирования: " + filename);
}

Logger::~Logger() {
    // по сути поток это RAII-Обьект и сам будет корректно закрываться но для полноты можно проверить 
    if (logFile_.is_open()) logFile_.close();
}

bool Logger::log(std::string_view message, LogLevel level) {
    if (static_cast<int>(level) < static_cast<int>(currentLevel_)) return false;

    if (!logFile_.is_open()) return false;

    logFile_ << "[" << "ВРЕМЯ" << "]\t" 
             << "[" << logLevelToStr(level) << "]\t";
    logFile_.write(message.data(), message.size());
    logFile_ << std::endl;
    
}

void Logger::setLogLevel(LogLevel new_level){
    currentLevel_ = new_level;
}

LogLevel Logger::getLogLevel()  const{
    return currentLevel_;
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

