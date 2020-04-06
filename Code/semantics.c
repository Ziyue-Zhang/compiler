#include"semantics.h"

int semantics_program(node* root){
    symbol_init();
    anonymous_struct=0;

    semantics_extdeflist(root->son[0]);

    symbol_print();
    check_func_def();
    return 0;
}

void semantics_extdeflist(node* root){
    if(!root)
        return;
    semantics_extdef(root->son[0]);
    semantics_extdeflist(root->son[1]);
}

void semantics_extdef(node* root){
    struct_list* struct_head=NULL;
    int type=semantics_specifier(root->son[0],&struct_head);
    
    if(type==SYMBOL_VOID){
        return;
    }

    if(strcmp(root->son[1]->name,"ExtDecList")==0){
        semantics_extdeclist(root->son[1],type);
    }
    else if(strcmp(root->son[1]->name,"SEMI")==0){
        //do nothing
    }
    else if(strcmp(root->son[1]->name,"FunDec")==0){
        if(strcmp(root->son[2]->name,"CompSt")==0){
            //FUNCTION DEFINE
        }
        else if(strcmp(root->son[2]->name,"SEMI")==0){
            //FUNCTION DEFINE
        }
        else{
            assert(0);
        }
    }
    else{
        assert(0);
    }

}

void semantics_extdeclist(node* root, int type){
    
}

int semantics_specifier(node* root,struct_list** struct_head){
    if(strcmp(root->son[0]->name,"TYPE")==0){
        if(strcmp(root->son[0]->type_char,"int")==0){
            *struct_head=NULL;
            return SYMBOL_INT;
        }
        else if(strcmp(root->son[0]->type_char,"float")==0){
            *struct_head=NULL;
            return SYMBOL_FLOAT;
        }
    }
    else if(strcmp(root->son[0]->name,"StructSpecifier")==0){
        *struct_head=semantics_structspecifier(root->son[0]);
        if(struct_head)
            return SYMBOL_STRUCT;
        else
            return SYMBOL_VOID;        
    }
    else{
        assert(0);
    }
    return 0;
}

struct_list* semantics_structspecifier(node* root){
    struct_list* struct_head=malloc(sizeof(struct_list));
    if(root->num==5){
        char *name;
        if(root->son[1]){
            name=semantics_opttag(root->son[1]);
        }
        else{
            name=malloc(80);
            snprintf(name,80,"%d_anonymous",anonymous_struct);
            anonymous_struct++;
        }
        field_push();
        struct_head->list=semantics_deflist(root->son[3]);
        field_pop();
        symbol* new_entry=add_entry(SYMBOL_STRUCT,name,0,0,1,0,root->lineno);
        new_entry->array_head=NULL;
        new_entry->param_head=NULL;
        new_entry->struct_head=struct_head;
        int temp=add_symbol(new_entry,0);
        if(temp==0){
            free_symbol(new_entry);
            return NULL;
        }
        else{
            return struct_head;
        }
    }
    else if(root->num==2){
        char*name = semantics_tag(root->son[1]);
        symbol* entry=find_symbol(name);
        if(!entry){
            printf("Error type 17 at Line %d: Undefined structure \"%s\".",root->son[1]->lineno,name);
            return NULL;
        }
        return entry->struct_head;
    }
    else{
        assert(0);
    }
}

char* semantics_opttag(node* root){
    return root->son[0]->type_char;
}

char* semantics_tag(node* root){
    return root->son[0]->type_char;
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

symbol_list* semantics_deflist(node* root){
    if(!root)
        return NULL;
    symbol_list*entry=semantics_def(root->son[0]);
    symbol_list*entry_next=semantics_deflist(root->son[1]);
    entry->next=entry_next;
    return entry;
}

symbol_list* semantics_def(node* root){
    struct_list* struct_head=NULL;
    int type=semantics_specifier(root->son[0],&struct_head);
    symbol_list* list=semantics_declist(root->son[1],type,struct_head);
    return NULL;
}

symbol_list* semantics_declist(node* root,int type,struct_list* struct_head){
    
}

void semantics_dec(node* root){

}

void semantics_exp(node* root){

}

void semantics_args(node* root){

}