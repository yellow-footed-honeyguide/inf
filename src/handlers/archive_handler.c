// Include necessary header files
#include "../file_info.h"  // For add_info() function
#include "../utils.h"      // For execute_command() function
#include <stdio.h>         // For sscanf()
#include <stdlib.h>        // For free()
#include <string.h>        // For strtok()

// Function to extract information from archive files
void get_archive_info(const char *path) {
    // Prepare the command to list archive contents
    // MAX_COMMAND_LENGTH is defined in utils.h
    char command[MAX_COMMAND_LENGTH];
    
    // Use snprintf for safe string formatting
    // '7z l' lists the contents of the archive
    // The single quotes around %s prevent issues with spaces in filenames
    snprintf(command, sizeof(command), "7z l '%s'", path);

    // Execute the command and get its output
    char *output = execute_command(command);
    
    // Check if the command execution was successful
    if (output) {
        // Variables to store archive information
        char *line = strtok(output, "\n");  // Split output into lines
        int file_count = 0;                 // Number of files in the archive
        size_t total_size = 0;              // Total uncompressed size of files

        // Iterate through each line of the output
        while (line) {
            // Look for the line containing the file count
            if (strstr(line, "files,")) {
                // Extract the file count using sscanf
                sscanf(line, "%d files,", &file_count);
            } 
            // Look for the line containing the total size
            else if (strstr(line, "bytes")) {
                // Extract the total size using sscanf
                sscanf(line, "%zu bytes", &total_size);
            }
            // Move to the next line
            line = strtok(NULL, "\n");
        }

        // Prepare strings to store the extracted information
        char count_str[50], size_str[50];
        
        // Format the file count as a string
        snprintf(count_str, sizeof(count_str), "%d", file_count);
        // Format the total size as a string
        snprintf(size_str, sizeof(size_str), "%zu bytes", total_size);

        // Add the extracted information to the global info array
        add_info("Files in archive", count_str);
        add_info("Total uncompressed size", size_str);

        // Free the memory allocated by execute_command()
        free(output);
    }
    // If output is NULL, the command failed, but we silently ignore it
    // You might want to add error handling here in a more robust version
}
