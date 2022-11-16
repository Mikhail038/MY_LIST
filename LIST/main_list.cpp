//MAIN FILE

#include <stdio.h>

#include "list.h"

#include "MYassert.h"

int main ()
{
    SList List = {};

    int size = 10;

    list_constructor (&List, size);

    int address = 0;
    int buf = 0;
    int tail = 0;

    //dump_list (&List);

    insert_after  (&List, 1, 0, &buf);

    insert_before (&List, 6, 0, &tail);

    insert_before (&List, 2, tail, &buf);

    insert_before (&List, 3, tail, &buf);

    insert_before (&List, 4, tail, &address);

    insert_before (&List, 5, tail, &buf);

    insert_after  (&List, 7, tail, &buf);

    pop_from_list (&List, address);

    pop_from_list (&List, buf);

    //dump_list (&List);

    //sort_list (&List);

    dump_list (&List);

    list_deconstructor (&List);

    return 0;
}
