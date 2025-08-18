cmake_minimum_required(VERSION 3.10.0)
project(logging_library VERSION 0.1.0 LANGUAGES CXX)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_library(logging_library SHARED src/logger.cpp)
target_include_directories(logging_library PUBLIC include)
set_target_properties(logging_library PROPERTIES OUTPUT_NAME "logging_library")

add_executable(main_app src/main.cpp)
target_link_libraries(main_app logging_library)


add_executable(tests_app src/test/logging_system_tests.cpp)
target_link_libraries(tests_app logging_library)

add_test(NAME LoggingAutoTests
    COMMAND tests_app
)

add_custom_command(TARGET tests_app POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND $<TARGET_FILE:tests_app>
    COMMENT "Running auto tests after build..."
)
