

%{
#include <string>
#include "node.h"
#include "parser.hpp"



%}

ident       [a-zA-Z_][a-zA-Z0-9_]*
integer		-?[0-9]+
float       -?[0-9]*"."[0-9]+

%option noyywrap
%option yylineno

%x comment

%%

"'''"                	{ BEGIN(comment); }
["]["]["]				{ BEGIN(comment); }
<comment>"'''"       	{ BEGIN(0); }
<comment>["]["]["]		{ BEGIN(0); }
<comment>"\n"       	{}
<comment>.          	{}


^[ \t]*\r?\n  			{}
^[ \t]*#.*\r?\n    		{}

#.*$        			{}



":"        	{ return((yylval.token = COLON)); }
","        	{ return((yylval.token = COMMA)); }

"("        	{ return((yylval.token = LPAREN)); }
")"        	{ return((yylval.token = RPAREN)); }
"{"         { return((yylval.token = LBRACE)); }
"}"         { return((yylval.token = RBRACE)); }


"+"        	{ return((yylval.token = PLUS)); }
"-"        	{ return((yylval.token = MINUS)); }
"*"        	{ return((yylval.token = MULT)); ; }
"/"        	{ return((yylval.token = DIV)); }
"%"        	{ return((yylval.token = MOD)); }

"="        	{ return((yylval.token = ASSIGN)); }


{ident}   	{
			yylval.string = new std::string(yytext, yyleng);
			return(IDENTIFIER);
}

{float}    	{ 
			yylval.string = new std::string(yytext, yyleng);
			return(FLOAT);
}

{integer}   { 
			yylval.string = new std::string(yytext, yyleng);
			return(INTEGER);
}

.           {}

%%
