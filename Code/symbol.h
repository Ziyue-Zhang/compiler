#ifndef SYMBOLH
#define SYMBOLH 

#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include"intercode.h"

#define SYMBOL_VOID 0
#define SYMBOL_INT 1
#define SYMBOL_FLOAT 2
#define SYMBOL_STRUCT 3
#define SYMBOL_OTHER 4

typedef struct symbol_t symbol;
typedef struct symbol_list_t symbol_list;
typedef struct param_list_t param_list;
typedef struct array_list_t array_list;
typedef struct struct_list_t struct_list;
typedef struct symbol_tbl_t symbol_tbl;
typedef struct field_list_t field_list;

field_list* field_head;
field_list* field_cur;


struct param_list_t{
    int param_num;
    symbol_list* list;
};

struct array_list_t{
    int array_size;
    array_list* next;
};

struct struct_list_t{
    symbol_list* list;
    int struct_size;
};

struct symbol_list_t{
    symbol* entry;
    symbol_list* next;
};

struct symbol_t{
    int type;
    char* name;
    int array_flag;             //是否是数组
    int func_flag;              //是否是函数
    int struct_flag;            //是否是结构体的定义
    int func_def_flag;          //函数是否被定义过
    int left_value_flag;
    int lineno;
    int dim;                    //数组维度
    int size;
    int struct_offset;                    
    array_list* array_head;
    param_list* param_head;
    struct_list* struct_head;

    int entry_name;
    int param_flag;
};

struct symbol_tbl_t{
    symbol* entry;
    symbol_tbl* next;
};

struct field_list_t{
    symbol_tbl* symbol_tbl_head; 
    field_list* next;
};

int symbol_num;

void symbol_init();
void free_param();
void free_array();
void free_struct();
void free_symbol();
void free_symbol_tbl();
void free_field();
void add_param();
void add_array();
void add_struct();
int same_param(param_list* p1, param_list*p2);
int same_array(array_list* p1, array_list* p2);
int same_struct(struct_list* p1, struct_list* p2);
int same_type(symbol* entry1, symbol* entry2);
symbol* add_entry(int type,char* name,int array_flag,int func_flag,int struct_flag,int func_def_flag,int lineno);
int add_symbol(symbol* entry,int struct_entry);
int add_struct_symbol(symbol* entry,int struct_entry);
symbol* find_symbol(char *name);
symbol* find_symbol_headfield(char *name);
symbol* find_symbol_curfield(char *name);
symbol* find_symbol_struct(char *name);
void field_push();
void field_pop();
void check_func_def();
void symbol_print();

#endif