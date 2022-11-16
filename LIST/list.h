
#include "MYassert.h"
// TODO print differnet types

const int coef = 2;

typedef struct
{
    int    date;
    int    time;
    char   name;
} SStruct;

typedef int TElem;

typedef struct
{
    TElem  data;
    int    next;
    int    prev;
} SElem;

typedef struct
{
    int    free;
    int    size;
    char   sort_marker;
    int    capacity;
    SElem* ArrData;
} SList;



void do_dead_ded (double* argument);

void dump_errors (void);

int list_validator (SList* List);

int anchor_validator (SList* List, int anchor);

void dump_list (SList* List);

int list_constructor (SList* List, int Size);

int list_deconstructor (SList* List);

int pop_from_list (SList* List, int address);

int insert_before (SList* List, TElem data, int number, int* address);

int insert_after (SList* List, TElem data, int number, int* address);

int make_list_bigger (SList* List);

int sort_list (SList* List);

void make_gv_list (SList* List);

char* gv_make_data (SList* List, int address);

void draw_gv_list (void);

void print_gv_node (TElem data, FILE* gvInputFile);

void print_node (TElem data);
