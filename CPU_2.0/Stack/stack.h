#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

static FILE * Logfile;

enum
{
    FALSE = 0,
    TRUE
};

#define BEZLAB   0xBE31AB
#define ICECUBE  0x1CECBE
#define FACE     0xFACE
#define AZEZBEK  0xA3E3BEC

// define для assert
#define MyAss(expresion, file, line, func, numb_er, name_er, stk)               \
{                                                                               \
    if (file == NULL)                                                           \
    {                                                                           \
        printf ("Assertion failed\n");                                          \
        printf ("Problem with opening the Logfile on %u line\n", line);         \
        printf ("In the function     %s\n", func);                              \
        printf ("Code of error:      %d\n", NULL_PTR_FILE);                     \
        printf ("Name of error:      %s\n\n", PR_NULL_PTR_FILE);                \
        exit (EXIT_SUCCESS);                                                    \
    }                                                                           \
    else if (!(expresion))                                                      \
    {                                                                           \
        Assert (FALSE, file, __LINE__, __PRETTY_FUNCTION__, numb_er, name_er);  \
        StackDtor (stk);                                                        \
        exit (EXIT_SUCCESS);                                                    \
    }                                                                           \
}                                                                               \
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                           // definitions of ERRORS
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
#define NULL_PTR_STK      10 //enum ????????? Yes, enum :)   Make a enum
#define NULL_PTR_DATA     11
#define ER_POP_SIZENULL   12
#define NULL_PTR_FILE     13
#define ERRORS            14
#define NO_ERRORS         15
#define WRONG_SIZE        16
#define CANARY_BEAT       17
#define HASH_ER           18
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
#define PR_NULL_PTR_STK        "NULL_PTR_STK"
#define PR_NULL_PTR_DATA       "NULL_PTR_DATA"
#define PR_ER_POP_SIZENULL     "ER_POP_SIZENULL"
#define PR_NULL_PTR_FILE       "NULL_PTR_FILE"
#define PR_ERRORS              "ERRORS"
#define PR_NO_ERRORS           "NO_ERRORS"
#define PR_WRONG_SIZE          "WRONG_SIZE"
#define PR_CANARY_BEAT         "CANARY_BEAT"
#define PR_HASH_ER             "HASH_ER"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
              // the block with conditional compilation DATA
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
#define CHAR    0
#define SHORT   1
#define INT     2
#define LONG    3
#define FLOAT   4
#define DOUBLE  5

#define Stack_Type DOUBLE

// Think about ifdefs :)
/*
#if 0
#ifdef DBL
#define Stack_Type DOUBLE
#endif
#ifdef GAY
#define Stack_Type INT
#endif
#endif
*/

#if Stack_Type == CHAR
    #define data_t char
    #define fmt "%c"
    #define type_d "char"
#endif

#if Stack_Type == SHORT
    #define data_t short
    #define fmt "%hi"
    #define tipe_d "short"
#endif

#if Stack_Type == INT
    #define data_t int
    #define fmt "%d"
    #define type_d "int"
#endif

#if Stack_Type == LONG
    #define data_t long
    #define fmt "%li"
    #define type_d "long"
#endif

#if Stack_Type == FLOAT
    #define data_t float
    #define fmt "%lf"
    #define type_d "float"
#endif

#if Stack_Type == DOUBLE
    #define data_t double
    #define fmt "%lf"
    #define type_d "double"
#endif


static size_t CAP_SAMPLE = sizeof (double);
struct Stack;
typedef struct Stack Stack;
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                         // definitions of functions
        // it doesn't need to write functions which don't need the user
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDump (Stack * stk, int status);
void StackCheck (Stack * stk, int error);
void StackCtor (Stack * stk);
void StackDtor (Stack * stk);
void StackPush (Stack * stk, data_t value);
data_t StackPop (Stack * stk);
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                         // definition of the struct
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
struct Stack {
    long canary1;
    data_t * data; //Try to make it void* (not obligatary)
    size_t size;
    size_t capacity;
    unsigned long hash;
    long canary2;
};
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
