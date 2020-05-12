#include "translate.h"

void translate_program(node* root){
    intercode_init();
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;
    if (root->son[0]) {
        translate_extdeflist(root->son[0], codes);
        intercodes_print(codes);
    }
}
void translate_extdeflist(node* root, intercodes* codes){
    if(!root)
        return;
    translate_extdef(root->son[0],codes);
    intercodes *codes1 = malloc(sizeof(intercodes));
    codes1->head = NULL;
    codes1->tail = NULL;
    translate_extdeflist(root->son[1],codes1);
    intercodes_merge(codes,codes1);
}
void translate_extdef(node* root, intercodes* codes){
    if(root->num==3&&strcmp(root->son[2]->name,"CompSt")==0){
        symbol* func_entry=find_symbol(root->son[1]->son[0]->type_char);
        intercode* code1=intercode_new(IR_FUNCTION);
        code1->func_name=func_entry->name;
        //printf("%s\n",code1->func_name);
        intercodes_add(codes,code1);
        symbol_list* temp=func_entry->param_head->list;
        while(temp){
            intercode* code2=intercode_new(IR_PARAM);
            code2->result.kind=IR_VARIABLE;
            code2->result.var_name=temp->entry->entry_name;
            intercodes_add(codes,code2);
            temp=temp->next;
        }

        translate_compst(root->son[2],codes);

        intercode* code3=intercode_new(IR_VOID);
        intercodes_add(codes,code3);
    }
}
void translate_compst(node* root, intercodes* codes){
    translate_deflist(root->son[1],codes);
    translate_stmtlist(root->son[2],codes);
}
void translate_stmtlist(node* root, intercodes* codes){
    if(!root){
        return;
    }
    intercodes* codes1=translate_stmt(root->son[0]);
    intercodes_merge(codes,codes1);
    translate_stmtlist(root->son[1],codes);

}
void translate_deflist(node* root, intercodes* codes){
    if(!root){
        return;
    }
    translate_def(root->son[0],codes);
    translate_deflist(root->son[1],codes);
}
void translate_def(node* root, intercodes* codes){
    translate_declist(root->son[1],codes);
}
void translate_declist(node* root, intercodes* codes){
    if(root->num==1){
        translate_dec(root->son[0],codes);
    }
    else if(root->num==3){
        translate_dec(root->son[0],codes);
        translate_declist(root->son[2],codes);
    }
}
void translate_dec(node* root, intercodes* codes){
    if(root->num==3){
        translate_vardec(root->son[0],codes);
        operand* op=malloc(sizeof(operand));
        op->kind=IR_VARIABLE;
        op->temp_flag=0;
        op->var_name=find_symbol(root->son[0]->son[0]->type_char)->entry_name;
        intercodes* codes1=translate_exp(root->son[2],op);
        intercodes_merge(codes,codes1);
    }
    else if(root->num==1){
        translate_vardec(root->son[0],codes);
    }
}
void translate_vardec(node* root, intercodes* codes){
    if(root->num==1){
        symbol* entry=find_symbol(root->son[0]->type_char);
        if(entry->type==SYMBOL_STRUCT||entry->array_flag==1){
            intercode* code=intercode_new(IR_DEC);
            code->result.var_name=entry->entry_name;
            code->size=entry->size;
            intercodes_add(codes,code);
        }
    }
    else if(root->num==4){
        translate_vardec(root->son[0],codes);
    }
}

