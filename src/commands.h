#ifndef __FLAGS__
#define __FLAGS__

typedef int (*command_ptr)(int, char**);

typedef struct {
    const char*  name;
    const char*  help_msg;
    command_ptr command;
} commands_map;

int commands_service(int argc, char **argv);

#endif // __FLAGS__
