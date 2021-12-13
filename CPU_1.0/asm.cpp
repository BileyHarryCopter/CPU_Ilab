#include "asm.h"

//    передаваемая инфа - текстовый файл от пользователя
//    завершение работы ассемблера - полностью созданный бинарник, который можно
//    процессору, или сообщение об ошибке в вводе команд.
//    возвращаемое значение

//    нужны функции:
// + //    1 -- открытия текстового файла,
// + //    2 -- считывния информации с него в некоторый буфер памяти
// + //    3 -- прохода по этому буферу и выделению команд и аргументов с помощью неравномерного массива
// - //    4 -- проверки содержимого (попутно надо создать Logfile для сообщения об ошибках)
// + //    5 -- перезапись информации из текстового вида  в машинный код (на даблах)
// - //    6 -- успешного запуска бинарника


    //  что если всё писать сразу в некоторый дабловский массив
    //  минуя массив binarnik
    //  плюсы - гораздо удобнее работать с этим массивом напрямую
    //  минусы - не знаем, сколько нужно места
    //  решения - пробежаться по массиву до общего прохода, узнать количетсво команд
    //  закалочить память на количество, вдвое большее, чем кол-во команд
    //  вернуться в начало массива и провести чтение в массив из даблов

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
FILE * File_opening (const char * name_txt, const char * mode)
{
    FILE * cmd_txt;
    cmd_txt = fopen (name_txt, mode);
    assert (cmd_txt != 0);
    return cmd_txt;
    //    добавить проверки из стэка
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void File_closing (FILE ** cmd_txt)
{
    fclose (* cmd_txt);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned int Size_buff (FILE * file)
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
void Fill_buff (char ** buff, unsigned int size, FILE * cmd_txt)
{
    * buff = (char *) calloc (size + 1, sizeof (char));
    assert (* buff != NULL);

    unsigned int bytes = fread (* buff, sizeof (char), size, cmd_txt);
    assert (bytes == size);

    * (*buff + size) = '\0';
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Readout_buff (const char * name_txt)
{
    char * buff;
    FILE * cmd_txt;

    cmd_txt = File_opening (name_txt, "r");

    unsigned int size = Size_buff (cmd_txt);

    Fill_buff (&buff, size, cmd_txt);

    File_closing (&cmd_txt);

    return buff;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  обработать ошибку ввода команд
        //  str1 - from buffer         str2 - from cmd_list
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int I_strcmp (char *str1, const char *str2)
{
    while (isalpha (*str1) != 0)
    {
        printf ("%c   %c\n", *str1, *str2);
        if (tolower (*str1) != tolower (*str2))
        {
            return (*str1 - *str2);
        }
        str1++;
        str2++;
    }
    return (* str1 == ' ' || *str1 == '\t' || *str1 == '\n') ? 0 : ERROR;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        // для пропуска лишнего места
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Gapspass (char *buff, unsigned int *ptrbuff)
{
    while (*(buff + *ptrbuff) == '\t' || *(buff + *ptrbuff) == ' ')
    {
        (*ptrbuff)++;
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  быстрая функция, делающая пробег по массиву
        //  buff и узнающая количество команд и аргументов
        //  большие проблемы связаны с ошибками ввода
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned int Numb_cmd (char * buff)
{
    unsigned int cmdnumb = 0;
    unsigned int ptrbuff = 0;
    while (*(buff + ptrbuff) != '\0')
    {
        Gapspass (buff, &ptrbuff);
        if (isalpha (* (buff + ptrbuff)) != 0 || isdigit (*(buff + ptrbuff)) != 0)
        {
            cmdnumb++;
            while (isalpha (*(buff + ptrbuff)) != 0 || isdigit (*(buff + ptrbuff)) != 0 || *(buff + ptrbuff) == '.')
            {
                ptrbuff++;
            }
        }
        Gapspass (buff, &ptrbuff);
        if (*(buff + ptrbuff) == '\n')
        {
            ptrbuff++;
        }
        ptrbuff++;
    }

    printf ("number of commands: %u\n", cmdnumb + 1);

    return cmdnumb + 1;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
double Read_arg (char *buff, unsigned int *ptrbuff)
{
    unsigned int length = 0;
    char dottestatus = 0;
    double arg = 0;

    while (isdigit (*(buff + *ptrbuff)) != 0 || *(buff + *ptrbuff) == '.')
    {
        if (dottestatus > 1)
        {
            return NAN;
        }
        else if (*(char *)(buff + *ptrbuff) == '.')
        {
            dottestatus++;
        }
        length++;
        (*ptrbuff)++;
    }
    if (sscanf ((buff + *ptrbuff - length), "%lf\n", &arg) == 0)
    {
        return NAN;
    }
    return arg;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Debil_check (double arg)
{
    if (arg != arg)
    {
        printf ("Some error with input\n");
        assert (1);
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Find_cmd (char * buff, unsigned int * ptrbuff,
               double * databin, unsigned int * ptrbin)
{
    int cmd = 0;
    char status = 0;
    for (cmd = HLT; cmd <= SQRT; cmd++)
    {
        if (I_strcmp ((buff + *ptrbuff), cmd_list[cmd].name) == 0)
        {
            status++;
            break;
        }
    }
    if (status == 0)
    {
        printf ("Your mamka is so tasty\n");
        return HLT;
    }
    printf ("%d       %s\n", cmd, cmd_list[cmd].name);
    switch (cmd_list[cmd].value)
    {
        case PUSH:
            {
                * (databin + *ptrbin) = PUSH;
                (*ptrbuff) += 4;
                (*ptrbin)++;
                Gapspass (buff, ptrbuff);

                if (isdigit (*(buff + *ptrbuff)) != 0)
                {
                    *(databin + *ptrbin) = Read_arg (buff, ptrbuff);
                    Debil_check (*(databin + *ptrbin));
                }
                else
                {
                    printf ("Some error with input\n");
                }
                (*ptrbin)++;
            }
            return PUSH;
        case HLT:
            {
                *(databin + *ptrbin) = HLT;
                (*ptrbuff) += 3;
                (*ptrbin)++;
            }
            return HLT;
        default:
            {
                if (cmd_list[cmd].value == SQRT)
                {
                    *(databin + *ptrbin) = SQRT;
                    (*ptrbuff) += 4;
                }
                else
                {
                    *(databin + *ptrbin) = cmd_list[cmd].value;
                    (*ptrbuff) += 3;
                }
                (*ptrbin)++;
            }
            return cmd_list[cmd].value;
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  The function Find_cmd:
        //  вспомогательная функция для пробега по буферу и обнаружению
        //  функций и их аргументов с последующей записью в массив code
        //  + free (buff)
        //  + free (binarnik)
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
double * Fill_bin (char *buff, unsigned int sizebin)
{
    unsigned int ptrbuff = 0;
    unsigned int ptrbin = 0;
    double *databin = (double *) calloc ( sizebin, sizeof (double));
    assert (databin != NULL);

    while (*(buff + ptrbuff) != '\0')
    {
        Gapspass (buff, &ptrbuff);
        if (isalpha(*(buff + ptrbuff)) != 0)
        {
              if (Find_cmd (buff, &ptrbuff, databin, &ptrbin) == HLT)
              {
                  break;
              }
        }
        ptrbuff++;
        //    printf ("%c\n", *(buff + ptrbuff));
    }

    free (buff);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
            //    free (databin)
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Record_bin (double * databin, FILE * data)
{
    unsigned int ptrbin = 0;
    while (*(databin + ptrbin) != HLT)
    {
        fprintf (data, "%.10lg ", *(databin + ptrbin));
        ptrbin++;
    }
    fprintf (data, "%lg\n", *(databin + ptrbin));
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  нужна для пробега по буфферу и заполнению массива code
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
double * Databin_ctor (const char * name_txt)
{
    char *buff = Readout_buff (name_txt);
    assert (buff != NULL);

    double *databin = Fill_bin (buff, Numb_cmd (buff));

    FILE *data;
    data = File_opening ("Data.txt", "w");

    Record_bin (databin, data);

    File_closing (&data);

    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
/*
int main (void)
{
    double * databin = Databin_ctor ("Text.txt");
    return 0;
}
*/
