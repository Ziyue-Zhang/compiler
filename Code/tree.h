#ifndef TREEH
#define TREEH 

#define true 1
#define false 0

#define INT_TYPE 0
#define FLOAT_TYPE 1
#define ID_TYPE 2
#define SYNTAX_TYPE 3
#define OTHER_TYPE 4
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct node_t node;

struct node_t{
    char name[20];
    int type;
    union{
        int type_int;
        float type_float;
        char* type_char;
    };
    struct node_t* son[7];
    int num;
    int lineno;
    int leaf;
};

node* root;
int space_num;

node* add_node(char *name, int type, const char* ytext, int num, int lineno, int leaf);
void print_tree(node* p);
#endif