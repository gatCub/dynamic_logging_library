#include <iostream>
#include <string>

#include "logger.h"

#ifdef _WIN32
    #include <windows.h>
#endif

void printHelp() {
    std::cout << "Implement it later..." << std::endl;
}


int main(int argc, char** argv) {

    if ( argc == 2 && argv[1] == "--help" ) {
        printHelp();
        return 0;
    }

    if( argc != 3 ) {
        std::cerr << "The program works as follows: " 
                  << argv[0] << " <logging_file> <standard_logging_level>\n" 
                  << "for more information: --help" << std::endl;

        return 1;
   }
   

   std::string logfile { argv[1] };
   LogLevel defualtLevel;
   

   if (auto levelInt { std::stoi(argv[2]) }; levelInt < 0 || levelInt > 2 ) {
        std::cerr << "Invalid default level. Use 0-2" << std::endl;
        return 1;
   } else {
        defualtLevel = static_cast<LogLevel>(levelInt);
   }
   
   try {
        //реализация проверки работы логера
   } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() <<std::endl;
        return 1;
   }


   system("PAUSE");

   return 0;
}