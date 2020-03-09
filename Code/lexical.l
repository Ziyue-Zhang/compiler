%{
    #include <stdio.h>
%}
%option yylineno

digit [0-9]
letter [_a-zA-Z]
delim[ \t\r\n]
ws {delim}+
int {digit}+
id {letter}({letter}|{digit})*
relop ">"|"<"|">="|"<="|"=="|"!="
type "int"|"float"

%%

{ws}    {/*no action*/}
"/*"[^\*]*[\*]+([^\*/][^\*]*[\*]+)*"/"    {printf("COMMENT\n");}
"/*" {printf("Wrong comment at Line %d: \'%s\'\n",yylineno, yytext);}
"*/" {printf("Wrong comment at Line %d: \'%s\'\n",yylineno, yytext);} 
"//".*  {printf("COMMENT\n");}

if      {printf("IF\n");}
else    {printf("ELSE\n");}
while   {printf("WHILE\n");}
struct  {printf("STRUCT\n");}
return  {printf("RETURN\n");}
{type}  {printf("TYPE:%s\n",yytext);}
";"     {printf("SEMI\n");}
","     {printf("COMMA\n");}

{int}   {printf("INT:%d\n",atoi(yytext));}
{id}    {printf("ID:%s\n",yytext);}

"("     {printf("LP\n");}
")"     {printf("RP\n");}
"["     {printf("LB\n");}
"]"     {printf("RB\n");}
"{"     {printf("LC\n");}
"}"     {printf("RC\n");}
"."     {printf("DOT\n");}
"!"     {printf("NOT\n");}
"*"     {printf("STAR\n");}
"/"     {printf("DIV\n");}
"+"     {printf("PLUS\n");}
"-"     {printf("MINUS\n");}
{relop} {printf("RELOP\n");}
"&&"    {printf("AND\n");}
"||"    {printf("OR\n");}
"="     {printf("ASSIGNOP\n");}

. {printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno, yytext);}
%%