
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     SEMI = 258,
     ENDFILE = 259,
     ERROR = 260,
     ID = 261,
     NUM = 262,
     FNUM = 263,
     DID = 264,
     SID = 265,
     FID = 266,
     AID = 267,
     STR = 268,
     SCHAR = 269,
     REGEX = 270,
     COMMA = 271,
     ASSIGN = 272,
     OR = 273,
     AND = 274,
     NEQ = 275,
     EQ = 276,
     NBL = 277,
     BL = 278,
     GE = 279,
     GT = 280,
     LE = 281,
     LT = 282,
     MATCH = 283,
     SUB = 284,
     PLUS = 285,
     MOD = 286,
     DIV = 287,
     MUT = 288,
     RFP = 289,
     LFP = 290,
     RSP = 291,
     LSP = 292,
     RP = 293,
     LP = 294,
     NOT = 295,
     UMINUS = 296
   };
#endif
/* Tokens.  */
#define SEMI 258
#define ENDFILE 259
#define ERROR 260
#define ID 261
#define NUM 262
#define FNUM 263
#define DID 264
#define SID 265
#define FID 266
#define AID 267
#define STR 268
#define SCHAR 269
#define REGEX 270
#define COMMA 271
#define ASSIGN 272
#define OR 273
#define AND 274
#define NEQ 275
#define EQ 276
#define NBL 277
#define BL 278
#define GE 279
#define GT 280
#define LE 281
#define LT 282
#define MATCH 283
#define SUB 284
#define PLUS 285
#define MOD 286
#define DIV 287
#define MUT 288
#define RFP 289
#define LFP 290
#define RSP 291
#define LSP 292
#define RP 293
#define LP 294
#define NOT 295
#define UMINUS 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




