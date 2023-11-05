/**
 * @author Christopher Scherling
 * @file cpair.h
 * @date 01.11.2023
 * @brief All "include" and "define" of cpair.
 * @details This header file contains all necessary standard library includes and
 * macro definitions required for the cpair program. It sets up the environment
 * by defining feature test macros, ensuring compatibility and enabling specific
 * functionalities from the POSIX.1-2008 version. It also includes headers for
 * standard I/O operations, string manipulation, dynamic memory management, program
 * options parsing, fixed-width integer types, system call wrappers, error handling,
 * assertions for debugging, mathematical functions, and character type functions.
 * Uncommented includes such as sys/types.h, limits.h, float.h, and stdbool.h are
 * reserved for future use if program requirements expand.
 **/

#ifndef OSVU23_INCLUDES_H
#define OSVU23_INCLUDES_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

#include <limits.h>
#include <float.h>

#endif //OSVU23_INCLUDES_H
