#ifndef DIR_H
#define DIR_H

#include <stdbool.h>
#include "filelist.h"
#include "filetype.h"
#include "config.h"

//check type of file given in param
//whether symlink, dir, file etc.
FILE_TYPE get_file_type(const char* path);

//check if dir is actually dir
//does it exist and user can open dir
//in case of error gives annoucement at stdout
bool check_dir(const char* name);


//check if dir contains other dir
bool contains(const char* name1, const char* name2);

//check if file exists
bool file_exists(const char* name);

//return contents of dir given as parameter and all subdirectories
file_list* read_dir_content(char* path, bool recursive);

//create blank file
void create_file(char* path);

//save whole buffer to file descriptor
ssize_t write_whole(int fd, const void* buffer, size_t count);

//copy content of one file and create identical copy
void copy_file_rw(char* source_path, char* destination_path);

//copy content of file using mmap
void copy_file_mmap(char* source_path, char* destination_path);

//choce proper copying method
void copy_file(char* source_path, char* destination_path, bool mmap_on);

//delete file and dir based on their presence in source dir
void remove_files(config c);
#endif