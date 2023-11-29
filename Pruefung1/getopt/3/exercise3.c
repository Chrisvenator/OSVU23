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

  char * a = "false";
  bool b_set = false;
  char * c = "false";

  // ./client {-a optargA | -b | -c [optargC] } file... (exactly 4 files)
  // - set flag, if -b occured
  // - add all four files in reversed mentioned-order to totalString

int opt;
while ((opt = getopt(argc, argv, "a:bc::")) != -1) {
    switch (opt) {
    case 'a':
        if (strncmp(a, "false", 4) || b_set == true || strncmp(c, "false", 4)) usage("a");

        a = optarg;
        break;
    case 'b':
        if (strncmp(a, "false", 4) || b_set == true || strncmp(c, "false", 4)) usage("b");

        b_set = true;
        break;
    case 'c':
        if (strncmp(a, "false", 4) || b_set == true || strncmp(c, "false", 4)) usage("c");
        
        c = "true";
        if (optarg){
          c = argv[optind];
          optind++;
        }
        break;
    default: /* '?' */
        usage("default");
        assert(0);
        break;
    }
}

if (argc - optind != 4) usage("files");

char * total_string = malloc(4 * sizeof(char));

for (size_t i = 0; i < 4; i++){
  strcat(total_string, argv[optind]);
  optind++;
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
