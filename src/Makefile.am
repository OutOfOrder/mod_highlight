CLEANFILES = .libs/libmod_highlight *~

libmod_highlight_la_SOURCES = mod_highlight.c colorer_interface.cpp ChunkLineSource.cpp BucketWriter.cpp ColorerFilter.cpp
libmod_highlight_la_CFLAGS = -Wall -I/usr/local/include/colorer ${MODULE_CFLAGS}
libmod_highlight_la_CPPFLAGS = -Wall -I/usr/local/include/colorer ${MODULE_CFLAGS}
libmod_highlight_la_LDFLAGS = -L/usr/local/lib -lcolorer

lib_LTLIBRARIES = libmod_highlight.la 

make_so:
	@if test ! -L mod_highlight.so ; then ln -s .libs/libmod_highlight.so mod_highlight.so ; fi

clean:
	rm -fr .libs
	rm -f *.o *.lo *.la
	rm -f mod_highlight.so

install: make_so
	@${APXS_BIN} -i -n highlight mod_highlight.so
	@echo ""
	@echo ""
	@echo "***********************************************"
	@echo ""
	@echo "   Please read the documentation at            "
	@echo "   http://mod-auth.sourceforge.net/docs/ for   "
	@echo "   details on configuration of this module     "
	@echo ""
	@echo "***********************************************"
	@echo ""

activate: make_so
	@${APXS_BIN} -i -a -n highlight mod_highlight.so
	@echo ""
	@echo ""
	@echo "***********************************************"
	@echo ""
	@echo "   Please read the documentation at            "
	@echo "   http://mod-auth.sourceforge.net/docs/ for   "
	@echo "   details on configuration of this module     "
	@echo ""
	@echo "***********************************************"
	@echo ""
