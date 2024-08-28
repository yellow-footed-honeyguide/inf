// Include necessary header files
#include "../file_info.h"  // For add_info() function
#include "../utils.h"      // For execute_command() function and MAX_COMMAND_LENGTH
#include <stdio.h>         // For snprintf()
#include <stdlib.h>        // For atof(), free()

// Function to extract duration information from video files
void get_video_duration(const char *path) {
    // Buffer to store the command
    char command[MAX_COMMAND_LENGTH];
    
    // Prepare ffprobe command to get video duration
    // -v error: Set loglevel to error to suppress unnecessary output
    // -show_entries format=duration: Only show the duration information
    // -of default=noprint_wrappers=1:nokey=1: Format output without labels, just the value
    snprintf(command, sizeof(command),
             "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 '%s'",
             path);
    
    // Execute the command and get its output
    char *output = execute_command(command);
    
    // Check if the command execution was successful
    if (output) {
        // Convert the string output to a double (duration in seconds)
        double duration = atof(output);
        
        // Calculate hours, minutes, seconds, and milliseconds
        int hours = (int)duration / 3600;
        int minutes = ((int)duration % 3600) / 60;
        int seconds = (int)duration % 60;
        int milliseconds = (int)((duration - (int)duration) * 1000);
        
        // Prepare a formatted string with the duration
        char duration_str[30];
        snprintf(duration_str, sizeof(duration_str),
                 "%02d:%02d:%02d.%03d",
                 hours, minutes, seconds, milliseconds);
        
        // Add the formatted duration to the global info array
        add_info("Duration", duration_str);
        
        // Free the memory allocated by execute_command()
        free(output);
    }
    // If output is NULL, the command failed, but we silently ignore it
    // You might want to add error handling here in a more robust version
}
