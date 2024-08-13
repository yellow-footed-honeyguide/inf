#include "../file_info.h"
#include <stdio.h>

void get_text_file_info(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", path);
        return;
    }

    int lines = 0, words = 0, chars = 0;
    int in_word = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        chars++;
        if (ch == '\n') {
            lines++;
        }
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    fclose(file);

    char count_str[50];
    snprintf(count_str, sizeof(count_str), "%d", lines);
    add_info("Lines", count_str);
    snprintf(count_str, sizeof(count_str), "%d", words);
    add_info("Words", count_str);
    snprintf(count_str, sizeof(count_str), "%d", chars);
    add_info("Characters", count_str);
}
