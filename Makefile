
all: src doc

clean:
	cd src; make clean
	cd doc; make clean
	cd lib; make clean


src:
	cd src; make

doc:
	git submodule update --init
	cd doc; make

lib:
	cd $(LIB_DIR); make

.PHONY: src doc clean lib
