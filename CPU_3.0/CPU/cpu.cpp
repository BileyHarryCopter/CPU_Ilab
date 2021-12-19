#include "cpu.h"
#include "../Stack/stack.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//    первым делом надо сделать ассемблер, в котором будет функция, принимающая
//    текстовый файл, возвращающая - файл с расширением в бин
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//    at the moment cpu has datatbin file with mashine code
//    cpu should read this code and complete all commands from massive with
//    mashine code
//    необходимые функции:
//    1 - чтение бинарного файла, перевод инфы из него в неравномерный массив
//    2 - пробега по неравномерному массиву
//    3 - чтение и выполнение команд из неравномерного массива
//    4 - для отслеживания ошибок (связано отчасти со стеком)
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//    TODO:
      //    1 - delete copypast with service functions in cpu.cpp and asm.cpp
      //    2 - ошибки открытия файлов
      //    3 - нормальный прием регистров в push
      //    4 - ошибка деления на ноль
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

    /*
    if (checksize != sizefile)
    {
        printf ("The compilator have killed your katty\n");
        free (databin);
    }
    */

    fclose (datafile);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Command_complete (char * databin, unsigned int * ipbin, struct Stack * stk, double * regist)
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
                value = value - StackPop (stk);
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
                double val2 = 0;
                value = StackPop (stk);
                val2 = StackPop (stk);
                //  очень простая реализация этого дерьма
                if (val2 != 0)
                {
                    value = (double)value / val2;
                    StackPush (stk, value);
                }
                else
                {
                    printf ("Is your mom sick slug?\n");
                }
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
void Run_bin (char * databin, struct Stack * stk, double * regist)
{
    unsigned int ipbin = 0;
    bool status = true;
    while (*(databin + ipbin) != '\0' && status == true)
    {
        if (Command_complete (databin, &ipbin, stk, regist) == HLT)
        {
            status = false;
        }
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int main (void)
{
    char * databin = Read_bin ("Data.bin");
    struct Stack stk = {};
    double regist [4] = {0.0};
    StackCtor (&stk);

    Run_bin (databin, &stk, regist);

    StackDtor (&stk);
    free (databin);
    return 0;
}
