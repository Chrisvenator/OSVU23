//
// Created by junioradmin on 03.11.23.
//
#include "cpair.h"

void usage(void) {
    fprintf(stderr, "USAGE: %s", programName);
    exit(EXIT_FAILURE);
}

struct Point {
    float x, y;
} ;

struct Pair // holds a pair of point and the distance between them
{
    Point p1; // first point
    Point p2; // second point
    float dist; // distance between p1 and p2
};


int is_float(char *str) {
    char *endptr;
    errno = 0;  // To distinguish success/failure after the call to strtod

    strtod(str, &endptr); //convert to a double

    // Check for various possible errors
    if (endptr == str) return 1;
    if (isspace((unsigned char) *str)) return 1;
    if (*endptr != '\0') return 1;
    if (errno == ERANGE) return 1;

    // If we get here, it's a float
    return 0;
}

void remove_all_chars(char *str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

double distance(struct Point p1, struct Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}


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
    if (token != NULL && is_float(token) == 0) x = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The first coordinate is not a float: %s\n", programName, token);
        exit(EXIT_FAILURE);
    }


    token = strtok(NULL, " ");
    if (token != NULL && is_float(token) == 0) y = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The second coordinate is not a float: %s\n", programName, token);
        exit(EXIT_FAILURE);
    }

    Point point;
    point.x = x;
    point.y = y;

    return point;
}

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
    while (getline(&line, &size, stdin) >= 0) { //TODO: replace with stdin
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