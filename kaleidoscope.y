%define parse.error verbose
%define api.token.prefix {}

%code requires // *.hh
{
#include "ast_node.hh"
}

%code // *.cc
{
#include <cstdio>
#include "utility.hh"
#include "visitor.hh"
#include "kal.parser.gen.hh"
void yyerror (char const *);
int yylex ();
}

%token EXTERN "extern"
%token DEFINE "def"

%union {
  ast_node* node;
  const char* str;
  function_declaration_node* decl;
  double_linked_list_node<ast_node*>* nlist;
  double_linked_list_node<variable_node*>* vlist;
}

%token <str> NUMBER
%token <str> SYMBOL
%token <str> ERROR

%type <node> expression
%type <decl> declaration
%type <nlist> expressions
%type <vlist> arguments

%left '+' '-';
%left '*' '/';

%start program;

%%

program: program command
  {
    ;
  }
  | /* empty */
  {
    ;
  }
  ;

command: EXTERN declaration ';'
  {
    $2->accept(get_the_visitor());
    delete $2;
  }
  | DEFINE declaration expression ';'
  {
    auto* func = make_function_definition_node($2, $3);
    func->accept(get_the_visitor());
    delete func;
  }
  | expression ';'
  {
    auto* decl = make_function_declaration_node(make_c_str(""), nullptr);
    auto* func = make_function_definition_node(decl, $1);
    func->accept(get_the_visitor());
    delete func;
  }
  | ERROR ';'
  {
    yyerror($1);
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
    $$ = $1; append($$, make_variable_node($3));
  }
  | SYMBOL
  {
    $$ = make_double_linked_list_node<variable_node*>(make_variable_node($1));
  }
  | /* empty */
  {
    $$ = nullptr;
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
    $$ = $1; append($$, $3);
  }
  | expression
  {
    $$ = make_double_linked_list_node<ast_node*>($1);
  }
  | /* empty */
  {
    $$ = nullptr;
  }
  ;

%%

void yyerror(const char *errmsg)
{
  extern int yylineno;
  extern char* yytext;
  fprintf(stderr, "[ERROR] Token \"%s\" at/near line %d: %s\n", yytext, yylineno, errmsg);
}