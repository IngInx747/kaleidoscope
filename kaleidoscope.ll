%{
#include <string>
#include "parser.hh"
%}

%option noyywrap
%option nounput
%option noinput
%option yylineno

NUMBER          [0-9]+\.?|[0-9]*\.[0-9]+
SYMBOL          [a-zA-Z][a-zA-Z0-9]*
EXTERN          extern
DEFINE          def
OPERATION       [+-*/]
SEPARATOR       [();,]
WHITESPACE      [\t\r ]
NEWLINE         \n
COMMENT         [#].*

%%

<INITIAL>{
{EXTERN} { return (EXTERN); }
{DEFINE} { return (DEFINE); }
{NUMBER} { yylval.emplace<std::string>(yytext); return (NUMBER); }
{SYMBOL} { yylval.emplace<std::string>(yytext); return (SYMBOL); }
{OPERATION} { return yytext[0]; }
{NEWLINE} { ++yylineno; }
{WHITESPACE} |
{SEPARATOR}  |
{COMMENT}    {}
. { yylval.emplace<std::string>("Unexpected token \"" + std::string(yytext) + "\""); return (ERROR); }
}

%%
