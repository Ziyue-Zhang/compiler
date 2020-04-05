#include"semantics.h"

int semantics_program(node* root){
    symbol_init();

    semantics_extdeflist(root->son[0]);

    symbol_print();
    check_func_def();
    return 0;
}

void semantics_extdeflist(node* root){
    if(!root)
        return;
    if(root->num==2){
        semantics_extdef(root->son[0]);
        semantics_extdeflist(root->son[1]);
    }
    else{
        printf("wrong in semantics_extdeflist\n");
    }
}

void semantics_extdef(node* root){

}

void semantics_extdeclist(node* root){

}

void semantics_specifier(node* root){

}

void semantics_structspecifier(node* root){

}

void semantics_opttag(node* root){

}

void semantics_tag(node* root){

}

void semantics_vardec(node* root){

}

void semantics_fundec(node* root){

}

void semantics_varlist(node* root){

}

void semantics_paramdec(node* root){

}

void semantics_compst(node* root){
    semantics_deflist(root->son[1]);
    semantics_stmtlist(root->son[2]);
}

void semantics_stmtlist(node* root){

}

void semantics_stmt(node* root){

}

void semantics_deflist(node* root){

}

void semantics_def(node* root){

}

void semantics_declist(node* root){

}

void semantics_dec(node* root){

}

void semantics_exp(node* root){

}

void semantics_args(node* root){

}