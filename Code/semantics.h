#ifndef SEMANTICSH
#define SEMANTICSH 
#include"tree.h"
#include "symbol.h"

int anonymous_struct;

int semantics_program(node* root);
void semantics_extdeflist(node* root);
void semantics_extdef(node* root);
void semantics_extdeclist(node* root, int type);
int semantics_specifier(node* root,struct_list** struct_head);
struct_list* semantics_structspecifier(node* root);
char* semantics_opttag(node* root);
char* semantics_tag(node* root);
void semantics_vardec(node* root);
void semantics_fundec(node* root);
void semantics_varlist(node* root);
void semantics_paramdec(node* root);
void semantics_compst(node* root);
void semantics_stmtlist(node* root);
void semantics_stmt(node* root);
symbol_list* semantics_deflist(node* root);
symbol_list* semantics_def(node* root);
symbol_list* semantics_declist(node* root,int type,struct_list* struct_head);
void semantics_dec(node* root);
void semantics_exp(node* root);
void semantics_args(node* root);

#endif