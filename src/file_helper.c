#include "file_helper.h" 
#include "da.h"
#include "sb.h"

#ifdef __linux__

#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h> 
#include <stdio.h> 
#include <time.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <unistd.h> 

#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

struct stat st = {0};

int is_dir_exist(const char *path) 
{
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return DIR_OK;
    }

    return DIR_ERR;
}

int create_dir(const char *path) 
{
    return mkdir(path, 0777);
}

int scan_path(const char *path, dir_content *output) 
{
    (void)output;

    DIR *dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "ERR: Path [%s] cant be open\n", path);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if      (strcmp(entry->d_name, ".") == 0) {}
        else if (strcmp(entry->d_name, "..") == 0) {}
        else { da_append(output, entry->d_name); }
    }

    return 0;
}

date get_current_utc_time(void)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    return (date) {
        .sec  = t->tm_sec,
        .min  = t->tm_min,
        .hour = t->tm_hour,
        .day  = t->tm_mday,
        .mon  = (t->tm_mon + 1),    // ;-; 
        .year = (t->tm_year + 1900) // ;-; 
    };
}

int get_current_epoch_time(void) 
{
    return (int)time(NULL);
}

int open_editor(const char *path_file) 
{
    // This function replace current process. We never return to this process unless 
    // we ocure some error.
    int ret = execlp("nvim", "nvim", path_file, (char *)NULL);
    exit(ret);
}

#endif

#ifdef _WIN32
#   error "WIN is not implementation yet"
#endif
