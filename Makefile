CORE_SRC = $(wildcard src/core/*.c)
GENE_SRC = $(wildcard src/genetic-ai/*.c)
RL_SRC   = $(wildcard src/rl-ai/*.c)
PI_SRC   = $(wildcard src/pi/*.c)

CORE_OBJS = $(patsubst src/core/%.c,obj/core/%.o,$(CORE_SRC))
GENE_OBJS = $(patsubst src/genetic-ai/%.c,obj/genetic-ai/%.o,$(GENE_SRC))
RL_OBJS   = $(patsubst src/rl-ai/%.c,obj/rl-ai/%.o,$(RL_SRC))
PI_OBJS   = $(patsubst src/pi/%.c,obj/pi/%.o,$(PI_SRC))
CORE_NO_MAIN = $(filter-out obj/core/tetris.o, $(CORE_OBJS))
DEPS = $(CORE_OBJS:%.o=%.d) $(GENE_OBJS:%.o=%.d) $(RL_OBJS:%.o=%.d) $(PI_OBJS:%.o=%.d)
 
CC          = gcc
IncludePath = $(addprefix -I, $(wildcard src/*/include)) $(addprefix -I, $(wildcard lib/*/include)) 
CFLAGS      = -MMD -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
GENEFLAGS   = -DLLVM_ENABLE_ASSERTIONS=On -Xpreprocessor -fopenmp -O3
LDLIBS      = -lncurses -lm $(addprefix -L, $(wildcard lib/*/.)) $(wildcard lib/*/*.a)
PILIBS      = -lwiringPi -lpthread 

all: lib core genetic genetic-train pi rl
core:     bin/tetris
genetic:  bin/genetic-ai-play
genetic-train: bin/genetic-train
rl-train: bin/rltrain
pi:       bin/tetrispi

bin/tetris: $(CORE_OBJS)
	$(CC) $^ $(LDLIBS) -o $@

bin/tetrispi: $(PI_OBJS) $(CORE_OBJS)
	$(CC) $^ $(LDLIBS) $(PILIBS) -o $@

bin/genetic-ai-play: $(filter-out obj/genetic-ai/genetic-train.o, $(GENE_OBJS)) $(filter-out obj/core/game.o, $(CORE_NO_MAIN))
	$(CC) $^ $(LDLIBS) -fopenmp -o $@

bin/genetic-train: $(filter-out obj/genetic-ai/genetic-ai-play.o, $(GENE_OBJS)) $(filter-out obj/core/game.o, $(CORE_NO_MAIN))
	$(CC) $^ $(LDLIBS) -fopenmp -o $@

bin/rltrain: lib bin/tetris $(RL_OBJS) $(filter-out obj/core/game.o, $(CORE_NO_MAIN))
	$(CC)  $(RL_OBJS) $(filter-out obj/core/game.o, $(CORE_NO_MAIN)) $(LDLIBS) -o $@

# pre-processor automatically adds headers as dependencies
-include $(DEPS)

obj/core/%.o: src/core/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/genetic-ai/%.o: src/genetic-ai/%.c
	$(CC) $(CFLAGS) $(GENEFLAGS) -c -o $@ $<

obj/rl-ai/%.o: src/rl-ai/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

obj/pi/%.o: src/pi/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


lib:
	cd lib; make

clean:
	rm -f $(DEPS)
	rm -f $(CORE_OBJS) $(GENE_OBJS) $(RL_OBJS) $(PI_OBJS)
	rm -f  bin/tetris bin/genetic-ai-play bin/rltrain bin/tetrispi
	cd lib; make clean

.PHONY: src clean lib core genetic pi rl
