PROJ        = ../..
BIN_DIR     = $(PROJ)/bin
LIB_DIR     = $(PROJ)/lib
OBJ_DIR     = $(PROJ)/obj/rl-ai
TETRIS_OBJ_DIR  = $(PROJ)/obj/core

SRC_FILES   = $(wildcard *.c)
OBJ_FILES   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
LIB_HEADERS = $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard $(LIB_DIR)/*/include))

TETRIS_OBJ_FILES = $(filter-out $(TETRIS_OBJ_DIR)/tetris.o, $(wildcard $(TETRIS_OBJ_DIR)/*.o)) 

CC          = gcc
IncludePath = -Iinclude -I../core/include $(LIB_HEADERS)
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = -lncurses $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs tetris $(BIN_DIR)/train

$(BIN_DIR)/train: $(OBJ_FILES) $(TETRIS_OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

tetris:
	cd ../core; make

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/tetris
	cd $(LIB_DIR); make clean
	cd ../core; make clean

.SUFFIXES: .c .o .h
.PHONY: clean test libs
