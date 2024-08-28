// Include the header file for this source file
#include "utils.h"

// Include necessary standard library headers
#include <stdio.h>   // For FILE*, popen(), pclose(), fgets()
#include <stdlib.h>  // For malloc(), free()
#include <string.h>  // For strcpy(), strlen()

// Function to execute a shell command and return its output
char *execute_command(const char *command) {
    // popen() creates a pipe, forks, and invokes the shell to execute the command
    // "r" means we're opening the pipe for reading
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        // If popen() fails, return NULL
        return NULL;
    }

    // Allocate memory for the output string
    // We're using MAX_OUTPUT_LENGTH defined in utils.h
    char *output = malloc(MAX_OUTPUT_LENGTH);
    if (output == NULL) {
        // If malloc fails, close the pipe and return NULL
        pclose(fp);
        return NULL;
    }

    // Initialize the output length to 0
    size_t output_len = 0;
    // Create a buffer for reading chunks of output
    char buffer[128];

    // Read the output of the command in chunks
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        // Check if we have enough space in our output buffer
        if (output_len + len < MAX_OUTPUT_LENGTH - 1) {
            // If we do, copy the chunk to our output buffer
            strcpy(output + output_len, buffer);
            output_len += len;
        } else {
            // If we don't have enough space, break the loop
            // This prevents buffer overflow
            break;
        }
    }

    // Close the pipe
    pclose(fp);

    // Return the collected output
    return output;
}

// Function to format file size into a human-readable string
char* format_size(off_t size) {
    // Array of size unit strings
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    // Start with the size in bytes
    double formatted_size = size;
    
    // Keep dividing by 1024 and increasing the unit index
    // until we get a number less than 1024 or reach the largest unit
    while (formatted_size >= 1024 && i < 4) {
        formatted_size /= 1024;
        i++;
    }
    
    // Allocate memory for the result string
    // 20 bytes should be enough for most sizes (e.g., "999.99 TB")
    char* result = malloc(20);
    if (result == NULL) {
        // If malloc fails, return NULL
        return NULL;
    }

    // Format the size with 2 decimal places and the appropriate unit
    // snprintf is used for safe string formatting
    snprintf(result, 20, "%.2f %s", formatted_size, units[i]);

    // Return the formatted string
    return result;
}
