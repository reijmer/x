

%{
#include <string>
#include "node.h"
#include "parser.hpp"


YYSTYPE yylval;
YYLTYPE yylloc;

yypstate* pstate = yypstate_new();


#define PUSH_TOKEN(token) do {  \
    int s = yypush_parse(pstate, token, &yylval, &yylloc);  \
    if (s != YYPUSH_MORE) {                                 \
        yypstate_delete(pstate);                              \
        return s;                                             \
    }                                                       \
} while(0)

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



":"        	{ PUSH_TOKEN((yylval.token = COLON)); }
","        	{ PUSH_TOKEN((yylval.token = COMMA)); }

"("        	{ PUSH_TOKEN((yylval.token = LPAREN)); }
")"        	{ PUSH_TOKEN((yylval.token = RPAREN)); }
"{"         { PUSH_TOKEN((yylval.token = LBRACE)); }
"}"         { PUSH_TOKEN((yylval.token = RBRACE)); }


"+"        	{ PUSH_TOKEN((yylval.token = PLUS)); }
"-"        	{ PUSH_TOKEN((yylval.token = MINUS)); }
"*"        	{ PUSH_TOKEN((yylval.token = MULT)); ; }
"/"        	{ PUSH_TOKEN((yylval.token = DIV)); }
"%"        	{ PUSH_TOKEN((yylval.token = MOD)); }

"="        	{ PUSH_TOKEN((yylval.token = ASSIGN)); }


{ident}   	{
			yylval.string = new std::string(yytext, yyleng);
			PUSH_TOKEN(IDENTIFIER);
}

{float}    	{ 
			yylval.string = new std::string(yytext, yyleng);
			PUSH_TOKEN(FLOAT);
}

{integer}   { 
			yylval.string = new std::string(yytext, yyleng);
			PUSH_TOKEN(INTEGER);
}

.           {}

%%
