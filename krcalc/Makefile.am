lib_LTLIBRARIES        = libkrcalc.la
libkrcalc_la_SOURCES   = \
						 kr_calc_lexer_flex.c   \
						 kr_calc_parser_flex.c   \
						 kr_calc_parser_flex.h   \
						 kr_calc_parser_json.c   \
						 kr_calc_parser_json.h   \
						 kr_calc_parser_gwt.c   \
						 kr_calc_parser_gwt.h   \
						 kr_calc_parser.c \
						 kr_calc_parser.h \
						 kr_calc_tree.c \
						 kr_calc_tree.h \
						 kr_calc.c \
						 kr_calc.h 
     
libkrcalc_la_LIBADD    = $(top_srcdir)/krutils/libkrutils.la
libkrcalc_la_LDFLAGS   =
libkrcalc_la_CPPFLAGS  =
EXTRA_DIST = kr_calc_lexer_flex.l kr_calc_parser_flex.y

kr_calc_lexer_flex.c: kr_calc_lexer_flex.l kr_calc_parser_flex.c
	$(LEX) --noline --outfile=$@ $<

kr_calc_parser_flex.c: kr_calc_parser_flex.y
	$(YACC) -d --no-lines --output=$@ $< 

clean-local:
	-rm -f kr_calc_lexer_flex.c kr_calc_parser_flex.h kr_calc_parser_flex.c

