CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -fPIC
LDFLAGS = -pthread

LIB_NAME = liblogging_library.so
LIB_SRC = src/logger.cpp

APP_NAME = main_app
APP_SRC = src/main.cpp

TESTS_NAME = tests_app
TESTS_SRC = src/test/logging_system_tests.cpp

.PHONY: all clean test

all: $(LIB_NAME) $(APP_NAME) $(TESTS_NAME)

$(LIB_NAME): $(LIB_SRC)
	$(CXX) $(CXXFLAGS) -shared -o $@ $<

$(APP_NAME): $(APP_SRC) $(LIB_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -llogging_library $(LDFLAGS)

$(TESTS_NAME): $(TESTS_SRC) $(LIB_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -llogging_library $(LDFLAGS)

test: 
	$(TESTS_NAME) LD_LIBRARY_PATH=. ./$(TESTS_NAME)

clean:
	rm -f $(LIB_NAME) $(APP_NAME) $(TESTS_NAME) *.log
