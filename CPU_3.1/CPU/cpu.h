#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <locale.h>
#include <stdbool.h>

struct cmd_name {
    const char * name;
    int value;
};

#include "../Service/commands.h"
#include "../Service/registers.h"




const double EPSILON = 10E-12;

char * Read_bin (const char * binfile);

int Dtor_bin (char ** databin);
int Error_check (unsigned int line);
int Run_bin (char * databin, struct Stack * stk, double * regist);
int Command_complete (char * databin, unsigned int * ipbin, struct Stack * stk,
                      double * regist, unsigned int line);
unsigned int Size_file (FILE * file);

bool Is_null (double x);
