calc:
	lex --noline --outfile=kr_lexer.c kr_lexer.l
	yacc -d --no-lines --output=kr_parser.c  kr_parser.y
	@mv -f kr_parser.c ..
	@mv -f kr_parser.h ..
	@mv -f kr_lexer.c ..
#	@mv -f kr_lexer.h ..
#	lex --noline --header-file=kr_lexer.h --outfile=kr_lexer.c kr_lexer.l
#	cc -g -o mycalc kr_rules.c kr_ruletree.c kr_parser.c kr_lexer.c -I"/home/tiger/Share/krproject/include" -L"/home/tiger/Share/krproject/lib" -lkrutils
