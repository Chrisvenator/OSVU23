#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

/** name of the executable (for printing messages) */
char *program_name = "<not yet set>";

void usage(const char *message);


int main(int argc, char **argv) {
    /* set program_name */
    if (argc > 0) {
        program_name = argv[0];
    }

    char *a = "";
    char *b = "";
    char *c = "";
    char *d_set = "false";
    char *e_set = "false";

    /*
    - ./client [-a optargA] [-e] -c [optargC] [-b optargB [-d] ]

    - hint: optargC can only be specified by -coptargC (not -c optargC)

    - all optargs are char* (for simplicity)
    - hardest test-cases:
        - -d specified, although -b not specified (FAIL)
        - -d not specified, -b specified (PASS)
    */


    int opt;
    while ((opt = getopt_long(argc, argv, "a:b:c::de", NULL, NULL)) != -1) {
        switch (opt) {
            case 'a':
                if (strlen(a) != 0) usage("a");
                a = optarg;
                break;
            case 'b':
                if (strlen(b) != 0) usage("b");
                b = optarg;
                break;
            case 'c':
                c = "true";
                if (optarg) {
                    c = argv[optind];
                }
                break;
            case 'd':
                if (strncmp(d_set, "true", 4) == 0 || strlen(b) == 0) usage("d");
                d_set = "true";
                break;
            case 'e':
                if (strncmp(e_set, "true", 4) == 0) usage("e");
                e_set = "true";
                break;
            default: /* '?' */
                usage("");
                assert(0);
                break;
        }
    }


    printf("a: %s\n", a);
    printf("b: %s\n", b);
    printf("c: %s\n", c);
    printf("d_set: %s\n", d_set ? "true" : "false");
    printf("e_set: %s\n", e_set ? "true" : "false");

    return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "Usage: %s [-a optargA] [-e] -c [optargC] [-b optargB [-d] ]\n",
            program_name);
    exit(EXIT_FAILURE);
}
