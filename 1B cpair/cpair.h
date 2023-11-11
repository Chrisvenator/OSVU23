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
 * @brief Number of all Point elements
 */
//size_t numberOfElements;

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

//TODO: comment
typedef struct Process Process;

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
 * same as distance
 */
double getPairDistance(Pair pair);

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
Point *loadData(size_t *ptr_numberOfElements);

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
bool is_float(char *str);

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
 * @brief Prints the coordinates of a Pair to standard output.
 * @details This function takes a Pair structure (which includes two Points and the distance between them)
 * and prints the coordinates of these points to stdout in ascending order based on the x-coordinate.
 * If the x-coordinates are the same, it uses the y-coordinate to determine the order.
 * The coordinates are printed with three decimal places of precision.
 *
 * @param pair The Pair structure containing the two points to print.
 */
void printPair(FILE *output, Pair pair);

/**
 * @brief Calculate the arithmetic mean of a coordinate (x or y) over a set of points.
 * @details This function calculates the arithmetic mean for the specified coordinate (x or y)
 *          of all points in the provided array. It includes error handling for null pointers
 *          and zero-element arrays.
 * @note The coordinate parameter should be 'x' for the x-coordinate mean and 'y' for the y-coordinate mean.
 *       If the points array is NULL or numberOfElements is zero, the function returns 0 and sets errno
 *       to EINVAL, indicating an invalid argument and prints to stderr.
 * @param points A pointer to the first element in an array of Point structs.
 * @param numberOfElements The number of elements in the points array.
 * @param coordinate A char indicating which coordinate to calculate the mean of ('x' or 'y').
 * @return The arithmetic mean as a double value, or 0 if an input error is detected.
 */
double calculateArithmeticMean(Point *points, char coordinate, size_t numberOfElements);


/**
 * @brief Check if all points have the same coordinates.
 * @details This function iterates through an array of points and checks whether all points have the same x and y coordinates.
 *          It assumes the array is not NULL and contains at least one element. The caller should ensure the array is properly initialized.
 * @param points A pointer to the first element in an array of Point structs.
 * @param numberOfElements The number of elements in the points array.
 * @return A boolean value indicating whether all coordinates are the same (true) or not (false).
 */
bool checkIfAllCoordinatesAreTheSame(Point *points, size_t numberOfElements);


/**
 * @brief Check if all x-coordinates are the same in an array of points.
 * @details Iterates through the array of points and compares the x-coordinate
 *          of each point to the x-coordinate of the first point. If any point has
 *          a different x-coordinate, the function returns false.
 * @note This function assumes that the points array has at least one element
 *       and that numberOfElements is the correct length of the array. If the
 *       array is empty or numberOfElements is incorrect, the behavior is undefined.
 * @param points A pointer to the first element of an array of Point structures.
 * @return Returns true if all points in the array have the same x-coordinate, false otherwise.
 */
bool checkIfAllXValuesAreTheSame(Point *points, size_t numberOfElements);


/**
  * @brief Comparator function for qsort that compares two points by their x-coordinate.
  * @details This function takes two arguments, casts them to Point types, and compares them based
  *          on their x-coordinate values. This comparator is intended to be used with the C standard
  *          library function qsort(). If the first point's x-coordinate is less than the second's,
  *          it returns -1, indicating that the first point should come before the second in a sorted
  *          array. If the first point's x-coordinate is greater, it returns 1, indicating that the
  *          first point should come after the second. If the x-coordinates are equal, it returns 0,
  *          indicating that the relative order of the points does not need to change.
  * @note If you need to sort an array of Points where points with equal x-coordinates should be
  *       sorted based on their y-coordinates, additional logic will be required in this function.
  * @param a A void pointer to the first element being compared.
  * @param b A void pointer to the second element being compared.
  * @return An integer less than, equal to, or greater than zero if the x-coordinate of the first
  *         point is found, respectively, to be less than, to match, or be greater than that of the
  *         second point.
  */
int compareX(const void *a, const void *b);

/**
  * @brief Comparator function for qsort that compares two points by their y-coordinate.
  * @details This function takes two arguments, casts them to Point types, and compares them based
  *          on their y-coordinate values. This comparator is intended to be used with the C standard
  *          library function qsort(). If the first point's y-coordinate is less than the second's,
  *          it returns -1, indicating that the first point should come before the second in a sorted
  *          array. If the first point's y-coordinate is greater, it returns 1, indicating that the
  *          first point should come after the second. If the y-coordinates are equal, it returns 0,
  *          indicating that the relative order of the points does not need to change.
  * @note If you need to sort an array of Points where points with equal y-coordinates should be
  *       sorted based on their x-coordinates, additional logic will be required in this function.
  * @param a A void pointer to the first element being compared.
  * @param b A void pointer to the second element being compared.
  * @return An integer less than, equal to, or greater than zero if the y-coordinate of the first
  *         point is found, respectively, to be less than, to match, or be greater than that of the
  *         second point.
  */
int compareY(const void *a, const void *b);

/**
 * @brief Divides the array of points into a new array containing points in the specified range.
 * @details This function allocates a new array of Points containing the elements from 'start' to 'end'.
 *          It is the caller's responsibility to free the allocated memory.
 * @note Ensure 'start' is less than 'end' and 'end' does not exceed the total number of elements.
 * @param points The original array of Points.
 * @param start The starting index for the division.
 * @param end The ending index (exclusive) for the division.
 * @return A pointer to of all elements from points from start to end
 */
Point *dividePoints(Point *points, int *status, size_t start, size_t end);

/**
 * @brief Gets the index of the first point whose coordinate is greater than or equal to the mean.
 * @details Iterates through the array of points and checks the specified coordinate (x or y)
 *          against the mean. It returns the index of the first point that meets or exceeds the mean.
 * @note Note that the program does only allow x or y coordinates or else it woll abort!
 * @param points A pointer to the first element in an array of Point structures.
 * @param mean The mean value to compare against the point's coordinate.
 * @param size The number of elements in the points array.
 * @param c The coordinate to check ('x' for x-coordinate, 'y' for y-coordinate).
 * @return The index of the first point with the specified coordinate greater than or equal to mean,
 *         or the last index if no such point is found.
 */
size_t getIndexOfMean(Point *points, double mean, size_t size, char c);


bool findClosestPair(Point *points, const size_t *n, int leftPipe[2], int rightPipe[2]);

Pair newPairFromTwoPairs(Pair p1, Pair p2);

Pair nearestPair(Pair p1, Pair p2, Pair p3);

Pair newPair(Point p1, Point p2);


#endif //OSVU23_CPAIR_H
