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

    insert_after  (&List, 1, 0, &buf);

    insert_before (&List, 6, 0, &buf);

    insert_before (&List, 2, 2, &buf);

    insert_before (&List, 3, 2, &buf);

    insert_before (&List, 4, 2, &address);

    insert_before (&List, 5, 2, &buf);

    insert_after  (&List, 7, 2, &buf);

    pop_from_list (&List, address);

    pop_from_list (&List, buf);



    dump_list (&List);

    list_deconstructor (&List);

    return 0;
}
