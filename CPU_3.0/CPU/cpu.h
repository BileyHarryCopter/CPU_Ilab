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
