# This Makefile is for testing purpose ONLY
PROJ        = ../../..
LIB_DIR     = $(PROJ)/lib
COMMON_DIR  = $(PROJ)/src/common

TEST_FILES  = $(wildcard *_test.c)
TESTS       = $(patsubst %_test.c, %_test, $(TEST_FILES))

SRC_FILES   = $(filter-out ../assemble.c, $(wildcard ../*.c))
OBJ_FILES   = $(patsubst ../%.c, %.o,$(SRC_FILES)) utils.o

CC          = gcc
IncludePath = -I$(PROJ)/lib/adts/include -I../include -I$(COMMON_DIR)/include $(addprefix -I, $(wildcard $(LIB_DIR)/*/.))
CFLAGS      = -g -Wall -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs $(TESTS)

%_test: %_test.o $(OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: ../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: $(COMMON_DIR)/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(TESTS) *.o core
	cd $(LIB_DIR); make clean	

.PHONY: clean libs
