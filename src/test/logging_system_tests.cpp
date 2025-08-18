
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cassert>
#include <vector>
#include <thread>

#include "../logger.h"

using namespace std;

static const string filename {"test_creation.log"};

string readFile(const string& filename) {
    ifstream file {filename};
    stringstream buf;
    buf << file.rdbuf();
    
    return buf.str();
}

/*Тест: Создание логгера и запись сообщения в журнал*/
void test_logger_creation() {
    const string message {"Test creation message"};
    filesystem::remove(filename.c_str());
    {
        Logger logger {filename, LogLevel::Info};
        logger.log(message);
    }

    string content {readFile(filename)};
    assert(content.find(message) != string::npos);

    filesystem::remove(filename);
}

/*Тест: Проверка корректности записи по уровнб логирования*/
void test_level_filter() {
    filesystem::remove(filename);

    {
        Logger logger {filename, LogLevel::Warning};
        logger.log("Info message", LogLevel::Info);
        logger.log("Warning message");
        logger.log("Error message", LogLevel::Error);
    }

    string content {readFile(filename)};
    assert(content.find("Info message") == string::npos);
    assert(content.find("Warning message") == string::npos);
    assert(content.find("Error message") != string::npos);

    filesystem::remove(filename);
}

/*Тест: Проверка корректности изменение уровня логирования*/
void test_level_set() {
    filesystem::remove(filename);

    {
        Logger logger{filename, LogLevel::Warning};
        logger.log("The first Info message", LogLevel::Info);

        logger.setLogLevel(LogLevel::Info);
        logger.log("The second Info message", LogLevel::Info);
    }

    string content {readFile(filename)};
    assert(content.find("The first Info message") == string::npos);
    assert(content.find("The second Info message") != string::npos);

    filesystem::remove(filename);
}

/*Тест: Проверка корректности получения уровня логирования*/
void test_level_get() {
    filesystem::remove(filename);
    {
        LogLevel level {LogLevel::Info};
        Logger logger {filename, level};

        assert(logger.getLogLevel() == level);
    }

    filesystem::remove(filename);
}

/*Тест: проверка формата записи логов*/
void test_log_format() {
    filesystem::remove(filename);

    {
        Logger logger {filename, LogLevel::Info};
        logger.log("Test format recordin");
    }

    string content {readFile(filename)};

    assert(content.find("[") == 0);
    assert(content.find("]\t[INFO]\tTest format recordin") != string::npos);
    filesystem::remove(filename);
}

/*Тест: Проверка потокобезопасной работы*/
void test_safe_multitherad() {
    filesystem::remove(filename);
    const int threads_nums {10}, messages_thread {100};

    {
        Logger logger {filename, LogLevel::Info};
        vector<thread> threads;

        for (int i {0}; i < threads_nums; ++i) {
            threads.emplace_back([&logger, i](){
                for (int j {0}; j < messages_thread; ++j) {
                    logger.log("Thread " + to_string(i) + " message " + to_string(j));
                }
            });
        }

        for (auto& t: threads) {
            t.join();
        }
    }

    string content {readFile(filename)};
    size_t count {0};
    for (int pos {0}; (pos = content.find("\n", pos)) != string::npos; ++pos) { ++count; }
    assert(count == threads_nums * messages_thread);

    filesystem::remove(filename);
}


int main () {
    cout << "Starting auto logger tests...\n";

    test_logger_creation();
    cout << "[TEST]\tThe \"test_logger_creation\" test was passed successfully\n";

    test_level_filter();
    cout << "[TEST]\tThe \"test_level_filter\" test was passed successfully\n";

    test_level_set();
    cout << "[TEST]\tThe \"test_level_set\" test was passed successfully\n";

    test_level_get();
    cout << "[TEST]\tThe \"test_level_get\" test was passed successfully\n";

    test_log_format();
    cout << "[TEST]\tThe \"test_log_format\" test was passed successfully\n";

    test_safe_multitherad();
    cout << "[TEST]\tThe \"test_safe_multitherad\" test was passed successfully\n";

    cout << "All tests passed!" << std::endl;
    return 0;
}