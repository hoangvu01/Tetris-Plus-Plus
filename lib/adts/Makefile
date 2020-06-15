CC = gcc
CFLAGS = -Wall -g -Werror -Iinclude -Itestutils 
LIB = libADT.a 
LIBOBJS = linked_list.o hash_table.o symbol_table.o
LDLIBS = -Ltestutils -ltestutils 

BUILD = $(LIB) 

all: $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)


clean:
	rm -f $(LIB) *.o core

