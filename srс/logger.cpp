#include "logger.h"

logger::logger(const std::string& filename, LogLevel defaultLogLevel): currentLevel_(defaultLogLevel) {
    logFile_.open(filename, std::ios::app);
    if (!logFile_.is_open()) throw std::runtime_error("Ошибка открытия файла логирования: " + filename);
}

logger::~logger() {
    // по сути поток это RAII-Обьект и сам будет корректно закрываться но для полноты можно проверить 
    if (logFile_.is_open()) logFile_.close();
}