AUTOMAKE_OPTIONS = foreign dist-bzip2

EXTRA_DIST = m4/apache.m4 \
	m4/apache_test.m4 \
	m4/outoforder.m4 \
	TODO \
	README

SUBDIRS = @COLORER_DIR@ src

TODO: TODO.in
	./gen_todo.pl
