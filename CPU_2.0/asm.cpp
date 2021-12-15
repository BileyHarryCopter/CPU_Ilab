#include "asm.h"
#include "Stack/stack.h"

FILE * User_file;
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                //  надо сделать Asm_check ()
                //  для обработок всех ошибок ассемблера
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
FILE * File_opening (const char * name_txt, const char * mode)
{
    char * problem = 0;
    FILE * cmd_txt;
    cmd_txt = fopen (name_txt, mode);
    Assemb_check (cmd_txt != 0, name_txt, 0, problem,
                  problem, errors_list[FILE_ERROR].name, FILE_ERROR);
    return cmd_txt;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void File_closing (FILE ** cmd_txt)
{
    fclose (* cmd_txt);
}
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
void Fill_buff (char ** buff, unsigned int size, FILE * cmd_txt)
{
    *buff = (char *) calloc (size + 1, sizeof (char));
    assert (*buff != NULL);

    unsigned int bytes = fread (*buff, sizeof (char), size, cmd_txt);
    assert (bytes == size);

    *(*buff + size) = '\0';
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned int Readout_buff (const char * name_txt, char ** buff)
{
    FILE * cmd_txt;

    cmd_txt = File_opening (name_txt, "r");

    unsigned int sizebuff = Size_file (cmd_txt);

    Fill_buff (buff, sizebuff, cmd_txt);

    File_closing (&cmd_txt);

    return sizebuff;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  обработать ошибку ввода команд
        //  str1 - from buffer         str2 - from cmd_list
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int I_strcmp (char * str1, const char * str2)
{
    while (isalpha (*str1) != 0)
    {
        //  printf ("%c   %c\n", *str1, *str2);
        if (tolower (*str1) != tolower (*str2))
        {
            return (*str1 - *str2);
        }
        str1++;
        str2++;
    }
    return (*str1 == ' ' || *str1 == '\t' || *str1 == '\n' || *str1 == '\0') ? 0 : SYNTAX_ERROR_CMD;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        // для пропуска лишнего места
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Gapspass (char * buff, unsigned int * ipbuff)
{
    while (*(buff + *ipbuff) == '\t' || *(buff + *ipbuff) == ' ')
    {
        (*ipbuff)++;
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  функциям PUSH, ADD добавить 2 аргумента - значение и путь:
        //  регистр или стэк
        //  остальным функциям добавить по 1 аргументу - путь
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
double Read_arg (char * buff, unsigned int * ipbuff)
{
    unsigned int length = 0;
    char dottestatus = 0;
    double arg = 0;

    while (isdigit (*(buff + *ipbuff)) != 0 || *(buff + *ipbuff) == '.')
    {
        if (dottestatus > 1)
        {
            return NAN;
        }
        else if (*(char *)(buff + *ipbuff) == '.')
        {
            dottestatus++;
        }
        length++;
        (*ipbuff)++;
    }
    if (sscanf ((buff + *ipbuff - length), "%lf\n", &arg) == 0)
    {
        return NAN;
    }
    return arg;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Debil_check (double arg)
{
    return (arg != arg) ? SYNTAX_ERROR_ARG : 0;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Find_cmd (char * buff, unsigned int * ipbuff,
               char * databin, unsigned int * ipbin, unsigned int line)
{
    int cmd = 0;
    char status = 0;
    char * stroka = 0;

    Gapspass (buff, ipbuff);

    //    stroka = Word_create (buff + *ipbuff);

    for (cmd = HLT; cmd <= SQRT; cmd++)
    {
        if (I_strcmp ((buff + *ipbuff), cmd_list[cmd].name) == 0)
        {
            status++;
            break;
        }
    }
    if (status == 0)
    {
        Assemb_check (false, "Text.txt", line, stroka, buff + *ipbuff,
                      errors_list[SYNTAX_ERROR_CMD].name, SYNTAX_ERROR_CMD);
        return HLT;
    }
    printf ("%d       %s\n", cmd, cmd_list[cmd].name);
    switch (cmd_list[cmd].value)
    {
        case PUSH:
            {
                char * commarg = buff + *ipbuff;
                *(databin + *ipbin) = PUSH;
                (*ipbuff) += 4;
                (*ipbin)++;
                Gapspass (buff, ipbuff);
                if (isdigit (*(buff + *ipbuff)) != 0)
                {
                    commarg = buff + *ipbuff;
                    *(double *)(databin + *ipbin) = Read_arg (buff, ipbuff);
                    if (Debil_check (*(double *)(databin + *ipbin)) != 0)
                    {
                        Assemb_check (false, "Text.txt", line, stroka, commarg,
                                      errors_list[SYNTAX_ERROR_ARG].name, SYNTAX_ERROR_ARG);
                    }
                    *(ipbin) += sizeof (double);
                }
                else
                {
                    Assemb_check (false, "Text.txt", line, stroka, commarg,
                                  errors_list[SYNTAX_ERROR_CMD].name, SYNTAX_ERROR_CMD);
                }
            }
            return PUSH;
        //  сделать человеческую обработку аргумента назначения
        case POP:
            {
                *(databin + *ipbin) = POP;
                stroka = buff + *ipbuff;
                (*ipbuff) += 3;
                (*ipbin)++;
                Gapspass (buff, ipbuff);
                if (*(buff + *ipbuff) != '\n')
                {
                    int reg = 0;
                    status = 0;
                    for (reg = AX; reg <= DX; reg++)
                    {
                        if (tolower (*(buff + *ipbuff)) == tolower (*(reg_list[reg - AX].name)) &&
                            tolower (*(buff + *ipbuff + 1)) == tolower (*(reg_list[reg - AX].name + 1)))
                        {
                            *(databin + *ipbin) = reg;
                            (*ipbin)++;
                            (*ipbuff) += 2;
                            status++;
                        }
                    }
                    if (status == 0)
                    {
                        Assemb_check (false, "Text.txt", line, stroka, buff + *ipbuff,
                                      errors_list[SYNTAX_ERROR_ARG].name, SYNTAX_ERROR_ARG);
                    }
                }
            }
            return POP;
        case HLT:
            {
                *(databin + *ipbin) = HLT;
                (*ipbuff) += 3;
                (*ipbin)++;
            }
            return HLT;
        case IN:
            {
                *(databin + *ipbin) = IN;
                (*ipbuff) += 2;
                (*ipbin)++;
            }
            return IN;
        default:
            {
                if (cmd_list[cmd].value == SQRT)
                {
                    *(databin + *ipbin) = SQRT;
                    (*ipbuff) += 4;
                }
                else
                {
                    *(databin + *ipbin) = cmd_list[cmd].value;
                    (*ipbuff) += 3;
                }
                (*ipbin)++;
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
char * Fill_bin (char * buff, unsigned int sizebin)
{
    unsigned int ipbuff = 0;
    unsigned int ipbin = 0;
    unsigned int line = 1;

    char *databin = (char *) calloc (sizebin, sizeof (char));
    assert (databin != NULL);

    while (*(buff + ipbuff) != '\0')
    {
        Gapspass (buff, &ipbuff);
        if (isalpha(*(buff + ipbuff)) != 0)
        {
              if (Find_cmd (buff, &ipbuff, databin, &ipbin, line) == HLT)
              {
                  break;
              }
              line++;
        }
        ipbuff++;
        printf ("%c\n", *(buff + ipbuff));
    }
    free (buff);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
            //    free (databin)
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Record_txt (char * databin, FILE * data)
{
    int status = 0;
    unsigned int ipbin = 0;
    char cmd;
    while (*(databin + ipbin) != HLT)
    {
        for (cmd = HLT; cmd <= SQRT; cmd++)
        {
            if (*(databin + ipbin) == cmd_list[cmd].value)
            {
                status++;
                break;
            }
        }
        if (status != 0)
        {
            switch (cmd_list[cmd].value)
            {
                case PUSH:
                    {
                        fprintf (data, "%d ", *(databin + ipbin));
                        ipbin++;
                        fprintf (data, "%lg ", *(double *)(databin + ipbin));
                        ipbin += sizeof (double);
                    }
                    break;
                case POP:
                    {
                        fprintf (data, "%d ", *(databin + ipbin));
                        ipbin++;
                        status = 0;
                        for (int i = AX; i <= DX; i++)
                        {
                            if (*(databin + ipbin) == i)
                            {
                                fprintf (data, "%d ", *(databin + ipbin));
                                ipbin++;
                                break;
                            }
                        }
                    }
                    break;
                default:
                    {
                        fprintf (data, "%d ", *(databin + ipbin));
                        ipbin++;
                    }
                    break;
            }
        }
    }
    fprintf (data, "%d\n", *(databin + ipbin));
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Record_bin (char * databin, size_t elem_s, unsigned int mass_s, FILE * bin)
{
    unsigned int sizebin = fwrite (databin, elem_s, mass_s, bin);
    assert (sizebin == mass_s);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  нужна для пробега по буфферу и заполнению массива code
        //  функция вызывается в процессоре
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Databin_ctor (const char * name_txt)
{
    char *buff = 0;
    unsigned sizebuff;
    sizebuff = Readout_buff (name_txt, &buff);
    assert (buff != NULL);

    char *databin = Fill_bin (buff, sizebuff);

    FILE *data;
    data = File_opening ("Data.bin", "w");

    Record_bin (databin, sizeof (char), sizebuff, data);

    File_closing (&data);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    //  length - длина строки underline []
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Move_underline (char * stroka, char * command)
{
    char * move = 0;
    unsigned length = 0;

    printf ("%d\n", length);
    for (length = 0; length < command - stroka; length++)
    {
        length++;
    }
    printf ("%d\n", length);

    move = (char *) calloc (length + 11, sizeof (char));

    for (int i = 0; i < length + 11; i++)
    {
        if (i < length)
        {
            *(move + i) = ' ';
        }
        else
        {
            *(move + i) = underline[i - length];
        }
    }

    return move;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Str_ctor (char * stroka)
{
    int length = 0;
    char * res_str = 0;
    for (length = 0; *(stroka + length) != '\n' && *(stroka + length) != '\0'; length++) {;}

    res_str = (char *) calloc (length + 1, sizeof (char));
    for (int i = 0; i <= length; i++)
    {
        *(res_str + i) = *(stroka + i);
    }
    *(res_str + length) = '\0';
    return res_str;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    //  Функция для обработки ошибок ассемблера
    //  виды ошибок:
    //  1 - ошибка ввода команды - SYNTAX ERROR
    //  2 - ошибка ввода аргумента - SYNTAX ERROR
    //  3 - ошибка открытия/считывания файлов
    //  для более корректной передачи ошибки сделать assert для asm.h




    //  line - строка в файле Text.text - считать в месте
    //  найти строку можно по предшествующему числу команд
    //  command - команда в строке line, которая была написана неправильно
    //  argument - аргумент команды в текстовом файле
    //  и то и другое можно найти fseek ()
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Assemb_check (bool expression, const char * file, unsigned int line,
                  char * stroka, char * command, const char * name_er, const int code_er)
{
    if (!expression)
    {
        printf ("FAILED. Check the Logfile...\n");
        fprintf (Logfile, "+|=================FAILED==================|+\n\n");
        switch (code_er)
        {
            case FILE_ERROR:
                {
                    fprintf (Logfile, "ERROR:           %s\n", errors_list[FILE_ERROR].name);
                    fprintf (Logfile, "FILE \"%s\" can't be detected in source\n\n", file);
                    fprintf (Logfile, "+|==============CORRECT_FILE==============|+\n");
                }
                exit (EXIT_FAILURE);
            default:
                {
                    char * Underline = 0;
                    char * fail_str = 0;
                    unsigned jumpline = command - stroka;

                    Underline = Move_underline (stroka, command);
                    fail_str = Str_ctor (stroka);

                    if (code_er == SYNTAX_ERROR_CMD)
                        fprintf (Logfile, "ERROR:           %s\n", errors_list[SYNTAX_ERROR_CMD].name);
                    else
                        fprintf (Logfile, "ERROR:           %s\n", errors_list[SYNTAX_ERROR_ARG].name);

                    fprintf (Logfile, "On %u:%u in the %s:\n", line, jumpline, file);
                    fprintf (Logfile, "%s\n", fail_str);
                    fprintf (Logfile, "%s\n", Underline);
                    fprintf (Logfile, "+|==============CORRECT_INPUT==============|+\n");
                    free (fail_str);
                    free (Underline);
                }
                exit (EXIT_FAILURE);
        }
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int main (void)
{
    char * databin = 0;
    Logfile = fopen ("Logfile.txt", "w");
    //  rewind (Logfile);
    databin = Databin_ctor ("Taxt.txt");
    return 0;
}
// добавить проверку переполнения неравномерного массива
