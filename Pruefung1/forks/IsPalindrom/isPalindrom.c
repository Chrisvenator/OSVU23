//
// Created by junioradmin on 28.11.23.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

static const char *pName;

static void usage() {
    fprintf(stderr, "[%s]: isPalindrom -s <palindrom>\n", pName);
    exit(EXIT_FAILURE);
}


int main(int argc, char **argv) {
    pName = argv[0];
    if (argc != 3) usage();


    int opt;
    int amountOfFlags = 0;

    char *input;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
            case 't':
                if (amountOfFlags >= 1) usage();
                input = optarg;
                amountOfFlags = 1;
                break;
            default: /* '?' */
                usage();
        }
    }


    fprintf(stdout, "input: %s\n", input);

    int length = strlen(input);

    for (int i = 0; i < length / 2; ++i) {
        fprintf(stdout, "%c - %c\n", input[i], input[length - i - 1]);
        if (input[i] != input[length - i - 1]) {
            fprintf(stdout, "\"%s\" is NOT a palindrom!\n", input);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "\"%s\" is a palindrom!\n", input);

    exit(EXIT_SUCCESS);
}
