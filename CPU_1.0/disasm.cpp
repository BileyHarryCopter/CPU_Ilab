#include "asm.cpp"

void Printf_to_file (double * databin)
{
    unsigned int pos = 0;
    FILE * disasm = fopen ("Disasm.txt", "w");
    assert (disasm != 0);
    while (*(databin + pos) != HLT)
    {
        if (*(databin + pos) == PUSH)
        {
            fprintf (disasm, "%s ", cmd_list[PUSH].name);
            pos++;
            fprintf (disasm, "%lf\n", *(databin + pos));
        }
        else
        {
            int cmd = (int)*(databin + pos);
            fprintf (disasm, "%s\n", cmd_list[cmd].name);
        }
        pos++;
    }
    fclose (disasm);
}

int main (void)
{
    double * databin = Databin_ctor ("Text.txt");

    Printf_to_file (databin);

    print ("Suck some dick\n");
    return 0;
}
