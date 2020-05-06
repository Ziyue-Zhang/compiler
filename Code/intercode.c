#include "intercode.h"

void intercode_init(){
    temp_num=1;
    label_num=1;
}

intercode* intercode_new(int kind){
    intercode* new_intercode=malloc(sizeof(intercode));
    new_intercode->kind=kind;
}

void intercodes_add(intercodes* irs,intercode *ir){
    intercode_list* irl=malloc(sizeof(intercode));
    irl->code=ir;
    irl->next=NULL;
    if(irs->tail){
        irs->tail->next=irl;
        irs->tail=irl;
    }
    else{
        irs->head=irl;
        irs->tail=irl;
    }
}

void intercodes_merge(intercodes* ir1,intercodes* ir2){
    if(ir1->tail){
        if(ir2->head){
            ir1->tail->next=ir2->head;
            ir1->tail=ir2->tail;
        }
    }
    else{
        ir1->head=ir2->head;
        ir1->tail=ir2->tail;
    }
    free(ir2);
}
void intercodes_print(intercodes* irs){
    intercode_list* irl=irs->head;
    while(irl){
        intercode_print(irl->code);
        irl=irl->next;
    }
}
void intercode_print(intercode* ir){
    if(ir->kind==IR_LABEL){
        intercode_label(ir);
    }
    else if(ir->kind==IR_FUNCTION){
        intercode_function(ir);
    }
    else if(ir->kind==IR_ASSIGN){
        intercode_assign(ir);
    }
    else if(ir->kind==IR_ADD){
        intercode_add(ir);
    }
    else if(ir->kind==IR_SUB){
        intercode_sub(ir);
    }
    else if(ir->kind==IR_MUL){
        intercode_mul(ir);
    }
    else if(ir->kind==IR_DIV){
        intercode_div(ir);
    }
    else if(ir->kind==IR_GOTO){
        intercode_goto(ir);
    }
    else if(ir->kind==IR_IF){
        intercode_if(ir);
    }
    else if(ir->kind==IR_RETURN){
        intercode_return(ir);
    }
    else if(ir->kind==IR_DEC){
        intercode_dec(ir);
    }
    else if(ir->kind==IR_ARG){
        intercode_arg(ir);
    }
    else if(ir->kind==IR_CALL){
        intercode_call(ir);
    }
    else if(ir->kind==IR_PARAM){
        intercode_param(ir);
    }
    else if(ir->kind==IR_READ){
        intercode_read(ir);
    }
    else if(ir->kind==IR_WRITE){
        intercode_write(ir);
    }
    else if(ir->kind==IR_VOID){
        fprintf(output,"\n");
    }
    else {
        assert(0);
    }
}

void intercode_op_left(operand op){
    if(op.kind==IR_POINTER){
        fprintf(output, "*");
        if(op.temp_flag==1){
            fprintf(output, "t%d",op.u.var_no);
        }
        else{
            fprintf(output, "%s",op.var_name);
        }
    }
    else if(op.kind==IR_VARIABLE){
        if(op.temp_flag==1){
            fprintf(output, "t%d",op.u.var_no);
        }
        else{
            fprintf(output, "%s",op.var_name);
        }
    }
    else{
        return;
    }
}

void intercode_op_right(operand op){
    if(op.kind==IR_ADDRESS){
        fprintf(output, "&");
        if(op.temp_flag==1){
            fprintf(output, "t%d",op.u.var_no);
        }
        else{
            fprintf(output, "%s",op.var_name);
        }
    }
    else if(op.kind==IR_POINTER){
        fprintf(output, "*");
        if(op.temp_flag==1){
            fprintf(output, "t%d",op.u.var_no);
        }
        else{
            fprintf(output, "%s",op.var_name);
        }
    }
    else if(op.kind==IR_CONSTANT){
        fprintf(output, "#%d",op.u.value);

    }
    else if(op.kind==IR_VARIABLE){
        if(op.temp_flag==1){
            fprintf(output, "t%d",op.u.var_no);
        }
        else{
            fprintf(output, "%s",op.var_name);
        }
    }
    else{
        return;
    }
}

void intercode_label(intercode* ir){
    fprintf(output, "LABEL label%d :\n", ir->label);
}
void intercode_function(intercode* ir){
    fprintf(output, "FUNCTION %s :\n", ir->func_name);
}
void intercode_assign(intercode* ir){
    intercode_op_left(ir->result);
    fprintf(output, " := ");
    intercode_op_right(ir->op1);
    fprintf(output, "\n");
}
void intercode_add(intercode* ir){
    intercode_op_left(ir->result);
    fprintf(output, " := ");
    intercode_op_right(ir->op1);
    fprintf(output, " + ");
    intercode_op_right(ir->op2);
    fprintf(output, "\n");
}
void intercode_sub(intercode* ir){
    intercode_op_left(ir->result);
    fprintf(output, " := ");
    intercode_op_right(ir->op1);
    fprintf(output, " - ");
    intercode_op_right(ir->op2);
    fprintf(output, "\n");
}
void intercode_mul(intercode* ir){
    intercode_op_left(ir->result);
    fprintf(output, " := ");
    intercode_op_right(ir->op1);
    fprintf(output, " * ");
    intercode_op_right(ir->op2);
    fprintf(output, "\n");
}
void intercode_div(intercode* ir){
    intercode_op_left(ir->result);
    fprintf(output, " := ");
    intercode_op_right(ir->op1);
    fprintf(output, " / ");
    intercode_op_right(ir->op2);
    fprintf(output, "\n");
}   
void intercode_goto(intercode* ir){
    fprintf(output, "GOTO label%d\n", ir->label);
}
void intercode_if(intercode* ir){
    fprintf(output, "IF ");
    intercode_op_right(ir->op1);
    if(ir->relop==IR_GT){
        fprintf(output, " > ");
    }
    else if(ir->relop==IR_LT){
        fprintf(output, " < ");
    }
    else if(ir->relop==IR_GE){
        fprintf(output, " >= ");
    }
    else if(ir->relop==IR_LE){
        fprintf(output, " <= ");
    }
    else if(ir->relop==IR_EQ){
        fprintf(output, " == ");
    }
    else if(ir->relop==IR_NEQ){
        fprintf(output, " != ");
    }
    intercode_op_right(ir->op2);
    fprintf(output, " GOTO label%d\n", ir->label);
}
void intercode_return(intercode* ir){
    fprintf(output, "RETURN "); 
    intercode_op_right(ir->result);
    fprintf(output, "\n");
}
void intercode_dec(intercode* ir){
    fprintf(output, "DEC %s %d\n",ir->result.var_name,ir->size); 
}
void intercode_arg(intercode* ir){
    fprintf(output, "ARG "); 
    intercode_op_right(ir->result);
    fprintf(output, "\n");
}
void intercode_call(intercode* ir){
    intercode_op_right(ir->result);
    fprintf(output, " := CALL %s\n", ir->func_name);
}
void intercode_param(intercode* ir){
    fprintf(output, "PARAM "); 
    intercode_op_right(ir->result);
    fprintf(output, "\n");
}
void intercode_read(intercode* ir){
    fprintf(output, "READ "); 
    intercode_op_right(ir->result);
    fprintf(output, "\n");
}
void intercode_write(intercode* ir){
    fprintf(output, "WRITE "); 
    intercode_op_right(ir->result);
    fprintf(output, "\n");
}

int new_temp(){
    return temp_num++;
}

int new_label(){
    return label_num++;
}