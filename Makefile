# Simple Makefile for the evil_word_guess project
CXX := clang++
CXXFLAGS := -std=c++23
CPPFLAGS := -Iincludes
OPTFLAGS := -O0 -gdwarf-4

SRCS := ./src/driver.cc ./src/word_guess_functions.cpp
TARGET := bin/exec

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) $(SRCS) -o $@

clean:
	rm -f $(TARGET)
