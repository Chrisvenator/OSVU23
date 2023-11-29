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

    // - ./client [-a optargA | -b optargB | -o ] -c [optargC] file... (max. 8
    // files)
    // - hint: optargC can only be specified by -coptargC (not -c optargC)
    // - optargA ... int [-50,300]
    // - optargB ... char
    // - optargC ... char[8] (exactly!)
    // - maximum of 8 pos-args, no minimum, cat all to one total_string

    int a = -51;
    char *b = "";
    bool o_set = false;
    char *c = "";

    int opt;
    while ((opt = getopt_long(argc, argv, "a:b:oc::", NULL, NULL)) != -1) {
        switch (opt) {
            case 'a':
                if (strlen(b) != 0 || o_set == true || a != -51) usage("a");
                sscanf(optarg, "%d", &a);
                if (a < -50 || a > 300) usage("a");
                break;
            case 'b':
                if (strlen(b) != 0 || o_set == true || a != -51) usage("b");
                b = optarg;
                break;
            case 'o':
                if (strlen(b) != 0 || o_set == true || a != -51) usage("o");
                o_set = true;
                break;
            case 'c':
                if (strlen(c) != 0) usage("c");
                c = "true";
                if (optarg) {
                    if (strlen(optarg) > 8) usage("c2");
                    c = argv[optind];
                    optind++;
                }
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argc - optind > 8) usage("files");

    int total_length = 1;
    for (size_t i = 0; i < argc - optind; i++) {
        total_length += strlen(argv[i]);
    }

    char *total_string = malloc(total_length * sizeof(char));
    while (argc - optind != 0) {
        strcat(total_string, argv[optind]);
        optind++;
    }

    if (a != -51)
        printf("a: %d\n", a);
    else
        printf("a: not initialized yet.\n");
    printf("b: %s\n", b);
    printf("c: %s\n", c);
    printf("o_set: %s\n", o_set ? "true" : "false");
    printf("total_string: %s\n", total_string);

    free(total_string);
    return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "Usage: %s [-a optargA | -b optargB | -o ] -c [optargC] file... "
            "(max. 8 files)\n",
            program_name);
    exit(EXIT_FAILURE);
}
