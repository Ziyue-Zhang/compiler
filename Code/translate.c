#include "translate.h"

void translate_program(node* root){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;
    if (root->son[0]) {
        translate_extdeflist(root->son[0], codes);
        intercodes_print(codes);
    }
}
void translate_extdeflist(node* root, intercodes* codes){
    intercode* code=intercode_new(IR_FUNCTION);
    char *name=malloc(5);
    strcpy(name,"main");
    code->func_name=name;
    intercodes_add(codes,code);
}

intercodes* translate_exp(node* root,operand* op){
    return NULL;
}
intercodes* translate_stmt(node* root){

}
intercodes* translate_cond(node* root,int label1,int label2){

}
intercodes* translate_args(node* root,param_list** param_head){

}