all:
	lex --noline --outfile=kr_calc_lexer.c kr_calc_lexer.l
	yacc -d --no-lines --output=kr_calc_parser.c  kr_calc_parser.y
	@cp -f kr_calc_parser.c ..
	@cp -f kr_calc_parser.h ..
	@cp -f kr_calc_lexer.c ..

clean:
	@rm -f kr_calc_parser.h kr_calc_parser.c kr_calc_lexer.c
