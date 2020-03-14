%{
    #include <stdio.h>
    //#define YYSTYPE float
    #include "lex.yy.c"
%}

%union {
  int type_int;
  float type_float;
}
%locations

%token <type_int> INT
%token <type_float> FLOAT
%token ADD SUB MUL DIV

%type <type_float> Exp Factor Term
%%
Calc : /* empty */
| Exp { printf("= %f\n", $1); }
;
Exp : Factor
| Exp ADD Factor { $$ = $1 + $3; }
| Exp SUB Factor { $$ = $1 - $3; }
;
Factor : Term
| Factor MUL Term { $$ = $1 * $3; }
| Factor DIV Term { $$ = $1 / $3; }
;
Term : INT { $$ = $1; }
| FLOAT { $$ = $1; }
;
%%
yyerror(char* msg) {
    fprintf(stderr, "error: %s\n", msg);
}