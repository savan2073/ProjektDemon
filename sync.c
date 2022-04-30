#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/stat.h>

#include "sync.h"
#include "file.h"
#include "timestamp.h"


void sync_all(config c){
    //remove files which not exist
    remove_files(c);

    //read files to prepare synchro to list and create return point(begin)
    file_list* list = read_dir_content(c.source_directory, c.recursive_sync);
    file_list* begin = list;

    while(list->next != NULL){
        list = list->next;
        bool mmap_on;

        //create full path(realpath) destination file
        int length = strlen(list->path) + strlen(list->name) - strlen(c.source_directory) + strlen(c.destination_directory) + 2;
        char full_destination_path[length];
        snprintf(full_destination_path, length, "%s%s/%s", c.destination_directory, list->path + strlen(c.source_directory), list->name);

        //create full path(realpath) source file
        length = 0;
        length = strlen(list->path) + strlen(list-> name) + 2;
        char full_source_path[length];
        snprintf(full_source_path, length, "%s/%s", list->path, list->name);

        //check file size
        struct stat st;
        stat(full_source_path, &st);
        int size = st.st_size;

        if(size >= c.mmap_size_treshold){
            syslog(LOG_INFO, "Using mmap");
            mmap_on = true;
        }

        //compare timestamp and updating files based on timestamps
        if(file_exists(full_destination_path)){
            if(list->type == DIRECTORY){
                if(!compare_timestamp(full_source_path,full_destination_path)){
                    clone_timestamp(full_source_path,full_destination_path);
                }
            }else if(list->type == REGULAR_FILE){
                if(!compare_timestamp(full_source_path,full_destination_path)){
                    syslog(LOG_INFO, "Out of date file being deleted: %s\n", full_destination_path);
                    remove(full_destination_path);
                    syslog(LOG_INFO, "Copying file to: %s\n", full_destination_path);
                    copy_file(full_source_path, full_destination_path, mmap_on);
                    clone_timestamp(full_source_path,full_destination_path);
                }
            }
        }
        //copy rest of files and dirs
        else{
            if(list->type == DIRECTORY){
                syslog(LOG_INFO, " Directory creation: %s\n", full_destination_path);
                mkdir(full_destination_path, 0700);
                clone_timestamp(full_source_path,full_destination_path);
            }else{
                syslog(LOG_INFO, "File copying: %s\n", full_destination_path);
                copy_file(full_source_path, full_destination_path, mmap_on);
                clone_timestamp(full_source_path, full_destination_path);
            }
        }
    }
    list_clear_all(begin);
}