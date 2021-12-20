#include "cpu.h"
#include "../Stack/stack.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//    TODO:
      //    1 - delete copypast with service functions in cpu.cpp and asm.cpp
      //    2 - ошибки открытия файлов
      //    3 - нормальный прием регистров в push
      //    4 - ошибка деления на ноль
      //    5 - нужна функция отслеживания ошибки процессора
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned int Size_file (FILE * file)
{
    unsigned int size = 0;
    assert (file != 0);
    fseek (file, 0L, SEEK_END);
    size = ftell (file);
    assert (size != 0);
    rewind (file);
    return size;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Read_bin (const char * binfile)
{
    char * databin;
    FILE * datafile = fopen (binfile, "r");
    unsigned int sizefile = Size_file (datafile);
    unsigned int checksize = 0;

    databin = (char *)calloc (sizefile, sizeof (char));

    checksize = fread (databin, sizefile, sizeof (char), datafile);

    //    checksize != sizefile   //

    fclose (datafile);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
bool Is_null (double x)
{
    if (x < 0.0)
        x = -x;
    if (x < EPSILON)
    {
        return true;
    }
    return false;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Command_complete (char * databin, unsigned int * ipbin, struct Stack * stk,
                      double * regist, unsigned int line)
{
    char cmd = *(char *)(databin + *ipbin);
    char reg = 0;
    double value = 0.0;

    switch (cmd)
    {
        case PUSH:
            {
                (*ipbin)++;
                reg = *(char *)(databin + *ipbin);
                if (reg == AX || reg == BX || reg == CX || reg == DX)
                {
                    StackPush (stk, regist[reg - AX]);
                    (*ipbin)++;
                }
                else
                {
                    value = *(double *)(databin + *ipbin);
                    StackPush (stk, value);
                    (*ipbin) += sizeof (double);
                }
            }
            return PUSH;
        case POP:
            {
                (*ipbin)++;
                reg = *(char *)(databin + *ipbin);

                if (reg == AX || reg == BX || reg == CX || reg == DX)
                {
                    regist[reg - AX] = StackPop (stk);
                }
                else
                {
                    value = StackPop (stk);
                }

                (*ipbin)++;
            }
            return POP;
        case IN:
            {
                scanf ("%lf", &value);
                StackPush (stk, value);
                (*ipbin)++;
            }
            return IN;
        case MUL:
            {
                value = StackPop (stk);

                value = value * StackPop (stk);

                StackPush (stk, value);
                (*ipbin)++;
            }
            return MUL;
        case SUP:
            {
                value = StackPop (stk);

                value -= StackPop (stk);

                StackPush (stk, value);
                (*ipbin)++;
            }
            return SUP;
        case OUT:
            {
                value = StackPop (stk);

                printf ("from OUT: %lf\n", value);
                (*ipbin)++;
            }
            return OUT;
        case ADD:
            {
                value = StackPop (stk);
                value += StackPop (stk);

                StackPush (stk, value);
                (*ipbin)++;
            }
            return ADD;
        case DIV:
            {
                double val2 = 0.0;
                value = StackPop (stk);
                val2 = StackPop (stk);

                if (Is_null (val2) == true)
                {
                    Error_Mass[1] = 1;
                    Error_check (line);
                    return HLT;
                }

                value = (double)value / val2;
                StackPush (stk, value);
                (*ipbin)++;
            }
            return DIV;
        case SQRT:
            {
                value = StackPop (stk);

                value = sqrt (value);
                StackPush (stk, value);
                (*ipbin)++;
            }
            return SQRT;
        default:
            return HLT;
    }
    return HLT;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Run_bin (char * databin, struct Stack * stk, double * regist)
{
    unsigned int ipbin = 0, line = 1;
    bool status = true;
    while (*(databin + ipbin) != '\0' && status == true)
    {
        if (Command_complete (databin, &ipbin, stk, regist, line) == HLT)
        {
            status = false;
        }
        line++;
    }
    return 0;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Dtor_bin (char ** databin)
{
    assert (*databin != NULL);
    free (*databin);
    return 0;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Error_check (unsigned int line)
{
    if (Error_Mass [1] == 1)
    {
        printf ("PROCESSING ERROR. Delete division on ZERO on %u line\n", line);
    }
    return 0;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int main (void)
{
    char * databin = Read_bin ("Data.bin");
    struct Stack stk = {};
    double regist [4] = {0.0};
    StackCtor (&stk);

    //  printf ("%d\n", Error_Mass[0]);

    Run_bin (databin, &stk, regist);

    StackDtor (&stk);
    Dtor_bin (&databin);
    return 0;
}
