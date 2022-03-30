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

%token <str> IDENT
%token <str> STR
%token <str> NUM

%token <str> ASSIGN "="
%token <str> EQ "=="
%token <str> NEQ "!="
%token <str> NOT "!"
%token <str> MUL "*"
%token <str> DIV "/"
%token <str> ADD "+"
%token <str> SUB "-"
%token <str> GT ">"
%token <str> LT "<"
%token <str> OR "||"
%token <str> AND "&&"

%token <str> INT "int"
%token <str> FLOAT "float"
%token <str> VOID "void"

%token DEF "def"
%token EXTERN "extern"

%token IF "if"
%token ELSE "else"
%token FOR "for"
%token WHILE "while"
%token RET "return"
%token REF "ref"
%token VAR "&"

%token LPAREN "("
%token RPAREN ")"
%token LBRACE "{"
%token RBRACE "}"
%token COMMA ","
%token COLON ":"
%token SEMICOLON ";"
%token DOT "."

%type <ProgType> program

%left "=";
%left "||";
%left "&&";
%left "==";
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
extern: "extern" type globid '(' ')' ';'
  {
    ;
  }
  | "extern" type globid '(' tdecls ')' ';'
  {
    ;
  }
  ;

/* <function> -> def <type> <globid> ( <vdecls>? ) <block> */
function: "def" type globid '(' ')' block
  {
    ;
  }
  | "def" type globid '(' vdecls ')' block
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
  | "return" ';'
  {
    ;
  }
  | "return" expression ';'
  {
    ;
  }
  | vdecl '=' expression ';'
  {
    ;
  }
  | "while" '(' expression ')' statement
  {
    ;
  }
  | "if" '(' expression ')' statement
  {
    ;
  }
  | "if" '(' expression ')' statement "else" statement
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
  | NUMBER
  {
    ;
  }
  | STRING
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
binop: expression '=' expression
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
  | expression "==" expression
  {
    ;
  }
  | expression "&&" expression
  {
    ;
  }
  | expression "||" expression
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

type: "int"
  {
    ;
  }
  | "float"
  {
    ;
  }
  | "str"
  {
    ;
  }
  | "void"
  {
    ;
  }
  | "ref" type
  {
    ;
  }
  | "noalias" "ref" type
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
