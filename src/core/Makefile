PROJ        = ../..
BIN_DIR     = $(PROJ)/bin
LIB_DIR     = $(PROJ)/lib
OBJ_DIR     = $(PROJ)/obj

PI_FILES    = $(OBJ_DIR)/gpio_button.o $(OBJ_DIR)/gpio_input.o $(OBJ_DIR)/gpio_sports.o
SRC_FILES   = $(wildcard *.c) 
OBJ_FILES   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES)) $(PI_FILES)
LIB_HEADERS = $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard $(LIB_DIR)/*/include))

CC          = gcc
IncludePath = -Iinclude -I../common/include -I../pi/include$(LIB_HEADERS)
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = -lncurses -lwiringPi $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs $(BIN_DIR)/tetris

$(BIN_DIR)/tetris: $(OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/tetris
	cd tests; make clean
	cd $(LIB_DIR); make clean

.SUFFIXES: .c .o .h
.PHONY: clean test libs
