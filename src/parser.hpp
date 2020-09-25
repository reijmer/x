/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     TCEQ = 258,
     TCNE = 259,
     TCLT = 260,
     TCLE = 261,
     TCGT = 262,
     TCGE = 263,
     ASSIGN = 264,
     LPAREN = 265,
     RPAREN = 266,
     LBRACE = 267,
     RBRACE = 268,
     COMMA = 269,
     COLON = 270,
     PLUS = 271,
     MINUS = 272,
     MULT = 273,
     DIV = 274,
     MOD = 275,
     INDENT = 276,
     DEDENT = 277,
     IF = 278,
     ELSE = 279,
     DEF = 280,
     RETURN = 281,
     WHILE = 282,
     CLASS = 283,
     IN = 284,
     OR = 285,
     AS = 286,
     IS = 287,
     BREAK = 288,
     TRY = 289,
     EXCEPT = 290,
     RAISE = 291,
     FINALLY = 292,
     GLOBAL = 293,
     DEL = 294,
     CONTINUE = 295,
     FOR = 296,
     IMPORT = 297,
     PASS = 298,
     YIELD = 299,
     ELIF = 300,
     LAMBDA = 301,
     ASSERT = 302,
     WITH = 303,
     NOT = 304,
     NONLOCAL = 305,
     FROM = 306,
     AND = 307,
     TRUE = 308,
     FALSE = 309,
     NONE = 310,
     IDENTIFIER = 311,
     INTEGER = 312,
     FLOAT = 313,
     STRING = 314
   };
#endif
/* Tokens.  */
#define TCEQ 258
#define TCNE 259
#define TCLT 260
#define TCLE 261
#define TCGT 262
#define TCGE 263
#define ASSIGN 264
#define LPAREN 265
#define RPAREN 266
#define LBRACE 267
#define RBRACE 268
#define COMMA 269
#define COLON 270
#define PLUS 271
#define MINUS 272
#define MULT 273
#define DIV 274
#define MOD 275
#define INDENT 276
#define DEDENT 277
#define IF 278
#define ELSE 279
#define DEF 280
#define RETURN 281
#define WHILE 282
#define CLASS 283
#define IN 284
#define OR 285
#define AS 286
#define IS 287
#define BREAK 288
#define TRY 289
#define EXCEPT 290
#define RAISE 291
#define FINALLY 292
#define GLOBAL 293
#define DEL 294
#define CONTINUE 295
#define FOR 296
#define IMPORT 297
#define PASS 298
#define YIELD 299
#define ELIF 300
#define LAMBDA 301
#define ASSERT 302
#define WITH 303
#define NOT 304
#define NONLOCAL 305
#define FROM 306
#define AND 307
#define TRUE 308
#define FALSE 309
#define NONE 310
#define IDENTIFIER 311
#define INTEGER 312
#define FLOAT 313
#define STRING 314




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 17 "parser.y"
{
    TERNARY_TREE tVal;
    int iVal;
}
/* Line 1529 of yacc.c.  */
#line 172 "parser.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYLTYPE yylloc;
