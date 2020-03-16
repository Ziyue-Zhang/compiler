#include"tree.h"

node* add_node(char *name, int type, const char* ytext, int num, int lineno, int leaf){
    node* new_node=(node*)malloc(sizeof(node));
    strcpy(new_node->name,name);
    new_node->type=type;
    if(type==OTHER_TYPE||type==ID_TYPE){
        new_node->type_char=(char*)malloc(strlen(ytext) + 1);
        strcpy(new_node->type_char,ytext);
        new_node->type_char[strlen(ytext)]='\0';
    }
    new_node->num=num;
    new_node->lineno=lineno;
    new_node->leaf=leaf;
    return new_node;
}

void print_tree(){
    
}