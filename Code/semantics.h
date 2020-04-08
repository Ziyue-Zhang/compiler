#ifndef SEMANTICSH
#define SEMANTICSH 
#include"tree.h"
#include "symbol.h"

int anonymous_struct;
symbol* error_entry;
symbol* int_entry;
symbol* float_entry;

int semantics_program(node* root);
void semantics_extdeflist(node* root);
void semantics_extdef(node* root);
void semantics_extdeclist(node* root, int type, struct_list* struct_head);
int semantics_specifier(node* root,struct_list** struct_head);
struct_list* semantics_structspecifier(node* root);
char* semantics_opttag(node* root);
char* semantics_tag(node* root);
symbol* semantics_vardec(node* root,int type,struct_list* struct_head);
symbol* semantics_fundec(node* root,int type,struct_list* struct_head,int func_def);
void semantics_varlist(node* root, param_list** param_head);
symbol* semantics_paramdec(node* root);
void semantics_compst(node* root);
void semantics_stmtlist(node* root);
void semantics_stmt(node* root);
symbol_list* semantics_deflist(node* root,int struct_entry);
symbol_list* semantics_def(node* root,int *flag,int struct_entry);
symbol_list* semantics_declist(node* root,int type,struct_list* struct_head,int struct_entry);
symbol* semantics_dec(node* root,int type,struct_list* struct_head,int struct_entry);
symbol* semantics_exp(node* root);
void semantics_args(node* root);

#endif