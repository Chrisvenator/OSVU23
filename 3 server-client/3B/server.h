//
// Created by junioradmin on 09.01.24.
//

#ifndef OSVU23_SERVER_H
#define OSVU23_SERVER_H

#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <getopt.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <linux/limits.h>
#include <sys/select.h>



#define BUFFER_SIZE 4048
#define DEFAULT_PORT 8080
#define DEFAULT_INDEX "index.html"
#define LISTEN_BACKLOG 1

/**
 * @brief Name of the Program (agrv[0])
 * @details This global variable is intended to store the name of the program as invoked
 * on the command line. Typically set at the beginning of the main function to argv[0],
 * it can be used throughout the program for logging, error messages, or any functionality
 * that requires the program's invocation name. Storing it globally allows for easy access
 * without the need to pass it around to various functions that might need it.
 */
static char *PROGRAM_NAME;

#endif //OSVU23_SERVER_H
