#ifndef LOGGER_H
#define LOGGER_H

#pragma once

#include <string>
#include <fstream>

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

struct logger
{
    logger(const std::string&, LogLevel);
    ~logger();

    void log(const std::string&, LogLevel = LogLevel::Info);
    void setLogLevel(LogLevel);


private:
    std::ofstream logFile_;
    LogLevel currentLevel_;

};

#endif