PROJ        = ../..
BIN_DIR     = $(PROJ)/bin
LIB_DIR     = $(PROJ)/lib
OBJ_DIR     = $(PROJ)/obj/genetic-ai
CORE_OBJ_DIR = $(PROJ)/obj/core

SRC_FILES   = $(wildcard *.c)
OBJ_FILES   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(CORE_OBJ_DIR)/state.o $(CORE_OBJ_DIR)/grid.o $(CORE_OBJ_DIR)/tetriminos.o $(CORE_OBJ_DIR)/levels.o
LIB_HEADERS = $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard $(LIB_DIR)/*/include))

CC          = gcc
IncludePath = -Iinclude -I../core/include $(LIB_HEADERS)
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath) -DLLVM_ENABLE_ASSERTIONS=On
LFLAGS      = -lncurses $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs $(BIN_DIR)/train

$(BIN_DIR)/train: $(OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(CORE_OBJ_DIR)/%.o:
	make -C $(PROJ)/src/core $@

libs:
	make -C $(LIB_DIR)

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/genetic-ai
	#cd tests; make clean
	cd $(LIB_DIR); make clean

.SUFFIXES: .c .o .h
.PHONY: clean test libs
