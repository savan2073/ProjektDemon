#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdbool.h>

//struct which stores current configuration of demon

typedef struct 
{
    bool is_valid; //defines is current config is valid
    int sleep_time; //time to sleep of demon (expressed in seconds)
    bool recursive_sync; //defines should sync be recursive
    size_t mmap_size_treshold; //threshold between small and large files (expressed in bytes)
    char* source_directory; //source dir path
    char* destination_directory; //destination dir path
}config;

config default_configuration(); //returns default configuration



#endif