#include <stdio.h>
#include "syntax.tab.h"
#include "semantics.h"
#include "tree.h"
#include "intercode.h"
#include "translate.h"

extern int yylex (void);
extern void yyrestart (FILE *input_file);
extern FILE* yyin;
extern int yylineno;
extern char eflag;

/*int main() {
    yyparse();
}*/
int main(int argc, char** argv) {
    if (argc <= 1) 
        return 1;
    FILE* f = fopen(argv[1], "r");
    output = fopen("1.out", "w+");
    if (!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yylineno = 1;
    eflag=false;
    root=NULL;
    space_num=0;
    yyparse();
    if(eflag==false){
        //print_tree(root);
        semantics_program(root);
        translate_program(root);
        fclose(output);
    }
    return 0;
}

/*int main(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    printf("%8d\n", yylineno);
    return 0;
}*/

/*int main(int argc, char** argv){
    int i, totchars = 0, to   twords = 0, totlines = 0;
    if (argc < 2) {
        yylex();
        printf("%8d%8d%8d\n", lines, words, chars);
        return 0;
    }
    for (i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            return 1;
        }
        yyrestart(f);
        yylex();
        fclose(f);
        printf("%8d%8d%8d %s\n", lines, words, chars, argv[i]);
        totchars += chars; chars = 0;
        totwords += words; words = 0;
        totlines += lines; lines = 0;
    }
    if (argc > 1)
        printf("%8d%8d%8d total\n", totlines, totwords, totchars);
    return 0;
}*/
