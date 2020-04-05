%{
    #include <stdio.h>
    #include "tree.h"
    //#define YYDEBUG 1
    //int yydebug=1;
    //#define YYSTYPE float
    void yyerror(char* msg) ;
    #include "lex.yy.c"
    extern char eflag;
%}

%union {
  struct node_t* node_type;
}
%locations

%token <node_type> INT
%token <node_type> FLOAT
%token <node_type> ID
%token <node_type> SEMI COMMA
%token <node_type> ASSIGNOP
%token <node_type> RELOP
%token <node_type> PLUS MINUS STAR DIV
%token <node_type> AND OR DOT NOT
%token <node_type> TYPE
%token <node_type> LP RP LB RB LC RC
%token <node_type> STRUCT
%token <node_type> RETURN
%token <node_type> IF ELSE WHILE

%type <node_type> ExtDefList ExtDef ExtDecList
%type <node_type> Specifier StructSpecifier OptTag Tag
%type <node_type> VarDec FunDec VarList ParamDec
%type <node_type> CompSt StmtList Stmt
%type <node_type> DefList Def DecList Dec
%type <node_type> Exp Args

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

Program : ExtDefList{
  root=add_node("Program",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  root->son[0]=$1;
}
;
ExtDefList : ExtDef ExtDefList{
  $$=add_node("ExtDefList",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
|{
  $$=NULL;@$.first_line=yylineno;
}
;
ExtDef : Specifier ExtDecList SEMI{
  $$=add_node("ExtDef",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Specifier SEMI{
  $$=add_node("ExtDef",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
| Specifier FunDec CompSt{
  $$=add_node("ExtDef",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Specifier FunDec SEMI{
  $$=add_node("ExtDef",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| error SEMI{
  $$ = NULL;
}  
;
ExtDecList : VarDec{
  $$=add_node("ExtDecList",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| VarDec COMMA ExtDecList{
  $$=add_node("ExtDecList",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
;

Specifier : TYPE{
  $$=add_node("Specifier",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| StructSpecifier{
  $$=add_node("Specifier",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
;
StructSpecifier : STRUCT OptTag LC DefList RC{
  $$=add_node("StructSpecifier",SYNTAX_TYPE,NULL,5,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
  $$->son[4]=$5;
}
| STRUCT Tag{
  $$=add_node("StructSpecifier",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
;
OptTag : ID{
  $$=add_node("OptTag",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
|{
  $$=NULL;
}
;
Tag : ID{
  $$=add_node("Tag",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
;

VarDec : ID{
  $$=add_node("VarDec",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| VarDec LB INT RB{
  $$=add_node("VarDec",SYNTAX_TYPE,NULL,4,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
}
| VarDec LB error RB {
  $$=NULL;
} 
;
FunDec : ID LP VarList RP{
  $$=add_node("FunDec",SYNTAX_TYPE,NULL,4,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
}
| ID LP RP{
  $$=add_node("FunDec",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| error RP{
  $$=NULL;
}
;
VarList : ParamDec COMMA VarList{
  $$=add_node("VarList",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| ParamDec{
  $$=add_node("VarList",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
;
ParamDec : Specifier VarDec{
  $$=add_node("ParamDec",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
;

CompSt : LC DefList StmtList RC{
  $$=add_node("CompSt",SYNTAX_TYPE,NULL,4,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
}
| error RC{
  $$=NULL;
}
;
StmtList : Stmt StmtList{
  $$=add_node("StmtList",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
| {
  $$=NULL;
}
;
Stmt : Exp SEMI{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
| CompSt{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| RETURN Exp SEMI{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,5,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
  $$->son[4]=$5;
}
| IF LP Exp RP Stmt ELSE Stmt{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,7,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
  $$->son[4]=$5;
  $$->son[5]=$6;
  $$->son[6]=$7;
}
| WHILE LP Exp RP Stmt{
  $$=add_node("Stmt",SYNTAX_TYPE,NULL,5,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
  $$->son[4]=$5;
}
| error SEMI{
  $$=NULL;
}
;

DefList : Def DefList{
  $$=add_node("DefList",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
|{
  $$=NULL;
}
;
Def : Specifier DecList SEMI{
  $$=add_node("Def",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Specifier error SEMI{
  $$=NULL;
}
;
DecList : Dec{
  $$=add_node("DecList",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| Dec COMMA DecList{
  $$=add_node("DecList",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
;
Dec : VarDec{
  $$=add_node("Dec",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| VarDec ASSIGNOP Exp{
  $$=add_node("Dec",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
;

Exp : Exp ASSIGNOP Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp AND Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp OR Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp RELOP Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp PLUS Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp MINUS Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp STAR Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp DIV Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| LP Exp RP{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| MINUS Exp %prec UMINUS{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
| NOT Exp{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,2,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
}
| ID LP Args RP{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,4,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
}
| ID LP RP{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp LB Exp RB{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,4,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
  $$->son[3]=$4;
}
| Exp DOT ID{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| ID{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| INT{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| FLOAT{
  $$=add_node("Exp",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| Exp LB error RB{
  $$=NULL;
}
| LP error RP{
  $$=NULL;
}
| ID LP error RP{
  $$=NULL;
}
;
Args : Exp COMMA Args{
  $$=add_node("Args",SYNTAX_TYPE,NULL,3,@1.first_line,false);
  $$->son[0]=$1;
  $$->son[1]=$2;
  $$->son[2]=$3;
}
| Exp{
  $$=add_node("Args",SYNTAX_TYPE,NULL,1,@1.first_line,false);
  $$->son[0]=$1;
}
| error COMMA Args{
  $$=NULL;
}
;




%%
void yyerror(char* msg) {
    fprintf(stdout, "Error type B at Line %d: %s.\n", yylineno,msg);
    eflag=true;
}