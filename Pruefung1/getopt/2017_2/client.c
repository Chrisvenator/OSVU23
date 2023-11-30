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

int main(int argc, char **argv) {
    struct args arguments = {DEFAULT_PORTNUM, DEFAULT_PORTNUM_STR, UNDEF, 0, 0};

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

    /* PARSING WITH GETOPT (had to be implemented almost from scratch) */

    bool p_set = false;
    bool g_set = false;
    bool s_set = false;

    int opt;
    while ((opt = getopt_long(argc, argv, "p:gs:", NULL, NULL)) != -1) {
        switch (opt) {
        case 'p':
            if (p_set == true) usage("p");
            p_set = true;

            arguments.portstr = optarg;
            sscanf(optarg, "%hu", &arguments.portnum);
            break;
        case 'g':
            if (g_set == true || s_set == true) usage("g");
            g_set = true;
            arguments.cmd = 0;
            break;
        case 's':
            if (g_set == true || s_set == true) usage("s");
            s_set = true;
            arguments.cmd = 1;
            //if (optarg){
                sscanf(optarg, "%hhu", &arguments.value);
                //optind++;
            //} 
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    /* DO NOT FORGET ABOUT THE POSITIONAL ARGUMENTS */
    if (argc - optind != 1) usage("ID");
    sscanf(argv[optind++], "%hhu", &arguments.id);

    printf("g_set: %d\n", g_set);
    printf("p_set: %d\n", p_set);
    printf("s_set: %d\n", s_set);

    printf("portstr: %s\n", arguments.portstr);
    printf("portnum: %d\n", arguments.portnum);
    printf("ID: %d\n", arguments.id);
    printf("Value: %d\n", arguments.value);

    /* DO NOT REMOVE THE FOLLOWING LINE */
    // apply_command(arguments);

    return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "Usage: %s { -a optargA | -b | -c [optargC] } file... (exactly 1 "
            "files)\n",
            program_name);
    exit(EXIT_FAILURE);
}