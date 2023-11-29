#include "client.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

/** default port number */
#define DEFAULT_PORTNUM (2017)
#define DEFAULT_PORTNUM_STR "2017"

/** name of the executable (for printing messages) */
char *program_name = "<not yet set>";

int main(int argc, char **argv)
{
    struct args arguments = {DEFAULT_PORTNUM, DEFAULT_PORTNUM_STR, UNDEF, 0, 0};

    /* set program_name */
    if (argc > 0)
    {
        program_name = argv[0];
    }

    /***********************************************************************
     * Task 1
     * ------
     * Implement argument parsing for the client. Synopsis:
     *   ./client [-p PORT] {-g|-s VALUE} ID
     *
     * Call usage() if invalid options or arguments are given (there is no
     * need to print a description of the problem).
     *
     * Hints: getopt(3), UINT16_MAX, parse_number (client.h),
     *        struct args (client.h)
     ***********************************************************************/

    /* PARSING WITH GETOPT (had to be implemented almost from scratch) */
    bool p = false;
    bool s = false;
    bool g = false;

    int opt;
    while ((opt = getopt(argc, argv, "p:s:g")) != -1)
    {
        switch (opt)
        {
        case 'p':
            if (p == true)
                usage("p");

            p = true;
            arguments.portstr = optarg;
            sscanf(optarg, "%hu", &arguments.portnum);
            break;
        case 's':
            if (s == true || g == true)
                usage("s");
            s = true;
            arguments.cmd = 1;
            sscanf(optarg, "%hhu", &arguments.value);
        case 'g':
            if (s == true || g == true)
                usage("s");
            g = true;
            arguments.cmd = 0;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (s == false && p == false) usage("set");
    if (argc - optind != 1)
        usage("files");

    sscanf(argv[optind], "%hhu", &arguments.id);

    /* DO NOT FORGET ABOUT THE POSITIONAL ARGUMENTS */

    /* DO NOT REMOVE THE FOLLOWING LINE */
    // apply_command(arguments);

    return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message)
{
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "Usage: %s { -a optargA | -b | -c [optargC] } file... (exactly 1 "
            "files)\n",
            program_name);
    exit(EXIT_FAILURE);
}