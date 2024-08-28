// Include necessary header files
#include "../file_info.h"  // For add_info() function
#include "../utils.h"      // For execute_command() function and MAX_COMMAND_LENGTH
#include <stdio.h>         // For NULL
#include <stdlib.h>        // For free()

// Function to extract information from image files
void get_image_info(const char *path) {
    // Buffer to store the command
    char command[MAX_COMMAND_LENGTH];
    
    // Prepare command to get image dimensions
    // 'identify' is a command-line utility from ImageMagick
    // -format '%wx%h' specifies the output format: width x height
    snprintf(command, sizeof(command), "identify -format '%%wx%%h' '%s'", path);
    
    // Execute the command and get its output
    char *output = execute_command(command);
    
    // Check if the command execution was successful
    if (output) {
        // Add the dimensions to the global info array
        add_info("Dimensions", output);
        
        // Free the memory allocated by execute_command()
        free(output);
    }

    // Prepare command to get image color space
    // -format '%r' specifies the output format: color space
    snprintf(command, sizeof(command), "identify -format '%%r' '%s'", path);
    
    // Execute the command and get its output
    output = execute_command(command);
    
    // Check if the command execution was successful
    if (output) {
        // Add the color space to the global info array
        add_info("Color space", output);
        
        // Free the memory allocated by execute_command()
        free(output);
    }
}
