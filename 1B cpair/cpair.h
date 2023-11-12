/**
 * @author Christopher Scherling
 * @file cpair.h
 * @date 01.11.2023
 * @brief header of cpair. Defines all structs, functions and comments
 * @details defines macros, structs and functions. This is the file in which all comments are written
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
 * @brief Struct to represent a point in 2D space.
 *
 * @details
 * This struct is used to hold the coordinates of a point in a two-dimensional
 * plane.
 * @details Each point is defined by a pair of floating-point numbers representing
 * its horizontal (x) and vertical (y) positions
 */
typedef struct Point Point;

/**
 * @brief Structure to represent a process with associated pipes for IPC (Inter-Process Communication).
 * @details This structure encapsulates the details of a process, including its process ID and file descriptors
 *          for read and write pipes. It is designed to facilitate IPC between processes, typically between
 *          a parent and its forked children. The readPipe is used for reading data sent by another process,
 *          and the writePipe is used for sending data to another process.
 */
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
 * the program will set the status pointer to EXIT_FAILURE and return an invalid Point
 * @param string A character array containing the two float numbers separated by a space.
 * It must be a null-terminated string and should not be NULL itself.
 * @param status A pointer in which the status of the function is being saved. It will be either EXIT_SUCCESS or EXIT_FAILURE
 * @return A Point structure with the first number as the x-coordinate and the second number as the y-coordinate.
 */
static Point getCoordinates(char *string, int *status);

/**
 * @brief Function to compute the Euclidean distance between two points.
 * @param p1 The first point as a Point structure.
 * @param p2 The second point as a Point structure.
 * @return The Euclidean distance as a double.
 */
static double distance(struct Point p1, struct Point p2);

/**
 * same as distance
 */
static double getPairDistance(Pair pair);

/**
 * @brief Prints the usage information for the program and exits.
 * @details This function is typically called when the program is invoked with incorrect arguments.
 * It will print the correct usage format of the program to the standard error and then exit
 * with EXIT_FAILURE to indicate that the program has terminated unsuccessfully.
 */
void usage(void);

/**
 * @brief Reads point data from standard input and loads it into a dynamically allocated array.
 * @details This function reads point data from stdin, expecting each line to contain
 * a pair of float numbers representing coordinates. It dynamically allocates an array
 * to store the points and resizes it as needed.
 * @return A pointer to an array of Point structures containing the loaded data. The array
 * is dynamically allocated and must be freed by the caller. The function will terminate
 * the program if stdin opening fails, memory allocation fails, or reallocating memory fails.
 * @warning The user needs to free the array manually later!
 * @param ptr_numberOfElements A pointer to a size_t variable where the number of elements read will be stored.
 */
static Point *loadData(size_t *ptr_numberOfElements);

/**
 * @brief Checks if the string is a valid representation of a float.
 * @details This function attempts to parse a string as a floating-point number and determine
 * its validity. It converts the string to a double using strtod, which sets a pointer to the
 * next character in the string after the numerical value. The function then checks:<br>
 * - If no numerical digits were found at the beginning of the string.<br>
 * - If the string starts with a space, it is not considered a valid float.<br>
 * - If there are any leftover non-numerical characters after the number.<br>
 * - If the value is out of the range of representable values of strtod, indicating an overflow or underflow.<br>
 * If any of these checks fail, the function returns false, indicating the string is not a valid float. Otherwise,
 * it returns true, indicating the string is a valid float.
 * @param str The input string to check for float representation.
 * @return Returns true if the string is a valid float, false otherwise.
 */
static bool is_float(char *str);

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
static void remove_all_chars(char *str, char c);

/**
 * @brief Prints the coordinates of a Pair to standard output.
 * @details This function takes a Pair structure (which includes two Points and the distance between them)
 * and prints the coordinates of these points to stdout in ascending order based on the x-coordinate.
 * If the x-coordinates are the same, it uses the y-coordinate to determine the order.
 * The coordinates are printed with three decimal places of precision.
 *
 * @param pair The Pair structure containing the two points to print.
 * @param output Output FILE pointer where the output should be printed
 */
static void printPair(FILE *output, Pair pair);

