#include "sb.h"

#define SB_INITIAL_CAP 64


#include <ctype.h>
#include <string.h>
#include <stdbool.h>


bool trim_char(char c) 
{
    return c == ' ' || c == '\n';
}

void trim(char *str) 
{
    char *l = str;
    char *r = str + (strlen(str) - 1);

    while (trim_char(*l) || trim_char(*r)) {
        if (trim_char(*l)) { l++; }
        if (trim_char(*r)) { r--; }
    }

    char *trimed_str = str;
    for (char *ptr = l; ptr != (r + 1); ptr++) {
        *trimed_str = *ptr;
        trimed_str++;
    }
    *trimed_str = '\0';
}

string_builder *sb_create(void) 
{
    string_builder *sb = malloc(sizeof(string_builder));
    sb->str    = malloc(SB_INITIAL_CAP);
    sb->str[0] = '\0';
    sb->len    = 0;
    sb->cap    = SB_INITIAL_CAP;
    return sb;
}

void sb_append(string_builder *sb, const char *fmt, ...) 
{
    va_list args;
    va_start(args, fmt);
    int needed = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (needed < 0) { return; }

    if (sb->len + needed + 1 > sb->cap) {
        sb->cap = sb->len + needed + 1;
        sb->str = realloc(sb->str, sb->cap);
    }

    va_start(args, fmt);
    vsprintf(sb->str + sb->len, fmt, args);
    va_end(args);

    sb->len += needed;
}

void sb_free(string_builder *sb) 
{
    free(sb->str);
    free(sb);
}
