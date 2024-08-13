#include "../file_info.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

void get_image_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "identify -format '%%wx%%h' '%s'", path);
    char *output = execute_command(command);
    if (output) {
        add_info("Dimensions", output);
        free(output);
    }

    snprintf(command, sizeof(command), "identify -format '%%r' '%s'", path);
    output = execute_command(command);
    if (output) {
        add_info("Color space", output);
        free(output);
    }
}
