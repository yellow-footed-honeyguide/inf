#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stddef.h>

typedef struct {
    char *key;
    char *value;
} KeyValue;

typedef struct {
    KeyValue *data;
    size_t size;
    size_t capacity;
} InfoArray;

extern InfoArray info_array;

void init_info_array();
void add_info(const char *key, const char *value);
void free_info_array();
void get_basic_info(const char *path);
void get_mime_type(const char *path);
void process_file(const char *path);
void display_info();

#endif // FILE_INFO_H
