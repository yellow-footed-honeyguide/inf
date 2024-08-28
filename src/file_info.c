// Define _GNU_SOURCE to enable certain GNU extensions in glibc
#define _GNU_SOURCE

// Include necessary header files
#include "file_info.h"   // Contains declarations for functions defined in this file
#include "utils.h"       // Contains utility functions like execute_command and format_size
#include "handlers.h"    // Contains declarations for file type specific handlers
#include <stdio.h>       // Standard I/O functions
#include <stdlib.h>      // Standard library functions, including memory allocation
#include <string.h>      // String manipulation functions
#include <sys/stat.h>    // File status and information functions
#include <time.h>        // Time and date functions
#include <magic.h>       // libmagic for file type detection

// Declare the global InfoArray variable (defined in file_info.h)
InfoArray info_array;

// Initialize the InfoArray
void init_info_array() {
    // Set all members to NULL or 0
    info_array.data = NULL;    // No data allocated yet
    info_array.size = 0;       // No elements in the array
    info_array.capacity = 0;   // No capacity allocated
}

// Add a new key-value pair to the InfoArray
void add_info(const char *key, const char *value) {
    // Check if we need to allocate more memory
    if (info_array.size == info_array.capacity) {
        // Double the capacity if it's not zero, otherwise set it to 1
        info_array.capacity = info_array.capacity ? info_array.capacity * 2 : 1;
        // Reallocate memory for the new capacity
        // realloc can be used here as we don't need to preserve the old data if it fails
        info_array.data = realloc(info_array.data, info_array.capacity * sizeof(KeyValue));
    }
    // Allocate memory for the new key and value strings and copy them
    // strdup allocates memory and copies the string, it's equivalent to malloc + strcpy
    info_array.data[info_array.size].key = strdup(key);
    info_array.data[info_array.size].value = strdup(value);
    // Increment the size of the array
    info_array.size++;
}

// Free all allocated memory in the InfoArray
void free_info_array() {
    // Iterate through all elements
    for (size_t i = 0; i < info_array.size; i++) {
        // Free the memory allocated for each key and value
        free(info_array.data[i].key);
        free(info_array.data[i].value);
    }
    // Free the memory allocated for the array itself
    free(info_array.data);
}

// Get basic file information using stat
void get_basic_info(const char *path) {
    struct stat st;  // Declare a stat struct to hold file information
    // Use stat to get file information, returns 0 on success
    if (stat(path, &st) == 0) {
        // Format the file size using the utility function
        char* formatted_size = format_size(st.st_size);
        // Add the formatted size to the info array
        add_info("Size", formatted_size);
        // Free the memory allocated by format_size
        free(formatted_size);

        // Convert the modification time to a string
        char *time_str = ctime(&st.st_mtime);
        // Remove the newline character at the end of the time string
        time_str[strlen(time_str) - 1] = '\0';
        // Add the modification time to the info array
        add_info("Last modified", time_str);

        // Create a string to hold the file permissions
        char perms[11];
        // Use snprintf to safely format the permissions string
        snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c%c",
                 S_ISDIR(st.st_mode) ? 'd' : '-',  // Is it a directory?
                 (st.st_mode & S_IRUSR) ? 'r' : '-',  // Owner read permission
                 (st.st_mode & S_IWUSR) ? 'w' : '-',  // Owner write permission
                 (st.st_mode & S_IXUSR) ? 'x' : '-',  // Owner execute permission
                 (st.st_mode & S_IRGRP) ? 'r' : '-',  // Group read permission
                 (st.st_mode & S_IWGRP) ? 'w' : '-',  // Group write permission
                 (st.st_mode & S_IXGRP) ? 'x' : '-',  // Group execute permission
                 (st.st_mode & S_IROTH) ? 'r' : '-',  // Others read permission
                 (st.st_mode & S_IWOTH) ? 'w' : '-',  // Others write permission
                 (st.st_mode & S_IXOTH) ? 'x' : '-'); // Others execute permission
        // Add the permissions string to the info array
        add_info("Permissions", perms);
    }
}

// Get the MIME type of the file using libmagic
void get_mime_type(const char *path) {
    // Initialize the magic library
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    if (magic == NULL) {
        fprintf(stderr, "Unable to initialize magic library\n");
        return;
    }

    // Load the default magic database
    if (magic_load(magic, NULL) != 0) {
        fprintf(stderr, "Cannot load magic database - %s\n", magic_error(magic));
        magic_close(magic);
        return;
    }

    // Get the MIME type of the file
    const char *mime = magic_file(magic, path);
    if (mime != NULL) {
        // Add the MIME type to the info array
        add_info("MIME type", mime);
    }

    // Clean up the magic library
    magic_close(magic);
}

// Process the file and gather all relevant information
void process_file(const char *path) {
    // Get basic file information (size, permissions, last modified date)
    get_basic_info(path);
    // Get the MIME type of the file
    get_mime_type(path);

    // Prepare a command to get the file type using the 'file' utility
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "file -b '%s'", path);
    // Execute the command and get the output
    char *file_type = execute_command(command);
    if (file_type) {
        // Add the file type to the info array
        add_info("File type", file_type);

        // Based on the file type, call the appropriate handler
        if (strstr(file_type, "text") || strstr(file_type, "ASCII")) {
            get_text_file_info(path);
        } else if (strstr(file_type, "image")) {
            get_image_info(path);
        } else if (strstr(file_type, "video") || strstr(file_type, "MP4")) {
            get_video_duration(path);
        } else if (strstr(file_type, "PDF")) {
            get_pdf_info(path);
        } else if (strstr(file_type, "archive") || strstr(file_type, "compressed")) {
            get_archive_info(path);
        }

        // Free the memory allocated by execute_command
        free(file_type);
    }
}

// Display all gathered information
void display_info() {
    printf("File Information\n");
    printf("================\n\n");

    // Find the maximum width of the keys for alignment
    int max_key_width = 0;
    for (size_t i = 0; i < info_array.size; i++) {
        int key_len = strlen(info_array.data[i].key);
        if (key_len > max_key_width) {
            max_key_width = key_len;
        }
    }

    // Display each key-value pair
    for (size_t i = 0; i < info_array.size; i++) {
        // Use %-*s for left-aligned string with dynamic width
        printf("%-*s : %s\n", max_key_width, info_array.data[i].key, info_array.data[i].value);
    }
}
