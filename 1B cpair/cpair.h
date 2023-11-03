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

#include "includes.h"

/**
 * @brief Name of the Program (agrv[0])
 * @details This global variable is intended to store the name of the program as invoked
 * on the command line. Typically set at the beginning of the main function to argv[0],
 * it can be used throughout the program for logging, error messages, or any functionality
 * that requires the program's invocation name. Storing it globally allows for easy access
 * without the need to pass it around to various functions that might need it.
 */
char *programName;

/**
 * @brief Structure to represent a point in 2D space.
 *
 * @details
 * This structure is used to hold the coordinates of a point in a two-dimensional
 * plane. Each point is defined by a pair of floating-point numbers representing
 * its horizontal (x) and vertical (y) positions.
 *
 * @note This structure can be extended to represent points in higher-dimensional
 * space by adding more members, such as 'z' for a three-dimensional space.
 */
typedef struct Point Point;

/**
 * @brief Structure to represent a pair of points and the distance between them.
 *
 * @details This structure is specifically designed to hold two points in a two-dimensional
 * space and the Euclidean distance between these points. It can be used in contexts where
 * it's necessary to keep track of two points along with their spatial relationship,
 * such as in geometric algorithms or spatial data analysis.
 *
 * @note The distance is typically calculated using the Euclidean distance formula. Ensure
 * that the distance is updated appropriately whenever the points change. This structure does
 * not automatically recalculate the distance if the points are altered.
 */
typedef struct Pair Pair;

/**
 * @brief Parses the string into two float coordinates.
 * @details This function assumes the input string contains two float numbers separated by a space.
 * It will parse the string, validate and convert the two numbers into a Point structure.
 * If the input is not in the correct format or does not contain exactly two floats,
 * the program will print an error message and terminate.
 * @param string A character array containing the two float numbers separated by a space.
 * It must be a null-terminated string and should not be NULL itself.
 * @return A Point structure with the first number as the x-coordinate and the second number as the y-coordinate.
 * If the function fails to parse the string or encounters an error, it will not return; instead, it will terminate the program.
 */
Point getCoordinates(char *string);

/**
 * @brief Function to compute the Euclidean distance between two points.
 * @param p1 The first point as a Point structure.
 * @param p2 The second point as a Point structure.
 * @return The Euclidean distance as a double.
 */
double distance(struct Point p1, struct Point p2);

/**
 * @brief Prints the usage information for the program and exits.
 * This function is typically called when the program is invoked with incorrect arguments.
 * It will print the correct usage format of the program to the standard error and then exit
 * with EXIT_FAILURE to indicate that the program has terminated unsuccessfully.
 */
void usage(void);

/**
 * @brief Loads point data from stdin into a dynamically allocated array.
 * @details This function reads point data from stdin, expecting each line to contain
 * a pair of float numbers representing coordinates. It dynamically allocates an array
 * to store the points and resizes it as needed.
 * @return A pointer to an array of Point structures containing the loaded data. The array
 * is dynamically allocated and must be freed by the caller. The function will terminate
 * the program if stdin opening fails, memory allocation fails, or reallocating memory fails.
 * @warning The user needs to free the array manually later!
 */
Point *loadData();

/**
 * @brief Checks if the string is a valid representation of a float.
 * @details This function attempts to parse a string as a floating-point number and determine
 * its validity. It converts the string to a double using strtod, which sets a pointer to the
 * next character in the string after the numerical value. The function then checks:<br>
 * - If no numerical digits were found at the beginning of the string.<br>
 * - If the string starts with a space, it is not considered a valid float.<br>
 * - If there are any leftover non-numerical characters after the number.<br>
 * - If the value is out of the range of representable values of strtod, indicating an overflow or underflow.<br>
 * If any of these checks fail, the function returns 0, indicating the string is not a valid float. Otherwise,<br>
 * it returns 1, indicating the string is a valid float.
 * @param str The input string to check for float representation.
 * @return Returns 1 if the string is a valid float, 0 otherwise.
 */
int is_float(char *str);

/**
 * @brief remove_all_chars removes all occurrences c from string str
 * @details This function iteratively scans through the provided string, copying characters
 * from the read pointer 'pr' to the write pointer 'pw' as long as they are not equal to 'c'.
 * Characters that are equal to 'c' are skipped, effectively removing them from the string.
 * The write pointer only advances if the character just copied is not 'c', ensuring all
 * instances of 'c' are not written to the resulting string. The function concludes by appending
 * a null terminator to the end of the string at the write pointer's final position, ensuring
 * the result is a valid C string.
 * @copyright Sergey Kalinichenko:<br>
 * https://stackoverflow.com/questions/9895216/how-to-remove-all-occurrences-of-a-given-character-from-string-in-c
 * @param str The input string from which the characters should be removed.
 * @param c character that should be removed
 */
void remove_all_chars(char *str, char c);

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



#endif //OSVU23_CPAIR_H