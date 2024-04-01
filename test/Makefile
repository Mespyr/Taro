CC=gcc
CCFLANGS=-Wall -Wextra -pedantic
LDFLAGS=-lm

SRC_FILES=main.c lib.c
OBJ_DIR=obj
OBJ_FILES=$(patsubst %.c, $(OBJ_DIR)/%.o, $(wildcard *.c))
BIN=test

all: $(BIN)

$(BIN): $(OBJ_FILES) $(OBJ_DIR)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(CC) $(CCFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(OBJ_DIR) $(BIN)
