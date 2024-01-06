/**
 * @author Christopher Scherling 12119060
 * @file headers.h
 * @date 05.01.2024
 *
 *
 * @brief Header file for OSVU23 network client.
 *
 * This header file contains necessary declarations for setting up and managing a network client
 * for the OSVU23 project. It includes standard libraries required for network communication,
 * defines constants for buffer sizes and default ports, and declares a structure for argument
 * handling in the client application.
 *
 * @details
 * The libraries included serve specific purposes:
 * - Standard I/O and string libraries for basic operations.
 * - Networking libraries like <netinet/in.h>, <sys/socket.h>, etc., for socket programming.
 * - <unistd.h> and <stdlib.h> for various system calls.
 * - <arpa/inet.h> specifically for address manipulation functions.
 * - <getopt.h> for parsing command-line arguments.
 * - <assert.h> and <stdbool.h> for debugging and boolean data types respectively.
 * - <errno.h>, <limits.h>, <netdb.h>, and <sys/stat.h> for handling errors, limits,
 *   network database operations, and file status.
 *
 * @note This file should be included in the client source files of the OSVU23 project.
 */

#ifndef OSVU23_CLIENT_H
#define OSVU23_CLIENT_H

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

#define BUFFER_SIZE 1024
#define DEFAULT_PORT "80"
#define DEFAULT_FILENAME "index.html"
#define DEFAULT_SUFFIX "/"

/**
 * @brief Name of the Program (agrv[0])
 * @details This global variable is intended to store the name of the program as invoked
 * on the command line. Typically set at the beginning of the main function to argv[0],
 * it can be used throughout the program for logging, error messages, or any functionality
 * that requires the program's invocation name. Storing it globally allows for easy access
 * without the need to pass it around to various functions that might need it.
 */
static char *PROGRAM_NAME;


#endif //OSVU23_CLIENT_H
