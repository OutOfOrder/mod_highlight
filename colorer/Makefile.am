
prefix=@prefix@

all: colorer

colorer:
	@echo ---------------- Building Colorer-take5 shared library
	make -C src/libcolorer/ -f makefile.gcc
	make -C src/colorer/ -f makefile.gcc


clean:
	make -C src/colorer/ -f makefile.gcc clean
	make -C src/libcolorer/ -f makefile.gcc clean


install:
	mkdir -p $(prefix)/lib
	cp -p src/libcolorer/libcolorer.so $(prefix)/lib

	mkdir -p $(prefix)/bin
	cp -p src/colorer/colorer $(prefix)/bin

	mkdir -p $(prefix)/share/colorer/bin
	cp -r hrc $(prefix)/share/colorer/
	cp -r hrd $(prefix)/share/colorer/
	cp catalog.xml LICENSE README $(prefix)/share/colorer

	mkdir -p $(prefix)/share/doc/colorer-take5
	cp -r doc/* $(prefix)/share/doc/colorer-take5

	mkdir -p $(prefix)/include/colorer
	cd src/shared
	for i in unicode common common/io cregexp xml colorer colorer/parsers colorer/editor colorer/handlers colorer/viewer exparse
	do
	  mkdir -p $(prefix)/include/colorer/${i}
	  cp ./${i}/*.h $(prefix)/include/colorer/${i}
	done
	cd ../..
