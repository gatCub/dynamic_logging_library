#include <iostream>
#include <string>
#include <string_view>
#include <queue>
#include <optional>

#include <mutex>
#include <atomic>
#include <thread>
#include <condition_variable>

#include "logger.h"

void printHelp() {
    //не забыть реализовать
    std::cout << "Implement it later..." << std::endl;
}

struct LogTask {
    std::string message;
    LogLevel level;
};

template<typename T>
struct ThreadSafeQueue {
    void push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
        cond_var_.notify_one();
    }

    bool pop(T& value) {
        std::unique_lock<std::mutex> lock (mutex_);
        cond_var_.wait(lock, [this] {return !queue_.empty() || stop_; });

        if (stop_ && queue_.empty()) {
            return false;
        }

        value = queue_.front();
        queue_.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }

        cond_var_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::atomic<bool> stop_ { false };    
};


void workerThread(ThreadSafeQueue<LogTask>& queue, Logger& logger) {
    LogTask task;

    while ( queue.pop(task) ) {
        try {
            logger.log(task.message, task.level);
        }
        catch (const std::exception& e) {
            std::cerr << "Logging faild: " << e.what() << std::endl;
        }
    }
}



int main(int argc, char** argv) {

    if ( argc == 2 && argv[1] == "--help" ) {
        printHelp();
        return 0;
    }

    if( argc != 3 ) {
        std::cerr << "The program works as follows: " 
                  << argv[0] << " <logging_file> <defualt_logging_level>\n\n" 
                  << "for more information: --help" << std::endl;

        return 1;
   }
   
   std::string logfile { argv[1] };
   LogLevel defualtLevel;

   try {
        defualtLevel = toLogLevel(std::stoi(argv[2]));
   }
   catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() <<std::endl;
        return 1;
   }

  
   
   try {
        Logger logger { logfile, defualtLevel };
        std::cout << "Logger initialization. Logging file: " << logfile 
                  << ". Defualt logging level: " << static_cast<int>(defualtLevel) << std::endl;
        
        ThreadSafeQueue<LogTask> queue;
        std::thread worker(workerThread, std::ref(queue), std::ref(logger));

        while ( true ) {

            std::string input_str;
            std::cout << "Enter message (Enter 'exit' to exit):" << std::endl;
            std::getline(std::cin, input_str);
            
            if (input_str == "exit") break;

            std::string level_str;
            std::cout << "Enter level (defualt - " << static_cast<int>(defualtLevel) << "):" << std::endl;
            std::getline(std::cin, level_str);
            
            LogLevel level { defualtLevel };
            try {
                level = toLogLevel(std::stoi(level_str));
            }
            catch(...) {
                std::cerr << "Invalid level. Use defualt!" << std::endl;
            }  
            
            queue.push( {input_str, level} );
        }

        queue.stop();
        worker.join();
        std::cout << "Aplicated stopped" << std::endl;
                          
   } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() <<std::endl;
        return 1;
   }


   std::cin.get();

   return 0;
}