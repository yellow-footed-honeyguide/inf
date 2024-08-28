// Include necessary header files
#include "../file_info.h"  // For add_info() function
#include "../utils.h"      // For execute_command() function and MAX_COMMAND_LENGTH
#include <stdio.h>         // For NULL
#include <stdlib.h>        // For free()
#include <string.h>        // For strtok(), strlen()

// Function to extract information from PDF files
void get_pdf_info(const char *path) {
    // Buffer to store the command
    char command[MAX_COMMAND_LENGTH];
    
    // Prepare command to get PDF information
    // 'pdfinfo' is a command-line utility that extracts metadata from PDF files
    snprintf(command, sizeof(command), "pdfinfo '%s'", path);
    
    // Execute the command and get its output
    char *output = execute_command(command);
    
    // Check if the command execution was successful
    if (output) {
        // Use strtok to split the output into lines
        // strtok modifies the original string, replacing delimiters with null characters
        char *line = strtok(output, "\n");
        
        // Process each line of the output
        while (line) {
            // Use strtok again to split each line into key and value
            // Note: this modifies 'line', replacing the first ':' with a null character
            char *key = strtok(line, ":");
            char *value = strtok(NULL, ":");
            
            // Check if both key and value were found
            if (key && value) {
                // Trim leading whitespace from value
                // This is a common C idiom for skipping leading spaces
                while (*value == ' ') value++;
                
                // Add the key-value pair to the global info array
                add_info(key, value);
            }
            
            // Move to the next line
            // Subsequent calls to strtok with NULL continue from where it left off
            line = strtok(NULL, "\n");
        }
        
        // Free the memory allocated by execute_command()
        free(output);
    }
    // If output is NULL, the command failed, but we silently ignore it
    // You might want to add error handling here in a more robust version
}
