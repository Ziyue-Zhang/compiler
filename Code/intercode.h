#ifndef INTERCODEH
#define INTERCODEH 

#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>

enum {IR_VARIABLE, IR_CONSTANT, IR_ADDRESS, IR_POINTER};

enum {IR_LABEL,IR_FUNCTION,IR_ASSIGN,IR_ADD,IR_SUB,IR_MUL,IR_DIV,
    IR_GOTO,IR_IF,IR_RETURN,IR_DEC,IR_ARG,IR_CALL,IR_PARAM,IR_READ,IR_WRITE,IR_VOID};

enum {IR_GT,IR_LT,IR_GE,IR_LE,IR_EQ,IR_NEQ};

typedef struct operand_t operand;
typedef struct intercode_t intercode;
typedef struct intercode_list_t intercode_list;
typedef struct intercodes_t intercodes;
int temp_num;
int label_num;

FILE* output;

struct operand_t{
    int kind;
    int temp_flag;
    union {
        int var_no;
        int value;
    } u;
    int var_name;
};

struct intercode_t {
    int kind;
    operand result, op1, op2;
    char* func_name;
    int label;
    int relop;
    int size;
};

struct intercode_list_t {
    intercode* code;
    intercode_list* next;
};

struct intercodes_t {
    intercode_list* head;
    intercode_list* tail;
};

void intercode_init();
intercode* intercode_new(int kind);
void intercode_op_left(operand op);
void intercode_op_right(operand op);
void intercodes_add(intercodes* irs,intercode *ir);
void intercodes_merge(intercodes* ir1,intercodes* ir2);
void intercodes_print(intercodes* irs);
void intercode_print(intercode* ir);
void intercode_label(intercode* ir);
void intercode_function(intercode* ir);
void intercode_assign(intercode* ir);
void intercode_add(intercode* ir);
void intercode_sub(intercode* ir);
void intercode_mul(intercode* ir);
void intercode_div(intercode* ir);
void intercode_goto(intercode* ir);
void intercode_if(intercode* ir);
void intercode_return(intercode* ir);
void intercode_dec(intercode* ir);
void intercode_arg(intercode* ir);
void intercode_call(intercode* ir);
void intercode_param(intercode* ir);
void intercode_read(intercode* ir);
void intercode_write(intercode* ir);
int new_temp();
int get_temp();
void reset_temp();
int new_label();

#endif