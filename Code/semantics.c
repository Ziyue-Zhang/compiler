#include"semantics.h"

int semantics_program(node* root){
    symbol_init();
    anonymous_struct=0;
    error_entry=add_entry(SYMBOL_VOID,"",0,0,0,0,0);
    int_entry=add_entry(SYMBOL_INT,"",0,0,0,0,0);
    float_entry=add_entry(SYMBOL_FLOAT,"",0,0,0,0,0);

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

    if(root->num==2){
        //have defined a struct already
    }
    else if(root->num==3){
        if(strcmp(root->son[1]->name,"ExtDecList")==0){
            semantics_extdeclist(root->son[1],type,struct_head);
        }
        else if(strcmp(root->son[1]->name,"FunDec")==0){
            if(strcmp(root->son[2]->name,"CompSt")==0){
            //FUNCTION DEFINE
                symbol* entry=semantics_fundec(root->son[1],type,struct_head,1);
                int temp=add_symbol(entry,0);
                if(temp==0){
                    free_symbol(entry);
                    return;
                }
                field_push();
                symbol_list* params=entry->param_head->list;
                while(params){
                    add_symbol(params->entry,0);
                    printf("%s\n",params->entry->name);
                    params=params->next;
                }
                semantics_compst(root->son[2]);
                field_pop();
            }
            else if(strcmp(root->son[2]->name,"SEMI")==0){
            //FUNCTION DECLEAR
                symbol* entry=semantics_fundec(root->son[1],type,struct_head,0);
                add_symbol(entry,0);
            }
            else{
                assert(0);
            }
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
    if(root->num==1){
        symbol* entry=semantics_vardec(root->son[0],type,struct_head);
        int temp=add_symbol(entry,0);
        if(temp==0){
            free_symbol(entry);
        }
    }
    else if(root->num==3){
        symbol* entry=semantics_vardec(root->son[0],type,struct_head);
        int temp=add_symbol(entry,0);
        if(temp==0){
            free_symbol(entry);
        }
        semantics_extdeclist(root->son[2],type,struct_head);
    }
    else{
        assert(0);
    }
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
        struct_list* struct_head=malloc(sizeof(struct_list));
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
        if(!entry||entry->type!=SYMBOL_STRUCT){
            printf("Error type 17 at Line %d: Undefined structure \"%s\".\n",root->lineno,name);
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

symbol* semantics_fundec(node* root,int type,struct_list* struct_head,int func_def){
    symbol* entry=add_entry(type,root->son[0]->type_char,0,1,0,func_def,root->lineno);
    param_list* param_head=malloc(sizeof(param_list));
    param_head->param_num=0;
    param_head->list=NULL;
    entry->array_head=NULL;
    entry->param_head=param_head;
    entry->struct_head=struct_head;
    if(root->num==4){
        field_push();
        semantics_varlist(root->son[2],&param_head);
        field_pop();
        entry->param_head=param_head;
        return entry;
    }
    else if(root->num=3){
        return entry;
    }
    else{
        assert(0);
    }
}

void semantics_varlist(node* root, param_list** param_head){
    if(root->num==3){
        symbol* entry=semantics_paramdec(root->son[0]);
        if(entry){
            symbol_list * list_head=malloc(sizeof(symbol_list));
            list_head->entry=entry;
            list_head->next=(*param_head)->list;
            (*param_head)->list=list_head;
            (*param_head)->param_num=(*param_head)->param_num+1;
        }
        semantics_varlist(root->son[2],param_head);
    }
    else if(root->num==1){
        symbol* entry=semantics_paramdec(root->son[0]);
        if(entry){
            symbol_list * list_head=malloc(sizeof(symbol_list));
            list_head->entry=entry;
            list_head->next=(*param_head)->list;
            (*param_head)->list=list_head;
            (*param_head)->param_num=(*param_head)->param_num+1;
        }
    }
    else{
        assert(0);
    }
}

symbol* semantics_paramdec(node* root){
    struct_list* struct_head=NULL;
    int type=semantics_specifier(root->son[0],&struct_head);
    if(type==SYMBOL_VOID)
        return NULL;
    symbol* entry=semantics_vardec(root->son[1],type,struct_head);
    int temp=add_symbol(entry,0);
    if(temp==0){
        free_symbol(entry);
        return NULL;
    }
    else{
        return entry;
    }    
}

void semantics_compst(node* root){
    semantics_deflist(root->son[1],0);
    semantics_stmtlist(root->son[2]);
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
    else if(struct_entry==0){
        if(root->num==1){
            symbol* entry=semantics_vardec(root->son[0],type,struct_head);
            return entry;
        }
        else if(root->num==3){
            symbol* entry=semantics_vardec(root->son[0],type,struct_head);
            symbol* entry2=semantics_exp(root->son[2]);
            int temp=same_type(entry,entry2);
            if(temp==1){
                return entry;
            }
            else{
                printf("Error type 5 at Line %d: Type mismatched for assignment.\n", root->lineno);
                return NULL;
            }
        }
        else{
            assert(0);
        }
    }
    else{
        assert(0);
    }
}

symbol* semantics_exp(node* root){
    return NULL;
    if(root->num==3){
        if(strcmp(root->son[1]->name,"ASSIGNOP")==0){

        }
        else if(strcmp(root->son[1]->name,"AND")==0){

        }
        else if(strcmp(root->son[1]->name,"OR")==0){

        }
        else if(strcmp(root->son[1]->name,"RELOP")==0){

        }
        else if(strcmp(root->son[1]->name,"PLUS")==0){

        }
        else if(strcmp(root->son[1]->name,"MINUS")==0){

        }
        else if(strcmp(root->son[1]->name,"STAR")==0){

        }
        else if(strcmp(root->son[1]->name,"DIV")==0){

        }
        else if(strcmp(root->son[1]->name,"Exp")==0){

        }
        else if(strcmp(root->son[1]->name,"LP")==0){

        }
        else if(strcmp(root->son[1]->name,"DOT")==0){

        }
        else{
            assert(0);
        }
    }
    else if(root->num==2){
        if(strcmp(root->son[0]->name,"MINUS")==0){

        }
        else if(strcmp(root->son[0]->name,"NOT")==0){

        }
        else{
            assert(0);
        }
    }
    else if(root->num==4){
        if(strcmp(root->son[0]->name,"ID")==0){

        }
        else if(strcmp(root->son[0]->name,"Exp")==0){

        }
        else{
            assert(0);
        }
    }
    else if(root->num==1){
        if(strcmp(root->son[0]->name,"ID")==0){
            symbol* entry=find_symbol(root->son[0]->type_char);
            if(entry){
                return entry;
            }
            else{
                entry=error_entry;
                return entry;
            }
        }
        else if(strcmp(root->son[0]->name,"INT")==0){
            return int_entry;
        }
        else if(strcmp(root->son[0]->name,"FLOAT")==0){
            return float_entry;
        }
    }
    else{
        assert(0);
    }
}

void semantics_args(node* root){

}