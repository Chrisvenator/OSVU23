//
// Created by junioradmin on 04.01.24.
//

#ifndef OSVU23_HEADERS_H
#define OSVU23_HEADERS_H

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h> // inet_addr()
#include <getopt.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <sys/stat.h>


#define MAX_BUFFER_SIZE 80
#define DEFAULT_PORT 80

static char *PROGRAM_NAME;

typedef struct arguments arguments;


#endif //OSVU23_HEADERS_H
