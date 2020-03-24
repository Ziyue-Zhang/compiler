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

void print_tree(node* p){
    for(int i=0;i<space_num;i++){
        printf(" ");
    }
    printf("%s (%d)\n",p->name,p->lineno);
    for(int i=0;i<p->num;i++){
        space_num+=2;
        if(p->son[i]!=NULL){
            if(p->son[i]->leaf==false){
                print_tree(p->son[i]);
            }
            else{
                if(p->son[i]->type==INT_TYPE){
                    for(int j=0;j<space_num;j++){
                        printf(" ");
                    }
                    printf("%s: %u\n", p->son[i]->name, p->son[i]->type_int);
                }
                else if(p->son[i]->type==FLOAT_TYPE){
                    for(int j=0;j<space_num;j++){
                        printf(" ");
                    }
                    printf("%s: %f\n", p->son[i]->name, p->son[i]->type_float);
                }
                else if(p->son[i]->type==ID_TYPE){
                    for(int j=0;j<space_num;j++){
                        printf(" ");
                    }
                    printf("%s: %s\n", p->son[i]->name, p->son[i]->type_char);
                }
                else{
                    for(int j=0;j<space_num;j++){
                        printf(" ");
                    }
                    printf("%s\n", p->son[i]->name);
                }
            }
        }
        space_num-=2;
    }
}