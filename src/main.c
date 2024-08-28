#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_info.h"  // Includes functions for processing and displaying file information
#include "version.h"    // Contains version information for the utility

// Function to print usage instructions
void print_usage(const char* program_name) {
    printf("Usage: %s [OPTION] <file_path>\n", program_name);
    printf("Get information about a file.\n\n");
    printf("Options:\n");
    printf("  -h, --help     Display this help and exit\n");
    printf("  -v, --version  Output version information and exit\n");
}

// Function to print version information
void print_version() {
    printf("inf version %s\n", FILE_INFO_VERSION);
}

int main(int argc, char *argv[]) {
    // Check if at least one argument is provided
    if (argc < 2) {
        print_usage(argv[0]);  // Print usage if no arguments
        return 1;  // Exit with error code
    }

    // Check for help option
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;  // Exit successfully after printing help
    }

    // Check for version option
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        print_version();
        return 0;  // Exit successfully after printing version
    }

    const char *file_path = argv[1];  // Get the file path from command line argument
    init_info_array();  // Initialize the info array (defined in file_info.h)
    process_file(file_path);  // Process the file and gather information
    display_info();  // Display the gathered information
    free_info_array();  // Free the memory used by the info array
    
    return 0;  // Exit successfully
}
