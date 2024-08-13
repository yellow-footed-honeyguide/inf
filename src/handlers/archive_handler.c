#include "../file_info.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_archive_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "7z l '%s'", path);
    char *output = execute_command(command);
    if (output) {
        char *line = strtok(output, "\n");
        int file_count = 0;
        size_t total_size = 0;
        while (line) {
            if (strstr(line, "files,")) {
                sscanf(line, "%d files,", &file_count);
            } else if (strstr(line, "bytes")) {
                sscanf(line, "%zu bytes", &total_size);
            }
            line = strtok(NULL, "\n");
        }
        char count_str[50], size_str[50];
        snprintf(count_str, sizeof(count_str), "%d", file_count);
        snprintf(size_str, sizeof(size_str), "%zu bytes", total_size);
        add_info("Files in archive", count_str);
        add_info("Total uncompressed size", size_str);
        free(output);
    }
}
