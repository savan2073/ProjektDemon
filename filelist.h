#ifndef FILELIST_H
#define FILELIST_H

#include <stdlib.h>
#include <stdbool.h>
#include "filetype.h"

//list is used to storage list of files in directory

typedef struct file_list file_list;

struct file_list{
    char* name; //name file or dir
    char* path; //path to parent dir
    bool mmap; //whether file should be copied with mmap
    FILE_TYPE type;
    file_list* next; //indicator on next element
};

//create new list
file_list* list_create();

//add new element to list, return address of last element
file_list* list_add(file_list* list, char* name, char* path, FILE_TYPE type, bool mmap);

//add on end of first list second
file_list* list_append(file_list* list, file_list* next_list);

//reverse list
file_list* list_reverse(file_list* list);

//clear list of all elements
void list_clear_all(file_list* first);


#endif