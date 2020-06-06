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

void mips_reg(operand op,char* reg_name,int reg_num){
    if(op.kind==IR_ADDRESS){
        if(op.temp_flag==1){
            fprintf(output, "  addi $%s%d, $fp, %d\n", reg_name, reg_num, -4*op.u.var_no);
        }
        else{
            fprintf(output, "  addi $%s%d, $fp, %d\n", reg_name, reg_num, -4*op.var_name);
        }
    }
    else if(op.kind==IR_POINTER){
        if(op.temp_flag==1){
            fprintf(output, "  lw $%s%d, %d($fp)\n", reg_name, reg_num, -4*op.u.var_no);
            fprintf(output, "  lw $%s%d, 0($%s%d)\n", reg_name, reg_num, reg_name, reg_num);
        }
        else{
            fprintf(output, "  lw $%s%d, %d($fp)\n", reg_name, reg_num, -4*op.var_name);
            fprintf(output, "  lw $%s%d, 0($%s%d)\n", reg_name, reg_num, reg_name, reg_num);
        }
    }
    else if(op.kind==IR_CONSTANT){
        fprintf(output, "  li $%s%d, %d\n", reg_name, reg_num, op.u.value);

    }
    else if(op.kind==IR_VARIABLE){
        if(op.temp_flag==1){
            fprintf(output, "  lw $%s%d, %d($fp)\n", reg_name, reg_num, -4*op.u.var_no);
        }
        else{
            fprintf(output, "  lw $%s%d, %d($fp)\n", reg_name, reg_num, -4*op.var_name);
        }
    }
    else{
        return;
    }
}

