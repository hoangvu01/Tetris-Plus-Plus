CC 					= gcc
IncludePath	= -I../include -I../../core/include
CFLAGS		  = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS 		  = -lwiringPi
SRC         = $(wildcard ../*.c)
OBJS 	= $(patsubst ../%.c,%.o)

all: gpio_test

gpio_test: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

%.o: ../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o gpio_test

.PHONY: clean
