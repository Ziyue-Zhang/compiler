%{
    #include <stdio.h>
    //#define YYDEBUG 1
    //int yydebug=1;
    //#define YYSTYPE float
    #include "lex.yy.c"
%}

%union {
  int type_int;
  float type_float;
}
%locations

%token INT
%token FLOAT
%token ID
%token SEMI COMMA
%token ASSIGNOP
%token RELOP
%token PLUS MINUS STAR DIV
%token AND OR DOT NOT
%token TYPE
%token LP RP LB RB LC RC
%token STRUCT
%token RETURN
%token IF ELSE WHILE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT 
%left LP RP LB RB DOT 

%%

Program : ExtDefList
;
ExtDefList : ExtDef ExtDefList
| 
;
ExtDef : Specifier ExtDecList SEMI
| Specifier SEMI
| Specifier FunDec CompSt
| error SEMI  

;
ExtDecList : VarDec
| VarDec COMMA ExtDecList
;

Specifier : TYPE
| StructSpecifier
;
StructSpecifier : STRUCT OptTag LC DefList RC
| STRUCT Tag
;
OptTag : ID
|
;
Tag : ID
;

VarDec : ID
| VarDec LB INT RB
| VarDec LB error RB  
;
FunDec : ID LP VarList RP
| ID LP RP
| error RP
;
VarList : ParamDec COMMA VarList
| ParamDec
;
ParamDec : Specifier VarDec
;

CompSt : LC DefList StmtList RC
| error RC
;
StmtList : Stmt StmtList
| 
;
Stmt : Exp SEMI
| CompSt
| RETURN Exp SEMI
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE
| IF LP Exp RP Stmt ELSE Stmt
| WHILE LP Exp RP Stmt
| error SEMI
;

DefList : Def DefList
|
;
Def : Specifier DecList SEMI
| Specifier error SEMI
;
DecList : Dec
| Dec COMMA DecList
;
Dec : VarDec
| VarDec ASSIGNOP Exp
;

Exp : Exp ASSIGNOP Exp
| Exp AND Exp
| Exp OR Exp
| Exp RELOP Exp
| Exp PLUS Exp
| Exp MINUS Exp
| Exp STAR Exp
| Exp DIV Exp
| LP Exp RP
| MINUS Exp %prec UMINUS
| NOT Exp
| ID LP Args RP
| ID LP RP
| Exp LB Exp RB
| Exp DOT ID
| ID
| INT
| FLOAT
| Exp LB error RB
| LP error RP
| ID LP error RP
;
Args : Exp COMMA Args
| Exp
| error COMMA Args
;




%%
void yyerror(char* msg) {
    fprintf(stdout, "Error type B at Line %d: %s.\n", yylineno,msg);
}