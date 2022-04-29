#ifndef DIR_H
#define DIR_H

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

//copy content of one file and create identical copy
void copy_file_rw(char* source_path, char* destination_path);
#endif