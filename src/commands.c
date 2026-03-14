#include "commands.h"
#include "issue.h"
#include "stdint.h"
#include "sb.h"

static commands_map global_commands[] = {
    (commands_map){
        .name     = "init", 
        .help_msg = "Creates the .issues directory that stores all issues. Must be run before any other commands.",
        .command  = issue_init
    },
    (commands_map){
        .name     = "add", 
        .help_msg = "Creates a new issue.\n\t\tUsage: issue add <issue name>",
        .command  = issue_append
    },
    (commands_map){
        .name     = "ls", 
        .help_msg = "Lists all issues.",
        .command  = issue_viewer,
    },
};

inline static void _commands_help(void) 
{
    int n = sizeof(global_commands)/sizeof(global_commands[0]);
    printf("issue [command] <arg>\n");
    for (int idx = 0; idx < n; ++idx) {
        printf("\t%s - ", global_commands[idx].name);
        printf("%s\n", global_commands[idx].help_msg);
    }
}

int commands_service(int argc, char **argv)
{
    int ret = 0xff;

    if (argc == 1 || (strcmp(argv[1], "-h") == 0)) {
        _commands_help();
        return ret;
    }

    const size_t commands_count = sizeof(global_commands)/sizeof(global_commands[0]);
    for (uint32_t command_idx = 0; command_idx < commands_count; command_idx++) {
        if (strcmp(global_commands[command_idx].name, argv[1]) == 0) {
            ret = global_commands[command_idx].command(argc, argv);
            break;
        }
    }

    if (ret == 0xff) {
        fprintf(stderr, "[%s] Can't finde provdied command.\nUse -h for list all avaliable commands.\n", SET_RED("ERR"));
    }

    return ret;
}
