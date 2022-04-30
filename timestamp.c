#include <utime.h>
#include <sys/stat.h>

#include "timestamp.h"

void clone_timestamp(char* source_file, char* destination_file){
    struct stat st;
    struct utimbuf new_time;
    stat(source_file, &st);
    new_time.actime = st.st_atim.tv_sec;// fking st_atim doesn't work
    new_time.modtime = st.st_mtim.tv_sec;// fking st_mtim doesn't work
    utime(destination_file, &new_time);
    chmod(destination_file, st.st_mode);
    
}

bool compare_timestamp(char* source_file, char* destination_file){
    struct stat st1;
    struct stat st2;
    stat(source_file, &st1);
    stat(destination_file, &st2);
    return (st1.st_mtim.tv_sec == st2.st_mtim.tv_sec);

}