#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include "config.h"

//parse time from text form to integer in seconds
int parse_time(const char* time);

//parse time from text to integer in bytes
size_t parse_size(const char* size);

//based on given args generate config structure defininf actual configuration
config parse_args(int argc, char* argv[]);

#endif