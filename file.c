#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <unistd.h>

#include "file.h"


FILE_TYPE get_file_type(const char* path){
    struct stat st;
    stat(path, &st);
    if(S_ISLNK(st.st_mode)){
        return SYMBOLIC_LINK;
    }else if(S_ISDIR(st.st_mode)){
        return DIRECTORY;
    }else if(S_ISCHR(st.st_mode)){
        return CHARACTER_DEVICE;
    }else if(S_ISBLK(st.st_mode)){
        return BLOCK_DEVICE;
    }else if(S_ISFIFO(st.st_mode)){
        return FIFO;
    }else if(S_ISSOCK(st.st_mode)){
        return SOCKET;
    }else if(S_ISREG(st.st_mode)){
        return REGULAR_FILE;
    }else{
        return UNKNOWN;
    }
}

bool check_dir(const char* name){
    DIR* directory = opendir(name);
    if(directory){
        closedir(directory);
        return true;
    }else{
        perror(name);
        return false;
    }
}
bool contains(const char* name1, const char* name2){
    char* a,* b, bufa[PATH_MAX+1], bufb[PATH_MAX+1];
    a = realpath(name1,bufa);
    b = realpath(name2,bufb);
    int c;
    if(strlen(a)<strlen(b)){
        c=strlen(a);
    }else{
        c=strlen(b);
    }
    if(strncmp(a,b,c)==0){
        return false;
    }else{
        return true;
    }
}
bool file_exists(const char* name){
    if(access(name, F_OK) == 0){
        return true;
    }else{
        return false;
    }
}

file_list* read_dir_content(char* path, bool recursive){
    file_list* list = list_create();
    DIR* directory;
    struct dirent* entry;
    directory = opendir(path);

    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }
        int length = strlen(path);
        length += strlen(entry->d_name)+2;
        char full_path[length];
        snprintf(full_path, length, "%s/%s", path, entry->d_name);
        FILE_TYPE type;
        type = get_file_type(full_path);
        if(type == REGULAR_FILE){
            list_add(list,entry->d_name, path, type, true);
        }
        if(type == DIRECTORY && recursive == true){
            list_add(list, entry->d_name, path, type, true);
            list_append(list,read_dir_content(full_path,true));
        }
    }
    closedir(directory);
    return list;
}