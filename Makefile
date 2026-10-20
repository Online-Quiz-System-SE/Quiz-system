CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = $(filter-out src/main.cpp, $(wildcard src/*.cpp))

all: quiz
quiz: $(SRC) src/main.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@
test: $(SRC) tests/test_main.cpp
	$(CXX) $(CXXFLAGS) $^ -o tests_bin && ./tests_bin
clean:
	rm -f quiz quiz.exe tests_bin tests_bin.exe
.PHONY: all test clean
