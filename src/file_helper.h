#ifndef FILE_HELPER
#define FILE_HELPER

#include <stdlib.h>

#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

typedef enum { 
    DIR_OK = 0,
    DIR_ERR = 0xff,
} DIR_RET;

typedef struct {
    char **items;
    size_t count;
    size_t capacity;
} dir_content;

typedef struct {
    int sec;
    int min;
    int hour;
    int day;
    int mon;
    int year;
} date;


int  is_dir_exist(const char *path);
int  create_dir(const char *path);
int  scan_path(const char *path, dir_content *output);
date get_current_utc_time(void);
int  get_current_epoch_time(void);
int  open_editor(const char *path_file);

#endif // FILE_HELPER
