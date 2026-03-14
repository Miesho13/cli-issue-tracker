#include "issue.h"
#include "file_helper.h"
#include "sb.h"
#include "da.h"
#include "type.h"

#include <assert.h> 
#include <stdbool.h> 
#include <stdint.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ISSUES_BASE_PATH ".issues"

static int gen_uid(void) 
{
    // TODO: If this issue tracker is used as a service for multiple users, a machine ID and 
    // millisecond precision should be added. For now, we use only the epocho time.
    return get_current_epoch_time();
}

void create_issue_dir(char *issue_path) 
{
    int ret = 0;
    if ((ret = create_dir(issue_path)) != DIR_OK) {
        fprintf(stderr, "[%s] Cant create [%s] dir. (%d)\n", SET_RED("ERR"), issue_path, ret);
    }
}

void create_issue_md_file(const char *issue_name, const char *issue_path, u64 uid) 
{
    string_builder *issue = sb_create();
    sb_append(issue, "%s/issue.md", issue_path);
    FILE *f = fopen(issue->str, "w");
    sb_free(issue);

    fprintf(f, "# %s\n", issue_name);
    fprintf(f, "- status: TODO\n");
    fprintf(f, "- priority: 1\n");
    fprintf(f, "- uid: %lu\n", uid);
    const date d = get_current_utc_time();
    fprintf(f, "- date: %02d.%02d.%d\n", d.day, d.mon, d.year);
    fprintf(f, "- tag: \n\n");  

    fclose(f);
}

int issue_init(int argc, char** argv) 
{
    (void)argc;
    (void)argv;

    if (is_dir_exist(".issues") == DIR_OK) {
        fprintf(stderr, "[%s] issue init .issues file currently exists\n", SET_RED("ERR"));
        return 1;
    }

    return create_dir("./"ISSUES_BASE_PATH);
}


int issue_append(int argc, char** argv) 
{
    if (is_dir_exist(".issues") != DIR_OK) {
        fprintf(stderr, "[%s] The .issues directory cannot be found.\n"
                        "Try: issue init or cd into a directory that contains .issues\n", 
                        SET_RED("ERR"));
        return 1;
    }

    if (argc != 3) {
        fprintf(stderr, "[%s] Provided bad count of arg\n\tUsege: issue add [name_of_issue]\n", SET_RED("ERR"));
        return  1;
    }

    const char *issue_name =  argv[2];
    const uint64_t uid  = gen_uid();

    string_builder *issue_dir = sb_create();
    sb_append(issue_dir, "./%s/ISSUE_%llu_%s", ISSUES_BASE_PATH, uid, issue_name);

    create_issue_dir(issue_dir->str);
    create_issue_md_file(issue_name, issue_dir->str, uid);

    string_builder *issue_open_path = sb_create();
    sb_append(issue_open_path, "%s/issue.md", issue_dir->str);

    open_editor(issue_open_path->str);

    sb_free(issue_dir);
    sb_free(issue_open_path);

    return 0;
}

inline static status_t check_issue_param(const char *key, char *line) 
{
    return (strstr(line, key) == NULL) ? STATUS_ERR : STATUS_OK;
}

static int gen_date_from_str(char *str, date *d)
{
    char *date = str;
    char *end;

    d->day = strtol(date, &end, 10);
    if (end == date || *end != '.') { 
        return 1;
    }

    date = end + 1; 
    d->mon = strtol(date, &end, 10);
    if (end == date || *end != '.') { 
        return 1;
    }

    date = end + 1; 
    d->year = strtol(date, &end, 10);
    if (end == date) { 
        return 1;
    }

    return 0;
}

static const char* val_to_str_status(task_status_t val)
{
    switch (val) {
        case TASK_TODO:     { return SET_BRIGHT_BLUE("TODO"); } 
        case TASK_DONE:     { return SET_BRIGHT_GREEN("DONE"); } 
        case TASK_PROGRESS: { return SET_BRIGHT_YELLOW("PROGRESS"); } 
        default:            { return "UNKNOW"; }
    }
}

static task_status_t str_to_val_status(const char *str)
{
    if (strncmp(str, "DONE", sizeof("DONE")) == 0) { return TASK_DONE; }
    if (strncmp(str, "TODO", sizeof("TODO")) == 0) { return TASK_TODO; }
    if (strncmp(str, "PROGRESS", sizeof("PROGRESS")) == 0) { return TASK_PROGRESS; }

    return TASK_UNKNOW;
}

static int _prio_comparasion(const void *a, const void *b)
{
    issue_view_record x = *(issue_view_record*)a;
    issue_view_record y = *(issue_view_record*)b;

    return (x.prio < y.prio);
}

static void _sort_by_prio_issues(issue_container *issue) 
{
    qsort(issue->items, issue->count, sizeof(issue->items[0]), _prio_comparasion);
}