/**
 * @brief Calculate the arithmetic mean of a coordinate (x or y) over a set of points.
 * @details This function calculates the arithmetic mean for the specified coordinate (x or y)
 *          of all points in the provided array.
 * @note The coordinate parameter should be 'x' for the x-coordinate mean and 'y' for the y-coordinate mean.
 * @param points A pointer to the first element in an array of Point structs.
 * @param numberOfElements The number of elements in the points array.
 * @param coordinate A char indicating which coordinate to calculate the mean of ('x' or 'y').
 * @return The arithmetic mean as a double value, or 0 if an input error is detected.
 */
static double calculateArithmeticMean(Point *points, char coordinate, size_t numberOfElements);


/**
 * @brief Check if all points have the same coordinates.
 * @details This function iterates through an array of points and checks whether all points have the same x and y coordinates.
 *          It assumes the array is not NULL and contains at least one element. The caller should ensure the array is properly initialized.
 * @param points A pointer to the first element in an array of Point structs.
 * @param numberOfElements The number of elements in the points array.
 * @return A boolean value indicating whether all coordinates are the same (true) or not (false).
 */
static bool checkIfAllCoordinatesAreTheSame(Point *points, size_t numberOfElements);


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
static bool checkIfAllXValuesAreTheSame(Point *points, size_t numberOfElements);


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
static int compareX(const void *a, const void *b);

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
static int compareY(const void *a, const void *b);

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
static Point *dividePoints(Point *points, int *status, size_t start, size_t end);

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
static size_t getIndexOfMean(Point *points, double mean, size_t size, char c);


static bool findClosestPair(Point *points, const size_t *n);

/**
 * @brief Creates a new Pair from the closest combination of points in two given Pairs.
 * @details This function takes two Pairs as input and creates four new Pairs by combining their points.
 *          It then calculates the distances between the points in each of these new Pairs. The function
 *          returns the Pair that has the shortest distance between its points, effectively identifying the
 *          closest pair of points from the four possible combinations.
 * @param p1 The first Pair of points.
 * @param p2 The second Pair of points.
 * @return The Pair from the combined points of p1 and p2 that has the shortest distance between its points.
 */
static Pair newPairFromTwoPairs(Pair p1, Pair p2);

/**
 * @brief Finds the nearest pair among three given pairs.
 * @details Compares three pairs of points and determines which pair has the shortest distance
 *          between its points.
 * @param p1 The first pair of points to consider.
 * @param p2 The second pair of points to consider.
 * @param p3 The third pair of points to consider.
 * @return The nearest pair among the three input pairs.
 */
static Pair nearestPair(Pair p1, Pair p2, Pair p3);

/**
 * @brief Creates a new Pair structure from two Point structures.
 * @details This function takes two Point structures and orders them into a new Pair.
 *          The points in the pair are ordered primarily by their x-coordinates, and
 *          secondarily by their y-coordinates. This ensures that the point with the
 *          smaller x-coordinate (or y-coordinate if x-coordinates are equal) becomes p1,
 *          and the other point becomes p2 in the Pair structure. The distance between
 *          the two points is also calculated and stored in the Pair.
 * @param p1 The first Point structure.
 * @param p2 The second Point structure.
 * @return A Pair structure containing the two points in the specified order and their distance.
 */
static Pair newPair(Point p1, Point p2);

/**
 * @brief Prints an array of Point structures to a specified file stream.
 * @details Iterates through an array of Point structures and prints each point's coordinates
 *          to the specified file stream. The coordinates are formatted to three decimal places
 *          and each point is printed on a new line.
 * @param file A pointer to a FILE structure where the points will be printed.
 *             This could be a file or standard output/error streams.
 * @param points A pointer to the first element of an array of Point structures.
 * @param size The number of Point elements in the array.
 */
static void printPointPointer(FILE *file, Point *points, size_t size);

/**
 * @brief Creates a new Pair from a given Pair and an additional Point.
 * @details This function takes a Pair and a single Point as input and creates two new Pairs
 *          by combining the given Point with each point of the input Pair. It then calculates
 *          the distances between the points in these new Pairs. The function returns the Pair
 *          that has the shortest distance between its points, effectively identifying the closest
 *          pair of points from the two possible combinations.
 * @param p1 The initial Pair of points.
 * @param p An additional Point to be paired with each point of the initial Pair.
 * @return The Pair from the combinations of p1 and p that has the shortest distance between its points.
 */
