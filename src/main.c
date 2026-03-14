#include "commands.h" 

int main(int argc, char **argv)
{
    int ret = commands_service(argc, argv);
    return ret;
}
