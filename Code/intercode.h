#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>

typedef struct operand_t operand;

struct operand_t{
    enum { VARIABLE, CONSTANT, ADDRESS, NONE } kind;
    union {
        int var_no;
        int value;
    } u;
};