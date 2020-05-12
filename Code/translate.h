#ifndef TRANSLATEH
#define TRANSLATEH 

#include "intercode.h"
#include "symbol.h"
#include"tree.h"

typedef struct arg_list_t arg_list;
typedef struct arglist_t arglist;

struct arg_list_t{
    operand* op;
    arg_list* next;
};

struct arglist_t{
    arg_list* head;
};

int call_flag=0;

void translate_program(node* root);
void translate_extdeflist(node* root, intercodes* codes);
void translate_extdef(node* root, intercodes* codes);
void translate_compst(node* root, intercodes* codes);
void translate_stmtlist(node* root, intercodes* codes);
void translate_deflist(node* root, intercodes* codes);
void translate_def(node* root, intercodes* codes);
void translate_declist(node* root, intercodes* codes);
void translate_dec(node* root, intercodes* codes);
void translate_vardec(node* root, intercodes* codes);

symbol* struct_array_offset(node* root,int *offset,array_list** array_head,intercodes* codes,int *temp);
intercodes* translate_array_struct1(node* root,operand* op);
intercodes* translate_array_struct2(node* root,operand* op);

intercodes* translate_exp(node* root,operand* op);
intercodes* translate_stmt(node* root);
intercodes* translate_cond(node* root,int label1,int label2);
intercodes* translate_args(node* root,arglist** arg_head);

#endif