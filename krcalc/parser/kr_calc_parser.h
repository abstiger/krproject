/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
     STR = 264,
     SCHAR = 265,
     CID = 266,
     FID = 267,
     SID = 268,
     DID = 269,
     HID = 270,
     SET = 271,
     MUTLI = 272,
     REGEX = 273,
     COMMA = 274,
     ASSIGN = 275,
     OR = 276,
     AND = 277,
     NEQ = 278,
     EQ = 279,
     NBL = 280,
     BL = 281,
     GE = 282,
     GT = 283,
     LE = 284,
     LT = 285,
     MATCH = 286,
     SUB = 287,
     PLUS = 288,
     MOD = 289,
     DIV = 290,
     MUT = 291,
     RFP = 292,
     LFP = 293,
     RSP = 294,
     LSP = 295,
     RP = 296,
     LP = 297,
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
#define MUTLI 272
#define REGEX 273
#define COMMA 274
#define ASSIGN 275
#define OR 276
#define AND 277
#define NEQ 278
#define EQ 279
#define NBL 280
#define BL 281
#define GE 282
#define GT 283
#define LE 284
#define LT 285
#define MATCH 286
#define SUB 287
#define PLUS 288
#define MOD 289
#define DIV 290
#define MUT 291
#define RFP 292
#define LFP 293
#define RSP 294
#define LSP 295
#define RP 296
#define LP 297
#define NOT 298
#define UMINUS 299




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




