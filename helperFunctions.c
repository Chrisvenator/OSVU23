//
// Created by junioradmin on 03.11.23.
//
#include "cpair.h"

void usage(void) {
    fprintf(stderr, "USAGE: %s\n", programName); 
    exit(EXIT_FAILURE);
}

struct Point {
    float x, y;
};

struct Pair {// holds a pair of point and the distance between them
    Point p1; // first point
    Point p2; // second point
    double dist; // distance between p1 and p2
};

Pair newPair(Point p1, Point p2) {
    Pair p;
    p.p1 = p1;
    p.p2 = p2;
    p.dist = distance(p1, p2);

    return p;
}

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

double distancePair(Pair pair) {
    return distance(pair.p1, pair.p2);
}

int compareX(const void *a, const void *b) {
    Point *p1 = (Point *) a;
    Point *p2 = (Point *) b;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}


int compareY(const void *a, const void *b) {
    Point *p1 = (Point *) a;
    Point *p2 = (Point *) b;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

double calculateArithmeticMean(Point *points, char coordinate) {
    if (points == NULL || numberOfElements == 0) {
        errno = EINVAL; // Set errno to indicate an invalid argument error.
        fprintf(stderr,
                "\nThere has been an error while calculating the arithmetic mean. Points == null : %d; number of elements: %zu",
                points == NULL, numberOfElements);
        return 0.0;
    }

    double mean = 0;
    for (size_t i = 0; i < (size_t) numberOfElements; i++) {
        if (coordinate == 'x') mean += points[i].x;
        if (coordinate == 'y') mean += points[i].y;
    }

    return (mean / numberOfElements);
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

void printPair(Pair pair) {
    if ((pair.p1.x < pair.p2.x) || (pair.p1.x == pair.p2.x && pair.p1.y < pair.p2.y)) {
        fprintf(stdout, "%.3f %.3f\n%.3f %.3f\n", pair.p1.x, pair.p1.y, pair.p2.x, pair.p2.y);
    } else {
        fprintf(stdout, "%.3f %.3f\n%.3f %.3f\n", pair.p2.x, pair.p2.y, pair.p1.x, pair.p1.y);
    }
}

void printPointPointer(Point *points, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        fprintf(stdout, "\n%.3f %.3f", points[i].x, points[i].y);
    }
}

//TODO: rewrite:
Point *loadData() {
    //TODO: Remove this once stdin is functional
    FILE *input = stdin;
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) <= 0) {
//        input = fopen("/home/junioradmin/CLionProjects/OSVU23/1B cpair/stdin.txt", "r");
//        if (input == NULL) {
            perror("Error opening file");
//            exit(EXIT_FAILURE);
//        }
      exit(EXIT_SUCCESS);
    }

    //Allocate memory for 2 points of the points []
    size_t capacity = 2;
    Point *points = malloc(capacity * sizeof(Point));
    if (points == NULL) {
        perror("Failed to allocate memory");
        fclose(input);
        exit(EXIT_FAILURE);
    }


    char *line = NULL;
    size_t size = 0;

    size_t i = 0; //<-- Count how many elements there have been
    while (getline(&line, &size, stdin) >= 0) { //TODO: replace with stdin
        if (strcmp(line, "\n") == 0) continue;
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
//        printf("Point %zu:\nx:%f\ny:%f\n\n", i, points[i].x, points[i].y);
        i++;
    }

    numberOfElements = i;
//    printf("Number of elements: %f", numberOfElements);


    free(line); // Free the buffer allocated by getline
    fclose(input); //TODO: Remove this once stdin is functional



    //TODO: free points
//    free(points);
    return points;
}


bool checkIfAllCoordinatesAreTheSame(Point *points) {
    if (points == NULL || numberOfElements == 0) {
        fprintf(stderr,
                "\nThere has been an error while calculating the arithmetic mean. Points == null : %d; number of elements: %zu",
                points == NULL, numberOfElements);
        return false;
    }

    // Get the first point to compare with others
    Point firstPoint = points[0];

    for (size_t i = 1; i < numberOfElements; ++i) {
        if (firstPoint.x != points[i].x || firstPoint.y != points[i].y) {
            return false; // If any point differs, they are not all the same
        }
    }

    return true; // If we've checked all and none differed, they are all the same
}

bool checkIfAllXValuesAreTheSame(Point *points) {
    if (points == NULL || numberOfElements == 0) {
        // Handle invalid input
        fprintf(stderr, "Invalid input: points is NULL or numberOfElements is 0.\n");
        return false;
    }

    Point p0 = points[0];
    for (size_t i = 1; i < numberOfElements; i++) {
        if (p0.x != points[i].x) return false;
    }
    return true;
}


