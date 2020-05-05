#ifndef TRANSLATEH
#define TRANSLATEH 

#include "intercode.h"
#include "symbol.h"
#include"tree.h"

void translate_program(node* root);
void translate_extdeflist(node* root, intercodes* code);
intercodes* translate_exp(node* root,operand* op);
intercodes* translate_stmt(node* root);
intercodes* translate_cond(node* root,int label1,int label2);
intercodes* translate_args(node* root,param_list** param_head);

#endif