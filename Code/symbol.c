#include "symbol.h"

void symbol_init(){
     field_head=malloc(sizeof(field_list));
     field_head->next=NULL;
     field_head->symbol_tbl_head=NULL;
     field_cur=field_head;
     symbol_num=1;
}

void free_param(param_list* param_head){
    symbol_list* list_head=param_head->list;
    symbol_list*p = list_head;
    while(list_head){
        p=list_head;
        list_head=list_head->next;
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
    symbol_list* list_head=struct_head->list;
    symbol_list*p = list_head;
    while(list_head){
        p=list_head;
        list_head=list_head->next;
        free_symbol(p->entry);
        free(p);
    }
}


void free_symbol(symbol* entry){
    free(entry->name);
    if(entry->param_head!=NULL){
        free_param(entry->param_head);
    }
    if(entry->array_head!=NULL){
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
        //free_symbol(p->entry);
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
    new_entry->dim=0;
    new_entry->array_size=1;
    new_entry->left_value_flag=0;
    new_entry->entry_name="";
    return new_entry;
}

int same_param(param_list* p1, param_list*p2){
    if(!p1&&!p2)
        return 1;
    symbol_list* q1=p1->list;
    symbol_list* q2=p2->list;
    while(q1 && q2){
        if(q1->entry->type!=q2->entry->type){
            return 0;
        }
        if(q1->entry->dim!=q2->entry->dim){
            return 0;
        }
        if(!same_struct(q1->entry->struct_head,q2->entry->struct_head)){
            return 0;
        }
        q1=q1->next;
        q2=q2->next;
    }
    if(q1!=q2){
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
    if(!p1){
        return 0;
    }
    if(!p2){
        return 0;
    }
    //printf("%s\n",p1->list->entry->name);
    symbol_list* q1=p1->list;
    symbol_list* q2=p2->list;
    while(q1 && q2){
        if(q1->entry->type!=q2->entry->type){
            return 0;
        }
        if(!same_struct(q1->entry->struct_head,q2->entry->struct_head)){
            return 0;
        }
        if(q1->entry->dim!=q2->entry->dim){
            return 0;
        }
        q1=q1->next;
        q2=q2->next;
    }
    if(q1!=q2){
        return 0;
    }
    return 1;
}

int same_type(symbol* entry1, symbol* entry2){
    if(!entry1||!entry2){
        assert(0);
    }
    if(entry1->type==SYMBOL_VOID)
        return 0;
    if(entry2->type==SYMBOL_VOID)
        return 0;
    if(entry1->struct_flag==1)
        return 0;
    if(entry2->struct_flag==1)
        return 0;
    if(entry1->type!=entry2->type)
        return 0;
    if(entry1->dim!=entry2->dim)
        return 0;
    if(entry1->struct_head==entry2->struct_head||same_struct(entry1->struct_head,entry2->struct_head)==1)
        return 1;
    else
        return 0;   
}

int add_symbol(symbol* entry, int struct_entry){
    symbol_tbl* p=field_cur->symbol_tbl_head;
    while(p){
        if(strcmp(p->entry->name,entry->name)==0){
            if(field_cur==field_head){
                if(p->entry->func_flag==1&&entry->func_flag==1){
                    if(p->entry->func_def_flag==1&&entry->func_def_flag==1){
                        printf("Error type 4 at Line %d: Redefined function \"%s\".\n", entry->lineno, entry->name);
                        return 0;
                    }
                    if(p->entry->type!=entry->type||!same_param(p->entry->param_head,entry->param_head)||(entry->type==SYMBOL_STRUCT&&!same_struct(p->entry->struct_head,entry->struct_head))){
                        printf("Error type 19 at Line %d: Inconsistent declaration of function \"%s\".\n", entry->lineno, entry->name);
                        return 0;
                    }
                    if(p->entry->func_def_flag==1||(p->entry->func_def_flag==0&&entry->func_def_flag==0)){
                        return 1;
                    }
                    if(entry->func_def_flag==1){
                        p->entry->func_def_flag=1;
                        return 1;
                    }
                }
            }
            if(entry->struct_flag==1){
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
            if(struct_entry==1){
                printf("Error type 15 at Line %d: Redefined field \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
            else{
                printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
        }
        p=p->next;
    }
    symbol_tbl* new_symbol_tbl=malloc(sizeof(symbol_tbl));
    if(entry->func_flag==0){
        /*entry->entry_name=malloc(40);
        snprintf(entry->entry_name,40,"v%d",symbol_num);
        //printf("%s\n",entry->entry_name);
        symbol_num++;*/
        if(entry->name!=NULL&&entry->name[0]=='t'){
            entry->entry_name=malloc(40);
            snprintf(entry->entry_name,40,"1%s",entry->name);
        }
        else{
            entry->entry_name=entry->name;
        }
        //printf("%s\n",entry->entry_name);
    }
    new_symbol_tbl->entry=entry;
    new_symbol_tbl->next=field_cur->symbol_tbl_head;
    field_cur->symbol_tbl_head=new_symbol_tbl;
    return 1;
}
int add_struct_symbol(symbol* entry, int struct_entry){
    symbol_tbl* p=field_head->symbol_tbl_head;
    while(p){
        if(strcmp(p->entry->name,entry->name)==0){
            if(entry->struct_flag==1){
                printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
            if(struct_entry==1){
                printf("Error type 15 at Line %d: Redefined field \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
            else{
                printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", entry->lineno, entry->name);
                return 0;
            }
        }
        p=p->next;
    }
    symbol_tbl* new_symbol_tbl=malloc(sizeof(symbol_tbl));
    new_symbol_tbl->entry=entry;
    new_symbol_tbl->next=field_head->symbol_tbl_head;
    field_head->symbol_tbl_head=new_symbol_tbl;
    return 1;
}
symbol* find_symbol(char *name){
    field_list* p1=field_cur;
    symbol_tbl* p2=p1->symbol_tbl_head;
    while(p1){
        p2=p1->symbol_tbl_head;
        while(p2){
            if(strcmp(p2->entry->name,name)==0){
                return p2->entry;
            }
            p2=p2->next;
        }
        p1=p1->next;
    }
    return NULL;
}
symbol* find_symbol_headfield(char *name){
    symbol_tbl* p=field_head->symbol_tbl_head;
    while(p){
        if(strcmp(p->entry->name,name)==0){
            return p->entry;
        }
        p=p->next;
    }
    return NULL;
}
symbol* find_symbol_curfield(char *name){
    symbol_tbl* p=field_cur->symbol_tbl_head;
    while(p){
        if(strcmp(p->entry->name,name)==0){
            return p->entry;
        }
        p=p->next;
    }
    return NULL;
}
symbol* find_symbol_struct(char *name){
    field_list* p1=field_cur;
    symbol_tbl* p2=p1->symbol_tbl_head;
    while(p1){
        p2=p1->symbol_tbl_head;
        while(p2){
            if(strcmp(p2->entry->name,name)==0&&p2->entry->type==SYMBOL_STRUCT&&p2->entry->struct_flag==1){
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
    if(field_cur){
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
        if(entry->func_flag==1&&entry->func_def_flag==0){
            printf("Error type 18 at Line %d: Undefined function \"%s\".\n", entry->lineno, entry->name);
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