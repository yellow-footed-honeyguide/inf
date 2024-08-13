#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_OUTPUT_LENGTH 4096

char *execute_command(const char *command);
char* format_size(off_t size);

#endif // UTILS_H
