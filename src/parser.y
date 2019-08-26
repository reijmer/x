%{
#include <iostream>
#include "node.h"
#include "parser.hpp"

#include <cstdio>
#include <cstdlib>
NBlock *programBlock;

extern int yylex();
void yyerror(const char *s) { std::printf("Error: %s \n", s);}
%}


%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector<NExpression*> *exprvec;
    std::vector<NVariableDeclaration*> *varvec;

    std::string *string;
    int token;
}


%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE ASSIGN
%token <token> LPAREN RPAREN LBRACE RBRACE COMMA COLON
%token <token> PLUS MINUS MULT DIV MOD INDENT DEDENT IF ELSE DEF RETURN WHILE
%token <token> TRUE FALSE

%token <string> IDENTIFIER INTEGER FLOAT STRING
 
%type <ident> ident
%type <expr> numeric expr func_call bool string
%type <exprvec> call_args
%type <varvec> decl_args
%type <block> program stmts block
%type <stmt> stmt var_decl if_stmt func_decl while_loop
%type <token> bin_op bool_op

%left PLUS MINUS
%left MULT DIV
%left MOD

%locations


%start program

%%

program : stmts { programBlock = $1; }
        | %empty { programBlock = new NBlock(); }
        ;

stmts   : stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
        | stmts stmt { $1->statements.push_back($<stmt>2); }
        ;

block   : INDENT stmts DEDENT { $$ = $2; }
        ;

stmt    : expr { $$ = new NExpressionStatement(*$1); }
        | var_decl
        | if_stmt
        | func_decl
        | while_loop
        | RETURN expr { $$ = new NReturnStatement(*$2); }
        ;
	
ident   : IDENTIFIER { $$ = new NIdentifier(*$1); delete $1;}
        ;

numeric : INTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
        | FLOAT { $$ = new NDouble(atof($1->c_str())); delete $1; }
        ;

bool    : TRUE { $$ = new NBool(true); }
        | FALSE { $$ = new NBool(false); }
        ;

string  : STRING { $$ = new NString($1->c_str()); delete $1; }
        ;
	
var_decl    : ident ASSIGN expr { $$ = new NVariableDeclaration(*$1, $3); }
            | ident { $$ = new NVariableDeclaration(*$1);}
            ;

if_stmt : IF expr COLON block { $$ = new NIfStatement($2, $4); }
        | IF expr COLON block ELSE COLON block { $$ = new NIfStatement($2, $4, $7); }
        ;

func_decl   : DEF ident LPAREN decl_args RPAREN COLON block { $$ = new NFunctionDeclaration(*$2, *$4, *$7); delete $4; }
            ;

decl_args   : var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
            | decl_args COMMA var_decl { $1->push_back($<var_decl>3); }
            | %empty { $$ = new VariableList(); }
	
func_call   : ident LPAREN call_args RPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
            ;

call_args   : expr { $$ = new ExpressionList(); $$->push_back($1); }
            | call_args COMMA expr { $1->push_back($3); }
            | %empty { $$ = new ExpressionList(); }
            ;

while_loop  : WHILE expr COLON block { $$ = new NWhileLoop($2, $4); }
            | WHILE expr COLON block ELSE COLON block { $$ = new NWhileLoop($2, $4, $7); }
            ;

expr    : ident { $<ident>$ = $1; }
        | numeric
        | bool
        | string
        | ident ASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
        | expr bin_op expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
        | expr bool_op expr { $$ = new NBooleanOperator(*$1, $2, *$3); }
        | LPAREN expr RPAREN { $$ = $2; }
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
