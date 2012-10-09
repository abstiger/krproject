all:
	lex --noline --outfile=kr_calc_lexer.c kr_calc_lexer.l
	yacc -d --no-lines --output=kr_calc_parser.c  kr_calc_parser.y
	@mv -f kr_calc_parser.c ..
	@mv -f kr_calc_parser.h ..
	@mv -f kr_calc_lexer.c ..
#	@mv -f kr_calc_lexer.h ..
#	lex --noline --header-file=kr_calc_lexer.h --outfile=kr_calc_lexer.c kr_calc_lexer.l
#	cc -g -o mycalc kr_calc.c kr_calc_tree.c kr_calc_parser.c kr_calc_lexer.c -I"/home/tiger/Share/krproject/include" -L"/home/tiger/Share/krproject/lib" -lkrutils
