#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

/** default port number */
#define DEFAULT_PORTNUM (2017)
#define DEFAULT_PORTNUM_STR "2017"

/** name of the executable (for printing messages) */
char *program_name = "<not yet set>";

/** Defines the command type. */
typedef enum { GET = 0, SET = 1, UNDEF = 2 } cmd_t;

/** Structure for the arguments. */
struct args {
  uint16_t portnum;    /**< port number */
  const char *portstr; /**< port number as string */
  cmd_t cmd;           /**< command (GET, SET) */
  uint8_t value;       /**< set value */
  uint8_t id;          /**< device id */
};

void usage(const char *message);

int main(int argc, char **argv) {
  struct args arguments = {DEFAULT_PORTNUM, DEFAULT_PORTNUM_STR, UNDEF, 0, 0};

  /* set program_name */
  if (argc > 0) {
    program_name = argv[0];
  }

int opt;
int pFlag = 0;
int gsFlag = 0;

while ((opt = getopt(argc, argv, "p:g:s:")) != -1) {
    switch (opt) {
    case 'p':
        if (pFlag != 0) usage("");
        pFlag = 1;
        
        arguments.portstr = optarg;
        sscanf(optarg, "%hu", &arguments.portnum);
        break;
    case 's':
        if (gsFlag != 0) usage("");
        gsFlag = 1;

        arguments.cmd = 1;
        sscanf(optarg, "%hhu", &arguments.value);
        break;
    case 'g':
        if (gsFlag != 0) usage("");
        gsFlag = 1;

        arguments.cmd = 0;
        sscanf(optarg, "%hhu", &arguments.value);
        break;

    default: /* '?' */
        usage("");
        assert(0);
        break;
    }
}

if (argc - optind != 1) usage("");

sscanf(argv[optind], "%hhu", &arguments.id);


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

  printf("pn: %d\n", arguments.portnum);
  printf("pns: %s\n", arguments.portstr);
  printf("cmd: %d\n", arguments.cmd);
  printf("value: %d\n", arguments.value);
  printf("id: %d\n", arguments.id);

  return 0;
}

/** Prints the usage message and exits with EXIT_FAILURE. */
void usage(const char *message) {
  fprintf(stderr, "%s\n", message);
  fprintf(stderr, "Usage: %s [-p PORT] {-g|-s VALUE} ID\n", program_name);
  exit(EXIT_FAILURE);
}
