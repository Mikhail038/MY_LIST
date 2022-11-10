
#include <stdlib.h>

#include "list.h"
#include "MYassert.h"

#define Error 1

#define StdDataSize 100
#define ZERO_ELEMENT 0
#define InvalidNumber -1
#define BITS_IN_BYTE 8

#define NOT_SORTED 0
#define SORTED 1


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

#define FREE_COLOR "dcd1e0"
#define DATA_COLOR "8ff7d6"
#define ZERO_COLOR "baf763"
/*
    fprintf (file,  "digraph { subgraph { rank=same \n");

    for (int i = 1; i < list->size; i++)
    {
        fprintf (file,
            R"(
                node_%02d
                [
                    style="filled",
                    fillcolor="#%X"
                    shape=plaintext,
                    label=
                    <
                        <table border="0" cellborder="1" cellspacing="0">
                            <tr>
                                <td colspan="2">Node #%d</td>
                            </tr>
                            <tr>
            )", i, (unsigned) list->list[i].status, i);

        DataPrinter (file, &(list->list[i].data));

        fprintf(file,
            R"(
                            </tr>
                            <tr>
                                <td port="prev_out" > prev: </td>
                                <td port="prev_in" > %d    </td>
                            </tr>
                            <tr>
                                <td port="next_in" > next: </td>
                                <td port="next_out"> %d    </td>
                            </tr>
                        </table>
                    >
                ];
            )", list->list[i].prev, list->list[i].next);
    }

    for (int i = 1; i < list->size; i++)
    {
        if (list->list[i].next > 0)
        {
            fprintf (file,  "\nnode_%02d:<next_out> -> node_%02d:<next_in>;\n",
                    i, list->list[i].next);
        }

        if (list->list[i].prev > 0)
        {
            fprintf (file,  "\n node_%02d:<prev_out> -> node_%02d:<prev_in>;\n",
                    i, list->list[i].prev);
        }
    }
    fprintf (file, "}\n");

    if (list->list[0].next == 0 || list->list[0].prev == 0)
    {
        fprintf (file, "node_00 [shape=rectangle, label=\"Terminating node\n(Node #0)\", style=\"filled\", fillcolor=grey];\n");
    }

    if (list->empty_start <= 0)
    {
        fprintf (file, "node_empty_start [shape=rectangle, label=\"Terminating free elements node\n(Node #-1)\", style=\"filled\", fillcolor=grey];\n"
        "empty_start [shape=rectangle]; empty_start  -> node_empty_start;\n");
    }

    else
    {
        fprintf (file, "empty_start [shape=rectangle]; empty_start -> node_%02d;\n", list->empty_start);
    }

    fprintf (file,  "head [shape=rectangle]; head -> node_%02d;\n"
                    "tail [shape=rectangle]; tail -> node_%02d;\n}",
           list->list[0].next, list->list[0].prev);

    fclose (file);

    void DataPrinter (FILE* file, ll_type* data_to_print)
{
    fprintf (file,
            R"(
                <td>mat:    %d</td>
                <td>kanava: %d</td>
            )",
            data_to_print->mat, data_to_print->kanava);
    return;
}

*/
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
    int address = ZERO_ELEMENT;
    for (int counter = 0; counter < List->size; ++counter)
    {
        MCE (address == List->ArrData[List->ArrData[address].next].prev, IncorrectLinks);
        address = List->ArrData[address].next;
    }
    MCE (List->capacity > 0, CapasityLessZero);
    MCE (List->size <= List->capacity, SizeMoreCapacity);

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
    printf (KYLW "capacity %d\n" KNRM, List->capacity);
    printf (KYLW "free %d\n" KNRM, List->free);


    printf (Kbright KWHT "[%02d] %12lg |%2d|%2d|\n" KNRM, 0,
            List->ArrData[ZERO_ELEMENT].data, List->ArrData[ZERO_ELEMENT].prev, List->ArrData[ZERO_ELEMENT].next);

    printf (KRED Kunderscore "In memory:\n" KNRM KRED);
    for (int counter = 1; counter <= List->capacity; ++counter)
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

    make_gv_list (List);

    draw_gv_list ();

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
    List->sort_marker = NOT_SORTED;

    List->ArrData = (SElem*) calloc (Size + 1, sizeof (*List->ArrData));

    TElem poison = 0;
    if (sizeof (TElem) == sizeof (double))
    {
        do_dead_ded (&poison);
    }

    for (int counter = 0; counter <= Size; ++counter)
    {
        List->ArrData[counter].data = poison;
    }

    List->ArrData[ZERO_ELEMENT].next = 0;
    List->ArrData[ZERO_ELEMENT].prev = 0;

    List->free = 1;

    for (int counter = List->free; counter < Size; ++counter)
    {
        List->ArrData[counter].prev = InvalidNumber;
        List->ArrData[counter].next = counter + 1;
    }

    List->ArrData[Size].prev = InvalidNumber;
    List->ArrData[Size].next = ZERO_ELEMENT;

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

    *address = ZERO_ELEMENT;

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

    MCA (address != ZERO_ELEMENT, Error);

    List->ArrData[List->ArrData[address].prev].next = List->ArrData[address].next;
    List->ArrData[List->ArrData[address].next].prev = List->ArrData[address].prev;

    List->ArrData[address].prev = InvalidNumber;
    List->ArrData[address].next = List->free;

    TElem poison = 0;

    if (sizeof (TElem) == sizeof (double))
    {
        do_dead_ded (&poison);
    }
    List->ArrData[address].data = poison;


    List->free = address;

    List->size--;

    List->sort_marker = NOT_SORTED;

    return 0;
}