intercodes* translate_exp(node* root,operand* op){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;
    if(root->num==1){
        if(strcmp(root->son[0]->name,"INT")==0){
            if(!op){
                return codes;
            }
            int value=root->son[0]->type_int;
            intercode* code=intercode_new(IR_ASSIGN);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_CONSTANT;
            code->op1.u.value=value;
            code->op1.temp_flag=0;
            intercodes_add(codes,code);
            return codes;
        }
        else if(strcmp(root->son[0]->name,"ID")==0){
            if(!op){
                return codes;
            }
            char* name=root->son[0]->type_char;
            symbol* entry=find_symbol(name);
            intercode* code=intercode_new(IR_ASSIGN);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_VARIABLE;
            code->op1.var_name=entry->entry_name;
            code->op1.temp_flag=0;
            if((entry->type==SYMBOL_STRUCT||entry->array_flag==1)&&entry->param_flag==0){
                code->op1.kind=IR_ADDRESS;
            }
            intercodes_add(codes,code);
            return codes;
        }
    }
    else if(root->num==2){
        if(strcmp(root->son[0]->name,"MINUS")==0){
            if(!op){
                return codes;
            }
            int t1=new_temp();
            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=t1;

            intercodes* codes1=translate_exp(root->son[1],op1);
            intercodes_merge(codes,codes1);

            intercode* code=intercode_new(IR_SUB);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_CONSTANT;
            code->op1.temp_flag=0;
            code->op1.u.value=0;
            code->op2.kind=IR_VARIABLE;
            code->op2.temp_flag=1;
            code->op2.u.var_no=t1;
            intercodes_add(codes,code);

            return codes;
        }
        else if(strcmp(root->son[0]->name,"NOT")==0){
            if(!op){
                return codes;
            }
            int label1=new_label();
            int label2=new_label();

            intercode* code0=intercode_new(IR_ASSIGN);
            code0->result.kind=op->kind;
            code0->result.temp_flag=op->temp_flag;
            code0->result.var_name=op->var_name;
            code0->result.u=op->u;
            code0->op1.kind=IR_CONSTANT;
            code0->op1.temp_flag=0;
            code0->op1.u.value=0;
            intercodes_add(codes,code0);

            intercodes* code1=translate_cond(root,label1,label2);
            intercodes_merge(codes,code1);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label1;
            intercodes_add(codes,code2);

            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_CONSTANT;
            code3->op1.temp_flag=0;
            code3->op1.u.value=1;
            intercodes_add(codes,code3);

            intercode* code4=intercode_new(IR_LABEL);
            code4->label=label2;
            intercodes_add(codes,code4);

            return codes;
        }
    }
    else if(root->num==3){
        if(strcmp(root->son[1]->name,"ASSIGNOP")==0){
            if(root->son[0]->num==1){
                char* name=root->son[0]->son[0]->type_char;
                symbol* entry=find_symbol(name);
                int t1=new_temp();
                operand* op1=malloc(sizeof(operand));
                op1->kind=IR_VARIABLE;
                op1->temp_flag=1;
                op1->u.var_no=t1;
                intercodes* codes1=translate_exp(root->son[2],op1);
                intercodes_merge(codes,codes1);

                intercode* code1=intercode_new(IR_ASSIGN);
                code1->result.kind=IR_VARIABLE;
                code1->result.var_name=entry->entry_name;
                code1->result.temp_flag=0;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=1;
                code1->op1.u.var_no=t1;
                intercodes_add(codes,code1);

                if(op){
                    intercode* code2=intercode_new(IR_ASSIGN);
                    code2->result.kind=op->kind;
                    code2->result.temp_flag=op->temp_flag;
                    code2->result.var_name=op->var_name;
                    code2->result.u=op->u;
                    code2->op1.kind=IR_VARIABLE;
                    code2->op1.var_name=entry->entry_name;
                    code2->op1.temp_flag=0;
                    intercodes_add(codes,code2);
                }

                return codes;
            }
            else if(root->son[0]->num==3){
                intercodes* codes1=translate_array_struct1(root,op);
                intercodes_merge(codes,codes1);
                return codes;
            }
            else if(root->son[0]->num==4){
                intercodes* codes1=translate_array_struct1(root,op);
                intercodes_merge(codes,codes1);
                return codes;
            }
            return codes;
        }
        else if(strcmp(root->son[1]->name,"PLUS")==0){
            if(!op){
                return codes;
            }
            int t1=new_temp();
            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=t1;

            int t2=new_temp();
            operand* op2=malloc(sizeof(operand));
            op2->kind=IR_VARIABLE;
            op2->temp_flag=1;
            op2->u.var_no=t2;

            intercodes* codes1=translate_exp(root->son[0],op1);
            intercodes_merge(codes,codes1);

            intercodes* codes2=translate_exp(root->son[2],op2);
            intercodes_merge(codes,codes2);

            intercode* code=intercode_new(IR_ADD);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_VARIABLE;
            code->op1.temp_flag=1;
            code->op1.u.var_no=t1;
            code->op2.kind=IR_VARIABLE;
            code->op2.temp_flag=1;
            code->op2.u.var_no=t2;
            intercodes_add(codes,code);

            return codes;
        }
        else if(strcmp(root->son[1]->name,"MINUS")==0){
            if(!op){
                return codes;
            }
            int t1=new_temp();
            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=t1;

            int t2=new_temp();
            operand* op2=malloc(sizeof(operand));
            op2->kind=IR_VARIABLE;
            op2->temp_flag=1;
            op2->u.var_no=t2;

            intercodes* codes1=translate_exp(root->son[0],op1);
            intercodes_merge(codes,codes1);

            intercodes* codes2=translate_exp(root->son[2],op2);
            intercodes_merge(codes,codes2);

            intercode* code=intercode_new(IR_SUB);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_VARIABLE;
            code->op1.temp_flag=1;
            code->op1.u.var_no=t1;
            code->op2.kind=IR_VARIABLE;
            code->op2.temp_flag=1;
            code->op2.u.var_no=t2;
            intercodes_add(codes,code);
            
            return codes;
        }
        else if(strcmp(root->son[1]->name,"STAR")==0){
            if(!op){
                return codes;
            }
            int t1=new_temp();
            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=t1;

            int t2=new_temp();
            operand* op2=malloc(sizeof(operand));
            op2->kind=IR_VARIABLE;
            op2->temp_flag=1;
            op2->u.var_no=t2;

            intercodes* codes1=translate_exp(root->son[0],op1);
            intercodes_merge(codes,codes1);

            intercodes* codes2=translate_exp(root->son[2],op2);
            intercodes_merge(codes,codes2);

            intercode* code=intercode_new(IR_MUL);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_VARIABLE;
            code->op1.temp_flag=1;
            code->op1.u.var_no=t1;
            code->op2.kind=IR_VARIABLE;
            code->op2.temp_flag=1;
            code->op2.u.var_no=t2;
            intercodes_add(codes,code);
            
            return codes;
        }
        else if(strcmp(root->son[1]->name,"DIV")==0){
            if(!op){
                return codes;
            }
            int t1=new_temp();
            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=t1;

            int t2=new_temp();
            operand* op2=malloc(sizeof(operand));
            op2->kind=IR_VARIABLE;
            op2->temp_flag=1;
            op2->u.var_no=t2;

            intercodes* codes1=translate_exp(root->son[0],op1);
            intercodes_merge(codes,codes1);

            intercodes* codes2=translate_exp(root->son[2],op2);
            intercodes_merge(codes,codes2);

            intercode* code=intercode_new(IR_DIV);
            code->result.kind=op->kind;
            code->result.temp_flag=op->temp_flag;
            code->result.var_name=op->var_name;
            code->result.u=op->u;
            code->op1.kind=IR_VARIABLE;
            code->op1.temp_flag=1;
            code->op1.u.var_no=t1;
            code->op2.kind=IR_VARIABLE;
            code->op2.temp_flag=1;
            code->op2.u.var_no=t2;
            intercodes_add(codes,code);
            
            return codes;
        }
        else if(strcmp(root->son[1]->name,"RELOP")==0){
            if(!op){
                return codes;
            }
            int label1=new_label();
            int label2=new_label();

            intercode* code0=intercode_new(IR_ASSIGN);
            code0->result.kind=op->kind;
            code0->result.temp_flag=op->temp_flag;
            code0->result.var_name=op->var_name;
            code0->result.u=op->u;
            code0->op1.kind=IR_CONSTANT;
            code0->op1.temp_flag=0;
            code0->op1.u.value=0;
            intercodes_add(codes,code0);

            intercodes* code1=translate_cond(root,label1,label2);
            intercodes_merge(codes,code1);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label1;
            intercodes_add(codes,code2);

            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_CONSTANT;
            code3->op1.temp_flag=0;
            code3->op1.u.value=1;
            intercodes_add(codes,code3);

            intercode* code4=intercode_new(IR_LABEL);
            code4->label=label2;
            intercodes_add(codes,code4);

            return codes;
        }
        else if(strcmp(root->son[1]->name,"AND")==0){
            if(!op){
                return codes;
            }
            int label1=new_label();
            int label2=new_label();

            intercode* code0=intercode_new(IR_ASSIGN);
            code0->result.kind=op->kind;
            code0->result.temp_flag=op->temp_flag;
            code0->result.var_name=op->var_name;
            code0->result.u=op->u;
            code0->op1.kind=IR_CONSTANT;
            code0->op1.temp_flag=0;
            code0->op1.u.value=0;
            intercodes_add(codes,code0);

            intercodes* code1=translate_cond(root,label1,label2);
            intercodes_merge(codes,code1);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label1;
            intercodes_add(codes,code2);

            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_CONSTANT;
            code3->op1.temp_flag=0;
            code3->op1.u.value=1;
            intercodes_add(codes,code3);

            intercode* code4=intercode_new(IR_LABEL);
            code4->label=label2;
            intercodes_add(codes,code4);

            return codes;
        }
        else if(strcmp(root->son[1]->name,"OR")==0){
            if(!op){
                return codes;
            }
            int label1=new_label();
            int label2=new_label();

            intercode* code0=intercode_new(IR_ASSIGN);
            code0->result.kind=op->kind;
            code0->result.temp_flag=op->temp_flag;
            code0->result.var_name=op->var_name;
            code0->result.u=op->u;
            code0->op1.kind=IR_CONSTANT;
            code0->op1.temp_flag=0;
            code0->op1.u.value=0;
            intercodes_add(codes,code0);

            intercodes* code1=translate_cond(root,label1,label2);
            intercodes_merge(codes,code1);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label1;
            intercodes_add(codes,code2);

            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_CONSTANT;
            code3->op1.temp_flag=0;
            code3->op1.u.value=1;
            intercodes_add(codes,code3);

            intercode* code4=intercode_new(IR_LABEL);
            code4->label=label2;
            intercodes_add(codes,code4);

            return codes;
        }
        else if(strcmp(root->son[0]->name,"LP")==0){
            if(!op){
                return codes;
            }
            return translate_exp(root->son[1],op);
        }
        else if(strcmp(root->son[1]->name,"LP")==0){
            symbol* func_entry=find_symbol(root->son[0]->type_char);
            //printf("%s\n",func_entry->name);
            if(strcmp(func_entry->name,"read")==0){
                if(!op){
                    return codes;
                }
                intercode* code1=intercode_new(IR_READ);
                code1->result.kind=op->kind;
                code1->result.temp_flag=op->temp_flag;
                code1->result.u=op->u;
                code1->result.var_name=op->var_name;
                intercodes_add(codes,code1);

                return codes;
            }
            else{
                if(!op){
                    int t1=new_temp();
                    intercode* code1=intercode_new(IR_CALL);
                    code1->result.kind=IR_VARIABLE;
                    code1->result.temp_flag=1;
                    code1->result.u.var_no=t1;
                    code1->func_name=func_entry->name;
                    intercodes_add(codes,code1);
                }
                else{
                    intercode* code1=intercode_new(IR_CALL);
                    code1->result.kind=op->kind;
                    code1->result.temp_flag=op->temp_flag;
                    code1->result.u=op->u;
                    code1->result.var_name=op->var_name;
                    code1->func_name=func_entry->name;
                    intercodes_add(codes,code1);
                }

                return codes;
            }
        }
        else if(strcmp(root->son[1]->name,"DOT")==0){
            if(!op){
                return codes;
            }
            intercodes* codes1=translate_array_struct2(root,op);
            intercodes_merge(codes,codes1);
            return codes;
        }
    }
    else if(root->num==4){
        if(strcmp(root->son[1]->name,"LP")==0){
            symbol* func_entry=find_symbol(root->son[0]->type_char);

            arglist* args=malloc(sizeof(arglist));
            args->head=NULL;
            call_flag=1;
            
            intercodes* codes1=translate_args(root->son[2],&args);
            intercodes_merge(codes,codes1);

            if(strcmp(func_entry->name,"write")==0){
                intercode* code1=intercode_new(IR_WRITE);
                code1->result.kind=args->head->op->kind;
                code1->result.temp_flag=args->head->op->temp_flag;
                code1->result.u=args->head->op->u;
                code1->result.var_name=args->head->op->var_name;
                intercodes_add(codes,code1);

                call_flag=0;

                return codes;
            }

            arg_list*p =args->head;
            while(p){
                intercode* code1=intercode_new(IR_ARG);
                code1->result.kind=p->op->kind;
                code1->result.temp_flag=p->op->temp_flag;
                code1->result.u=p->op->u;
                code1->result.var_name=p->op->var_name;
                intercodes_add(codes,code1);
                p=p->next;
            }
            
            if(!op){
                int t1=new_temp();
                intercode* code2=intercode_new(IR_CALL);
                code2->result.kind=IR_VARIABLE;
                code2->result.temp_flag=1;
                code2->result.u.var_no=t1;
                code2->func_name=func_entry->name;
                intercodes_add(codes,code2);
            }
            else{
                intercode* code2=intercode_new(IR_CALL);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.u=op->u;
                code2->result.var_name=op->var_name;
                code2->func_name=func_entry->name;
                intercodes_add(codes,code2);
            }

            call_flag=0;

            return codes;

        }
        else if(strcmp(root->son[1]->name,"LB")==0){
            if(!op){
                return codes;
            }
            intercodes* codes1=translate_array_struct2(root,op);
            intercodes_merge(codes,codes1);
            return codes;
        }
    }
    return codes;
}
intercodes* translate_stmt(node* root){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;
    if(root->num==1){
        translate_compst(root->son[0],codes);
        return codes;
    }
    else if(root->num==2){
        return translate_exp(root->son[0],NULL);
    }
    else if(root->num==3){
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;

        intercodes* code1=translate_exp(root->son[1],op1);
        intercodes_merge(codes,code1);

        intercode* code2=intercode_new(IR_RETURN);
        code2->result.kind=IR_VARIABLE;
        code2->result.temp_flag=1;
        code2->result.u.var_no=t1;        
        intercodes_add(codes,code2);

        return codes;
    }
    else if(root->num==5){
        if(strcmp(root->son[0]->name,"IF")==0){
            int label1=new_label();
            int label2=new_label();

            intercodes* codes1=translate_cond(root->son[2],label1,label2);
            intercodes_merge(codes,codes1);

            intercode* code1=intercode_new(IR_LABEL);
            code1->label=label1;
            intercodes_add(codes,code1);

            intercodes* codes2=translate_stmt(root->son[4]);
            intercodes_merge(codes,codes2);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label2;
            intercodes_add(codes,code2);

            return codes;
        }
        else if(strcmp(root->son[0]->name,"WHILE")==0){
            int label1=new_label();
            int label2=new_label();
            int label3=new_label();

            intercode* code1=intercode_new(IR_LABEL);
            code1->label=label1;
            intercodes_add(codes,code1);

            intercodes* codes1=translate_cond(root->son[2],label2,label3);
            intercodes_merge(codes,codes1);

            intercode* code2=intercode_new(IR_LABEL);
            code2->label=label2;
            intercodes_add(codes,code2);

            intercodes* codes2=translate_stmt(root->son[4]);
            intercodes_merge(codes,codes2);

            intercode* code3=intercode_new(IR_GOTO);
            code3->label=label1;
            intercodes_add(codes,code3);

            intercode* code4=intercode_new(IR_LABEL);
            code4->label=label3;
            intercodes_add(codes,code4);

            return codes;
        }
    }
    else if(root->num==7){
        int label1=new_label();
        int label2=new_label();
        int label3=new_label();

        intercodes* codes1=translate_cond(root->son[2],label1,label2);
        intercodes_merge(codes,codes1);

        intercode* code1=intercode_new(IR_LABEL);
        code1->label=label1;
        intercodes_add(codes,code1);

        intercodes* codes2=translate_stmt(root->son[4]);
        intercodes_merge(codes,codes2);

        intercode* code2=intercode_new(IR_GOTO);
        code2->label=label3;
        intercodes_add(codes,code2);

        intercode* code3=intercode_new(IR_LABEL);
        code3->label=label2;
        intercodes_add(codes,code3);

        intercodes* codes3=translate_stmt(root->son[6]);
        intercodes_merge(codes,codes3);

        intercode* code4=intercode_new(IR_LABEL);
        code4->label=label3;
        intercodes_add(codes,code4);

        return codes;
    }
    return codes;
}
intercodes* translate_cond(node* root,int label1,int label2){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;    
    if(root->num==2&&strcmp(root->son[0]->name,"NOT")==0){
        return translate_cond(root->son[1],label2,label1);
    }
    else if(root->num==3&&strcmp(root->son[1]->name,"RELOP")==0){
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;

        int t2=new_temp();
        operand* op2=malloc(sizeof(operand));
        op2->kind=IR_VARIABLE;
        op2->temp_flag=1;
        op2->u.var_no=t2;

        intercodes* codes1=translate_exp(root->son[0],op1);
        intercodes_merge(codes,codes1);

        intercodes* codes2=translate_exp(root->son[2],op2);
        intercodes_merge(codes,codes2);

        int relop=0;
        if(strcmp(root->son[1]->type_char,">")==0){
            relop=IR_GT;
        }
        else if(strcmp(root->son[1]->type_char,"<")==0){
            relop=IR_LT;
        }
        else if(strcmp(root->son[1]->type_char,">=")==0){
            relop=IR_GE;
        }
        else if(strcmp(root->son[1]->type_char,"<=")==0){
            relop=IR_LE;
        }
        else if(strcmp(root->son[1]->type_char,"==")==0){
            relop=IR_EQ;
        }
        else if(strcmp(root->son[1]->type_char,"!=")==0){
            relop=IR_NEQ;
        }

        intercode* code3=intercode_new(IR_IF);
        code3->relop=relop;
        code3->label=label1;
        code3->op1.kind=IR_VARIABLE;
        code3->op1.temp_flag=1;
        code3->op1.u.var_no=t1;
        code3->op2.kind=IR_VARIABLE;
        code3->op2.temp_flag=1;
        code3->op2.u.var_no=t2;
        intercodes_add(codes,code3);

        intercode* code4=intercode_new(IR_GOTO);
        code4->label=label2;
        intercodes_add(codes,code4);

        return codes;
    }   
    else if(root->num==3&&strcmp(root->son[1]->name,"AND")==0){
        int label0=new_label();

        intercodes* codes1=translate_cond(root->son[0],label0,label2);
        intercodes_merge(codes,codes1);

        intercode* code1=intercode_new(IR_LABEL);
        code1->label=label0;
        intercodes_add(codes,code1);

        intercodes* codes2=translate_cond(root->son[2],label1,label2);
        intercodes_merge(codes,codes2);

        return codes;
    }
    else if(root->num==3&&strcmp(root->son[1]->name,"OR")==0){
        int label0=new_label();

        intercodes* codes1=translate_cond(root->son[0],label1,label0);
        intercodes_merge(codes,codes1);

        intercode* code1=intercode_new(IR_LABEL);
        code1->label=label0;
        intercodes_add(codes,code1);

        intercodes* codes2=translate_cond(root->son[2],label1,label2);
        intercodes_merge(codes,codes2);

        return codes;
    }
    else {
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;

        intercodes* codes1=translate_exp(root,op1);
        intercodes_merge(codes,codes1);

        intercode* code2=intercode_new(IR_IF);
        code2->relop=IR_NEQ;
        code2->label=label1;
        code2->op1.kind=IR_VARIABLE;
        code2->op1.temp_flag=1;
        code2->op1.u.var_no=t1;
        code2->op2.kind=IR_CONSTANT;
        code2->op2.temp_flag=0;
        code2->op2.u.value=0;
        intercodes_add(codes,code2);

        intercode* code3=intercode_new(IR_GOTO);
        code3->label=label2;
        intercodes_add(codes,code3);
        
        return codes;
    }
}
intercodes* translate_args(node* root,arglist** arg_head){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;

    if(root->num==1){
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;

        intercodes* code1=translate_exp(root->son[0],op1);
        intercodes_merge(codes,code1);

        arg_list * head=malloc(sizeof(arg_list));
        head->op=op1;
        head->next=(*arg_head)->head;
        (*arg_head)->head=head;

        return codes;
    }
    else if(root->num==3){
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;

        intercodes* code1=translate_exp(root->son[0],op1);
        intercodes_merge(codes,code1);

        arg_list * head=malloc(sizeof(arg_list));
        head->op=op1;
        head->next=(*arg_head)->head;
        (*arg_head)->head=head;

        intercodes* code2=translate_args(root->son[2],arg_head);
        intercodes_merge(codes,code2);

        return codes;
    }

    return codes;
}   

