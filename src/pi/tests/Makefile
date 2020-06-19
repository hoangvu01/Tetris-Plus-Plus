CC          = gcc
IncludePath = -I../include -I../../core/include
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = -lwiringPi -lpthread
SRC         = $(wildcard ../*.c)
TEST_SRC    = $(wildcard *.c)
OBJS        = $(patsubst ../%.c,%.o, $(SRC)) $(patsubst %.c, %.o, $(TEST_SRC))

all: gpio_test

gpio_test: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

%.o: ../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o gpio_test

.PHONY: clean
