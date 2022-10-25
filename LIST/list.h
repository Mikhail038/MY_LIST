
#include "MYassert.h"

#define Error 1

typedef double Telem;

typedef struct
{
    Telem  data;
    int    next;
    int    prev;
} SElem;

typedef struct
{
    int    free;
    int    size;
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

int insert_before (SList* List, Telem data, int number, int* address);

int insert_after (SList* List, Telem data, int number, int* address);