intercodes* translate_array_struct1(node* root,operand* op){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;
  
    int offset=0;
    int temp=0;
    array_list* array_head=NULL;
    symbol* entry=struct_array_offset(root->son[0],&offset,&array_head,codes,&temp);
    if(temp!=0&&offset!=0){
        intercode* code=intercode_new(IR_ADD);
        code->result.kind=IR_VARIABLE;
        code->result.temp_flag=1;
        code->result.u.var_no=temp;
        code->op1.kind=IR_VARIABLE;
        code->op1.temp_flag=1;
        code->op1.u.var_no=temp;
        code->op2.kind=IR_CONSTANT;
        code->op2.temp_flag=0;
        code->op2.u.value=offset;
        intercodes_add(codes,code);
    }

    if(entry->param_flag==0){
                  
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;
        intercodes* codes1=translate_exp(root->son[2],op1);
        intercodes_merge(codes,codes1);

        int size=offset;
        int t2=new_temp();
        if(size==0){
            if(temp==0){
                intercode* code1=intercode_new(IR_ASSIGN);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }

        }
        else{
            if(temp==0){
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_CONSTANT;
                code1->op2.temp_flag=0;
                code1->op2.u.value=size;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }
        }

        intercode* code2=intercode_new(IR_ASSIGN);
        code2->result.kind=IR_POINTER;
        code2->result.temp_flag=1;
        code2->result.u.var_no=t2;                
        code2->op1.kind=IR_VARIABLE;
        code2->op1.temp_flag=1;
        code2->op1.u.var_no=t1;
        intercodes_add(codes,code2);

        if(op){
            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_POINTER;
            code2->op1.temp_flag=1;
            code2->op1.u.var_no=t2;
            intercodes_add(codes,code3);
        }

        return codes;
    }
    else{
        int t1=new_temp();
        operand* op1=malloc(sizeof(operand));
        op1->kind=IR_VARIABLE;
        op1->temp_flag=1;
        op1->u.var_no=t1;
        intercodes* codes1=translate_exp(root->son[2],op1);
        intercodes_merge(codes,codes1);

        int size=offset;
        int t2=new_temp();
        if(size==0){
            if(temp==0){
                intercode* code1=intercode_new(IR_ASSIGN);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }
        }
        else{
            if(temp==0){
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_CONSTANT;
                code1->op2.temp_flag=0;
                code1->op2.u.value=size;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t2;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }
        }

        intercode* code2=intercode_new(IR_ASSIGN);
        code2->result.kind=IR_POINTER;
        code2->result.temp_flag=1;
        code2->result.u.var_no=t2;                
        code2->op1.kind=IR_VARIABLE;
        code2->op1.temp_flag=1;
        code2->op1.u.var_no=t1;
        intercodes_add(codes,code2);

        if(op){
            intercode* code3=intercode_new(IR_ASSIGN);
            code3->result.kind=op->kind;
            code3->result.temp_flag=op->temp_flag;
            code3->result.var_name=op->var_name;
            code3->result.u=op->u;
            code3->op1.kind=IR_POINTER;
            code2->op1.temp_flag=1;
            code2->op1.u.var_no=t2;
            intercodes_add(codes,code3);
        }

       return codes;
    }
}
intercodes* translate_array_struct2(node* root,operand* op){
    intercodes *codes = malloc(sizeof(intercodes));
    codes->head = NULL;
    codes->tail = NULL;

    int offset=0;
    int temp=0;
    array_list* array_head=NULL;
    symbol* entry=struct_array_offset(root,&offset,&array_head,codes,&temp);
    if(temp!=0&&offset!=0){
        intercode* code=intercode_new(IR_ADD);
        code->result.kind=IR_VARIABLE;
        code->result.temp_flag=1;
        code->result.u.var_no=temp;
        code->op1.kind=IR_VARIABLE;
        code->op1.temp_flag=1;
        code->op1.u.var_no=temp;
        code->op2.kind=IR_CONSTANT;
        code->op2.temp_flag=0;
        code->op2.u.value=offset;
        intercodes_add(codes,code);
    }

    if(entry->param_flag==0){
        int size=offset;
        if(size==0){
            int t1=new_temp();
            if(temp==0){
                intercode* code1=intercode_new(IR_ASSIGN);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }
            if(call_flag==0){
                intercode* code2=intercode_new(IR_ASSIGN);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.var_name=op->var_name;
                code2->result.u=op->u;
                code2->op1.kind=IR_POINTER;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
                intercodes_add(codes,code2);
            }
            else{
                intercode* code2=intercode_new(IR_ASSIGN);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.var_name=op->var_name;
                code2->result.u=op->u;
                code2->op1.kind=IR_VARIABLE;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
                intercodes_add(codes,code2);
            }
            return codes;
        }
        else{
            int t1=new_temp();
            if(temp==0){
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_CONSTANT;
                code1->op2.temp_flag=0;
                code1->op2.u.value=size;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_ADDRESS;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }

            if(call_flag==0){
                intercode* code2=intercode_new(IR_ASSIGN);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.var_name=op->var_name;
                code2->result.u=op->u;
                code2->op1.kind=IR_POINTER;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
                intercodes_add(codes,code2);
            }
            else{
                intercode* code2=intercode_new(IR_ASSIGN);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.var_name=op->var_name;
                code2->result.u=op->u;
                code2->op1.kind=IR_VARIABLE;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
                intercodes_add(codes,code2);
            }
            return codes;
        }
    }
    else{
        int size=offset;
        if(size==0){
            if(temp==0){
                intercode* code1=intercode_new(IR_ASSIGN);
                code1->result.kind=op->kind;
                code1->result.temp_flag=op->temp_flag;
                code1->result.var_name=op->var_name;
                code1->result.u=op->u;
                code1->op1.kind=IR_POINTER;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                if(call_flag==1){
                    code1->op1.kind=IR_VARIABLE;
                    code1->op1.temp_flag=0;
                    code1->op1.var_name=entry->entry_name;
                }
                intercodes_add(codes,code1);
            }
            else{
                int t1=new_temp();
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);

                intercode* code2=intercode_new(IR_ASSIGN);
                code2->result.kind=op->kind;
                code2->result.temp_flag=op->temp_flag;
                code2->result.var_name=op->var_name;
                code2->result.u=op->u;
                code2->op1.kind=IR_POINTER;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
                if(call_flag==1){
                    code2->op1.kind=IR_VARIABLE;
                    code2->op1.temp_flag=1;
                    code2->op1.u.var_no=t1;
                }
                intercodes_add(codes,code2);
            }
            return codes;
        }
        else{
            int t1=new_temp();

            if(temp==0){
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_CONSTANT;
                code1->op2.temp_flag=0;
                code1->op2.u.value=size;
                intercodes_add(codes,code1);
            }
            else{
                intercode* code1=intercode_new(IR_ADD);
                code1->result.kind=IR_VARIABLE;
                code1->result.temp_flag=1;
                code1->result.u.var_no=t1;
                code1->op1.kind=IR_VARIABLE;
                code1->op1.temp_flag=0;
                code1->op1.var_name=entry->entry_name;
                code1->op2.kind=IR_VARIABLE;
                code1->op2.temp_flag=1;
                code1->op2.u.var_no=temp;
                intercodes_add(codes,code1);
            }

            intercode* code2=intercode_new(IR_ASSIGN);
            code2->result.kind=op->kind;
            code2->result.temp_flag=op->temp_flag;
            code2->result.var_name=op->var_name;
            code2->result.u=op->u;
            code2->op1.kind=IR_POINTER;
            code2->op1.temp_flag=1;
            code2->op1.u.var_no=t1;
            if(call_flag==1){
                code2->op1.kind=IR_VARIABLE;
                code2->op1.temp_flag=1;
                code2->op1.u.var_no=t1;
            }
            intercodes_add(codes,code2);
            return codes;
        }
    }
}
symbol* struct_array_offset(node* root,int *offset,array_list** array_head,intercodes* codes,int *temp){
    if(root->num==1){
        symbol*entry=find_symbol(root->son[0]->type_char);
        *array_head=entry->array_head;
        *offset=0;
        return entry;
    }
    else if(root->num==3){
        symbol*entry=struct_array_offset(root->son[0],offset,array_head,codes,temp);
        symbol*entry2=find_symbol(root->son[2]->type_char);
        *offset=*offset+entry2->struct_offset;
        *array_head=entry2->array_head;
        return entry;
    }
    else if(root->num==4){
        symbol*entry=struct_array_offset(root->son[0],offset,array_head,codes,temp);
        if(strcmp(root->son[2]->son[0]->name,"INT")==0){
            *offset=*offset+root->son[2]->son[0]->type_int*(*array_head)->array_size;
        }
        else{
            if(*temp==0){
                *temp=new_temp();
                intercode* code=intercode_new(IR_ASSIGN);
                code->result.kind=IR_VARIABLE;
                code->result.temp_flag=1;
                code->result.u.var_no=*temp;
                code->op1.kind=IR_CONSTANT;
                code->op1.temp_flag=0;
                code->op1.u.value=0;
                intercodes_add(codes,code);
            }
            int temp1=new_temp();

            operand* op1=malloc(sizeof(operand));
            op1->kind=IR_VARIABLE;
            op1->temp_flag=1;
            op1->u.var_no=temp1;
            intercodes* codes1=translate_exp(root->son[2],op1);
            intercodes_merge(codes,codes1);

            intercode* code1=intercode_new(IR_MUL);
            code1->result.kind=IR_VARIABLE;
            code1->result.temp_flag=1;
            code1->result.u.var_no=temp1;
            code1->op1.kind=IR_VARIABLE;
            code1->op1.temp_flag=1;
            code1->op1.u.var_no=temp1;
            code1->op2.kind=IR_CONSTANT;
            code1->op2.temp_flag=0;
            code1->op2.u.value=(*array_head)->array_size;
            intercodes_add(codes,code1);

            intercode* code2=intercode_new(IR_ADD);
            code2->result.kind=IR_VARIABLE;
            code2->result.temp_flag=1;
            code2->result.u.var_no=*temp;
            code2->op1.kind=IR_VARIABLE;
            code2->op1.temp_flag=1;
            code2->op1.u.var_no=*temp;
            code2->op2.kind=IR_CONSTANT;
            code2->op2.temp_flag=0;
            code2->op2.u.value=*offset;
            intercodes_add(codes,code2);

            intercode* code3=intercode_new(IR_ADD);
            code3->result.kind=IR_VARIABLE;
            code3->result.temp_flag=1;
            code3->result.u.var_no=*temp;
            code3->op1.kind=IR_VARIABLE;
            code3->op1.temp_flag=1;
            code3->op1.u.var_no=*temp;
            code3->op2.kind=IR_VARIABLE;
            code3->op2.temp_flag=1;
            code3->op2.u.var_no=temp1;
            intercodes_add(codes,code3);
            
            *offset=0;
        }
        *array_head=(*array_head)->next;
        return entry;
    }
}