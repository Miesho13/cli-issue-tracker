#ifndef _ISSUE_
#define _ISSUE_

#include "sb.h"
#include "type.h"
#include "file_helper.h"

typedef enum {
   TASK_TODO, 
   TASK_DONE, 
   TASK_PROGRESS,
   TASK_UNKNOW, 
} task_status_t;

#define ISSUE_VIEWER_BUFFER_SIZE (255)
/*
 * [1] (date) (uid) (name) (prio) (short_breaf)
 */ 
typedef struct {
    date date;
    task_status_t status;
    u32 prio;
    u64 uid;
    char path[ISSUE_VIEWER_BUFFER_SIZE];
    char name[ISSUE_VIEWER_BUFFER_SIZE];
} issue_view_record;

typedef struct {
    issue_view_record *items;
    size_t count;
    size_t capacity;
} issue_container;


int issue_viewer(int argc, char** argv);
int issue_append(int argc, char** argv);
int issue_init(int argc, char** argv);

#endif // _ISSUE_
