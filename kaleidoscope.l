%{
#include <string>
#include "utility.hh"
#include "kal.parser.gen.hh"
%}

%option noyywrap
%option yylineno

NUMBER          [0-9]+\.?|[0-9]*\.[0-9]+
SYMBOL          [a-zA-Z_][a-zA-Z0-9_]*
EXTERN          extern
DEFINE          def
IF              if
THEN            then
ELSE            else
FOR             for
OPERATOR        [\+\-\*\/\=\<\>\(\)\{\};\,]
WHITESPACE      [\t\r ]
NEWLINE         \n
COMMENT         [#].*$

%%

<INITIAL>{
{EXTERN}        { return (EXTERN); }
{DEFINE}        { return (DEFINE); }
{IF}            { return (IF); }
{THEN}          { return (THEN); }
{ELSE}          { return (ELSE); }
{FOR}           { return (FOR); }

{NUMBER}        { yylval.str = make_c_str(yytext); return (NUMBER); }
{SYMBOL}        { yylval.str = make_c_str(yytext); return (SYMBOL); }

{OPERATOR}      { return yytext[0]; }

{WHITESPACE}    {}
{COMMENT}       {}
{NEWLINE}       {} /* yylineno increments automatically */

<<EOF>>         { yyterminate(); }
.               { yylval.str = "Unexpected token"; return (ERROR); }
}

%%
