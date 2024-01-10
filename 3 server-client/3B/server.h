/**
 * @author Christopher Scherling 12119060
 * @file server.h
 * @date 10.01.2024
 *
 *
 * @brief Header file for OSVU23 network server.
 *
 * @details
 * This header file contains necessary declarations for setting up and managing a network client
 * for the OSVU23 project. It includes standard libraries required for network communication,
 * defines constants for buffer sizes and default ports, and declares a structure for argument
 * handling in the client application.
 *
 * @note This file should be included in the client source files of the OSVU23 server project.
 */

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
