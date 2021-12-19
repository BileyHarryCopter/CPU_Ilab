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
