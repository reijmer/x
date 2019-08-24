

%{
#include <string>
#include <stack>
#include "node.h"
#include "parser.hpp"


int current_line_indent = 0;
std::stack<size_t> indent_stack;
int is_fake_outdent_symbol = 0;

static const unsigned int TAB_WIDTH = 2;

#define YY_USER_INIT { \
        indent_stack.push(0); \
        BEGIN(initial); \
    }

int yycolumn = 1;

void set_yycolumn(int val) {
    yycolumn = val;
    yylloc.first_column = yycolumn;
    yylloc.last_column = yycolumn + yyleng - 1;
}

#define YY_USER_ACTION { \
    yylloc.first_line = yylloc.last_line = yylineno; \
    set_yycolumn(yycolumn); \
    yycolumn += yyleng; \
}

%}

ident       [a-zA-Z_][a-zA-Z0-9_]*
integer		-?[0-9]+
float       -?[0-9]*"."[0-9]+

%option nodefault
%option noyywrap
%option yylineno

%x initial
%x indent
%x comment
%x s_string

%%

%{

int indent_caller = 0;

%}

"'''"                   { BEGIN(comment); }
["]["]["]               { BEGIN(comment); }
<comment>"'''"          { BEGIN(0); }
<comment>["]["]["]	    { BEGIN(0); }
<comment>"\n"           {}
<comment>.              {}

^[ \t]*#.*\r?\n    	    {}
#.*$                    {}

 
<*>\n           { set_yycolumn(0); yylineno--; REJECT; }


<initial>.      { set_yycolumn(yycolumn-1); indent_caller = 0; yyless(0); BEGIN(indent); }
<initial>\n     { indent_caller = 0; yyless(0); BEGIN(indent); }


<indent>" "     { current_line_indent++; }
<indent>\t      { current_line_indent = (current_line_indent + TAB_WIDTH) & ~(TAB_WIDTH-1); }
<indent>\n      { current_line_indent = 0; }

<indent>.       {
        if(!is_fake_outdent_symbol) {
            unput(*yytext);
        }
        set_yycolumn(yycolumn-1);
        is_fake_outdent_symbol = 0;

        if(current_line_indent > indent_stack.top()) {
            indent_stack.push(current_line_indent);
            BEGIN(indent_caller);
            return((yylval.token = INDENT));
        } else if(current_line_indent < indent_stack.top()) {
                        
            indent_stack.pop();

            if(current_line_indent != indent_stack.top()) {
                
                for(size_t i = 0 ; i < current_line_indent ; ++i) {
                    unput(' ');
                }
                unput('\n');
                unput('.');
                
                is_fake_outdent_symbol = 1;
                for(size_t i = 0 ; i < indent_stack.top() ; ++i) {
                    unput(' ');
                }
                unput('\n');
            } else {
                BEGIN(indent_caller);
            }

            return((yylval.token = DEDENT));
        } else {
            BEGIN(indent_caller);
        }
    }

<<EOF>>     {
        if(indent_stack.top() != 0) {
            indent_stack.pop();

            if(current_line_indent != indent_stack.top()) {
                unput('\n');
                for(size_t i = 0 ; i < indent_stack.top() ; ++i) {
                    unput(' ');
                }
            } else {
                BEGIN(indent_caller);
            }

            return((yylval.token = DEDENT));
        } else {
            yyterminate();
        }
    }

\n          { current_line_indent = 0; indent_caller = YY_START; BEGIN(indent); }


":"         { return((yylval.token = COLON)); }
","         { return((yylval.token = COMMA)); }
"("         { return((yylval.token = LPAREN)); }
")"         { return((yylval.token = RPAREN)); }
"{"         { return((yylval.token = LBRACE)); }
"}"         { return((yylval.token = RBRACE)); }

"+"        	{ return((yylval.token = PLUS)); }
"-"        	{ return((yylval.token = MINUS)); }
"*"        	{ return((yylval.token = MULT)); ; }
"/"        	{ return((yylval.token = DIV)); }
"%"        	{ return((yylval.token = MOD)); }

"="         { return((yylval.token = ASSIGN)); }
"!=="       { return((yylval.token = TCNE)); }
"=="        { return((yylval.token = TCEQ)); }
">"         { return((yylval.token = TCGT)); }
"<"         { return((yylval.token = TCLT)); }
">="        { return((yylval.token = TCGE)); }
"<="        { return((yylval.token = TCLE)); }

"if"        { return((yylval.token = IF)); }
"else"      { return((yylval.token = ELSE)); }
"def"       { return((yylval.token = DEF)); }
"return"    { return((yylval.token = RETURN)); }


"True"      { return((yylval.token = TRUE)); }
"False"     { return((yylval.token = FALSE)); }


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

\"                          BEGIN(s_string);
<s_string>{
  \n                        {}
  <<EOF>>                   {}
  ([^\\\"]|\\.)*            { 
        yylval.string = new std::string(yytext, yyleng);
        return(STRING);
    }
  \"                        BEGIN(0);
  .                         ;
}

.           {}

%%
