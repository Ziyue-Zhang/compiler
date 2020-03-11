%{
    #include <stdio.h>
    #include "syntax.tab.h"
    int char2int8(char* s);
    int char2int16(char* s);
%}
%option yylineno

digit       [0-9]
letter      [_a-zA-Z]
delim       [ \t\r\n]
ws          {delim}+

int         0|[1-9]+{digit}*
int8        0[0-7]+
wint8       0({digit}|{letter})+
int16       0[xX][0-9a-fA-F]+
wint16      0[xX]({digit}|{letter})+

float1      {digit}+[\.]{digit}+
wfloat1     {digit}+[\.]|[\.]{digit}+
float2      ([0-9]*\.[0-9]+|[0-9]+\.)[eE][-+]?{digit}+
wfloat2     \.[eE][-+]?{digit}*|([0-9]*\.[0-9]+|[0-9]+\.)[eE][-+]?

id          {letter}({letter}|{digit})*
wid         {digit}+{letter}+({letter}|{digit})*
relop       ">"|"<"|">="|"<="|"=="|"!="
type        "int"|"float"

%%

{ws}        {/*no action*/}
"/*"[^\*]*[\*]+([^\*/][^\*]*[\*]+)*"/"      {printf("COMMENT\n");}
"/*"        {printf("Wrong comment at Line %d: \'%s\'\n",yylineno, yytext);}
"*/"        {printf("Wrong comment at Line %d: \'%s\'\n",yylineno, yytext);} 
"//".*      {printf("COMMENT\n");}

if          {printf("IF\n");}
else        {printf("ELSE\n");}
while       {printf("WHILE\n");}
struct      {printf("STRUCT\n");}
return      {printf("RETURN\n");}
{type}      {printf("TYPE: %s\n",yytext);}
";"         {printf("SEMI\n");}
","         {printf("COMMA\n");}

{int}       {printf("INT: %s %d\n",yytext,atoi(yytext));
            yylval=atoi(yytext);
            return INT;}
{int8}      {printf("INT8: %s %d\n",yytext,char2int8(yytext));
            yylval=char2int8(yytext);
            return INT;}
{int16}     {printf("INT16: %s %d\n",yytext,char2int16(yytext));
            yylval=char2int16(yytext);
            return INT;}
{wint16}    {printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'\n",yylineno, yytext);}
{wint8}     {printf("Error type A at Line %d: Illegal octal number \'%s\'\n",yylineno, yytext);}


{float1}    {printf("FLOAT: %s %f\n",yytext,atof(yytext));
            yylval=atof(yytext);
            return FLOAT;}
{wfloat1}   {printf("Error type A at Line %d: Illegal floating point number \'%s\'\n",yylineno, yytext);}
{float2}    {printf("FLOAT: %s %f\n",yytext,atof(yytext));
            yylval=atof(yytext);
            return FLOAT;}
{wfloat2}   {printf("Error type A at Line %d: Illegal floating point number \'%s\'\n",yylineno, yytext);}


{id}        {printf("ID: %s\n",yytext);}
{wid}       {printf("Error type A at Line %d: Wrong ID \'%s\'\n",yylineno, yytext);}

"("         {printf("LP\n");}
")"         {printf("RP\n");}
"["         {printf("LB\n");}
"]"         {printf("RB\n");}
"{"         {printf("LC\n");}
"}"         {printf("RC\n");}
"."         {printf("DOT\n");}
"!"         {printf("NOT\n");}
"*"         {printf("STAR\n");return MUL;}
"/"         {printf("DIV\n");return DIV;}
"+"         {printf("PLUS\n");return ADD;}
"-"         {printf("MINUS\n");return SUB;}
{relop}     {printf("RELOP\n");}
"&&"        {printf("AND\n");}
"||"        {printf("OR\n");}
"="         {printf("ASSIGNOP\n");}

.           {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
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