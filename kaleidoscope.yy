%code top
{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <vector>
#include <string>
#include "ast_node.hh"
#include "visitor.hh"

extern visitor* the_visitor;

void yyerror (char const *);

int yylex (void);
}

/* Generate the parser description file (parser.output). */
%verbose

/* Nice error messages with details. */
%define parse.error detailed

/* Enable run-time traces (yydebug). */
%define parse.trace

/* Generate YYSTYPE from the types used in %token and %type. */
%define api.value.type variant

%define api.token.prefix {}

%token EXTERN "extern"
%token DEFINE "def"

%token <std::string> NUMBER
%token <std::string> SYMBOL
%token <std::string> ERROR

%type <ast_node*> expression
%type <function_declaration_node*> declaration
%type <std::vector<ast_node*>> expressions
%type <std::vector<ast_node*>> arguments

%left "+" "-";
%left "*" "/";

%start program;

%%

program: program command
  {
    ;
  }
  | command
  {
    ;
  }
  | /* empty */
  {
    ;
  }
  ;

command: DEFINE declaration expression ';'
  {
    auto func = make_function_definition_node($2, $3);
    func->accept(the_visitor);
    delete func;
  }
  | EXTERN declaration ';'
  {
    ($2)->accept(the_visitor);
    delete ($2);
  }
  | expression ';'
  {
    ($1)->accept(the_visitor);
    delete ($1);
  }
  | ERROR ';'
  {
    yyerror(($1).c_str());
    yyerrok;
  }
  ;

declaration: SYMBOL '(' arguments ')'
  {
    $$ = make_function_declaration_node($1, $3);
  }
  ;

arguments: arguments ',' SYMBOL
  {
    std::swap($$, $1); $$.push_back($3);
  }
  | SYMBOL
  {
    $$.push_back($1);
  }
  | /* empty */
  {
    ;
  }
  ;

expression: expression '+' expression
  {
    $$ = make_binary_expression_node($1, $3, '+');
  }
  | expression '-' expression
  {
    $$ = make_binary_expression_node($1, $3, '-');
  }
  | expression '*' expression
  {
    $$ = make_binary_expression_node($1, $3, '*');
  }
  | expression '/' expression
  {
    $$ = make_binary_expression_node($1, $3, '/');
  }
  | SYMBOL '(' expressions ')'
  {
    $$ = make_call_function_node($1, $3);
  }
  | SYMBOL
  {
    $$ = make_variable_node($1);
  }
  | NUMBER
  {
    $$ = make_number_node($1);
  }
  ;

expressions: expressions ',' expression
  {
    std::swap($$, $1); $$.push_back($3);
  }
  | expression
  {
    $$.push_back($1);
  }
  | /* empty */
  {
    ;
  }
  ;

%%

void yyerror(const char *errmsg)
{
  extern int yylineno;
  fprintf(stderr, "%s at/near line %d\n", errmsg, yylineno);
}