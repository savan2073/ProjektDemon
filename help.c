#include "help.h"
#include <stdio.h>

void help(bool full){
    printf("To use demon properly:\n\n");

    printf("1) demon <source path> <destination path> [option]\n");
    printf("2) demon [option]\n\n");

    printf("To start demon and initialize synchronisation use the first variant (1)\n");
    printf("To force synchronisation or to stop already running demon use the second variant (2)\n");
    
    if(!full){
        "\nDisplay help: demon -h\n";
        return;
    }

    printf("\n Description of variants used in first use case:\n\n");
    help_params();

    printf("\nDescription of varianst used in second use case:\n\n");
    help_launch();
}

void help_params(){
    printf(" -R : recursive copying\n\n");

    printf(" -s <number> [unit] : sleep time of demon, units:\n");
    printf("[none] : by default seconds\n");
    printf("s: seconds\n");
    printf("m: minutes\n");
    printf("h: hours\n\n");

    printf(" -m <number>[unit]: files with this size or greater will be copied by mmap, units:\n");
    printf("[none] : bytes\n");
    printf("k: kilobytes\n");
    printf("m: megabytes\n");
    printf("g: gigabytes\n");

}
void help_launch(){
    printf(" -F : immediate demon start(synchronisation)\n");
    printf(" -S : safe demon kill(end synchronisation if already running)\n");
    printf(" -K : immediate demon kill\n");
}