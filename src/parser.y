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

	std::string *string;
	int token;
}


%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE ASSIGN
%token <token> LPAREN RPAREN LBRACE RBRACE COMMA COLON
%token <token> PLUS MINUS MULT DIV MOD

%token <string> IDENTIFIER INTEGER FLOAT
 
%type <ident> ident
%type <expr> numeric expr func_call
%type <exprvec> call_args
%type <block> program stmts
%type <stmt> stmt var_decl
%type <token> bin_op bool_op

%left PLUS MINUS
%left MULT DIV
%left MOD

%locations


%start program

%%

program	: stmts { programBlock = $1; }
		| %empty { programBlock = new NBlock(); }
		;

stmts	: stmt { $$ = new NBlock(); $$->statements.push_back($<stmt>1); }
		| stmts stmt { $1->statements.push_back($<stmt>2); }
		;

stmt	: expr { $$ = new NExpressionStatement(*$1); }
		| var_decl
		;
	
ident	: IDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
		;

numeric	: INTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
		| FLOAT { $$ = new NDouble(atof($1->c_str())); delete $1; }
		;
	
var_decl	: ident ASSIGN expr { $$ = new NVariableDeclaration(*$1, $3); }
			;
	
func_call	: ident LPAREN call_args RPAREN { std::cout << programBlock; $$ = new NMethodCall(*$1, *$3); delete $3; }
			;

call_args	: expr { $$ = new ExpressionList(); $$->push_back($1); }
			| call_args COMMA expr { $1->push_back($3); }
			| %empty { $$ = new ExpressionList(); }
			;

expr	: ident { $<ident>$ = $1; }
		| numeric
		| ident ASSIGN expr { $$ = new NAssignment(*$<ident>1, *$3); }
		| expr bin_op expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
    	| LPAREN expr RPAREN { $$ = $2; }
    	| func_call
    	;

bin_op	: PLUS
		| MINUS
		| MULT
		| DIV
		| MOD
		;

	
%%
