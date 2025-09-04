# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude

# Directories
SRC_DIR = src
OBJ_DIR = obj
TARGET  = my_program

# Pick up .cpp in src/ and key subdirs
SRCS := $(wildcard $(SRC_DIR)/*.cpp) \
        $(wildcard $(SRC_DIR)/core/*.cpp) \
        $(wildcard $(SRC_DIR)/disassembler/*.cpp) \
        $(wildcard $(SRC_DIR)/assembler/*.cpp)

# Map e.g. src/disassembler/foo.cpp -> obj/disassembler/foo.o
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean

# Default rule
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile (Windows-safe: make sure the obj subdir exists)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Clean (Windows-safe)
clean:
	@if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	@if exist "$(TARGET).exe" del /q "$(TARGET).exe"
	@if exist "$(TARGET)" del /q "$(TARGET)"
