CLEANFILES = .libs/libmod_highlight *~

libmod_highlight_la_SOURCES = mod_highlight.c
libmod_highlight_la_CFLAGS = -Wall ${XSLT_CFLAGS} ${MODULE_CFLAGS}
libmod_highlight_la_LDFLAGS = ${XSLT_LIBS}

lib_LTLIBRARIES = libmod_highlight.la 

make_so:
	@if test ! -L mod_highlight.so ; then ln -s .libs/libmod_highlight.so mod_highlight.so ; fi

clean:
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
