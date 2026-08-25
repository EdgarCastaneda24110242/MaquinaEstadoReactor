CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET := reactor
BUILD_DIR := build
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

MKDIR_BUILD = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@$(MKDIR_BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(TARGET).exe" del /q "$(TARGET).exe"
	@if exist "$(TARGET)" del /q "$(TARGET)"
