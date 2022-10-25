
#include <stdlib.h>

#include "list.h"
#include "MYassert.h"


#define InvalidNumber -1
#define BITS_IN_BYTE 8

static int Errors = 0;

#define DEF_ERR(e_num, num) e_num = 1 << num,

enum EErrors
{
    #include "errors.h"
};
#undef DEF_ERR

typedef struct
{
    EErrors  num;
    char     name[60];
} StructError;

#define DEF_ERR(e_num, num) {e_num, #e_num},

static const StructError ArrStructErr[] =
{
    #include "errors.h"
};
#undef DEF_ERR
//////////////////////////////////////////////////////////////////////////////////////////////

void dump_errors (void)
{
    printf ("==ERROR DUMP==\n");
    int counter = 1;
    do
    {
        if (Errors & ArrStructErr[counter].num)
        {
            printf (Kbright KRED "%s\n" KNRM, ArrStructErr[counter].name);
        }
        ++counter;
    } while (counter < (sizeof(ArrStructErr) / sizeof (StructError))) ;

    return;
}

int list_validator (SList* List)
{
    int address = 0;
    for (int counter = 0; counter < List->size; ++counter)
    {
        MCE (address == List->ArrData[List->ArrData[address].next].prev, IncorrectLinks);
        address = List->ArrData[address].next;
    }

    //printf ("!!%d!!\n", Errors);
    if (Errors != 0)
    {
        dump_list (List);
        dump_errors ();

        return Error;
    }

    return 0;
}

int anchor_validator (SList* List, int anchor)
{
    MCE (anchor <= List->capacity, AnchorMoreCapacity);
    MCE (List->ArrData[anchor].prev != InvalidNumber, NotInvalidFreedData);

    if (Errors != 0)
    {
        dump_list (List);
        dump_errors ();

        return Error;
    }

    return 0;
}

void dump_list (SList* List)
{
    printf (KGRN "===LIST DUMP=============\n");
    printf ("%s %s\n\n" KNRM , __DATE__, __TIME__);

    printf (KYLW "size %d\n" KNRM, List->size);
    printf (KYLW "free %d\n" KNRM, List->free);


    printf (Kbright KWHT "[%02d] %12lg |%2d|%2d|\n" KNRM, 0,
            List->ArrData[0].data, List->ArrData[0].prev, List->ArrData[0].next);

    printf (KRED Kunderscore "In memory:\n" KNRM KRED);
    for (int counter = 1; counter < List->capacity; ++counter)
    {
        printf ("[%02d] %12lg |%2d|%2d|\n", counter,
        List->ArrData[counter].data, List->ArrData[counter].prev, List->ArrData[counter].next);
    }

    int address = List->ArrData[0].next;

    printf (KBLU Kunderscore "In logic way:\n" KNRM KBLU);
    for (int counter = 1; counter <= List->size; ++counter)
    {
        printf ("[%02d] %12lg |%2d|%2d|\n", address,
        List->ArrData[address].data, List->ArrData[address].prev, List->ArrData[address].next);

        address = List->ArrData[address].next;
    }


    printf (KNRM);
    return;
}

void do_dead_ded (double* argument)
{
    ((unsigned char*) argument)[0] = 16 * 13 + 14;
    ((unsigned char*) argument)[1] = 16 * 10 + 13;
    ((unsigned char*) argument)[2] = 16 * 13 + 14;
    ((unsigned char*) argument)[3] = 16 * 13 + 8;
    ((unsigned char*) argument)[4] = 16 * 11 + 10;
    ((unsigned char*) argument)[5] = 16 * 13 + 15;
    ((unsigned char*) argument)[6] = 16 * 0 + 0;
    ((unsigned char*) argument)[7] = 16 * 0 + 13;

    return;
}

int list_constructor (SList* List, int Size)
{
    List->capacity = Size;
    List->size = 0;

    List->ArrData = (SElem*) calloc (Size, sizeof (*List->ArrData));

    for (int counter = 0; counter < Size; ++counter)
    {
        do_dead_ded (&(List->ArrData[counter].data));
    }

    List->ArrData[0].next = 0;
    List->ArrData[0].prev = 0;

    List->free = 1;

    for (int counter = List->free; counter < Size - 1; ++counter)
    {
        List->ArrData[counter].prev = InvalidNumber;
        List->ArrData[counter].next = counter + 1;
    }

    List->ArrData[Size - 1].prev = InvalidNumber;
    List->ArrData[Size - 1].next =  0;

    // for (int counter = 0; counter < sizeof (Telem); ++counter)
    // {
    //     printf ("%02X", ((unsigned char*) &(List->ArrData[0].data))[counter]);
    // }

    return 0;
}

int list_deconstructor (SList* List)
{
    free (List->ArrData);

    return 0;
}

int find_address (SList* List, int number, int* address)
{
    MCA (list_validator (List) == 0, Error);
    //MCA (List->size > 0, Error);

    *address = 0;

    if (number <= List->size / 2)
    {
        for (int counter = 0; counter < number; ++counter)
        {
            *address = List->ArrData[*address].next;
        }
    }
    else
    {
        for (int counter = 0; counter < List->size - number + 1; ++counter)
        {
            *address = List->ArrData[*address].prev;
        }
    }

    return 0;
}

int pop_from_list (SList* List, int address)
{
    MCA (list_validator (List) == 0, Error);
    MCA (anchor_validator (List, address) == 0, Error);

    MCA (address != 0, Error);

    List->ArrData[List->ArrData[address].prev].next = List->ArrData[address].next;
    List->ArrData[List->ArrData[address].next].prev = List->ArrData[address].prev;

    List->ArrData[address].prev = InvalidNumber;
    List->ArrData[address].next = List->free;

    Telem poison = 0;

    if (sizeof (Telem) == sizeof (double))
    {
        do_dead_ded (&poison);
    }
    List->ArrData[address].data = poison;


    List->free = address;

    List->size--;


    return 0;
}

int insert_before (SList* List, Telem data, int anchor, int* address)
{
    MCA (list_validator (List) == 0, Error);
    MCA (anchor_validator (List, *address) == 0, Error);


    //printf ("%lg  anch %d\n", data, anchor);

    *address = List->free;

    List->free = List->ArrData[*address].next;

    List->ArrData[*address].next = anchor;
    List->ArrData[*address].prev = List->ArrData[anchor].prev;

    List->ArrData[anchor].prev  = *address;

    List->ArrData[List->ArrData[*address].prev].next = *address;


    List->ArrData[*address].data = data;


    List->size++;

    return 0;
}

int insert_after  (SList* List, Telem data, int anchor, int* address)
{
    MCA (list_validator (List) == 0, Error);
    MCA (anchor_validator (List, *address) == 0, Error);

    *address = List->free;
    List->free = List->ArrData[*address].next;


    List->ArrData[*address].prev = anchor;
    List->ArrData[*address].next = List->ArrData[anchor].next;

    List->ArrData[anchor].next  = *address;

    List->ArrData[List->ArrData[*address].next].prev = *address;

    List->ArrData[*address].data = data;

    List->size++;

    return 0;
}



