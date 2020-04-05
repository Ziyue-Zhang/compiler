#ifndef SEMANTICSH
#define SEMANTICSH 
#include"tree.h"
#include "symbol.h"


int semantics_program(node* root);
void semantics_extdeflist(node* root);
void semantics_extdef(node* root);
void semantics_extdeclist(node* root);
void semantics_specifier(node* root);
void semantics_structspecifier(node* root);
void semantics_opttag(node* root);
void semantics_tag(node* root);
void semantics_vardec(node* root);
void semantics_fundec(node* root);
void semantics_varlist(node* root);
void semantics_paramdec(node* root);
void semantics_compst(node* root);
void semantics_stmtlist(node* root);
void semantics_stmt(node* root);
void semantics_deflist(node* root);
void semantics_def(node* root);
void semantics_declist(node* root);
void semantics_dec(node* root);
void semantics_exp(node* root);
void semantics_args(node* root);

#endif