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


    // ./client {-a optargA | -b | -c [optargC] } file... (exactly 4 files)
    // - set flag, if -b occured
    // - add all four files in reversed mentioned-order to totalString

    bool a_set = false;
    bool b_set = false;
    bool c_set = false;

    char* a = "false";
    char* c = "false";
	
    int opt;
    while ((opt = getopt(argc, argv, "a:bc::")) != -1) {
               switch (opt) {
               case 'a':
		       if(a_set == true || b_set == true || c_set == true) usage("a");
                   a_set = true;
		   a = optarg;
                   break;
               case 'b':
		   if(a_set == true || b_set == true || c_set == true) usage("a");
                   b_set = true;
                   break;
	       case 'c':
		   if(a_set == true || b_set == true || c_set == true) usage("a");
		   c_set = true;
		   if (optarg) c = argv[optind++];
		   else c = "true";
		   break;
               default: /* '?' */
                   usage("other");
               }
           }

    if (argc - optind != 4) usage("files");
    
    int length = strlen(argv[optind]) +strlen(argv[optind+1]) +strlen(argv[optind+2]) +strlen(argv[optind+3]);

    char * total_string = malloc(length * sizeof(char));
    for (int i = 0; i<4; i++){
	    strcat(total_string, argv[optind++]);
    }
    	
    printf("a: %s\n", a);
    printf("b_set: %s\n", b_set ? "true" : "false");
    printf("c: %s\n", c);
    printf("total_string: %s\n", total_string);

    free(total_string);
    return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "Usage: %s { -a optargA | -b | -c [optargC] } file... (exactly 4 "
            "files)\n",
            program_name);
    exit(EXIT_FAILURE);
}