static status_t fill_issue(const char* issue_path, issue_view_record *view) 
{
    char tmp[ISSUE_VIEWER_BUFFER_SIZE] = {0};
    sprintf(tmp, ".issues/%s/issue.md", issue_path);

    FILE *f = fopen(tmp, "r");
    if (!f) {
        return STATUS_ERR;
    }
    strcpy(view->path, tmp);

    char line[1024] = {0};
    fgets(line, sizeof(line), f);
    if (check_issue_param("# ", line) == STATUS_OK) {
        char *value = line + sizeof("# ") - 1;
        strncpy(view->name, value, ISSUE_VIEWER_BUFFER_SIZE);
        trim(view->name);
    } else {
        return STATUS_ERR;
    }

    fgets(line, sizeof(line), f);
    if (check_issue_param("- status: ", line) == STATUS_OK) {
        char *value = line + (sizeof("- status: ") - 1);
        trim(value);
        view->status = str_to_val_status(value);
    } else {
        return STATUS_ERR;
    }

    fgets(line, sizeof(line), f);
    if (check_issue_param("- priority: ", line) == STATUS_OK) {
        char *value = line + (sizeof("- priority: ") - 1);
        trim(value);
        view->prio = strtol(value, NULL, 10);
    } else {
        return STATUS_ERR;
    }

    fgets(line, sizeof(line), f);
    if (check_issue_param("- uid: ", line) == STATUS_OK) {
        char *value = line + sizeof("- uid: ") - 1;
        trim(view->name);
        view->uid = strtol(value, NULL, 10);
    } else {
        return STATUS_ERR;
    }

    fgets(line, sizeof(line), f);
    if (check_issue_param("- date: ", line) == STATUS_OK) {
        char *value = line + (sizeof("- date: ") - 1);
        gen_date_from_str(value, &view->date);
    } else {
        return STATUS_ERR;
    }

    fclose(f);
    return STATUS_OK;
}

int issue_viewer(int argc, char** argv) 
{
    (void)argc;
    (void)argv;

    if (is_dir_exist(".issues") != DIR_OK) {
        fprintf(stderr, "[%s] The .issues directory cannot be found.\n"
                        "Try: issue init or cd into a directory that contains .issues\n", 
                        SET_RED("ERR"));
        return 1;
    }

    dir_content output_scan = {0};
    int ret = scan_path(ISSUES_BASE_PATH, &output_scan);
    if (ret != 0) {
        fprintf(stderr, "Cant finde .issues dir.");
        return 1;
    }

    issue_container issue_container = {0};
    for (size_t i = 0; i < output_scan.count; ++i) {
        issue_view_record issue = {0};
        status_t status = fill_issue(output_scan.items[i], &issue);

        if (status == STATUS_OK) { da_append(&issue_container, issue); }
    }

    if (output_scan.count == 0) {
        printf(".issues folder is empty.\n");
    }

    _sort_by_prio_issues(&issue_container);

    // PRINT PROGRESS TASKS
    for (size_t i = 0; i < issue_container.count; ++i) {
        if (issue_container.items[i].status != TASK_PROGRESS) {
            continue;
        }

        printf("%lu ", issue_container.items[i].uid);
        printf("%02d.%02d.%d ", issue_container.items[i].date.day, 
                                issue_container.items[i].date.mon, 
                                issue_container.items[i].date.year);
        printf("(%s%d%s) ",     _SET_RED, issue_container.items[i].prio, _SET_RESET);

        int offset; 
        if (issue_container.items[i].prio < 10)       { offset = 2; }
        else if (issue_container.items[i].prio < 100) { offset = 1; }
        else                                          { offset = 0; }
        printf("%*s", offset, "");
        printf("[%s] | ", val_to_str_status(issue_container.items[i].status));
        printf("%s\n", issue_container.items[i].name);
    }

    // PRINT TODO TASKS
    for (size_t i = 0; i < issue_container.count; ++i) {
        if (issue_container.items[i].status != TASK_TODO) {
            continue;
        }

        printf("%lu ", issue_container.items[i].uid);
        printf("%02d.%02d.%d ", issue_container.items[i].date.day, 
                                issue_container.items[i].date.mon, 
                                issue_container.items[i].date.year);
        printf("(%s%d%s) ",     _SET_RED, issue_container.items[i].prio, _SET_RESET);

        int offset; 
        if (issue_container.items[i].prio < 10)       { offset = 2; }
        else if (issue_container.items[i].prio < 100) { offset = 1; }
        else                                          { offset = 0; }
        printf("%*s", offset, "");
        printf("[%s] | ", val_to_str_status(issue_container.items[i].status));
        printf("%s\n", issue_container.items[i].name);
    }

    // PRINT DONE TASKS
    for (size_t i = 0; i < issue_container.count; ++i) {
        if (issue_container.items[i].status != TASK_DONE) {
            continue;
        }

        printf("%lu ", issue_container.items[i].uid);
        printf("%02d.%02d.%d ", issue_container.items[i].date.day, 
                                issue_container.items[i].date.mon, 
                                issue_container.items[i].date.year);
        printf("(%s%d%s) ",     _SET_RED, issue_container.items[i].prio, _SET_RESET);

        int offset; 
        if (issue_container.items[i].prio < 10)       { offset = 2; }
        else if (issue_container.items[i].prio < 100) { offset = 1; }
        else                                          { offset = 0; }
        printf("%*s", offset, "");
        printf("[%s] | ",       val_to_str_status(issue_container.items[i].status));
        printf("%s\n",          issue_container.items[i].name);
    }

    return 0;
}
