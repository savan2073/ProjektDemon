#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <assert.h>
#include <sys/mman.h>

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

void create_file(char* path){
    __mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH; //permissions
    int fd = open(path, O_WRONLY | O_EXCL | O_CREAT, mode); //file creation
    if(fd == -1){
        syslog(LOG_CRIT, "ERROR: file can't be created");
        exit(EXIT_FAILURE);
    }
    close(fd);
}
ssize_t write_whole(int fd, const void* buffer, size_t count){
    size_t left_to_write = count;
    while(left_to_write > 0){
        size_t written = write(fd, buffer, count);
        if(written == -1){
            return -1;
        }else{
            //check how much left to be written
            left_to_write -= written;
        }
        //making sure to not write too much
        assert ( left_to_write == 0);
        //number of bytes written is count
        return count;
    }
}

void copy_file_rw(char* source_path, char* destination_path){
    unsigned char buffer[16];
    size_t offset = 0;
    size_t bytes_read;

    //open source file
    int sfd = open(source_path, O_RDONLY);

    //create destination file
    create_file(destination_path);
    int dfd = open(destination_path, O_WRONLY);

    //read part by part
    //not finished!!!!!! do reading and writing to dest file
    do{
        bytes_read = read(sfd, buffer, sizeof(buffer));
        write_whole(dfd, buffer, bytes_read);
        //keep track of position in file
        offset += bytes_read;
    }while(bytes_read == sizeof(buffer));

    //close file descriptor
    close(sfd);
    close(dfd);
}

void copy_file_mmap(char* source_path, char* destination_path){
    int sfd, dfd;
    char *source,* destination;
    struct stat s;
    size_t filesize;

    //source file
    sfd = open(source_path,O_RDONLY);
    filesize = lseek(sfd, 0, SEEK_END);
    source = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    //destination file
    dfd = open(destination_path, O_RDWR | O_CREAT, 0666);
    ftruncate(dfd, filesize);
    destination = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);

    //making actual copy
    memcpy(destination, source, filesize);
    munmap(source, filesize);
    munmap(destination, filesize);

    close(sfd);
    close(dfd);
}

void copy_file(char* source_path, char* destination_path, bool mmap_on){
    if(mmap_on == true){
        copy_file_mmap(source_path,destination_path);
    }else{
        copy_file_rw(source_path, destination_path);
    }
}