void mips_exp(intercode* code){
    mips_reg(code->op1,"t",0);
    mips_reg(code->op2,"t",1);
    if(code->kind==IR_ADD){
        fprintf(output, "  add $t2, $t0, $t1\n");
    }
    else if(code->kind==IR_SUB){
        fprintf(output, "  sub $t2, $t0, $t1\n");
    }
    else if(code->kind==IR_MUL){
        fprintf(output, "  mul $t2, $t0, $t1\n");
    }
    else if(code->kind==IR_DIV){
        fprintf(output, "  div $t0, $t1\n");
        fprintf(output, "  mflo $t2\n");
    }

    if(code->result.kind==IR_POINTER){
        if(code->result.temp_flag==1){
            fprintf(output, "  lw $t3, %d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "  lw $t3, %d($fp)\n", -4*code->result.var_name);
        }
        fprintf(output, "  sw $t2, 0($t3)\n");
    }
    else if(code->result.kind==IR_VARIABLE){
        fprintf(output, "  sw $t2, ");
        if(code->result.temp_flag==1){
            fprintf(output, "%d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "%d($fp)\n", -4*code->result.var_name);
        }
    }
}
void mips_assign(intercode* code){
    mips_reg(code->op1,"t",0);

    if(code->result.kind==IR_POINTER){
        if(code->result.temp_flag==1){
            fprintf(output, "  lw $t1, %d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "  lw $t1, %d($fp)\n", -4*code->result.var_name);
        }
        fprintf(output, "  sw $t0, 0($t1)\n");
    }
    else if(code->result.kind==IR_VARIABLE){
        fprintf(output, "  sw $t0, ");
        if(code->result.temp_flag==1){
            fprintf(output, "%d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "%d($fp)\n", -4*code->result.var_name);
        }
    }
}
void mips_if(intercode* code){
    mips_reg(code->op1,"t",0);
    mips_reg(code->op2,"t",1);
    if(code->relop==IR_GT){
        fprintf(output, "  bgt $t0, $t1, label%d\n", code->label);
    }
    else if(code->relop==IR_LT){
        fprintf(output, "  blt $t0, $t1, label%d\n", code->label);
    }
    else if(code->relop==IR_GE){
        fprintf(output, "  bge $t0, $t1, label%d\n", code->label);
    }
    else if(code->relop==IR_LE){
        fprintf(output, "  ble $t0, $t1, label%d\n", code->label);
    }
    else if(code->relop==IR_EQ){
        fprintf(output, "  beq $t0, $t1, label%d\n", code->label);
    }
    else if(code->relop==IR_NEQ){
        fprintf(output, "  bne $t0, $t1, label%d\n", code->label);
    }
}
void mips_return(intercode* code){
    mips_reg(code->result,"v",0);
    fprintf(output, "  jr $ra\n");
}
void mips_arg(intercode* code){
    arg_num++;
    mips_reg(code->result,"t",0);
    
    fprintf(output, "  addi $sp, $sp, -4\n");
    fprintf(output, "  sw $t0, 0($sp)\n");
}
void mips_call(intercode* code){
    fprintf(output, "  addi $sp, $sp, -8\n");
    fprintf(output, "  sw $ra, 4($sp)\n");
    fprintf(output, "  sw $fp, 0($sp)\n");

    fprintf(output, "  jal %s\n", code->func_name);
    
    fprintf(output, "  move $sp, $fp\n");
    fprintf(output, "  lw $fp, 0($sp)\n");
    fprintf(output, "  lw $ra, 4($sp)\n");

    fprintf(output, "  add $sp, $sp, %d\n", 8+4*arg_num);
    arg_num=0;

    if(code->result.kind==IR_POINTER){
        if(code->result.temp_flag==1){
            fprintf(output, "  lw $t0, %d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "  lw $t0, %d($fp)\n", -4*code->result.var_name);
        }
        fprintf(output, "  sw $v0, 0($t0)\n");
    }
    else if(code->result.kind==IR_VARIABLE){
        fprintf(output, "  sw $v0, ");
        if(code->result.temp_flag==1){
            fprintf(output, "%d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "%d($fp)\n", -4*code->result.var_name);
        }
    }
}
void mips_param(intercode* code){
    fprintf(output, "  lw $t0, %d($fp)\n", 4+4*code->result.var_name);
    fprintf(output, "  sw $t0, %d($fp)\n", -4*code->result.var_name);
}
void mips_read(intercode* code){
    fprintf(output, "  addi $sp, $sp, -4\n");
    fprintf(output, "  sw $ra, 0($sp)\n");
    fprintf(output, "  jal read\n");
    fprintf(output, "  lw $ra, 0($sp)\n");
    fprintf(output, "  addi $sp, $sp, 4\n");

    if(code->result.kind==IR_POINTER){
        if(code->result.temp_flag==1){
            fprintf(output, "  lw $t0, %d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "  lw $t0, %d($fp)\n", -4*code->result.var_name);
        }
        fprintf(output, "  sw $v0, 0($t0)\n");
    }
    else if(code->result.kind==IR_VARIABLE){
        fprintf(output, "  sw $v0, ");
        if(code->result.temp_flag==1){
            fprintf(output, "%d($fp)\n", -4*code->result.u.var_no);
        }
        else{
            fprintf(output, "%d($fp)\n", -4*code->result.var_name);
        }
    }
}
void mips_write(intercode* code){
    mips_reg(code->result,"a",0);

    fprintf(output, "  addi $sp, $sp, -4\n");
    fprintf(output, "  sw $ra, 0($sp)\n");
    fprintf(output, "  jal write\n");
    fprintf(output, "  lw $ra, 0($sp)\n");
    fprintf(output, "  addi $sp, $sp, 4\n");
}

void mips_print(intercode* code){
    if(code->kind==IR_LABEL){
        fprintf(output, "label%d:\n", code->label);
    }
    else if(code->kind==IR_FUNCTION){
        fprintf(output, "%s:\n", code->func_name);
        fprintf(output, "  move $fp, $sp\n");
    }
    else if(code->kind==IR_ASSIGN){
        mips_assign(code);
    }
    else if(code->kind==IR_ADD){
        mips_exp(code);
    }
    else if(code->kind==IR_SUB){
        mips_exp(code);
    }
    else if(code->kind==IR_MUL){
        mips_exp(code);
    }
    else if(code->kind==IR_DIV){
        mips_exp(code);
    }
    else if(code->kind==IR_GOTO){
        fprintf(output, "  j label%d\n", code->label);
    }
    else if(code->kind==IR_IF){
        mips_if(code);
    }
    else if(code->kind==IR_RETURN){
        mips_return(code);
    }
    else if(code->kind==IR_DEC){
        fprintf(output, "  addi $sp, $sp, %d\n", -code->size);
    }
    else if(code->kind==IR_ARG){
        mips_arg(code);
    }
    else if(code->kind==IR_CALL){
        mips_call(code);
    }
    else if(code->kind==IR_PARAM){
        mips_param(code);
    }
    else if(code->kind==IR_READ){
        mips_read(code);
    }
    else if(code->kind==IR_WRITE){
        mips_write(code);
    }
    else {
        assert(0);
    }
}
void mips_generate(intercodes* codes){
    mips_init();
    arg_num=0;
    intercode_list* irl=codes->head;
    while(irl){
        mips_print(irl->code);
        irl=irl->next;
    }
}