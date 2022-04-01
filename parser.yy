%code top
{
  #include <assert.h>
  #include <ctype.h>  /* isdigit. */
  #include <stdio.h>  /* printf. */
  #include <stdlib.h> /* abort. */
  #include <string.h> /* strcmp. */

  int yylex (void);
  void yyerror (char const *);
}

/* Generate the parser description file (parser.output).  */
%verbose

/* Nice error messages with details. */
%define parse.error detailed

/* Enable run-time traces (yydebug).  */
%define parse.trace

/* Generate YYSTYPE from the types used in %token and %type.  */
%define api.value.type union

%define api.token.prefix {}

%token IDENTITY
%token CONST_STRING
%token CONST_INTEGER
%token CONST_FLOAT

%token EXTERN "extern"
%token DEFINE "def"
%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token RETURN "return"
%token NOALIAS "noalias"
%token REF "ref"

%token VOID "void"
%token TYPE_STRING "str"
%token TYPE_INTEGER "int"
%token TYPE_FLOAT "float"

%token ASSIGN "="
%token EQ "=="
%token OR "||"
%token AND "&&"

%type <ProgType> program

%left ASSIGN;
%left OR;
%left AND;
%left EQ;
%left ">" "<";
%left "+" "-";
%left "*" "/";

%start program;

%%

/* <program> -> <extern>* <function>+ */
program: extern_list func_list
  {
    ;
  }
  ;

/* <extern>* */
extern_list: /* empty */
  {
    ;
  }
  | extern extern_list
  {
    ;
  }
  ;

/* <function>+ */
func_list: function
  {
    ;
  }
  | function func_list
  {
    ;
  }
  ;

/* <extern> -> extern <type> <globid> ( <tdecls>? ) ; */
extern: EXTERN type globid '(' ')' ';'
  {
    ;
  }
  | EXTERN type globid '(' tdecls ')' ';'
  {
    ;
  }
  ;

/* <function> -> def <type> <globid> ( <vdecls>? ) <block> */
function: DEFINE type globid '(' ')' block
  {
    ;
  }
  | DEFINE type globid '(' vdecls ')' block
  {
    ;
  }
  ;

/* <block> -> { statement* } */
block: '{' stmt_list '}'
  {
    ;
  }
  ;

/* statement* */
stmt_list: /* empty */
  {
    ;
  }
  | statement stmt_list
  {
    ;
  }
  ;

/* <statement> */
statement: expression ';'
  {
    ;
  }
  | RETURN ';'
  {
    ;
  }
  | RETURN expression ';'
  {
    ;
  }
  | vdecl ASSIGN expression ';'
  {
    ;
  }
  | WHILE '(' expression ')' statement
  {
    ;
  }
  | IF '(' expression ')' statement
  {
    ;
  }
  | IF '(' expression ')' statement ELSE statement
  {
    ;
  }
  | block
  {
    ;
  }
  ;

/* <expr_list> -> <expression> | <expression> , <expr_list> */
expr_list: expression
  {
    ;
  }
  | expression ',' expr_list
  {
    ;
  }
  ;

/* <expression> */
expression: '(' expression ')'
  {
    ;
  }
  | globid '(' ')'
  {
    ;
  }
  | globid '(' expr_list ')'
  {
    ;
  }
  | binop
  {
    ;
  }
  | unrop
  {
    ;
  }
  | variable
  {
    ;
  }
  | CONST_INTEGER
  {
    ;
  }
  | CONST_FLOAT
  {
    ;
  }
  | CONST_STRING
  {
    ;
  }
  ;

/* <unary operation> */
unrop: '!' expression
  {
    ;
  }
  | '-' expression
  {
    ;
  }
  ;

/* <binary operation> */
binop: expression ASSIGN expression
  {
    ;
  }
  | expression '+' expression
  {
    ;
  }
  | expression '-' expression
  {
    ;
  }
  | expression '*' expression
  {
    ;
  }
  | expression '/' expression
  {
    ;
  }
  | expression '<' expression
  {
    ;
  }
  | expression '>' expression
  {
    ;
  }
  | expression EQ expression
  {
    ;
  }
  | expression AND expression
  {
    ;
  }
  | expression OR expression
  {
    ;
  }
  ;

variable: '$' IDENTITY
  {
    ;
  }
  ;

globid: IDENTITY
  {
    ;
  }
  ;

type: TYPE_INTEGER
  {
    ;
  }
  | TYPE_FLOAT
  {
    ;
  }
  | TYPE_STRING
  {
    ;
  }
  | VOID
  {
    ;
  }
  | REF type
  {
    ;
  }
  | NOALIAS REF type
  {
    ;
  }
  ;

tdecls: type
  {
    ;
  }
  | type ',' tdecls
  {
    ;
  }
  ;

vdecls: vdecl
  {
    ;
  }
  | vdecl ',' vdecls
  {
    ;
  }
  ;

vdecl: type variable
  {
    ;
  }
  ;

%%
