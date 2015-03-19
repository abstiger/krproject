/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_KR_CALC_PARSER_FLEX_H_INCLUDED
# define YY_YY_KR_CALC_PARSER_FLEX_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    SEMI = 258,
    ENDFILE = 259,
    ERROR = 260,
    ID = 261,
    NUM = 262,
    FNUM = 263,
    STR = 264,
    SCHAR = 265,
    CID = 266,
    FID = 267,
    SID = 268,
    DID = 269,
    HID = 270,
    SET = 271,
    MULTI = 272,
    REGEX = 273,
    COMMA = 274,
    ASSIGN = 275,
    OR = 276,
    AND = 277,
    EQ = 278,
    NEQ = 279,
    LT = 280,
    LE = 281,
    GT = 282,
    GE = 283,
    BL = 284,
    NBL = 285,
    MATCH = 286,
    PLUS = 287,
    SUB = 288,
    MUT = 289,
    DIV = 290,
    MOD = 291,
    LP = 292,
    RP = 293,
    LSP = 294,
    RSP = 295,
    LFP = 296,
    RFP = 297,
    NOT = 298,
    UMINUS = 299
  };
#endif
/* Tokens.  */
#define SEMI 258
#define ENDFILE 259
#define ERROR 260
#define ID 261
#define NUM 262
#define FNUM 263
#define STR 264
#define SCHAR 265
#define CID 266
#define FID 267
#define SID 268
#define DID 269
#define HID 270
#define SET 271
#define MULTI 272
#define REGEX 273
#define COMMA 274
#define ASSIGN 275
#define OR 276
#define AND 277
#define EQ 278
#define NEQ 279
#define LT 280
#define LE 281
#define GT 282
#define GE 283
#define BL 284
#define NBL 285
#define MATCH 286
#define PLUS 287
#define SUB 288
#define MUT 289
#define DIV 290
#define MOD 291
#define LP 292
#define RP 293
#define LSP 294
#define RSP 295
#define LFP 296
#define RFP 297
#define NOT 298
#define UMINUS 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (T_KRCalc *krcalc, void *scanner);

#endif /* !YY_YY_KR_CALC_PARSER_FLEX_H_INCLUDED  */
