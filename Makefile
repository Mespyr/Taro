CPP=g++
CPPFLAGS=-Wall -Wextra -pedantic
LDFLAGS=

# directories
SRC_DIR=src
LEXER_SRC_DIR=src/lexer
OBJ_DIR=obj
INCLUDE_DIR=src/include
# src files
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
LEXER_SRC_FILES=$(wildcard $(LEXER_SRC_DIR)/*.cpp)
# object files
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
LEXER_OBJ_FILES=$(patsubst $(LEXER_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LEXER_SRC_FILES))

HDR_FILES=$(wildcard $(INCLUDE_DIR)/*.h)
BIN=rambutan

all: $(BIN)

$(BIN): $(OBJ_FILES) $(LEXER_OBJ_FILES) $(OBJ_DIR)
	$(CPP) $(OBJ_FILES) $(LEXER_OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(LEXER_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

test: all
	python3 run_tests.py

clean:
	$(RM) -r $(OBJ_DIR) *.o out *.asm
	$(RM) $(BIN)
