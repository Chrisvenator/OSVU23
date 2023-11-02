/**
 * @author Christopher Scherling
 * @file cpair.h
 * @date 01.11.2023
 * @brief header of cpair. Defines structs and includes
 * @details defines macros, which are used as indices for the pipes @see cpair.c.
 * Furthermore, structs for storing points and pair of points are defined.
 **/
#ifndef OSVU23_CPAIR_H
#define OSVU23_CPAIR_H

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

//#include <sys/types.h>
//#include <limits.h>
//#include <float.h>
//#include <stdbool.h>


/**
 * @brief Structure to represent a point in 2D space
 */
typedef struct Point {
    float x, y;
} Point;

typedef struct Pair // holds a pair of point and the distance between them
{
    Point p1; // first point
    Point p2; // second point
    float dist; // distance between p1 and p2
} Pair;




/**
 * @brief root calculates the square root of the float n
 * the root function is a backup if the math library is not supported (like on my PC...)
 * Source: https://stackoverflow.com/questions/3581528/how-is-the-square-root-function-implemented
 * @param n
 * @return square root of n
 */
//double root(float n){
//    // Max and min are used to take into account numbers less than 1
//    double lo = 1, hi = n, mid;
//
//    // Update the bounds to be off the target by a factor of 10
//    while(100 * lo * lo < n) lo *= 10;
//    while(0.01 * hi * hi > n) hi *= 0.1;
//
//    for(int i = 0 ; i < 100 ; i++){
//        mid = (lo+hi)/2;
//        if(mid*mid == n) return mid;
//        if(mid*mid > n) hi = mid;
//        else lo = mid;
//    }
//    return mid;
//}

/**
 * @brief is_float checks if the string is a float
 * @param str input string pointer
 * @return returns 1 if true and 0 if false
 */
int is_float(char *str) {
    char *endptr;
    errno = 0;  // To distinguish success/failure after the call to strtod

    // Use strtod to convert to a double
    strtod(str, &endptr);

    // Check for various possible errors

    // No digits were found
    if (endptr == str) {
        return 0;
    }

    // If the string starts with a space, it is not considered a valid float
    if (isspace((unsigned char)*str)) {
        return 0;
    }

    // Check for any leftover characters after the number
    if (*endptr != '\0') {
        return 0;
    }

    // If out of range of representable values by strtod
    if (errno == ERANGE) {
        return 0;
    }

    // If we get here, it's a float
    return 1;
}

#endif //OSVU23_CPAIR_H
