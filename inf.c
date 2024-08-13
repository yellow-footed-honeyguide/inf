#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <magic.h>
#include <ftw.h>
#include <libgen.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_OUTPUT_LENGTH 4096

typedef struct {
    char *key;
    char *value;
} KeyValue;

typedef struct {
    KeyValue *data;
    size_t size;
    size_t capacity;
} InfoArray;

InfoArray info_array;

void init_info_array() {
    info_array.data = NULL;
    info_array.size = 0;
    info_array.capacity = 0;
}

void add_info(const char *key, const char *value) {
    if (info_array.size == info_array.capacity) {
        info_array.capacity = info_array.capacity ? info_array.capacity * 2 : 1;
        info_array.data = realloc(info_array.data, info_array.capacity * sizeof(KeyValue));
    }
    info_array.data[info_array.size].key = strdup(key);
    info_array.data[info_array.size].value = strdup(value);
    info_array.size++;
}

void free_info_array() {
    for (size_t i = 0; i < info_array.size; i++) {
        free(info_array.data[i].key);
        free(info_array.data[i].value);
    }
    free(info_array.data);
}

char *execute_command(const char *command) {
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return NULL;
    }

    char *output = malloc(MAX_OUTPUT_LENGTH);
    size_t output_len = 0;
    char buffer[128];

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        size_t len = strlen(buffer);
        if (output_len + len < MAX_OUTPUT_LENGTH - 1) {
            strcpy(output + output_len, buffer);
            output_len += len;
        } else {
            break;
        }
    }

    pclose(fp);
    return output;
}

char* format_size(off_t size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double formatted_size = size;
    
    while (formatted_size >= 1024 && i < 4) {
        formatted_size /= 1024;
        i++;
    }
    
    char* result = malloc(20);  // Достаточно для большинства размеров
    snprintf(result, 20, "%.2f %s", formatted_size, units[i]);
    return result;
}

void get_basic_info(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        char* formatted_size = format_size(st.st_size);
        add_info("Size", formatted_size);
        free(formatted_size);

        char *time_str = ctime(&st.st_mtime);
        time_str[strlen(time_str) - 1] = '\0';  // Remove newline
        add_info("Last modified", time_str);

        char perms[11];
        snprintf(perms, sizeof(perms), "%c%c%c%c%c%c%c%c%c%c",
                 S_ISDIR(st.st_mode) ? 'd' : '-',
                 (st.st_mode & S_IRUSR) ? 'r' : '-',
                 (st.st_mode & S_IWUSR) ? 'w' : '-',
                 (st.st_mode & S_IXUSR) ? 'x' : '-',
                 (st.st_mode & S_IRGRP) ? 'r' : '-',
                 (st.st_mode & S_IWGRP) ? 'w' : '-',
                 (st.st_mode & S_IXGRP) ? 'x' : '-',
                 (st.st_mode & S_IROTH) ? 'r' : '-',
                 (st.st_mode & S_IWOTH) ? 'w' : '-',
                 (st.st_mode & S_IXOTH) ? 'x' : '-');
        add_info("Permissions", perms);
    }
}

void get_mime_type(const char *path) {
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    if (magic == NULL) {
        fprintf(stderr, "Unable to initialize magic library\n");
        return;
    }

    if (magic_load(magic, NULL) != 0) {
        fprintf(stderr, "Cannot load magic database - %s\n", magic_error(magic));
        magic_close(magic);
        return;
    }

    const char *mime = magic_file(magic, path);
    if (mime != NULL) {
        add_info("MIME type", mime);
    }

    magic_close(magic);
}

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

void get_image_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "identify -format '%%wx%%h' '%s'", path);
    char *output = execute_command(command);
    if (output) {
        add_info("Dimensions", output);
        free(output);
    }

    snprintf(command, sizeof(command), "identify -format '%%r' '%s'", path);
    output = execute_command(command);
    if (output) {
        add_info("Color space", output);
        free(output);
    }
}

void get_video_duration(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 '%s'", path);
    char *output = execute_command(command);
    if (output) {
        double duration = atof(output);
        int hours = (int)duration / 3600;
        int minutes = ((int)duration % 3600) / 60;
        int seconds = (int)duration % 60;
        int milliseconds = (int)((duration - (int)duration) * 1000);
        
        char duration_str[30];
        snprintf(duration_str, sizeof(duration_str), "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
        add_info("Duration", duration_str);
        
        free(output);
    }
}

void get_pdf_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "pdfinfo '%s'", path);
    char *output = execute_command(command);
    if (output) {
        char *line = strtok(output, "\n");
        while (line) {
            char *key = strtok(line, ":");
            char *value = strtok(NULL, ":");
            if (key && value) {
                // Trim leading whitespace from value
                while (*value == ' ') value++;
                add_info(key, value);
            }
            line = strtok(NULL, "\n");
        }
        free(output);
    }
}

void get_archive_info(const char *path) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "7z l '%s'", path);
    char *output = execute_command(command);
    if (output) {
        char *line = strtok(output, "\n");
        int file_count = 0;
        size_t total_size = 0;
        while (line) {
            if (strstr(line, "files,")) {
                sscanf(line, "%d files,", &file_count);
            } else if (strstr(line, "bytes")) {
                sscanf(line, "%zu bytes", &total_size);
            }
            line = strtok(NULL, "\n");
        }
        char count_str[50], size_str[50];
        snprintf(count_str, sizeof(count_str), "%d", file_count);
        snprintf(size_str, sizeof(size_str), "%zu bytes", total_size);
        add_info("Files in archive", count_str);
        add_info("Total uncompressed size", size_str);
        free(output);
    }
}

void process_file(const char *path) {
    get_basic_info(path);
    get_mime_type(path);

    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "file -b '%s'", path);
    char *file_type = execute_command(command);
    if (file_type) {
        add_info("File type", file_type);

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

        free(file_type);
    }
}

void display_info() {
    printf("File Information\n");
    printf("================\n\n");

    int max_key_width = 0;
    for (size_t i = 0; i < info_array.size; i++) {
        int key_len = strlen(info_array.data[i].key);
        if (key_len > max_key_width) {
            max_key_width = key_len;
        }
    }

    for (size_t i = 0; i < info_array.size; i++) {
        printf("%-*s : %s\n", max_key_width, info_array.data[i].key, info_array.data[i].value);
    }
}

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
