
all: src

clean:
	cd src; make clean
	cd lib; make clean


src:
	cd src; make

lib:
	cd $(LIB_DIR); make

.PHONY: src clean lib
