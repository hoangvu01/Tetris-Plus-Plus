CC 					= gcc
IncludePath	= -I../include -I../../core/include
CFLAGS		= -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS 		= -lwiringPi
OBJ_FILES 	= gpio_test.o gpio_input.o gpio_button.o gpio_sports.o

pi: gpio_input

gpio_input: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o gpio_test $(LFLAGS)

gpio_input.o: ../gpio_input.c

gpio_button.o: ../gpio_button.c

gpio_sports.o: ../gpio_sports.c

clean:
	rm -f *.o gpio_test

.PHONY: clean
