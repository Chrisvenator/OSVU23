#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <getopt.h>
#include <string.h>

#include "client.h"

/** default port number */
#define DEFAULT_PORTNUM     (2017)
#define DEFAULT_PORTNUM_STR "2017"

/** name of the executable (for printing messages) */
char *program_name = "<not yet set>";

void usage() {
    perror("usage");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    struct args arguments =
            {DEFAULT_PORTNUM, DEFAULT_PORTNUM_STR, UNDEF, 0, 0};

    /* set program_name */
    if (argc > 0) {
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

    /* COMPLETE AND EXTEND THE FOLLOWING CODE */

    int opt;
    bool p_set = false;
    bool s_set = false;
    bool g_set = false;

    while ((opt = getopt(argc, argv, "p:gs:")) != -1) {
        switch (opt) {
            case 'p':
                if (p_set == true) usage();
                p_set = true;

                arguments.portstr = optarg;
                arguments.portnum = parse_number(arguments.portstr);

                break;
            case 'g':
                if (g_set == true || s_set == true) usage();
                g_set = true;
                arguments.value = parse_number(optarg);

                break;
            case 's':
                if (g_set == true || s_set == true) usage();
                s_set = true;
                arguments.value = parse_number(optarg);

                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (argc - optind != 1) usage();
    arguments.id = parse_number(argv[optind]);


    /* DO NOT FORGET ABOUT THE POSITIONAL ARGUMENTS */


    /* DO NOT REMOVE THE FOLLOWING LINE */
//apply_command(arguments);

    return 0;
}
