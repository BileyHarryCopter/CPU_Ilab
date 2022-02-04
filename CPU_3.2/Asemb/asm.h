#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <locale.h>
#include <stdbool.h>


enum cmd_names
{
    HLT = 0,
    PUSH,
    IN,
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

struct cmd_name cmd_list [10] = {
    {"HLT", HLT},
    {"PUSH", PUSH},
    {"IN", IN},
    {"POP", POP},
    {"MUL", MUL},
    {"SUP", SUP},
    {"OUT", OUT},
    {"ADD", ADD},
    {"DIV", DIV},
    {"SQRT", SQRT}
};

enum errors
{
    SYNTAX_ERROR_CMD = 0,
    SYNTAX_ERROR_ARG,
    HLT_ERROR,
    FILE_ERROR
};

struct cmd_name errors_list [4] = {
    {"SYNTAX ERROR WITH COMMAND", SYNTAX_ERROR_CMD},
    {"SYNTAX ERROR WITH ARGUMENT", SYNTAX_ERROR_ARG},
    {"SYNTAX ERROR: NO HLT", HLT_ERROR},
    {"FILE ERROR", FILE_ERROR}
};

const char underline[] = "^~~~~~~~~~";


FILE * File_opening (const char * name_txt, const char * mode);

void File_closing (FILE ** cmd_txt);
void Gapspass (char *buff, unsigned int *ptrbuff);
void Record_bin (double * databin, FILE * data);
void Fill_buff (char ** buff, unsigned int size, FILE * cmd_txt);
void Assemb_check (bool expression, const char * file, unsigned int line,
                  char * stroka, char * command, const int code_er);
unsigned int Numb_cmd (char * buff);
unsigned int Size_buff (FILE * file);
unsigned int Readout_buff (const char * name_txt);

bool Command_detect (char * buff, unsigned * ipbuff, int * cmd);
bool Regist_detect (char * buff, unsigned * ipbuff, char * databin, unsigned ipbin);

int Debil_check (double arg);
int I_strcmp (char *str1, const char *str2);
int Find_cmd (char * buff, unsigned int * ptrbuff,
               double * databin, unsigned int * ptrbin);

double Read_arg (char *buff, unsigned int *ptrbuff);

char * Fill_bin (char *buff, unsigned int sizebin);
char * Databin_ctor (const char * name_txt, const char * name_bin);
char * Word_create (char * buff);
