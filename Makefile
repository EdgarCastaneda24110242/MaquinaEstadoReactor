CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pedantic -Iinclude
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system
TARGET := bin/ZombieAStar.exe
SOURCES := $(wildcard src/*.cpp)

all: zombie

$(TARGET): $(SOURCES)
	@if not exist bin mkdir bin
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

run: $(TARGET)
	$(TARGET)

zombie: run

clean:
	@if exist bin\ZombieAStar.exe del /Q bin\ZombieAStar.exe

.PHONY: all run zombie clean