size_t getIndexOfMean(Point *points, double mean, size_t size, char c) {
    if (points == NULL) {
        fprintf(stderr, "Invalid input in 'getIndexOfMean': points is NULL.\n");
        return SIZE_MAX; // Indicates an error.
    }

    if (size == 0) {
        fprintf(stderr, "Invalid input in 'getIndexOfMean': size is 0.\n");
        return SIZE_MAX; // Indicates an error.
    }

    if (c != 'x' && c != 'y') {
        fprintf(stderr, "Invalid input in 'getIndexOfMean': parameter c is not x or y. Aborting...\n");
        return SIZE_MAX; // Indicates an error.
    }

    for (size_t i = 0; i < size; ++i) {
        if ((c == 'x' && mean <= points[i].x) ||
            (c == 'y' && mean <= points[i].y))
            return i;
    }

    return size - 1;
}


Point *dividePoints(Point *points, size_t start, size_t end) {
    if (points == NULL) {
        fprintf(stderr, "Invalid input in 'dividePoints': points is NULL.\n");
        assert(0);
        exit(EXIT_FAILURE);
    }

    if (start > end || numberOfElements < end) {
        fprintf(stderr, "Invalid input in 'dividePoints': end is not a valid number.\n");
        assert(0);
        exit(EXIT_FAILURE);
    }


    size_t numPoints = end - start;
    Point *newPoints = malloc(numPoints * sizeof(Point));
    if (newPoints == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = start, pos = 0; i < end; ++i, ++pos) {
        newPoints[pos] = points[i];
    }

    return newPoints;
}

//A Methode to test if the program works.
Pair calculateNearestPointsBruteForce(Point *points, size_t size) {
    if (size < 2) {
        fprintf(stderr,
                "[%s] ERROR: the size to calculate the nearest distance must be bigger or equals than 2. Size: %zu",
                programName, size);
        assert(1);
    } else if (points == NULL) {
        fprintf(stderr, "Invalid input in 'getIndexOfMean': points is NULL.\n");
        assert(1);
    }

    Pair p;
    p.p1 = points[0];
    p.p2 = points[1];
    p.dist = distance(p.p1, p.p2);
    if (size == 2) return p;


    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i + 1; j < size; ++j) {
            double dist = distance(points[i], points[j]);
            if (dist < p.dist) {
                p.p1 = points[i];
                p.p2 = points[j];
                p.dist = dist;
            }
        }
    }
    return p;
}

Pair newPairFromTwoPairs(Pair p1, Pair p2) {
    Pair pair3 = newPair(p1.p1, p2.p1);
    Pair pair4 = newPair(p1.p2, p2.p2);
    Pair pair5 = newPair(p1.p1, p2.p2);
    Pair pair6 = newPair(p1.p2, p2.p1);

    pair3.dist = distancePair(pair3);
    pair4.dist = distancePair(pair4);
    pair5.dist = distancePair(pair5);
    pair6.dist = distancePair(pair6);

    Pair nearest = pair3;
    if (pair4.dist < nearest.dist) nearest = pair4;
    if (pair5.dist < nearest.dist) nearest = pair5;
    if (pair6.dist < nearest.dist) nearest = pair6;


    return nearest;
}

Pair nearestPair(Pair p1, Pair p2, Pair p3) {
    Pair nearest = p1;
    if (p2.dist < nearest.dist) nearest = p2;
    if (p3.dist < nearest.dist) nearest = p3;

    return nearest;
}


/*
 * pid_t leftChild, rightChild;
    switch (leftChild = fork()) {
        case -1:
            fprintf(stderr, "Cannot fork!\n");
            close(leftPipe[0]);
            close(leftPipe[1]);
            close(rightPipe[0]);
            close(rightPipe[1]);
            exit(EXIT_FAILURE);
        case 0: //Child Element:
            if (dup2(leftPipe[1], STDOUT_FILENO) == -1 ||
                dup2(leftPipe[0], STDIN_FILENO) == -1) { //TODO: stimmt das?
                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n",
                        programName, strerror(errno));
                free(points);
                exit(EXIT_FAILURE);
            }
            close(rightPipe[0]);
            close(rightPipe[1]);
            close(leftPipe[1]); //Close left read pipe since we don't need that one

            execlp(programName, programName, NULL);

            //We shouldn't get here
            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            free(points);
            assert(1);
            exit(EXIT_FAILURE);
        default:
            switch (rightChild = fork()) {
                case -1:
                    fprintf(stderr, "Cannot fork!\n");
                    close(leftPipe[0]);
                    close(leftPipe[1]);
                    close(rightPipe[0]);
                    close(rightPipe[1]);
                    exit(EXIT_FAILURE);
                case 0: //Child Element:
                    if (dup2(rightPipe[1], STDOUT_FILENO) == -1 ||
                        dup2(rightPipe[0], STDIN_FILENO) == -1) { //TODO: stimmt das?
                        fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n",
                                programName, strerror(errno));
                        free(points);
                        exit(EXIT_FAILURE);
                    }
                    close(leftPipe[0]);
                    close(leftPipe[1]);
                    close(rightPipe[1]); //Close left read pipe since we don't need that one

                    execlp(programName, programName, NULL);

                    //We shouldn't get here
                    fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
                    free(points);
                    assert(1);
                    exit(EXIT_FAILURE);
                default:
                    writeToParent();
                    readFromChild();
                    computeClosestPair();
                    break;
            }
            break;
    }
 */