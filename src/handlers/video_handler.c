#include "../file_info.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

void get_video_duration(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 '%s'", path);
    char *output = execute_command(command);
    if (output) {
        double duration = atof(output);
        int hours = (int)duration / 3600;
        int minutes = ((int)duration % 3600) / 60;
        int seconds = (int)duration % 60;
        int milliseconds = (int)((duration - (int)duration) * 1000);
        
        char duration_str[30];
        snprintf(duration_str, sizeof(duration_str), "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
        add_info("Duration", duration_str);
        
        free(output);
    }
}
