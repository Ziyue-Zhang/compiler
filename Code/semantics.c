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
        return;         //wrong struct
    }

    if(strcmp(root->son[1]->name,"ExtDecList")==0){
        semantics_extdeclist(root->son[1],type, struct_head);
    }
    else if(strcmp(root->son[1]->name,"SEMI")==0){
        //do nothing
    }
    else if(strcmp(root->son[1]->name,"FunDec")==0){
        if(strcmp(root->son[2]->name,"CompSt")==0){
            //FUNCTION DEFINE
        }
        else if(strcmp(root->son[2]->name,"SEMI")==0){
            //FUNCTION DECLEAR
        }
        else{
            assert(0);
        }
    }
    else{
        assert(0);
    }

}

void semantics_extdeclist(node* root, int type, struct_list* struct_head){
    
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
        struct_head->list=semantics_deflist(root->son[3],1);
        field_pop();
        symbol* new_entry=add_entry(SYMBOL_STRUCT,name,0,0,1,0,root->lineno);
        new_entry->dim=0;
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
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",root->son[1]->lineno,name);
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

symbol* semantics_vardec(node* root,int type,struct_list* struct_head){
    if(root->num==1){
        symbol* entry=add_entry(type,root->son[0]->type_char,0,0,0,0,root->lineno);
        entry->dim=0;
        entry->array_head=NULL;
        entry->param_head=NULL;
        entry->struct_head=struct_head;
        return entry;
    }
    else if(root->num==4){
        symbol* entry=add_entry(type,"",1,0,0,0,root->lineno);
        entry->array_head=NULL;
        entry->param_head=NULL;
        entry->struct_head=struct_head;
        symbol* pre_entry=semantics_vardec(root->son[0],type,struct_head);
        entry->name=pre_entry->name;
        entry->dim=pre_entry->dim+1;
        return entry;        
    }
    else{
        assert(0);
    }
}

void semantics_fundec(node* root){

}

void semantics_varlist(node* root){

}

void semantics_paramdec(node* root){

}

void semantics_compst(node* root){
    //semantics_deflist(root->son[1]);
    //semantics_stmtlist(root->son[2]);
}

void semantics_stmtlist(node* root){

}

void semantics_stmt(node* root){

}

symbol_list* semantics_deflist(node* root,int struct_entry){
    if(!root)
        return NULL;
    int flag=0;
    symbol_list*entry=semantics_def(root->son[0],&flag,struct_entry);
    symbol_list*entry_next=semantics_deflist(root->son[1],struct_entry);
    if(flag==0||!entry){
        return entry_next;
    }
    else{
        entry->next=entry_next;
        return entry;
    }
}

symbol_list* semantics_def(node* root,int *flag,int struct_entry){
    struct_list* struct_head=NULL;
    int type=semantics_specifier(root->son[0],&struct_head);
    if(type==SYMBOL_VOID){
        flag=0;
        return NULL;
    }
    symbol_list* list=semantics_declist(root->son[1],type,struct_head,struct_entry);
    return list;
}

symbol_list* semantics_declist(node* root,int type,struct_list* struct_head,int struct_entry){
    if(root->num==1){
        symbol* entry=semantics_dec(root->son[0],type,struct_head,struct_entry);
        if(!entry){
            return NULL;
        }
        int temp=add_symbol(entry,struct_entry);
        if(temp==0){
            free_symbol(entry);
            return NULL;
        }
        else{
            symbol_list* symbol_list_head=malloc(sizeof(symbol_list));
            symbol_list_head->next=NULL;
            symbol_list_head->entry=entry;
            return symbol_list_head;
        }
    }
    else if(root->num==3){
        symbol* entry=semantics_dec(root->son[0],type,struct_head,struct_entry);
        if(!entry){
            return semantics_declist(root->son[2],type,struct_head,struct_entry);
        }
        int temp=add_symbol(entry,struct_entry);
        if(temp==0){
            free_symbol(entry);
            return semantics_declist(root->son[2],type,struct_head,struct_entry);
        }
        else{
            symbol_list* symbol_list_head=malloc(sizeof(symbol_list));
            symbol_list_head->entry=entry;
            symbol_list* symbol_list_next=semantics_declist(root->son[2],type,struct_head,struct_entry);
            symbol_list_head->next=symbol_list_next;
            return symbol_list_head;
        }
    }
    else{

    }
}

symbol* semantics_dec(node* root,int type,struct_list* struct_head,int struct_entry){
    if(struct_entry==1){
        if(root->num==1){
            symbol* entry=semantics_vardec(root->son[0],type,struct_head);
            return entry;
        }
        else if(root->num==3){
            printf("Error type 15 at Line %d: assign in struct.\n", root->lineno);
            return NULL;
        }
        else{
            assert(0);
        }
    }
    return NULL;
}

void semantics_exp(node* root){

}

void semantics_args(node* root){

}