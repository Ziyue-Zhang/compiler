#include "mips.h"

void mips_init(){
    fprintf(output, ".data\n");
    fprintf(output, "_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(output, "_ret: .asciiz \"\\n\"\n");
    fprintf(output, ".globl main\n");
    fprintf(output, ".text\n");
    fprintf(output, "read:\n");
    fprintf(output, "  li $v0, 4\n");
    fprintf(output, "  la $a0, _prompt\n");
    fprintf(output, "  syscall\n");
    fprintf(output, "  li $v0, 5\n");
    fprintf(output, "  syscall\n");
    fprintf(output, "  jr $ra\n\n");
    fprintf(output, "write:\n");
    fprintf(output, "  li $v0, 1\n");
    fprintf(output, "  syscall\n");
    fprintf(output, "  li $v0, 4\n");
    fprintf(output, "  la $a0, _ret\n");
    fprintf(output, "  syscall\n");
    fprintf(output, "  move $v0, $0\n");
    fprintf(output, "  jr $ra\n\n");
}

void mips_print(intercode* code){
    if(code->kind==IR_LABEL){
        fprintf(output, "label%d:\n", code->label);
    }
    else if(code->kind==IR_FUNCTION){
        fprintf(output, "%s :\n", code->func_name);
        if(strcmp(code->func_name,"main")==0){
            fprintf(output, "  move $fp, $sp\n");
        }
    }
    else if(code->kind==IR_ASSIGN){
        intercode_assign(code);
    }
    else if(code->kind==IR_ADD){
        intercode_add(code);
    }
    else if(code->kind==IR_SUB){
        intercode_sub(code);
    }
    else if(code->kind==IR_MUL){
        intercode_mul(code);
    }
    else if(code->kind==IR_DIV){
        intercode_div(code);
    }
    else if(code->kind==IR_GOTO){
        fprintf(output, "  j label%d\n", code->label);
    }
    else if(code->kind==IR_IF){
        intercode_if(code);
    }
    else if(code->kind==IR_RETURN){
        intercode_return(code);
    }
    else if(code->kind==IR_DEC){
        fprintf(output, "  addi $sp, $sp, %d\n", -code->size);
    }
    else if(code->kind==IR_ARG){
        intercode_arg(code);
    }
    else if(code->kind==IR_CALL){
        intercode_call(code);
    }
    else if(code->kind==IR_PARAM){
    }
    else if(code->kind==IR_READ){
        intercode_read(code);
    }
    else if(code->kind==IR_WRITE){
        intercode_write(code);
    }
    else {
        assert(0);
    }
}
void mips_generate(intercodes* codes){
    mips_init();
    intercode_list* irl=codes->head;
    while(irl){
        mips_print(irl->code);
        irl=irl->next;
    }
}