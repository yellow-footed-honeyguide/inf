// Include necessary header files
#include "../file_info.h"  // For add_info() function
#include <stdio.h>         // For FILE, fopen(), fclose(), fgetc(), EOF

// Function to extract information from text files
void get_text_file_info(const char *path) {
    // Attempt to open the file in read mode
    FILE *file = fopen(path, "r");
    // Check if file opening was successful
    if (file == NULL) {
        // If file couldn't be opened, print an error message and return
        fprintf(stderr, "Cannot open file: %s\n", path);
        return;
    }

    // Initialize counters for lines, words, and characters
    int lines = 0, words = 0, chars = 0;
    // Flag to keep track of whether we're currently inside a word
    int in_word = 0;
    // Variable to store each character as we read through the file
    int ch;

    // Read the file character by character until we reach the end (EOF)
    while ((ch = fgetc(file)) != EOF) {
        // Increment the character count for each character read
        chars++;
        
        // Check for newline characters to count lines
        if (ch == '\n') {
            lines++;
        }
        
        // Check for word boundaries (space, newline, or tab)
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            // If we were in a word, we've now left it
            in_word = 0;
        } else if (!in_word) {
            // If we weren't in a word and we've hit a non-whitespace character,
            // we've entered a new word
            in_word = 1;
            words++;
        }
    }

    // Close the file as we're done reading from it
    fclose(file);

    // Prepare a buffer to store our count strings
    char count_str[50];
    
    // Convert line count to string and add to info
    snprintf(count_str, sizeof(count_str), "%d", lines);
    add_info("Lines", count_str);
    
    // Convert word count to string and add to info
    snprintf(count_str, sizeof(count_str), "%d", words);
    add_info("Words", count_str);
    
    // Convert character count to string and add to info
    snprintf(count_str, sizeof(count_str), "%d", chars);
    add_info("Characters", count_str);
}
