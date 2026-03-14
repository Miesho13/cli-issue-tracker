#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    size_t len;
    size_t cap;
} string_builder;

#define _SET_RED           "\x1b[31m"
#define _SET_BRIGHT_BLUE   "\x1b[94m"
#define _SET_BRIGHT_GREEN  "\x1b[92m"
#define _SET_BRIGHT_YELLOW "\x1b[93m"
#define _SET_RESET         "\x1b[0m"

#define SET_RED(s)           _SET_RED           s _SET_RESET
#define SET_BRIGHT_BLUE(s)   _SET_BRIGHT_BLUE   s _SET_RESET
#define SET_BRIGHT_GREEN(s)  _SET_BRIGHT_GREEN  s _SET_RESET
#define SET_BRIGHT_YELLOW(s) _SET_BRIGHT_YELLOW s _SET_RESET

void trim(char *str);
string_builder *sb_create(void);
void sb_append(string_builder *sb, const char *fmt, ...);
void sb_free(string_builder *sb);

#endif // STRING_BUILDER_H
