CPP=clang++
CPPFLAGS=-Wall -Wextra -pedantic
LDFLAGS=

# directories
SRC_DIR=src
LEXER_SRC_DIR=src/lexer
PARSER_SRC_DIR=src/parser
TYPE_CHECKER_SRC_DIR=src/type_checker
COMPILER_SRC_DIR=src/compiler
ASSEMBLY_SRC_DIR=src/assembly
OBJ_DIR=obj
INCLUDE_DIR=src/include
# src files
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
LEXER_SRC_FILES=$(wildcard $(LEXER_SRC_DIR)/*.cpp)
PARSER_SRC_FILES=$(wildcard $(PARSER_SRC_DIR)/*.cpp)
TYPE_CHECKER_SRC_FILES=$(wildcard $(TYPE_CHECKER_SRC_DIR)/*.cpp)
COMPILER_SRC_FILES=$(wildcard $(COMPILER_SRC_DIR)/*.cpp)
ASSEMBLY_SRC_FILES=$(wildcard $(ASSEMBLY_SRC_DIR)/*.cpp)
# object files
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
LEXER_OBJ_FILES=$(patsubst $(LEXER_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LEXER_SRC_FILES))
PARSER_OBJ_FILES=$(patsubst $(PARSER_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(PARSER_SRC_FILES))
TYPE_CHECKER_OBJ_FILES=$(patsubst $(TYPE_CHECKER_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TYPE_CHECKER_SRC_FILES))
COMPILER_OBJ_FILES=$(patsubst $(COMPILER_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(COMPILER_SRC_FILES))
ASSEMBLY_OBJ_FILES=$(patsubst $(ASSEMBLY_SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(ASSEMBLY_SRC_FILES))

HDR_FILES=$(wildcard $(INCLUDE_DIR)/*.h)
BIN=orcc

all: $(BIN)

$(BIN): $(OBJ_FILES) $(LEXER_OBJ_FILES) $(PARSER_OBJ_FILES) $(TYPE_CHECKER_OBJ_FILES) $(COMPILER_OBJ_FILES) $(ASSEMBLY_OBJ_FILES) $(OBJ_DIR)
	$(CPP) \
		$(OBJ_FILES) \
		$(LEXER_OBJ_FILES) \
		$(PARSER_OBJ_FILES) \
		$(TYPE_CHECKER_OBJ_FILES) \
		$(COMPILER_OBJ_FILES) \
		$(ASSEMBLY_OBJ_FILES) \
		-o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(LEXER_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(PARSER_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(TYPE_CHECKER_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(COMPILER_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(ASSEMBLY_SRC_DIR)/%.cpp $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

test: all
	python3 run_tests.py

clean:
	$(RM) -r $(OBJ_DIR) *.o a.out *.asm
	$(RM) $(BIN)
