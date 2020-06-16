CC = gcc
CFLAGS = -g -Wall -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

libtestutils.a: testutils.o
	ar rcs $@ $^

testutils.o: testutils.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f libtestutils.a *.o core

.PHONY: clean
