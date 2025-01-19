CC=g++
CCFLAGS=-O3 -flto -s -w -finline-functions -funroll-loops -march=native -std=c++11
LDFLAGS=

SRC_FILES=main.cpp lib.cpp
OBJ_DIR=obj
OBJ_FILES=$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(wildcard *.cpp))
BIN=test

all: $(BIN)

$(BIN): $(OBJ_FILES) $(OBJ_DIR)
	$(CC) $(OBJ_FILES) -o $@ $(CCFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
