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
%token EXT "extern"

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

%type <progType> primary

%%

%start primary;

expression:
  {
    ;
  }
  | '(' expression ')'
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
  ;

%%