int insert_before (SList* List, TElem data, int anchor, int* address)
{
    MCA (list_validator (List) == 0, Error);
    MCA (anchor_validator (List, *address) == 0, Error);

    //printf ("%d %d %d\n", List->size, List->capacity, List->free); dump_list (List);
    if (List->free == ZERO_ELEMENT)
    {
        make_list_bigger (List);
    }
    //printf ("%lg  anch %d\n", data, anchor);

    *address = List->free;
    List->free = List->ArrData[*address].next;

    List->ArrData[*address].next = anchor;
    List->ArrData[*address].prev = List->ArrData[anchor].prev;

    List->ArrData[anchor].prev  = *address;

    List->ArrData[List->ArrData[*address].prev].next = *address;


    List->ArrData[*address].data = data;


    List->size++;

    List->sort_marker = NOT_SORTED;

    return 0;
}

int insert_after  (SList* List, TElem data, int anchor, int* address)
{
    MCA (list_validator (List) == 0, Error);
    MCA (anchor_validator (List, *address) == 0, Error);

    if (List->free == ZERO_ELEMENT)
    {
        make_list_bigger (List);
    }

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

int make_list_bigger (SList* List)
{
    //printf ("before |\n");
    //dump_list (List);

    MCA (list_validator (List) == 0, Error);

    int address = List->size + 1;

    List->ArrData = (SElem*) realloc (List->ArrData, (List->capacity * coef + 1) * sizeof (SElem));
    List->capacity *= coef;

    List->free = List->size + 1;

    TElem poison = 0;
    if (sizeof (TElem) == sizeof (double))
    {
        do_dead_ded (&poison);
    }

    for (int counter = 1; counter <= List->capacity - List->size; ++counter)
    {
        List->ArrData[address].next = address + 1;
        List->ArrData[address].prev = InvalidNumber;
        List->ArrData[address].data = poison;

        ++address;
    }
    List->ArrData[List->capacity].next = ZERO_ELEMENT;

    //printf ("after|\n");
    //dump_list (List);
    return 0;
}

int sort_list (SList* List)
{
    SElem* NewArray = (SElem*) calloc (List->capacity + 1, sizeof (*NewArray));

    int address = ZERO_ELEMENT;
    int counter = 0;

    for (; counter <= List->size; counter++)
    {
        NewArray[counter].data = List->ArrData[address].data;

        if (counter == 1)
        {
            NewArray[counter].prev = ZERO_ELEMENT;
        }
        else
        {
            NewArray[counter].prev = counter - 1;
        }

        if (counter == List->size)
        {
            NewArray[counter].next = ZERO_ELEMENT;
        }
        else
        {
            NewArray[counter].next = counter + 1;
        }

        address = List->ArrData[address].next;
    }

    NewArray[ZERO_ELEMENT].prev = counter - 1;
    NewArray[ZERO_ELEMENT].next = 1;


    address = List->free;
    List->free = counter;

    for (; counter <= List->capacity; counter++)
    {
        NewArray[counter] = List->ArrData[address];

        if (counter == List->capacity)
        {
            NewArray[counter].next = ZERO_ELEMENT;
        }
        else
        {
            NewArray[counter].next = counter + 1;
        }

        address = List->ArrData[address].next;
    }

    free (List->ArrData);

    List->ArrData = NewArray;

    return 0;
}

void make_gv_list (SList* List)
{
    FILE* gvInputFile = fopen ("gvList.dot", "w");
    MCE (gvInputFile != NULL, GVCannotOpenDotFile);

    int address = ZERO_ELEMENT;
    char* BufStr = NULL;


    fprintf (gvInputFile,   "digraph { subgraph { rank=same \n"
                            //"subgraph {\n"
                            //"\t""graph [dpi = 1000];\n"
                            // "\t""rankdir = LR;\n"
                            // "\t""ranksep = 1;\n"
                            //"\t""splines=\"spline\"\n"
                            //"\t""splines = ortho\n"

                            // "node[color=\"black\", fontsize=14];\n"
                            // "edge[color=\"blue\", fontcolor=\"blue\", fontsize=12];\n"
                            // "LIST [shape=\"Mrecord\", style=\"rounded\", style=\"filled\", fillcolor=\"#FF0EDD\","
                            // "label = \"{size: %d| capacity: %d |<fr> free: %d | <ar> Array pointer %p}\"];\n"
                            // , List->size, List->capacity, List->free, List->ArrData
                            );

//
//     BufStr = gv_make_data (List, ZERO_ELEMENT);
//     fprintf (gvInputFile,
//                             "ELEM_0[shape=\"Mrecord\", style=\"rounded\", style=\"filled\", fillcolor=\"#FF0EDD\", label = \"""%s""\"];\n"
//                             "ELEM_0:<pr> ->ELEM_%d:<ad> [weight = 1, color = \"lightcyan3\", arrowhead = empty, style = dashed];\n"
//                             "LIST:<ar> ->ELEM_0:<ad> [weight = 1, color = \"pink\", arrowhead = open, style = dashed];\n"
//                             //"ELEM_0:<nx> ->ELEM_0:<ad>;\n"
//                             , BufStr
//                             , List->ArrData[ZERO_ELEMENT].prev);
//     free (BufStr);

    for (int counter = 0; counter <= List->capacity; counter++)
    {
        fprintf (gvInputFile,
                R"(
                    node_%d
                    [
                        style="filled",
                        fillcolor="#)", counter);

        if (counter == 0)
        {
            fprintf (gvInputFile, ZERO_COLOR);
        }
        else if (List->ArrData[counter].prev != InvalidNumber)
        {
            fprintf (gvInputFile, DATA_COLOR);
        }
        else
        {
            fprintf (gvInputFile, FREE_COLOR);
        }

        if ((List->ArrData[counter].prev != InvalidNumber) && (counter != ZERO_ELEMENT))
        {
            fprintf (gvInputFile,
                    R"("
                            shape=plaintext,
                            label=
                            <
                                <table border="0" cellborder="1" cellspacing="0">
                                    <tr>
                                        <td bgcolor = "#ffffff" colspan="2">Node #%d</td>
                                    </tr>
                                    <tr>)", counter);
        }
        else
        {
            fprintf (gvInputFile,
                    R"("
                            shape=plaintext,
                            label=
                            <
                                <table border="0" cellborder="1" cellspacing="0">
                                    <tr>
                                        <td colspan="2">Node #%d</td>
                                    </tr>
                                    <tr>)", counter);
        }

        print_node (List->ArrData[counter].data, gvInputFile);

        if (List->ArrData[counter].prev != InvalidNumber)
        {
            fprintf(gvInputFile,
                R"(
                                </tr>
                                <tr>
                                    <td bgcolor = "#5fbcfa" port="prev_out" > prev: </td>
                                    <td bgcolor = "#5fbcfa" port="prev_in" > %d    </td>
                                </tr>
                                <tr>
                                    <td bgcolor = "#f27798" port="next_in" > next: </td>
                                    <td bgcolor = "#f27798" port="next_out"> %d    </td>
                                </tr>
                            </table>
                        >
                    ];
                )", List->ArrData[counter].prev, List->ArrData[counter].next);
        }
        else
        {
            fprintf(gvInputFile,
            R"(
                            </tr>
                            <tr>
                                <td  port="prev_out" > prev: </td>
                                <td  port="prev_in" > %d    </td>
                            </tr>
                            <tr>
                                <td bgcolor = "#f27798" port="next_in" > next: </td>
                                <td bgcolor = "#f27798" port="next_out"> %d    </td>
                            </tr>
                        </table>
                    >
                ];
            )", List->ArrData[counter].prev, List->ArrData[counter].next);
        }

    }

    for (int counter = 0; counter <= List->capacity; counter++)
    {
        if (List->ArrData[counter].next > 0)
        {
            fprintf (gvInputFile,  "\n node_%d:<next_out> -> node_%d:<next_in>;\n",
                    counter, List->ArrData[counter].next);
        }

        if ((List->ArrData[counter].prev >= 0) && (counter != ZERO_ELEMENT))
        {
            fprintf (gvInputFile,  "\n node_%d:<prev_out> -> node_%d:<prev_in>;\n",
                    counter, List->ArrData[counter].prev);
        }

        if ((List->ArrData[counter].prev == InvalidNumber) && (List->ArrData[counter].next == ZERO_ELEMENT))
        {
            fprintf (gvInputFile,  "\n node_%d:<next_out> -> node_%d:<next_in>;\n",
                    counter, List->ArrData[counter].next);
        }

    }


    fprintf (gvInputFile, "}\n");

    fprintf (gvInputFile, "FREE [shape=rectangle]; FREE -> node_%d;\n", List->free);

    fprintf (gvInputFile, "ZERO_ELEMENT [shape=rectangle, fillcolor = \"#" DATA_COLOR "\"]; ZERO_ELEMENT -> node_%d;\n", ZERO_ELEMENT);

    fprintf (gvInputFile, "HEAD [shape=rectangle, fillcolor = \"#" DATA_COLOR "\"]; HEAD -> node_%d;\n", List->ArrData[ZERO_ELEMENT].next);

    fprintf (gvInputFile, "TAIL [shape=rectangle, fillcolor = \"#" DATA_COLOR "\"]; TAIL -> node_%d;\n", List->ArrData[ZERO_ELEMENT].prev);




    fprintf (gvInputFile, "}\n");



//     for (int counter = 1; counter <= List->size; counter++)
//     {
//         address = List->ArrData[address].next;
//         BufStr = gv_make_data (List, address);
//
//         fprintf (gvInputFile,
//                             "ELEM_%d[shape=\"Mrecord\", style=\"rounded\", style=\"filled\", fillcolor=\"lightgrey\", label = \"""%s""\"];\n"
//                             "ELEM_%d:<nx> ->ELEM_%d:<ad> [weight = 5];\n"
//                             "ELEM_%d:<pr> ->ELEM_%d:<ad> [weight = 1, color = \"lightcyan3\", arrowhead = empty, style = dashed];\n",
//                             address, BufStr,
//                             List->ArrData[address].prev, address,
//                             address, List->ArrData[address].prev);
//
//         free (BufStr);
//     }

    // address = List->ArrData[address].next;
    // fprintf (gvInputFile,
    //                     "ELEM_%d:<nx> ->ELEM_%d:<ad> [weight = 1000];\n",
    //                     List->ArrData[address].prev, address);

//Waaghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
//
//     address = List->free;
//
//     for (; address != ZERO_ELEMENT;)
//     {
//         BufStr = gv_make_data (List, address);
//
//         fprintf (gvInputFile,
//                             "ELEM_%d[shape=\"Mrecord\", style=\"rounded\", style=\"filled\", fillcolor=\"sienna\", label = \"""%s""\"];\n"
//                             "ELEM_%d:<nx> ->ELEM_%d:<ad> [weight = 1];\n"
//                             //"ELEM_%d:<pr> ->ELEM_%d:<ad> [weight = 1, color = \"lightcyan3\", arrowhead = empty, style = dashed];\n",
//                             ,address, BufStr,
//                             address, List->ArrData[address].next);
//
//         free (BufStr);
//         address = List->ArrData[address].next;
//     }

//Waaghhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh

    // fprintf (gvInputFile,
    //                     //"FREE[shape=\"Mrecord\", style=\"rounded\", style=\"filled\", fillcolor=\"#FF0EDD\", label = \"{ FREE | <nx> %d}\"];\n"
    //                     "LIST:<fr>->ELEM_%d:<ad> [weight = 1];\n"
    //                     //"ELEM_%d:<pr> ->ELEM_%d:<ad> [weight = 1, color = \"lightcyan3\", arrowhead = empty, style = dashed];\n",
    //                     //, List->free
    //                     , List->free);


    //fprintf (gvInputFile,  "}");

    // fprintf (gvInputFile,  "head [shape=rectangle]; head -> node_%d;\n"
    //             "tail [shape=rectangle]; tail -> node_%d;\n}",
    //     List->ArrData[1], list->list[0].prev);


    fclose (gvInputFile);

    return;
}

void print_node (TElem data, FILE* gvInputFile)
{
        fprintf (gvInputFile,R"(
                <td>   data:  </td>
                <td>   %lg    </td>
            )", data);
}

char* gv_make_data (SList* List, int address)
{
    char* ReturnValue = (char*) calloc (StdDataSize + sizeof (TElem) + 3 * sizeof(int), sizeof (*ReturnValue)); //!
    sprintf (ReturnValue, "{{<ad> Node#%d |" Elem_print "| {<pr> prev: %d  | <nx> next: %d} } }", address, List->ArrData[address].data, List->ArrData[address].prev, List->ArrData[address].next);
    return ReturnValue;
}

void draw_gv_list (void)
{
    system ("dot -Tpng gvList.dot -o gvList.png");

    system ("xdg-open gvList.png");
}

//TODO ?? maybe tmp folder mktmp

//TODO variable filename


// <table>
//   <tr>
//     <td>...</td>
//   </tr>
// </table>
