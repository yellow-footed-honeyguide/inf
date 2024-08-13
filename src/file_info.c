#define _GNU_SOURCE
#include "file_info.h"
#include "utils.h"
#include "handlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <magic.h>

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
