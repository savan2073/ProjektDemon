#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <syslog.h>
#include <stdbool.h>

#include "help.h"

int main(int argc, char* argv[])
{
    int pid = lock(false);

    if(argc == 2 &&argv[1][0] == '-' && argv[1][1] == 'h'){
        help(true);
        return 0;
    }

return 0;
}
