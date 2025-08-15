#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include <mutex>
#include <atomic>

/*Уровни логирования: 
    Info - Информационное сообщение.
    Warning - Предупреждающие соотбщение (не критическое).
    Error - критическая ошибка
*/
enum class LogLevel {
    Info = 0,
    Warning = 1,
    Error = 2

};

// Из условий задачи мы ограничиваемся 3 уровнями поэтому я выбрал перечисления и не использовал полиморфизм во время выполнения для получения уровней логирования.  

struct Logger
{
    Logger(const std::string&, LogLevel);
    ~Logger();

    bool log(std::string_view, LogLevel = LogLevel::Info);
    void setLogLevel(LogLevel);
    LogLevel getLogLevel() const;


private:

    std::string logLevelToStr(LogLevel) const;
    std::string getCurrentTime() const;

    std::ofstream logFile_;
    std::atomic<LogLevel> currentLevel_;
    mutable std::mutex logMutex_;

};

#endif