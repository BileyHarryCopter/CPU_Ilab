#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <locale.h>

#define ERROR -2

enum
{
    HLT = 0,
    PUSH,  // с аргументом
    POP,
    MUL,
    SUP,
    OUT,
    ADD,
    DIV,
    SQRT
};

struct cmd_name {
    const char * name;
    int value;
};

struct cmd_name cmd_list [9] = {
    {"HLT", HLT},
    {"PUSH", PUSH},
    {"POP", POP},
    {"MUL", MUL},
    {"SUP", SUP},
    {"OUT", OUT},
    {"ADD", ADD},
    {"DIV", DIV},
    {"SQRT", SQRT}
};

FILE * File_opening (const char * name_txt, const char * mode);
void File_closing (FILE ** cmd_txt);
void Debil_check (double arg);
void Fill_buff (char ** buff, unsigned int size, FILE * cmd_txt);
void Gapspass (char *buff, unsigned int *ptrbuff);
void Record_bin (double * databin, FILE * data);
char * Readout_buff (const char * name_txt);
int I_strcmp (char *str1, const char *str2);
int Find_cmd (char * buff, unsigned int * ptrbuff,
               double * databin, unsigned int * ptrbin);
unsigned int Size_buff (FILE * file);
unsigned int Numb_cmd (char * buff);
double Read_arg (char *buff, unsigned int *ptrbuff);
double * Fill_bin (char *buff, unsigned int sizebin);
double * Databin_ctor (const char * name_txt);
