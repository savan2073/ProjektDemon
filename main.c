#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <errno.h>
#include <syslog.h>
#include <stdbool.h>
#include <signal.h>

#include "help.h"

int main(int argc, char* argv[])
{
    int pid = lock(false);

    if(argc == 2 &&argv[1][0] == '-' && argv[1][1] == 'h'){
        help(true);
        return EXIT_FAILURE;
    }
    if(pid == -1){
        printf("Error: cannot load file demon\n");
        return EXIT_FAILURE;
    }
    if(pid != 0){
        if(argc != 2 || argv[1][0] != '-'){
            printf("Demon is already running\n");
            help_launch();
            return EXIT_FAILURE;
        }else{
            switch (argv[1][1])
            {
            case 'F':
                printf("Sending SIGUSR1\n");
                kill(pid, SIGUSR1);
                return EXIT_SUCCESS;
                break;
            case 'S':
                printf("Sending SIGUSR2\n");
                kill(pid,SIGUSR2);
                return EXIT_SUCCESS;
                break;
            case 'K':
                printf("Sending SIGTTERM\n");
                kill(pid,SIGTERM);
                return EXIT_SUCCESS;
                break;
            
            default:
                printf("Unidentified argument");
                return EXIT_FAILURE;
                break;
            }
        }
    }

return 0;
}
