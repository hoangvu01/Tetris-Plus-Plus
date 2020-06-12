SUBDIRS  = $(wildcard */.)
SUBCLEAN = $(addsuffix .clean,$(SUBDIRS))

all: $(SUBDIRS)
clean: $(SUBCLEAN)

$(SUBDIRS):
	make -C $@

$(SUBCLEAN): %.clean:
	make -C $* clean

.PHONY: all clean $(SUBDIRS) $(SUBCLEAN)
