#include "asm.h"
#include "../Stack/stack.h"
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                //  надо сделать Asm_check ()
                //  для обработок всех ошибок ассемблера
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
FILE * File_opening (const char * name_txt, const char * mode)
{
    char *problem = 0;
    FILE *cmd_txt = NULL;
    cmd_txt = fopen (name_txt, mode);
    Assemb_check (cmd_txt != 0, name_txt, 0, problem,
                  problem, FILE_ERROR);
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
    //  Функция производит чтение текстового файла
    //  и запись его содержимого в буффер
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
unsigned int Readout_buff (const char * name_txt, char ** buff)
{
    FILE * cmd_txt = NULL;

    cmd_txt = File_opening (name_txt, "r");
    assert (cmd_txt != NULL);

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
    //    char sign = 1;
    char dottestatus = 0;
    double arg = 0;

    if (*(buff + *ipbuff) == '-')
    {
        length++;
        (*ipbuff)++;
    }

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
bool Command_detect (char * buff, unsigned * ipbuff, int * cmd)
{
    bool status = false;
    for (int i = HLT; i <= SQRT; i++)
    {
        if (I_strcmp ((buff + *ipbuff), cmd_list[i].name) == 0)
        {
            *cmd = cmd_list[i].value;
            status = true;
            break;
        }
    }
    return status;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
bool Regist_detect (char * buff, unsigned * ipbuff, char * databin, unsigned * ipbin)
{
    bool status = false;
    for (int reg = AX; reg <= DX && status == false; reg++)
    {
        if (I_strcmp ((buff + *ipbuff), reg_list[reg - AX].name) == 0)
        {
            *(databin + *ipbin) = reg;
            (*ipbin)++;
            (*ipbuff) += 2;
            status = true;
        }
    }
    return status;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    //    brake the face of the func
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Find_cmd (char * buff, unsigned int * ipbuff,
               char * databin, unsigned int * ipbin, unsigned int line)
{
    int reg = 0;                    //   для записи регистра
    int cmd = 0;                    //   для нахождения строки команды
    bool status = false;            //   для отслеживания состояния статуса чтения команд из текстового файла
    char * stroka = 0;
    char * commarg = 0;

    Gapspass (buff, ipbuff);
    stroka = buff + *ipbuff;

    status = Command_detect (buff, ipbuff, &cmd);

    if (status == false)
    {
        Assemb_check (false, "Text.txt", line, stroka,
                      buff + *ipbuff, SYNTAX_ERROR_CMD);
    }

    switch (cmd)
    {
        case PUSH:
            {
                *(databin + *ipbin) = PUSH;
                (*ipbuff) += 4;
                (*ipbin)++;
                Gapspass (buff, ipbuff);
                commarg = buff + *ipbuff;

                if (isdigit (*(buff + *ipbuff)) != 0 || *(buff + *ipbuff) == '-')
                {
                    *(double *)(databin + *ipbin) = Read_arg (buff, ipbuff);
                    if (Debil_check (*(double *)(databin + *ipbin)) != 0)
                    {
                        Assemb_check (false, "Text.txt", line, stroka,
                                      commarg, SYNTAX_ERROR_ARG);
                    }
                    *(ipbin) += sizeof (double);
                }

                else if (isalpha (*(buff + *ipbuff)) != 0)
                {
                    status = Regist_detect (buff, ipbuff, databin, ipbin);
                    if (status == false)
                    {
                        Assemb_check (false, "Text.txt", line, stroka,
                                      commarg, SYNTAX_ERROR_ARG);
                    }
                }

                else
                {
                    Assemb_check (false, "Text.txt", line, stroka,
                                  commarg, SYNTAX_ERROR_CMD);
                }

            }
            return PUSH;
        case POP:
            {
                *(databin + *ipbin) = POP;
                (*ipbuff) += 3;
                (*ipbin)++;
                Gapspass (buff, ipbuff);

                if (*(buff + *ipbuff) != '\n')
                {
                    status = Regist_detect (buff, ipbuff, databin, ipbin);
                    if (status == false)
                    {
                        Assemb_check (false, "Text.txt", line, stroka,
                                      buff + *ipbuff, SYNTAX_ERROR_ARG);
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
    //  функция нужна для пробега по буфферу с нахождением команд, аргументов
    //  и записью этой инфы в массив code
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Fill_bin (char * buff, unsigned int sizebin)
{
    bool status = true;
    char *stroka = 0;
    unsigned int ipbuff = 0;
    unsigned int ipbin = 0;
    unsigned int line = 1;

    char *databin = (char *) calloc (sizebin, sizeof (char));
    assert (databin != NULL);

    while (status == true && *(buff + ipbuff) != '\0')
    {
        Gapspass (buff, &ipbuff);
        if (Find_cmd (buff, &ipbuff, databin, &ipbin, line) == HLT)
        {
            if (isalpha (*(buff + ipbuff) != '\0'))
            {
                stroka = buff + ipbuff - 2;
                status = false;
            }
        }
        if (*(buff + ipbuff) == '\n')
             line++;
        ipbuff++;
    }
    if (status == false)
    {
        while (*(buff + ipbuff) != '\0')
        {
            stroka = buff + ipbuff;
            Gapspass (buff, &ipbuff);
            Assemb_check (*(buff + ipbuff) == '\n', "Text.txt", line,
                          stroka, (buff + ipbuff), HLT_ERROR);
            ipbuff++;
            line++;
        }
    }
    free (buff);
    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    //  запись бинарника - как оригинально, да
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Record_bin (char * databin, size_t elem_s, unsigned int mass_s, FILE * bin)
{
    unsigned int sizebin = fwrite (databin, elem_s, mass_s, bin);
    assert (sizebin == mass_s);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
        //  нужна для создания буффера ввода из текстового файла
        //  и заполнению массива code по этому буфферу
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Databin_ctor (const char * name_txt, const char * name_bin)
{
    char *buff = NULL;
    unsigned int sizebuff = 0;
    sizebuff = Readout_buff (name_txt, &buff);

    assert (buff != NULL);
    char *databin = Fill_bin (buff, sizebuff);
    assert (databin != NULL);

    FILE *data = NULL;
    data = File_opening (name_bin, "w");
    Record_bin (databin, sizeof (char), sizebuff, data);
    File_closing (&data);

    return databin;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    //   перемещение позиции ошибки
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
char * Move_underline (char * stroka, char * command)
{
    char *move = 0;
    unsigned length = 0;

    for (length = 0; length < command - stroka; length++)
    {
        length++;
    }
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
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void Assemb_check (bool expression, const char * file, unsigned int line,
                  char * stroka, char * command, const int code_er)
{
    if (!expression)
    {
        printf ("ASSEMBLING FAILED. Check the Logfile...\n");
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

                    fprintf (Logfile, "ERROR:           %s\n", errors_list[code_er].name);
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
    char * databin = NULL;
    Logfile = fopen ("Logfile.txt", "w");
    databin = Databin_ctor ("Text.txt", "Data.bin");
    assert (databin != NULL);
    return 0;
}
