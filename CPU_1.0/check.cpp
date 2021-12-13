#include <stdio.h>
#include <stdlib.h>

void gapspass (char * buff, unsigned int * pos)
{
    while (* (buff + *pos) == '\t' || * (buff + *pos) == ' ')
    {
        (*pos)++;
    }
}

int main (void)
{
    char * STR = "      ";  // 6 пробелов
    unsigned int i = 0;
    gapspass (STR, &i);

    printf ("%u\n", i);

    return 0;
}
