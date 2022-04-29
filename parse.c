#include <string.h>
#include <stdio.h>

#include "parse.h"

int parse_time(const char* time){
    int seconds = 0;
    for(int i=0;i<strlen(time)-1;i++){
        //check if character is integer
        if(time[i] > 47 && time[i] < 58){
            seconds *= 10;
            seconds += (time[i]-48);
        }else{
            return -1; //error: string contains illegal characters
        }
    }

    char last = time[strlen(time)-1]; //last character of string
    //if last character is integer add
    if(last > 47 && last < 57){
        seconds *= 10;
        seconds += (last-48);
    }else{
        //if not then multiple by appropriate multiplier
        switch(last){
        case 's':
            seconds *= 1;
            break;
        case 'm':
            seconds *= 60;
            break;
        case 'h':
            seconds *=3600;
            break;
        default:
            return -1;
            break; //error: last character is illegal
        }
    }
    return seconds;
}

size_t parse_size(const char* size){
    size_t bytes = 0;
    for(int i=0;i<strlen(size)-1;i++){
        if(size[i] > 47 && size[i] < 58){
            bytes *= 10;
            bytes += (size[i]-48);
        }else{
            return -1; //error string contains illegal characters
        }
    }
    char last = size[strlen(size)-1];
    if(last > 47 && last < 58){
        bytes *= 10;
        bytes += (last-48);
    }else{
        switch (last)
        {
        case 'K':
            bytes *= 1024;
            break;
        case 'k':
            bytes *= 1024;
            break;
        case 'M':
            bytes *= 1024*1024;
            break;
        case 'm':
            bytes *= 1024*1024;
            break;
        case 'G':
            bytes *= 1024*1024*1024;
            break;
        case 'g':
            bytes *= 1024*1024*1024;
            break;
        default:
            return -1; //error illegal character
            break;
        }
    }
    return bytes;
}

config parse_args(int argc, char* argv[]){
    config c = default_configuration();
    if(argc < 3){
        return c;
    }
    int dir_param_count = 0;
    for(int i=1;i<argc;i++){
        char* arg = argv[i];
        if(arg[0] == '-'){
            char opt = arg[1];
            if(strlen(arg) != 2){
                printf("Error: unknown option: %s\n", arg);
                return c;
            }
            switch (opt)
            {
            case 's':
                c.sleep_time = parse_time(argv[i+1]);
                if(c.sleep_time == -1){
                    printf("Error: invalid format of time: %s\n", argv[i+1]);
                    return c;
                }
                i++;
                break;
            case 'R':
                c.recursive_sync = true;
                break;
            case 'm':
                c.mmap_size_treshold = parse_size(argv[i+1]);
                if(c.mmap_size_treshold == -1){
                    printf("Error : invalid format of size: %s\n", argv[i+1]);
                    return c;
                }
                i++;
                break;
            default:
                break;
            }
        }else{
            if(arg[strlen(arg)-1] == '/'){
                arg[strlen(arg)-1] = '\0';
            }
            if(dir_param_count == 0){
                c.source_directory = arg;
            }else if (dir_param_count == 1)
            {
                c.destination_directory = arg;
            }
            dir_param_count++;
            
        }
    }
    if(dir_param_count == 2){
        c.is_valid = true;
    }
    return c;

}