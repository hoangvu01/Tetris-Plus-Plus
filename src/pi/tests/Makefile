CC 					= gcc
IncludePath = -I../include
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS 			=
OBJ_FILES 	= gpio_test.o gpio_input.o

pi: gpio_input

gpio_input.o: ../gpio_input.c
	$(CC) $(CFLAGS) -c -o $@ $<

gpio_input: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o gpio_test $(LFLAGS)

clean:
	rm -f *.o gpio_test

.PHONY: clean