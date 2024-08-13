#include "../file_info.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_pdf_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "pdfinfo '%s'", path);
    char *output = execute_command(command);
    if (output) {
        char *line = strtok(output, "\n");
        while (line) {
            char *key = strtok(line, ":");
            char *value = strtok(NULL, ":");
            if (key && value) {
                // Trim leading whitespace from value
                while (*value == ' ') value++;
                add_info(key, value);
            }
            line = strtok(NULL, "\n");
        }
        free(output);
    }
}
