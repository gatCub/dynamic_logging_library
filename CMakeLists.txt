cmake_minimum_required(VERSION 3.10.0)
project(logging_library VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_library(logging_library SHARED src/logger.cpp)
target_include_directories(logging_library PUBLIC include)
set_target_properties(logging_library PROPERTIES OUTPUT_NAME "logging_library")

add_executable(main src/main.cpp)
target_link_libraries(main logging_library)

#Добавить сборку тестов как закончу с main

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
