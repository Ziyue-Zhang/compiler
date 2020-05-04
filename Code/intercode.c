#include "intercode.h"

void intercode_init(){
    temp_num=1;
    label_num=1;
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
    else {
        assert(0);
    }
}

void intercode_label(intercode* ir){

}
void intercode_function(intercode* ir){

}
void intercode_assign(intercode* ir){

}
void intercode_add(intercode* ir){

}
void intercode_sub(intercode* ir){

}
void intercode_mul(intercode* ir){

}
void intercode_div(intercode* ir){

}
void intercode_goto(intercode* ir){

}
void intercode_if(intercode* ir){

}
void intercode_return(intercode* ir){

}
void intercode_dec(intercode* ir){

}
void intercode_arg(intercode* ir){

}
void intercode_call(intercode* ir){

}
void intercode_param(intercode* ir){

}
void intercode_read(intercode* ir){

}
void intercode_write(intercode* ir){

}

int new_temp(){
    return temp_num++;
}

int new_label(){
    return label_num++;
}