#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *execute_command(const char *command) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return NULL;
    }

    char *output = malloc(MAX_OUTPUT_LENGTH);
    size_t output_len = 0;
    char buffer[128];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (output_len + len < MAX_OUTPUT_LENGTH - 1) {
            strcpy(output + output_len, buffer);
            output_len += len;
        } else {
            break;
        }
    }

    pclose(fp);
    return output;
}

char* format_size(off_t size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double formatted_size = size;
    
    while (formatted_size >= 1024 && i < 4) {
        formatted_size /= 1024;
        i++;
    }
    
    char* result = malloc(20);  // Достаточно для большинства размеров
    snprintf(result, 20, "%.2f %s", formatted_size, units[i]);
    return result;
}
