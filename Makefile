# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic

# Project files
SRC_DIR = source
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp) tanks_game.cpp
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,obj/%.o,$(SRC_FILES))
HEADER_FILES = $(wildcard headers/*.h)

# Main target
TARGET = tanks_game
OBJS = tanks_game.o

# Rules
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

obj/%.o: $(SRC_DIR)/%.cpp $(HEADER_FILES)
	@mkdir -p obj
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf obj $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run