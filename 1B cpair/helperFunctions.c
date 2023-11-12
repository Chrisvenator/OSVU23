/**
 * @file cpair.c
 * @brief collection functions for cpair.c
 * @details This file implements functions defined in cpair.h
 *          I split it into multiple files so that the main file is easier to read
 * @author Christopher Scherling 12119060
 * @date 11.11.2023
 */
#include <sys/select.h>

void usage(void) {
    fprintf(stderr, "[%s]USAGE: %s\n", programName, programName);
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

struct Process {
    pid_t pid;       // Process ID
    int readPipe[2]; // Read pipe (file descriptors)
    int writePipe[2]; // Write pipe (file descriptors)
};

static bool initProcess(Process *process) {
    process->pid = -1; // Initialize PID to an invalid value
    if (pipe(process->readPipe) == -1 ||
        pipe(process->writePipe) == -1) {
        fprintf(stderr, "[%s] Failed to create pipe!", programName);
        return false;
    }

    return true;
}

static void cleanupProcess(Process *process) {
    close(process->readPipe[0]);  // Close read pipe input
    close(process->readPipe[1]);  // Close read pipe output
    close(process->writePipe[0]); // Close write pipe input
    close(process->writePipe[1]); // Close write pipe output
}

static Pair newPair(Point p1, Point p2) {
    Pair p;

    if (p1.x < p2.x) {
        p.p1 = p1;
        p.p2 = p2;
    } else if (p1.x == p2.x) {
        if (p1.y < p2.y) {
            p.p1 = p1;
            p.p2 = p2;
        } else {
            p.p1 = p2;
            p.p2 = p1;
        }
    } else {
        p.p1 = p2;
        p.p2 = p1;
    }
    p.dist = distance(p1, p2);

    return p;
}

static bool is_float(char *str) {
    char *endptr;
    errno = 0;  // To distinguish success/failure after the call to strtod

    strtod(str, &endptr); //convert to a double

    // Check for various possible errors
    if (endptr == str) return false;
    if (isspace((unsigned char) *str)) return false;
    if (*endptr != '\0') return false;
    if (errno == ERANGE) return false;

    // If we get here, it's a float
    return true;
}

static void remove_all_chars(char *str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

static double distance(struct Point p1, struct Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}

static double getPairDistance(Pair pair) {
    return distance(pair.p1, pair.p2);
}

static int compareX(const void *a, const void *b) {
    Point *p1 = (Point *) a;
    Point *p2 = (Point *) b;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}

static int compareY(const void *a, const void *b) {
    Point *p1 = (Point *) a;
    Point *p2 = (Point *) b;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

static double calculateArithmeticMean(Point *points, char coordinate, size_t numberOfElements) {
    //I don't need to check if the points are NULL or numberOfElements is 0, because it wouldn't come this far

    double mean = 0;
    for (size_t i = 0; i < (size_t) numberOfElements; i++) {
        if (coordinate == 'x') mean += points[i].x;
        if (coordinate == 'y') mean += points[i].y;
    }

    return (mean / numberOfElements);
}

static Point getCoordinates(char *string, int *status) {
    int amountOfSpaces = 0;
    float x;
    float y;


    //initialize Point so that the compiler shuts up...
    Point point;
    point.x = 0;
    point.y = 0;

    for (int i = 0; i < strlen(string); ++i) if (string[i] == ' ') amountOfSpaces++;
    if (string == NULL || amountOfSpaces != 1) {
        fprintf(stderr, "[%s] ERROR: There must only be 2 coordinates. Not more, not less!\n", programName);
        *status = EXIT_FAILURE;
        return point;
    }

    remove_all_chars(string, '\n');

    // Extract the first float
    char *token = strtok(string, " ");
    if (token != NULL && is_float(token) == true) x = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The first coordinate is not a float: %s\n", programName, token);
        *status = EXIT_FAILURE;
        return point;
    }


    // Extract the second float
    token = strtok(NULL, " ");
    if (token != NULL && is_float(token) == true) y = strtof(token, NULL);
    else {
        fprintf(stderr, "[%s] ERROR: The second coordinate is not a float: %s\n", programName, token);
        *status = EXIT_FAILURE;
        return point;
    }

    point.x = x;
    point.y = y;

    return point;
}

static void printPair(FILE *output, Pair pair) {
    if ((pair.p1.x < pair.p2.x) || (pair.p1.x == pair.p2.x && pair.p1.y < pair.p2.y)) {
        fprintf(output, "%.3f %.3f\n%.3f %.3f\n", pair.p1.x, pair.p1.y, pair.p2.x, pair.p2.y);
    } else {
        fprintf(output, "%.3f %.3f\n%.3f %.3f\n", pair.p2.x, pair.p2.y, pair.p1.x, pair.p1.y);
    }
}

static void printPointPointer(FILE *file, Point *points, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        fprintf(file, "%.3f %.3f\n", points[i].x, points[i].y);
    }
}

static Point *loadData(size_t *ptr_numberOfElements) {


    size_t capacity = 2;
    Point *points = malloc(capacity * sizeof(Point));
    if (points == NULL) {
        perror("Failed to allocate memory");
        free(points);
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t size = 0;
    size_t i = 0; //<-- Count how many elements there have been

    while (getline(&line, &size, stdin) >= 0) {
        if (strncmp(line, "\n", strlen(line)) == 0) continue;
        if (i >= capacity) { // check if we need to increase the size of the array
            capacity *= 2;

            //You should instead store the return value into a temporary variable so you can free the original pointer in case of failure, thus avoiding a memory leak.
            Point *temp = (Point *) realloc(points, capacity * sizeof(Point));
            if (temp == NULL) {
                perror("Failed to reallocate memory");
                free(points);
                free(line);
                free(temp);
                temp = NULL;
                exit(EXIT_FAILURE);
            }
            points = temp;
            temp = NULL;
        }
        int status = EXIT_SUCCESS;
        points[i] = getCoordinates(line, &status);
        if (status == EXIT_FAILURE) {
            free(points);
            free(line);
            exit(EXIT_FAILURE);
        }


        i++;
    }

    *ptr_numberOfElements = i;

    free(line); // Free the buffer allocated by getline
    return points;
}

static bool checkIfAllCoordinatesAreTheSame(Point *points, size_t numberOfElements) {
    if (points == NULL || numberOfElements == 0) {
        fprintf(stderr, "\n[%s]There has been an error while calculating the arithmetic mean. Points == null : %d; number of elements: %zu", programName, points == NULL, numberOfElements);
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

static bool checkIfAllXValuesAreTheSame(Point *points, size_t numberOfElements) {
    if (points == NULL || numberOfElements == 0) {
        // Handle invalid input
        fprintf(stderr, "[%s]Invalid input: points is NULL or numberOfElements is 0.\n", programName);
        return false;
    }

    Point p0 = points[0];
    for (size_t i = 1; i < numberOfElements; i++) {
        if (p0.x != points[i].x) return false;
    }
    return true;
}

static size_t getIndexOfMean(Point *points, double mean, size_t size, char c) {
    for (size_t i = 0; i < size; ++i) {
        if ((c == 'x' && mean <= points[i].x) ||
            (c == 'y' && mean <= points[i].y))
            return i;
    }

    //If all values are smaller (which is actually not possible. but it may happen because of rounding)
    return size - 1;
}

static Point *dividePoints(Point *points, int *status, size_t start, size_t end) {
    size_t numPoints = end - start;
    Point *newPoints = malloc(numPoints * sizeof(Point));
    if (newPoints == NULL) {
        fprintf(stderr, "[%s]Memory allocation failed!\n", programName);
        *status = EXIT_FAILURE;
        return newPoints;
    }

    for (size_t i = start, pos = 0; i < end; ++i, ++pos) {
        newPoints[pos] = points[i];
    }

    return newPoints;
}

static Pair newPairFromTwoPairs(Pair p1, Pair p2) {
    Pair pair3 = newPair(p1.p1, p2.p1);
    Pair pair4 = newPair(p1.p2, p2.p2);
    Pair pair5 = newPair(p1.p1, p2.p2);
    Pair pair6 = newPair(p1.p2, p2.p1);

    pair3.dist = getPairDistance(pair3);
    pair4.dist = getPairDistance(pair4);
    pair5.dist = getPairDistance(pair5);
    pair6.dist = getPairDistance(pair6);

    Pair nearest = pair3;
    if (pair4.dist < nearest.dist) nearest = pair4;
    if (pair5.dist < nearest.dist) nearest = pair5;
    if (pair6.dist < nearest.dist) nearest = pair6;


    return nearest;
}

static Pair newPairFromOnePairAndOnePoint(Pair p1, Point p) {
    Pair pair3 = newPair(p1.p1, p);
    Pair pair4 = newPair(p1.p2, p);

    pair3.dist = getPairDistance(pair3);
    pair4.dist = getPairDistance(pair4);

    Pair nearest = p1;
    if (pair3.dist < nearest.dist) nearest = pair3;
    if (pair4.dist < nearest.dist) nearest = pair4;

    return nearest;
}

static Pair nearestPair(Pair p1, Pair p2, Pair p3) {
    Pair nearest = p1;
    if (p2.dist < nearest.dist) nearest = p2;
    if (p3.dist < nearest.dist) nearest = p3;

    if (nearest.p1.x > nearest.p2.x) {
        Point temp = nearest.p1;
        nearest.p1 = nearest.p2;
        nearest.p2 = temp;
    } else if (nearest.p1.x == nearest.p2.x) {
        if (nearest.p1.y > nearest.p2.y) {
            Point temp = nearest.p1;
            nearest.p1 = nearest.p2;
            nearest.p2 = temp;
        }
    }

    return nearest;
}

// if this function returns -1, then there has been nothing returned from the child process. Which is ok.
// If this function returns -2, then something has gone wrong. See stderr for details:
static ssize_t readPair(FILE *file, Pair *pair) {
    ssize_t stored = 0;
    size_t size = 0;
    char *line = NULL;

    if ((getline(&line, &size, file)) == -1) {
        free(line);
        return -1;
    }

    int status = EXIT_SUCCESS;
    pair->p1 = getCoordinates(line, &status);
    if (status == EXIT_FAILURE) {
        free(line);
        return -2;
    }

    stored++;

    if ((getline(&line, &size, file)) == -1) {
        fprintf(stderr, "[%s]ONLY ONE LINE PRESENT IN CHILD!\n", programName);
        free(line);
        return -2;
    }

    pair->p2 = getCoordinates(line, &status);
    if (status == EXIT_FAILURE) {
        free(line);
        return -2;
    }

    stored++;

    pair->dist = distance(pair->p1, pair->p2);

    free(line);
    return stored;
}

static void checkFile(FILE *file, int *status, const char *description) {
    if (file == NULL) {
        fprintf(stderr, "[%s]%s", programName, description);  // Print error message
        *status = EXIT_FAILURE;
    }
}

static Pair calculateNearestPointsBruteForce(Point *points, size_t size) {
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

static Pair closestPairIncludingMeanProblem(Point *points, size_t numberOfElements, Pair nearestPair, double mean, char axis) {
    double delta = nearestPair.dist;
    if (numberOfElements < 2) return nearestPair;

    size_t capacity = 2;
    Point *pointsCloseToMean = malloc(capacity * sizeof(Point));
    size_t position = 0;
    if (pointsCloseToMean == NULL) {
        fprintf(stderr, "[%s]Failed to allocate memory", programName);
        free(points);
        free(pointsCloseToMean);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < numberOfElements; ++i) {
        if (i >= capacity) { // check if we need to increase the size of the array
            capacity *= 2;
            Point *temp = (Point *) realloc(pointsCloseToMean, capacity * sizeof(Point));
            if (temp == NULL) {
                fprintf(stderr, "[%s]Failed to reallocate memory", programName);
                free(points);
                free(pointsCloseToMean);
                exit(EXIT_FAILURE);
            }
            pointsCloseToMean = temp;
        }

        if ((points[i].x < mean && points[i].x > mean - delta) ||
            (points[i].x > mean && points[i].x < mean + delta)) {
            pointsCloseToMean[position] = points[i];
            ++position;
        }
    }

    if (position < 2) {
        free(pointsCloseToMean);
        return nearestPair;
    }
    Pair nearest = calculateNearestPointsBruteForce(pointsCloseToMean, position);


    free(pointsCloseToMean);
    return nearest.dist < nearestPair.dist ? nearest : nearestPair;
}