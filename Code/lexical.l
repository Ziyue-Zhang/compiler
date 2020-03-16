%{
    #include <stdio.h>
    #include "tree.h"
    #include "syntax.tab.h"
    int char2int8(char* s);
    int char2int16(char* s);
    int yycolumn = 1;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
    char eflag=false;
%}
%option yylineno

digit       [0-9]
letter      [_a-zA-Z]
delim       [ \t\r]
ws          {delim}+

int         0|[1-9]+{digit}*
int8        0[0-7]+
wint8       0({digit}|{letter})+
int16       0[xX][0-9a-fA-F]+
wint16      0[xX]({digit}|{letter})+
wint        {digit}*|{digit}+({digit}|[a-zA-Z])+

float1      {digit}+[\.]{digit}+
wfloat1     {digit}+[\.]|[\.]{digit}+
float2      ([0-9]*[\.][0-9]+|[0-9]+[\.])[eE][-+]?{digit}+
wfloat2     \.[eE][-+]?{digit}*|([0-9]*\.[0-9]+|[0-9]+\.)[eE][-+]?
wfloat3     [0-9]*\.[eE]({digit}|[a-zA-Z])*

id          {letter}({letter}|{digit})*
wid         {digit}+{letter}+({letter}|{digit})*
relop       ">"|"<"|">="|"<="|"=="|"!="
type        "int"|"float"

%%

"/*"[^\*]*[\*]+([^\*/][^\*]*[\*]+)*"/"   {
            int temp=yycolumn;
            int flag=false;
            for(int i=0;i<strlen(yytext);i++){
                temp++;
                if(yytext[i]=='\n'){
                    temp = 1;
                    flag=true;
                }
            }
            if(flag)
                yycolumn=temp;
            /*printf("COMMENT\n");*/}
"/*"        {printf("Error type A at Line %d: Illegal comment \'%s\'.\n",yylineno, yytext);
            eflag=true;}
"*/"        {printf("Error type A at Line %d: Illegal comment \'%s\'.\n",yylineno, yytext);
            eflag=true;} 
"//".*      {/*printf("COMMENT\n");*/}

if          {//printf("IF\n");
            yylval.node_type=add_node("IF",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return IF;}
else        {//printf("ELSE\n"); 
            yylval.node_type=add_node("ELSE",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return ELSE;}
while       {//printf("WHILE\n");
            yylval.node_type=add_node("WHILE",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return WHILE;}
struct      {//printf("STRUCT\n");
            yylval.node_type=add_node("STRUCT",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return STRUCT;}
return      {//printf("RETURN\n"); 
            yylval.node_type=add_node("RETURN",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return RETURN;}
{type}      {//printf("TYPE: %s\n",yytext); 
            yylval.node_type=add_node("TYPE",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return TYPE;}
";"         {//printf("SEMI\n");
            yylval.node_type=add_node("SEMI",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return SEMI;}
","         {//printf("COMMA\n");
            yylval.node_type=add_node("COMMA",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return COMMA;}

{int}       {//printf("INT: %s %d\n",yytext,atoi(yytext));
            yylval.node_type=add_node("INT",INT_TYPE,NULL,0,yylloc.first_line,true);
            yylval.node_type->type_int=atoi(yytext);
            return INT;}
{int8}      {//printf("INT8: %s %d\n",yytext,char2int8(yytext));
            yylval.node_type=add_node("INT",INT_TYPE,NULL,0,yylloc.first_line,true);
            yylval.node_type->type_int=char2int8(yytext);
            return INT;}
{int16}     {//printf("INT16: %s %d\n",yytext,char2int16(yytext));
            yylval.node_type=add_node("INT",INT_TYPE,NULL,0,yylloc.first_line,true);
            yylval.node_type->type_int=char2int16(yytext);
            return INT;}
{wint16}    {printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return INT;}
{wint8}     {printf("Error type A at Line %d: Illegal octal number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return INT;}
{wint}      {printf("Error type A at Line %d: Illegal number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return INT;}

{float1}    {//printf("FLOAT: %s %f\n",yytext,atof(yytext));
            yylval.node_type=add_node("FLOAT",FLOAT_TYPE,NULL,0,yylloc.first_line,true);
            yylval.node_type->type_float=atof(yytext);
            return FLOAT;}
{wfloat1}   {printf("Error type A at Line %d: Illegal floating point number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return FLOAT;}
{float2}    {//printf("FLOAT: %s %f\n",yytext,atof(yytext));
            yylval.node_type=add_node("FLOAT",FLOAT_TYPE,NULL,0,yylloc.first_line,true);
            yylval.node_type->type_float=atof(yytext);
            return FLOAT;}
{wfloat2}   {printf("Error type A at Line %d: Illegal floating point number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return FLOAT;}
{wfloat3}   {printf("Error type A at Line %d: Illegal floating point number \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return FLOAT;}

{id}        {//printf("ID: %s\n",yytext);
            yylval.node_type=add_node("ID",ID_TYPE,yytext,0,yylloc.first_line,true);
            return ID;}
{wid}       {printf("Error type A at Line %d: Wrong ID \'%s\'.\n",yylineno, yytext);
            yylval.node_type = NULL;
            eflag=true;
            return ID;}

"("         {//printf("LP\n");
            yylval.node_type=add_node("LP",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return LP;}
")"         {//printf("RP\n");
            yylval.node_type=add_node("RP",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return RP;}
"["         {//printf("LB\n");
            yylval.node_type=add_node("LB",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return LB;}
"]"         {//printf("RB\n");
            yylval.node_type=add_node("RB",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return RB;}
"{"         {//printf("LC\n");
            yylval.node_type=add_node("LC",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return LC;}
"}"         {//printf("RC\n");
            yylval.node_type=add_node("RC",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return RC;}
"."         {//printf("DOT\n");
            yylval.node_type=add_node("DOT",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return DOT;}
"!"         {//printf("NOT\n");
            yylval.node_type=add_node("NOT",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return NOT;}
"*"         {//printf("STAR\n");
            yylval.node_type=add_node("STAR",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return STAR;}
"/"         {//printf("DIV\n");
            yylval.node_type=add_node("DIV",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return DIV;}
"+"         {//printf("PLUS\n");
            yylval.node_type=add_node("PLUS",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return PLUS;}
"-"         {//printf("MINUS\n");
            yylval.node_type=add_node("MINUS",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return MINUS;}
{relop}     {//printf("RELOP\n");
            yylval.node_type=add_node("RELOP",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return RELOP;}
"&&"        {//printf("AND\n");
            yylval.node_type=add_node("AND",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return AND;}
"||"        {//printf("OR\n");
            yylval.node_type=add_node("OR",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return OR;}
"="         {//printf("ASSIGNOP\n");
            yylval.node_type=add_node("ASSIGNOP",OTHER_TYPE,yytext,0,yylloc.first_line,true);
            return ASSIGNOP;}

{ws}        {/*no action*/}
\n          { yycolumn = 1; }

.           {printf("Error type A at Line %d: Mysterious characters \'%s\'.\n",yylineno, yytext);
            eflag=true;}
%%

int char2int8(char* s){
    int i=0;
    int res=0;
    int temp=0;
    while(s[i]!='\0'){
        temp=s[i]-'0';
        res=res*8+temp;
        i++;
    }
    return res;
}

int char2int16(char* s){
    int i=2;
    int res=0;
    int temp=0;
    while(s[i]!='\0'){
        if(s[i]>='0'&&s[i]<='9'){
            temp=s[i]-'0';
        }   
        else{
            temp=s[i]-'a';
            temp+=10;
        } 
        res=res*16+temp;
        i++;
    }
    return res;
}