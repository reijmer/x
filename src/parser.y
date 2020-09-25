%{
#include <iostream>
#include "node.h"
#include "parser.hpp"

#include <cstdio>
#include <cstdlib>

TERNARY_TREE *programBlock;

extern int yylex();
void yyerror(const char *s) { std::printf("Error: %s \n", s);}

%}


%union {
    TERNARY_TREE tVal;
    int iVal;
}


%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE ASSIGN
%token <token> LPAREN RPAREN LBRACE RBRACE COMMA COLON
%token <token> PLUS MINUS MULT DIV MOD INDENT
                DEDENT IF ELSE DEF RETURN WHILE CLASS
                IN OR AS IS BREAK TRY EXCEPT RAISE FINALLY
                GLOBAL DEL CONTINUE FOR IMPORT PASS YIELD
                ELIF LAMBDA ASSERT WITH NOT NONLOCAL FROM
                AND

%token <token> TRUE FALSE NONE
%token <string> IDENTIFIER INTEGER FLOAT STRING
 
%type <tVal> ident
%type <tVal> numeric expr func_call bool string
%type <tVal> call_args
%type <tVal> decl_args
%type <tVal> program stmts block
%type <tVal> stmt var_decl if_stmt func_decl while_loop
%type <tVal> bin_op bool_op

%left PLUS MINUS
%left MULT DIV
%left MOD

%locations


%start program

%%

program : stmts { TERNARY_TREE programBlock = create_node(NOTHING, PROGRAM, $1, NULL, NULL); }
        | {}
        ;

stmts   : stmt { $$ = create_node(NOTHING, STATEMENTS, $1, NULL, NULL); }
        | stmts stmt { $$ = create_node(NOTHING, STATEMENTS, $1, $2, NULL); }
        ;

block   : INDENT stmts DEDENT { $$ = create_node(NOTHING, STATEMENTS, $2, NULL, NULL); }
        ;

stmt    : expr { $$ = NULL; }
        | var_decl
        | if_stmt
        | func_decl
        | while_loop
        | RETURN expr { $$ = NULL; }
        ;
	
ident   : IDENTIFIER { $$ = NULL; }
        ;

numeric : INTEGER { $$ = NULL; }
        | FLOAT { $$ = NULL; }
        ;

bool    : TRUE { $$ = NULL; }
        | FALSE { $$ = NULL; }
        ;

string  : STRING { $$ = NULL; }
        ;
	
var_decl    : ident ASSIGN expr { $$ = NULL; }
            | ident { $$ = NULL; }
            ;

if_stmt : IF expr COLON block { $$ = NULL; }
        | IF expr COLON block ELSE COLON block { $$ = NULL; }
        ;

func_decl   : DEF ident LPAREN decl_args RPAREN COLON block { $$ = NULL; }
            ;

decl_args   : var_decl { $$ = NULL; }
            | decl_args COMMA var_decl { $$ = NULL; }
            | { $$ = NULL; }
	
func_call   : ident LPAREN call_args RPAREN { $$ = NULL; }
            ;

call_args   : expr { $$ = NULL;}
            | call_args COMMA expr { $$ = NULL; }
            | { $$ = NULL; }
            ;

while_loop  : WHILE expr COLON block { $$ = NULL; }
            | WHILE expr COLON block ELSE COLON block { $$ = NULL; }
            ;

expr    : ident { $$ = NULL; }
        | numeric
        | bool
        | string
        | ident ASSIGN expr { $$ = NULL; }
        | expr bin_op expr { $$ = NULL; }
        | expr bool_op expr { $$ = NULL; }
        | LPAREN expr RPAREN { $$ = NULL; }
        | func_call
        ;

bin_op  : PLUS
        | MINUS
        | MULT
        | DIV
        | MOD
        ;

bool_op : TCEQ
        | TCNE
        | TCLT
        | TCLE
        | TCGT
        | TCGE
        ;

	
%%


TERNARY_TREE create_node(int iVal, int identifier, TERNARY_TREE left, TERNARY_TREE center, TERNARY_TREE right) {

    TERNARY_TREE t;
    t = (TERNARY_TREE)malloc(sizeof(TREE_NODE));
    t->value = iVal;
    t->identifier = identifier;

    t->left = left;
    t->center = center;
    t->right = right;

    return(t);
}