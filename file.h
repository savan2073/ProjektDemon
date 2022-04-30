#ifndef DIR_H
#define DIR_H

#include <stdbool.h>
#include "filelist.h"
#include "filetype.h"
#include "config.h"
#include <stdio.h>

//check type of file given in param
//whether symlink, dir, file etc.
FILE_TYPE get_file_type(const char* path);//done

//check if dir is actually dir
//does it exist and user can open dir
//in case of error gives annoucement at stdout
bool check_dir(const char* name);//done


//check if dir contains other dir
bool contains(const char* name1, const char* name2);//done

//check if file exists
bool file_exists(const char* name);//done

//return contents of dir given as parameter and all subdirectories
file_list* read_dir_content(char* path, bool recursive);//done

//create blank file
void create_file(char* path);//done

//save whole buffer to file descriptor
__ssize_t write_whole(int fd, const void* buffer, size_t count);//done

//copy content of one file and create identical copy
void copy_file_rw(char* source_path, char* destination_path);//done

//copy content of file using mmap
void copy_file_mmap(char* source_path, char* destination_path);//done

//choce proper copying method
void copy_file(char* source_path, char* destination_path, bool mmap_on);//done

//delete file and dir based on their presence in source dir
void remove_files(config c);// not finished
#endif