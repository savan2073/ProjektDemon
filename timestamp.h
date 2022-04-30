#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdbool.h>

void clone_timestamp(char* source_file, char* destination_file);

bool compare_timestamp(char* source_file, char* destination_file);

#endif