#include "cpair.h"

char *programName;

/**
 * @brief Prints the usage information for the program and exits.
 * This function is typically called when the program is invoked with incorrect arguments.
 * It will print the correct usage format of the program to the standard error and then exit
 * with EXIT_FAILURE to indicate that the program has terminated unsuccessfully.
 */
void usage(void) {
    fprintf(stderr, "USAGE: %s", programName);
    exit(EXIT_FAILURE);
}

/**
 * @brief Function to compute the Euclidean distance between two points.
 * @param p1 The first point as a Point structure.
 * @param p2 The second point as a Point structure.
 * @return The Euclidean distance as a double.
 */
double distance(struct Point p1, struct Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}


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
Point getCoordinates(char *string) {
    int amountOfSpaces = 0;
    float x;
    float y;

    for (int i = 0; i < strlen(string); ++i) if (string[i] == ' ') amountOfSpaces++;
    if (string == NULL || amountOfSpaces != 1) {
        fprintf(stderr, "[%s] ERROR: There must only be 2 coordinates. Not more, not less!\n", programName);
        exit(EXIT_FAILURE);
    }

    remove_all_chars(string, '\n');

    // Extract the first token
    char *token = strtok(string, " ");
    if (token != NULL && is_float(token) == 1) x = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The first coordinate is not a float: %s\n", programName, token);
        exit(EXIT_FAILURE);
    }


    token = strtok(NULL, " ");
    if (token != NULL && is_float(token) == 1) y = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The second coordinate is not a float: %s\n", programName, token);
        exit(EXIT_FAILURE);
    }

    Point point;
    point.x = x;
    point.y = y;

    return point;
}

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
Point *loadData() {
    //TODO: Remove this once stdin is functional
    FILE *input = fopen("/home/junioradmin/CLionProjects/OSVU23/1B cpair/stdin.txt", "r");
    if (input == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    //Allocate memory for 10 points of the points []
    size_t capacity = 1;
    Point *points = malloc(capacity * sizeof(Point));
    if (points == NULL) {
        perror("Failed to allocate memory");
        fclose(input);
        exit(EXIT_FAILURE);
    }


    char *line = NULL;
    size_t size = 0;

    int i = 1; //<-- Count how many elements there have been
    while (getline(&line, &size, input) >= 0) { //TODO: replace with stdin
        if (i >= capacity) { // check if we need to increase the size of the array
            capacity *= 2;
            Point *temp = (Point *) realloc(points, capacity * sizeof(Point));
            if (temp == NULL) {
                perror("Failed to reallocate memory");
                free(points);
                fclose(input);
                exit(EXIT_FAILURE);
            }
            points = temp;
        }

        points[i] = getCoordinates(line);
        printf("Point %d:\nx:%f\ny:%f\n\n", i, points[i].x, points[i].y);
        i++;
    }


    free(line); // Free the buffer allocated by getline
    fclose(input); //TODO: Remove this once stdin is functional



    //TODO: free points
//    free(points);
    return points;
}

/**
 *
 * @param argc
 * @param argv
 * @return
 * @author Christopher Scherling 12119060
 */
int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc != 1) usage(); //If the wrong number of arguments have been passed, throw an error

    Point *points = loadData();


    free(points);
    exit(EXIT_SUCCESS);
}

//Command "cat" on Windows: "tpye .\stdin.txt"