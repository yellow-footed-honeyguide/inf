#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_info.h"
#include "version.h"

void print_usage(const char* program_name) {
    printf("Usage: %s [OPTION] <file_path>\n", program_name);
    printf("Get information about a file.\n\n");
    printf("Options:\n");
    printf("  -h, --help     Display this help and exit\n");
    printf("  -v, --version  Output version information and exit\n");
}

void print_version() {
    printf("file_info version %s\n", FILE_INFO_VERSION);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        print_version();
        return 0;
    }

    const char *file_path = argv[1];
    
    init_info_array();
    
    process_file(file_path);
    
    display_info();
    
    free_info_array();
    
    return 0;
}
