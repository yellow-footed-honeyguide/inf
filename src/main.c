#include <stdio.h>
#include <stdlib.h>
#include "file_info.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    const char *file_path = argv[1];
    
    init_info_array();
    
    process_file(file_path);
    
    display_info();
    
    free_info_array();
    
    return 0;
}
