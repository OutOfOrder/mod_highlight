
prefix=@prefix@

all: colorer

colorer:
	@echo ---------------- Building Colorer-take5 shared library
	$(MAKE) -C src/libcolorer/ -f makefile.gcc
	$(MAKE) -C src/colorer/ -f makefile.gcc


clean:
	$(MAKE) -C src/colorer/ -f makefile.gcc clean
	$(MAKE) -C src/libcolorer/ -f makefile.gcc clean


install:
	mkdir -p $(prefix)/lib
	cp -p src/libcolorer/libcolorer.so $(prefix)/lib

	mkdir -p $(prefix)/bin
	cp -p src/colorer/colorer $(prefix)/bin

	mkdir -p $(prefix)/share/colorer/bin
	cp -r hrc $(prefix)/share/colorer/
	cp -r hrd $(prefix)/share/colorer/
	cp catalog.xml LICENSE README $(prefix)/share/colorer

	@(cd $(prefix)/share/colorer && find . -name ".svn" -type d -print | xargs rm -rf 2>/dev/null ) || true

	mkdir -p $(prefix)/include/colorer
	for i in unicode common common/io cregexp xml colorer colorer/parsers colorer/editor colorer/handlers colorer/viewer exparse; do \
	  mkdir -p $(prefix)/include/colorer/$${i}; \
	  cp src/shared/$${i}/*.h $(prefix)/include/colorer/$${i}; \
	done
	@(cd $(prefix)/include/colorer && find . -name ".svn" -type d -print | xargs rm -rf 2>/dev/null ) || true

