#include "stack.h"

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                      //    Turn on the asserts    //
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Assert (int expresion, FILE * file, const unsigned int line,
            const char * function, const int numb_er, const char * name_er)
{
    //pointers on function, name_er, file --- is it impossible, that they would be Null??? :|
    if (numb_er == ER_POP_SIZENULL)
        Error_Mass[0] = 1;
    printf ("%d\n", Error_Mass[0]);

    if (expresion == FALSE)
    {
        fseek (file, 0L, SEEK_SET);
        fprintf (file, "Assertion failed\n");
        fprintf (file, "Problem on %u line\n", line);
        fprintf (file, "In the function     %s\n", function);
        fprintf (file, "Code of error:      %d\n", numb_er);
        fprintf (file, "Name of error:      %s\n\n", name_er);
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                           // opening the Logfile
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void LogOpen (Stack * stk)
{
    Logfile = fopen ("Logfile.txt", "w");
    MyAss(Logfile != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__,
          NULL_PTR_FILE, PR_NULL_PTR_FILE, stk);
    fprintf (Logfile, "STATUS of the Stack:     good\n"
                      "Logfile is opened successfully\n"
                      "////////////////////////////////\n\n");
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                         //  closing the Logfile
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void LogClose (Stack * stk)
{
    stk = NULL; // MY ASS DOESN'T WORK CORRECTLY!!!!!!!!
    MyAss(Logfile != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__,
          NULL_PTR_FILE, PR_NULL_PTR_FILE, stk);

    fprintf (Logfile, "STACK finished the work successfully\n"
                      "Logfile is closed successfully\n"
                      "////////////////////////////////////\n\n");
    fclose (Logfile);

}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                          // Hash calculating
                         // It is very simple
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned long HashCalc (Stack * stk)
{
    unsigned long crc = 0;
    unsigned long numb = sizeof(data_t)*(stk -> capacity);
    char * Data = (char *)(stk -> data);

    for (int i = 0; i < numb; i++)
    {
        crc += (*(Data + i)) * i;
    }
    crc += (unsigned long)(stk -> capacity + stk -> size);
    return crc;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                               // Hash check
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int HashCheck (Stack * stk)
{
    if (stk -> hash == HashCalc (stk))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackCtor (Stack * stk)
{
    LogOpen (stk);
    MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__,
          NULL_PTR_STK, PR_NULL_PTR_STK, stk);
    stk -> canary1 = BEZLAB;
    stk -> canary2 = AZEZBEK;
    stk -> capacity = CAP_SAMPLE;
    stk -> data = (data_t *) ((char *)calloc ((stk -> capacity)*sizeof (data_t) + 2*sizeof (long), sizeof (char)) + sizeof (long));
    *(long *)((char *)stk -> data - sizeof(long)) = ICECUBE;
    *(long *)((char *)stk -> data + (stk -> capacity)*sizeof(data_t)) = FACE;
    stk -> size = 0;
    stk -> hash = HashCalc (stk);
    StackCheck (stk, ERRORS);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDtor (Stack * stk)
{
    StackCheck (stk, ERRORS);
    stk -> canary1 = 0;
    stk -> canary2 = 0;
    stk -> size = 0;
    stk -> capacity = 0;
    LogClose (stk);
    free ((char *)stk -> data - sizeof(long));
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
// Enc = encrease - увеличить
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackResEnc (Stack * stk)
{
    StackCheck (stk, ERRORS);
    *(long *)((char *)(stk -> data) + (stk -> capacity)*sizeof(data_t)) = 0;

    stk -> capacity = 2 * (stk -> capacity);

    stk -> data = (data_t *)((char *)realloc ((char *)(stk -> data) - sizeof(long), (stk -> capacity)*sizeof(data_t) + 2*sizeof(long)) + sizeof(long));
    assert (stk -> data != NULL);
    *(long *)((char *)(stk -> data) + (stk -> capacity)*sizeof(data_t)) = FACE;
    stk -> hash = HashCalc (stk);
    StackCheck (stk, ERRORS);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
// Red = reduce - уменьшить
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackResRed (Stack * stk)
{
    StackCheck (stk, ERRORS);

    if (stk -> capacity != CAP_SAMPLE)
    {
        stk -> capacity = (stk -> capacity) / 2;
    }

    stk -> data = (data_t *)((char *)realloc ((char *)(stk -> data) - sizeof(long), (stk -> capacity)*sizeof(data_t) + 2*sizeof(long)) + sizeof(long));
    *(long *)((char *)(stk -> data) + (stk -> capacity)*sizeof(data_t)) = FACE;
    stk -> hash = HashCalc (stk);
    StackCheck (stk, ERRORS);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackPush (Stack * stk, data_t value)
{
    StackCheck (stk, ERRORS);

    if (stk -> size == ((3 * stk -> capacity) / 4) - 1)
    {
        StackResEnc (stk);
    }

    * (stk -> data + stk -> size) = value;
    (stk -> size)++;
    stk -> hash = HashCalc (stk);
    StackCheck (stk, ERRORS);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
data_t StackPop (Stack * stk)
{
    MyAss((stk -> size) != 0, Logfile, __LINE__,
          __PRETTY_FUNCTION__, ER_POP_SIZENULL, PR_ER_POP_SIZENULL, stk);
    //    StackCheck (stk, ER_POP_SIZENULL);

    data_t value = * (stk -> data + stk -> size - 1);
    * (stk -> data + stk -> size - 1) = 0;
    stk -> hash = HashCalc (stk);

    if (stk -> size == ((3 * stk -> capacity) / 8))
    {
        StackResRed (stk);
    }

    (stk -> size)--;
    stk -> hash = HashCalc (stk);
    StackCheck (stk, ERRORS);
    return value;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDump (Stack * stk, int status)
{
    unsigned int nprob = 0;
    switch (status)
    {
        case NO_ERRORS:
        {
            fseek (Logfile, 0L, SEEK_SET);
            fprintf (Logfile, "STATUS of the Stack:     good\n");

            fprintf (Logfile, "Specificator of Data:    "type_d"\n");
            fprintf (Logfile, "CANARY1 of the Stack:    %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "SIZE of the Stack:       %zu\n", stk -> size);
            fprintf (Logfile, "CAPACITY of the Stack:   %zu\n", stk -> capacity);
            fprintf (Logfile, "CANARY1 of the Data:     %0X\n",
                               (unsigned int)* (long *)((char *)(stk -> data) - sizeof (long)));
            fprintf (Logfile, "DATA:                    \n\n");
            for (size_t i = 0; i < stk -> capacity; i++)
            {
                if (i < stk -> size)
                {
                    fprintf (Logfile, ">>> data[%zu] = "fmt"\n", i, * (stk -> data + i));
                }
                else
                {
                    fprintf (Logfile, "    data[%zu] = "fmt"\n", i, * (stk -> data + i));
                }
            }
            fprintf (Logfile, "\nEND OF DATA\n");
            fprintf (Logfile, "CANARY2 of the Data:     %0X\n",
                               (unsigned int) * (long *)((char *)(stk -> data) + (stk -> capacity)*sizeof(data_t)));
            fprintf (Logfile, "CANARY2 of the Stack:    %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "////////////////////////////////\n\n\n");
            break;
        }
        case ERRORS:
        {
            // так как по умолчанию считается, что со стеком всё в порядке
            fseek (Logfile, 0L, SEEK_SET);
            fprintf (Logfile, "STATUS of the Stack:        bad\n");

            if (stk == NULL)
            {
                fprintf (Logfile, "Pointer on the stack is NULL\n");
                fprintf (Logfile, "Code of the error:      %d\n", NULL_PTR_STK);
                fprintf (Logfile, "Name of the error:      %s\n", PR_NULL_PTR_STK);
                fprintf (Logfile, "Good buy\n");
            }
            if (stk -> data == NULL)
            {
                fprintf (Logfile, "Pointer of the data is NULL\n");
                fprintf (Logfile, "Code of the error:      %d\n", NULL_PTR_DATA);
                fprintf (Logfile, "Name of the error:      %s\n", PR_NULL_PTR_DATA);
                fprintf (Logfile, "Good buy\n");
            }
            if (stk -> capacity == 0)
            {
                fprintf (Logfile, "Value of CAPACITY is wrong: %zu\n", stk -> capacity);
                fprintf (Logfile, "Code of the error:      %d\n", WRONG_SIZE);
                fprintf (Logfile, "Name of the error:      %s\n", PR_WRONG_SIZE);
            }
            if (stk -> canary1 != BEZLAB)
            {
                fprintf (Logfile, "Stk CANARY1 error        \n");
                fprintf (Logfile, "Code of the error:      %d\n", CANARY_BEAT);
                fprintf (Logfile, "Name of the error:      %s\n", PR_CANARY_BEAT);
                fprintf (Logfile, "Initial value of CANARY1 of the stack:   %0X\n", (unsigned int) 0xBE31AB);
                fprintf (Logfile, "Value of CANARY1 after using the stack:  %0X\n", (unsigned int) stk -> canary1);
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            if (stk -> canary2 != AZEZBEK)
            {
                fprintf (Logfile, "Stk CANARY2 error        \n");
                fprintf (Logfile, "Code of the error:      %d\n", CANARY_BEAT);
                fprintf (Logfile, "Name of the error:      %s\n", PR_CANARY_BEAT);
                fprintf (Logfile, "Initial value of CANARY2 of the stack:   %0X\n", (unsigned int) 0xA3E3BEC);
                fprintf (Logfile, "Value of CANARY2 after using the stack:  %0X\n", (unsigned int) stk -> canary2);
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            if (*(long *)((char *)stk -> data - sizeof(long)) != ICECUBE)
            {
                fprintf (Logfile, "Data CANARY1 error        \n");
                fprintf (Logfile, "Code of the error:      %d\n", CANARY_BEAT);
                fprintf (Logfile, "Name of the error:      %s\n", PR_CANARY_BEAT);
                fprintf (Logfile, "Initial value of CANARY1:   %0X\n", (unsigned int) 0x1CECBE);
                fprintf (Logfile, "Value of CANARY1 after using the stack:  %0X\n", (unsigned int) *(long *)((char *)stk -> data - sizeof(long)));
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            if (*(long *)((char *)stk -> data + (stk -> capacity)*sizeof(data_t)) != FACE)
            {
                fprintf (Logfile, "Data CANARY2 error        \n");
                fprintf (Logfile, "Code of the error:      %d\n", CANARY_BEAT);
                fprintf (Logfile, "Name of the error:      %s\n", PR_CANARY_BEAT);
                fprintf (Logfile, "Initial value of CANARY2:   %0X\n", (unsigned int) 0xFACE);
                fprintf (Logfile, "Value of CANARY2 after using the stack:  %0X\n", (unsigned int) *(long *)((char *)stk -> data + (stk -> capacity)*sizeof(data_t)));
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            if (!(HashCheck (stk)))
            {
                fprintf (Logfile, "Problem with the Hash\n");
                fprintf (Logfile, "Expected value of Hash: %lu\n", HashCalc (stk));
                fprintf (Logfile, "Real value of Hash:     %lu\n", stk -> hash);
            }
            fprintf (Logfile, "//////////////////////////////// \n\n\n");
            break;
        }
        default:
        {
            fprintf (Logfile, "     ___UNKNOWN PROBLEM___     \n");
            fprintf (Logfile, "STATUS of the Stack:    unknown\n");
            fprintf (Logfile, "Whole checking:                \n");
            fprintf (Logfile, "CANARY1 of the Stack:       %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "SIZE of the Stack:          %zu\n", stk -> size);
            fprintf (Logfile, "CAPACITY of the Stack:      %zu\n", stk -> capacity);
            fprintf (Logfile, "CANARY1 of the Data:        %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "CANARY2 of the Data:        %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "CANARY2 of the Stack:       %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "Thanks for working with Stack  \n");
        }
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackCheck (Stack * stk, int error)
{
    if ( (stk == NULL) || ((stk -> data) == NULL) ||
         ((stk -> canary1) != BEZLAB) || ((stk -> canary2) != AZEZBEK) ||
         (*(long *)((char *)stk -> data - sizeof(long)) != ICECUBE) ||
         (*(long *)((char *)stk -> data + (stk -> capacity)*sizeof(data_t)) != FACE) ||
         !(HashCheck (stk)))
    {
        StackDump (stk, ERRORS);
    }
    else if (error == NO_ERRORS)
    {
        StackDump (stk, NO_ERRORS);
    }
}
