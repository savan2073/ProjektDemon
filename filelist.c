#include <string.h>
#include "filelist.h"

file_list* list_create(){
    file_list* list = malloc(sizeof(file_list));
    list->name = NULL;
    list->path = NULL;
    list->next = NULL;
    return list;
}

file_list* list_add(file_list* list, char* name, char* path, FILE_TYPE type, bool mmap){
    while(list->next != NULL){
        list = list->next;
    }
    list->next = malloc(sizeof(file_list));
    list = list->next;
    list->name = malloc(strlen(name)+1);
    strcpy(list->name, name);
    list->path = malloc(strlen(path)+1);
    strcpy(list->path, path);
    list->mmap = mmap;
    list->type = type;
    list->next = NULL;
    return list;
}

file_list* list_append(file_list* list, file_list* next_list){
    while(first->next != NULL){
        first = first->next;
    }
    first->next = next_list->next;
    free(next_list->name);
    free(next_list->path);
    free(next_list);
    return first;
}

file_list* list_revere(file_list* list){
    file_list* first = NULL;
    file_list* next_list = NULL;
    while(list->next != NULL){
        list = list->next;
        next = first;
        first = malloc(sizeof(file_list));
        first->next = next;
        first->name = malloc(strlen(list->name)+1);
        strcpy(first->name, list->name);
        first->path = malloc(strlen(list->path)+1);
        strcpy(first->path, list->path);
        first->mmap = list->mmap;
        first->type = list->type;
    }
    next_list = first;
    first = malloc(sizeof(file_list));
    first->next = next;
    first->name = NULL;
    first->path = NULL;
    return first;

}

void list_clear_all(file_list* first){
    if(first == NULL){
        return;
    }
    while(first->next !=NULL){
        file_list* previous = first;
        first = first->next;
        free(previous->name);
        free(previous->path);
        free(previous);
    }
    free(first);

}