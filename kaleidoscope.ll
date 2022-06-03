%{
#include <string>
#include "utility.hh"
#include "kal.parser.gen.hh"
%}

%option noyywrap
%option yylineno

NUMBER          [0-9]+\.?|[0-9]*\.[0-9]+
SYMBOL          [a-zA-Z][a-zA-Z0-9]*
EXTERN          extern
DEFINE          def
OPERATION       [\+\-\*\/]
SEPARATOR       [\(\);,]
WHITESPACE      [\t\r ]
NEWLINE         \n
COMMENT         [#].*$

%%

<INITIAL>{
{EXTERN}        { return (EXTERN); }
{DEFINE}        { return (DEFINE); }
{NUMBER}        { yylval.str = make_c_str(yytext); return (NUMBER); }
{SYMBOL}        { yylval.str = make_c_str(yytext); return (SYMBOL); }

{SEPARATOR}     |
{OPERATION}     { return yytext[0]; }

{WHITESPACE}    |
{COMMENT}       {}
{NEWLINE}       { ++yylineno; }
<<EOF>>         { yyterminate(); }
.               { yylval.str = "Unexpected token"; return (ERROR); }
}

%%