static Pair newPairFromOnePairAndOnePoint(Pair p1, Point p);

/**
 * @brief Reads and constructs a Pair from two lines of input from a file stream.
 * @details This function attempts to read two lines from a given file stream, converting each line
 *          into a Point structure using the `getCoordinates` function, and then constructs a Pair
 *          from these two points. It also calculates the distance between these points. The function
 *          returns the number of lines successfully read and processed. It handles partial or failed
 *          reads by returning specific error codes.
 * @param file A FILE pointer to the file stream from which the lines are to be read.
 * @param pair A pointer to a Pair structure where the read points and their distance will be stored.
 * @return The number of lines successfully read and processed (should be 2 for a valid pair).
 *         Returns -1 if there is no line to read (EOF or empty stream), indicating that no data
 *         was returned from the child process. Returns -2 in case of an error, with a message
 *         printed to stderr for partial reads or other failures.
 */
static ssize_t readPair(FILE *file, Pair *pair);

/**
 * @brief Checks the validity of a file pointer and updates status accordingly.
 * @details This function examines a file pointer to determine if it is NULL. If the file pointer is NULL,
 *          indicating that the file failed to open or is otherwise invalid, the function prints an
 *          error message and updates the status to indicate failure.
 * @param file A FILE pointer to be checked.
 * @param status A pointer to an integer where the status of the check will be stored.
 *               Status is set to EXIT_FAILURE if the file pointer is NULL.
 * @param description A string describing the error, which will be printed if the file pointer is NULL.
 */
static void checkFile(FILE *file, int *status, const char *description);

/**
 * @brief Calculates the closest pair of points using a brute-force approach.
 * @details This function iterates through all pairs of points in the given array to find the pair
 *          with the smallest distance between them. It uses a brute-force approach, comparing each pair
 *          of points, which makes it straightforward but potentially inefficient for large datasets.
 *          This method is typically effective for small to moderately sized arrays of points.
 * @param points An array of Point structures to be examined.
 * @param size The number of elements in the points array.
 * @return A Pair structure containing the two closest points and their distance.
 */
static Pair calculateNearestPointsBruteForce(Point *points, size_t size);


/**
 * @brief Finds the closest pair of points, considering points close to a given mean.
 * @details This function examines a subset of points that are within a certain distance (delta)
 *          from a specified mean value. It uses a brute-force approach to find the closest pair
 *          within this subset.
 * @param points An array of Point structures to be examined.
 * @param numberOfElements The number of elements in the points array.
 * @param nearestPair The currently identified nearest pair from previous computations.
 * @param mean The mean value (x or y coordinate) around which to search for close points.
 * @param axis The axis ('x' or 'y') relative to which the mean value is defined.
 * @return The closest pair of points either in the given subset or the previously identified nearest pair.
 */
static Pair closestPairIncludingMeanProblem(Point *points, size_t numberOfElements, Pair nearestPair, double mean, char axis);

/**
 * @brief Cleans up and closes the pipes for a Process struct.
 * @details This function is responsible for closing the file descriptors associated with both the read and write pipes of a Process struct.
 *          It ensures that all ends of both pipes are properly closed to prevent resource leaks. This is a crucial step in the lifecycle
 *          management of processes and their associated inter-process communication channels.
 * @param process Pointer to the Process struct whose resources are to be cleaned up.
 */
static void cleanupProcess(Process *process);

/**
 * @brief Initializes a Process struct by setting up its pipes.
 * @details This function is responsible for initializing a Process struct. It sets the process ID (pid) to an invalid
 *          value (-1) and creates two pipes: one for reading and one for writing. The pipes are used for inter-process
 *          communication, typically between a parent process and its child process.
 * @param process Pointer to the Process struct that needs to be initialized.
 * @return Returns `true` if both pipes are successfully created, `false` otherwise. If false is returned,
 *         an error message is printed indicating the failure to create the pipes.
 */
static bool initProcess(Process *process);

#endif //OSVU23_CPAIR_H
