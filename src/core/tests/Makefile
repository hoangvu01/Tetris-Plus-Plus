# This Makefile is for testing purpose ONLY
PROJ        = ../../..
LIB_DIR     = $(PROJ)/lib

TEST_FILES  = $(wildcard *_test.c)
TESTS       = $(patsubst %_test.c, %_test, $(TEST_FILES))

SRC_FILES   = $(filter-out ../tetris.c, $(wildcard ../*.c))
OBJ_FILES   = $(patsubst ../%.c, %.o,$(SRC_FILES))
DEPS        = $(OBJ_FILES:%.o=%.d)

CC          = gcc
IncludePath = -I$(PROJ)/lib/adts/include -I../include $(addprefix -I, $(wildcard $(LIB_DIR)/*/.))
CFLAGS      = -g -Wall -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = -lncurses $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs $(TESTS)

# pre-processor automatically adds headers as dependencies
-include $(DEPS)

%_test: %_test.o $(OBJ_FILES)
	$(CC) $^ $(LFLAGS) -o $@

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: ../%.c
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(DEPS)
	rm -f $(TESTS) *.o core
	cd $(LIB_DIR); make clean	

.PHONY: clean libs
