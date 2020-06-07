#include <stdio.h>
#include "intercode.h"

void mips_init();
void mips_exp(intercode* code);
void mips_assign(intercode* code);
void mips_if(intercode* code);
void mips_return(intercode* code);
void mips_arg(intercode* code);
void mips_call(intercode* code);
void mips_param(intercode* code);
void mips_reg(operand op,char* reg_name,int reg_num);
void mips_res(operand op,char* reg_name1,int reg_num1,char* reg_name2,int reg_num2);
void mips_read(intercode* code);
void mips_write(intercode* code);
void mips_print(intercode* code);
void mips_generate();

int arg_num;