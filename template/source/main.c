#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "arguments.h"

#define CMDLINE_PARSER_ERROR    1

int main(int argc, char** argv)
{
    struct gengetopt_args_info args_info;

    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        fprintf(stderr, "Error parsing command line arguments.");
        return CMDLINE_PARSER_ERROR;
    }

    /* do somthing here */
}
