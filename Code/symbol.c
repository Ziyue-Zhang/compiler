#include "symbol.h"

void symbol_init(){
     field_head=malloc(sizeof(field_list));
     field_head->next=NULL;
     field_head->symbol_tbl_head=NULL;
     field_cur=field_head;
}

void free_param(param_list* param_head){
    param_list* p=param_head;
    while(param_head){
        p=param_head;
        param_head=param_head->next;
        free_symbol(p->entry);
        free(p);
    }
}

void free_array(array_list* array_head){
    array_list* p=array_head;
    while(array_head){
        p=array_head;
        array_head=array_head->next;
        free_symbol(p->entry);
        free(p);
    }
}

void free_struct(struct_list* struct_head){
    struct_list* p=struct_head;
    while(struct_head){
        p=struct_head;
        struct_head=struct_head->next;
        free_symbol(p->entry);
        free(p);
    }
}


void free_symbol(symbol* entry){
    free(entry->name);
    if(entry->func_flag==1){
        free_param(entry->param_head);
    }
    if(entry->array_flag==1){
        free_array(entry->array_head);
    }
    /*if(entry->struct_flag==1){
        free_struct(entry->struct_head);
    }*/
    free(entry);
}
void free_symbol_tbl(symbol_tbl* symbol_tbl_head){
    symbol_tbl* p=symbol_tbl_head;
    while(symbol_tbl_head){
        p=symbol_tbl_head;
        symbol_tbl_head=symbol_tbl_head->next;
        free_symbol(p->entry);
        free(p);
    }
}
void free_field(field_list* field){
    free_symbol_tbl(field->symbol_tbl_head);
    free(field);
}
symbol* add_entry(int type,char* name,int array_flag,int func_flag,int struct_flag,int func_def_flag,int lineno){
    symbol* new_entry=malloc(sizeof(symbol));
    new_entry->type=type;
    new_entry->name=(char*)malloc(strlen(name) + 1);
    strcpy(new_entry->name,name);
    new_entry->name[strlen(name)]='\0';
    new_entry->array_flag=array_flag;
    new_entry->func_flag=func_flag;
    new_entry->struct_flag=struct_flag;
    new_entry->func_def_flag=func_def_flag;
    new_entry->lineno=lineno;
    return new_entry;
}

int same_param(param_list* p1, param_list*p2){
    if(!p1&&!p2)
        return 1;
    while(p1 && p2){
        if(p1->entry->type!=p2->entry->type){
            return 0;
        }
        if(!same_struct(p1->entry->struct_head,p2->entry->struct_head)){
            return 0;
        }
        p1=p1->next;
        p2=p2->next;
    }
    if(p1!=p2){
        return 0;
    }
    return 1;
}

int same_array(array_list* p1, array_list* p2){
    if(!p1&&!p2)
        return 1;
    while(p1 && p2){
        if(p1->array_dim!=p2->array_dim){
            return 0;
        }
        if(p1->array_size!=p2->array_size){
            return 0;
        }
        p1=p1->next;
        p2=p2->next;
    }
    if(p1!=p2){
        return 0;
    }
    return 1;
}

int same_struct(struct_list* p1, struct_list* p2){
    if(!p1&&!p2)
        return 1;
    while(p1 && p2){
        if(p1->entry->type!=p2->entry->type){
            return 0;
        }
        if(!same_struct(p1->entry->struct_head,p2->entry->struct_head)){
            return 0;
        }
        if(!same_array(p1->entry->array_head,p2->entry->array_head)){
            p1=p1->next;
            p2=p2->next;
        }
    }
    if(p1!=p2){
        return 0;
    }
    return 1;
}

int add_symbol(symbol* entry, int struct_entry){
    symbol_tbl* p=field_cur->symbol_tbl_head;
    while(p){
        if(strcmp(p->entry->name,entry->name)==0){
            if(field_cur==field_head){
                if(p->entry->func_flag==1&&entry->func_flag==1){
                    if(p->entry->type!=entry->type||!same_param(p->entry->param_head,entry->param_head)||(entry->type==SYMBOL_STRUCT&&!same_struct(p->entry->struct_head,entry->struct_head))){
                        printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".", entry->lineno, entry->name);
                        return -1;
                    }
                    if(p->entry->func_def_flag==1&&entry->func_def_flag==1){
                        printf("Error type 4 at Line %d: Redefined symbol \"%s\".", entry->lineno, entry->name);
                        return -1;
                    }
                    if(p->entry->func_def_flag==1||(p->entry->func_def_flag==0&&entry->func_def_flag==0)){
                        return 0;
                    }
                    if(entry->func_def_flag==1){
                        p->entry->func_def_flag=1;
                        return 0;
                    }
                }
            }
            if(p->entry->struct_flag==1&&entry->struct_flag==1){
                printf("Error type 16 at Line %d: Defined a struct with an existing id \"%s\".", entry->lineno, entry->name);
                return -1;
            }
            if(struct_entry==1){
                printf("Error type 15 at Line %d: Redefined field \"%s\".", entry->lineno, entry->name);
                return -1;
            }
            else{
                printf("Error type 3 at Line %d: Redefined symbol \"%s\".", entry->lineno, entry->name);
                return -1;
            }
        }
        p=p->next;
    }
    symbol_tbl* new_symbol_tbl=malloc(sizeof(symbol_tbl));
    new_symbol_tbl->entry=entry;
    new_symbol_tbl->next=field_cur->symbol_tbl_head;
    field_cur->symbol_tbl_head=new_symbol_tbl;
    return 0;
}
symbol* find_symbol(char *name){
    field_list* p1=field_cur;
    symbol_tbl* p2=p1->symbol_tbl_head;
    while(p1){
        p2=p1->symbol_tbl_head;
        while(p2){
            if(strcmp(p2->entry->name,name)==1){
                return p2->entry;
            }
            p2=p2->next;
        }
        p1=p1->next;
    }
    return NULL;
}

void field_push(){
    field_list* new_field_list=malloc(sizeof(field_list));
    new_field_list->symbol_tbl_head=NULL;
    new_field_list->next=field_cur;
    field_cur=new_field_list;
}

void field_pop(){
    field_list* old_field_list=field_cur;
    if(!field_cur){
        assert(field_cur);
        field_cur=field_cur->next;
    }
    free_field(old_field_list);
}

void check_func_def(){
    assert(field_cur==field_head);
    symbol_tbl* symbol_tbl_head=field_cur->symbol_tbl_head;
    symbol_tbl* p=symbol_tbl_head;
    while(p){
        symbol *entry=p->entry;
        if(entry->func_flag){
            if(entry->func_def_flag==0){
                printf("Error type 18 at Line %d: Undefined function \"%s\".", entry->lineno, entry->name);
            }
        }        
        p=p->next;
    }
}

void symbol_print(){
    int num=0;
    field_list* p1=field_cur;
    symbol_tbl* p2=p1->symbol_tbl_head;
    while(p1){
        p2=p1->symbol_tbl_head;
        while(p2){
            printf("field:%d,type:%d,name:%s\n",num,p2->entry->type,p2->entry->name);
            p2=p2->next;
        }
        num++;
        p1=p1->next;
    }
}