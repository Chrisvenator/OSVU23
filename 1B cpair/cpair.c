#include "cpair.h"
#include "helperFunctions.c" //TODO: ABSOLUTELY REMOVE THIS LINE!!!!!!


/**
 * @brief
 * @details
 * @note
 * @example
 * @param argc
 * @param argv
 * @return
 * @author Christopher Scherling 12119060
 */
int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc != 1) usage(); //If the wrong number of arguments have been passed, throw an error
    size_t myNumberOfElements;
    size_t *ptr_numberOfElements = &myNumberOfElements;

    Point *points = loadData(ptr_numberOfElements);
    Point *smaller = NULL; //Values smaller than mean
    Point *bigger = NULL; //Values bigger than mean

//    printf("%zu Number of elements: %zu", numberOfElements, *ptr_numberOfElements);

    int leftPipe[2];
    int rightPipe[2];

    //TODO: THIS:
    if (findClosestPair(points, &myNumberOfElements, smaller, bigger, leftPipe, rightPipe) == false) {
        assert(1);
    }
    if (close(leftPipe[0]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(leftPipe[1]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(rightPipe[0]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(rightPipe[1]) != 0) fprintf(stderr, "Error closing pipes!\n");

    free(points);
    free(smaller);
    free(bigger);


    exit(EXIT_SUCCESS);
}

bool findClosestPair(Point *points, const size_t *n, Point *smaller, Point *bigger, int leftPipe[2], int rightPipe[2]) {
    size_t numberOfElements = *n;

    // Base cases
    if (numberOfElements <= 1) {
        return true;
    } else if (numberOfElements == 2) {
        Pair pair;
        pair.p1.x = points[0].x;
        pair.p1.y = points[0].y;
        pair.p2.x = points[1].x;
        pair.p2.y = points[1].y;

        printPair(pair);
        return true;
    }
    // Calculate mean x-coordinate, xm

    double mean;
    size_t index;

    if (checkIfAllCoordinatesAreTheSame(points, numberOfElements) == true) {
        Point p0 = points[0];
        Pair p;
        p.p1 = p0;
        p.p2 = p0;
        printPair(p);
        return true;
    } else if (checkIfAllXValuesAreTheSame(points, numberOfElements) == true) {
        mean = calculateArithmeticMean(points, 'y', numberOfElements);
        qsort(points, (size_t) numberOfElements, sizeof(Point), compareY);
        index = getIndexOfMean(points, mean, numberOfElements, 'y');
    } else {
        mean = calculateArithmeticMean(points, 'x', numberOfElements);
        qsort(points, (size_t) numberOfElements, sizeof(Point), compareX);
        index = getIndexOfMean(points, mean, numberOfElements, 'x');
    }

    // /////////////////////// //
    // Divide points around xm //
    // /////////////////////// //

    //Check if the index, where the mean is located, is an odd number. If yes, we need to include the mean as well or else it will be cut out
    index = index % 2 == 0 ? index : index + 1;

    smaller = dividePoints(points, 0, index, numberOfElements); //Size of smaller: index
    bigger = dividePoints(points, index, numberOfElements, numberOfElements); //Size of bigger: numberOfElements - index

//    printPointPointer(stdout, smaller, index);
//    printPointPointer(stdout, bigger, numberOfElements - index);

    Process processLeft;
    Process processRight;


    if (pipe(leftPipe) == -1 || pipe(rightPipe) == -1) {
        fprintf(stderr, "[%s] Error creating pipe!", programName);
        return false;
    }

    processLeft.pipeRead = leftPipe[0];
    processLeft.pipeWrite = leftPipe[1];
    processRight.pipeRead = rightPipe[0];
    processRight.pipeWrite = rightPipe[1];

    switch (processLeft.pid = fork()) {
        case -1:
            fprintf(stderr, "Cannot fork!\n");
            return false;
        case 0: //We are now in the first child element
            if (dup2(processLeft.pipeWrite, STDOUT_FILENO) == -1 || dup2(processLeft.pipeRead, STDIN_FILENO) == -1) {
                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            close(processRight.pipeRead);
            close(processRight.pipeWrite);
            close(processLeft.pipeWrite);
            close(processRight.pipeRead);

            //communicate with child
            execlp(programName, programName, NULL);


            //We shouldn't get here
            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            return false;
        default:
            switch (processRight.pid = fork()) {
                case -1:
                    fprintf(stderr, "[%s]Cannot fork!", programName);
                    return false;
                case 0:
                    if (dup2(processRight.pipeWrite, STDOUT_FILENO) == -1 || dup2(processRight.pipeRead, STDIN_FILENO) == -1) {
                        fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                        return false;
                    }
                    close(processLeft.pipeRead);
                    close(processLeft.pipeWrite);
                    close(processRight.pipeWrite);
                    close(processRight.pipeRead);

                    //communicate with child
                    execlp(programName, programName, NULL);

                    //We shouldn't get here
                    fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
                    return false;
                default:
                    //in Prozesse schreiben (so wie lesen) //TODO: should I use index or index+1?
                    break;
            }
            break;
    }

    if (writeToChild(processRight, bigger, numberOfElements - index) != true) return false;
    if (writeToChild(processLeft, smaller, index) != true) return false;


//    printPointPointer(stdout, smaller, index);
//    printPointPointer(stdout, bigger, numberOfElements - index);


    //waitpit
    if (waitForChild(processLeft) == false) return false;
    if (waitForChild(processRight) == false) return false;

    //prozesse lesen:
    FILE *leftRead = fdopen(processLeft.pipeRead, "r");
    FILE *rightRead = fdopen(processRight.pipeRead, "r");

    //TODO: Fix if no value is present
    //mit getLine lesen und parsen. Es sind IMMER 2 Punkte vorhanden
    Pair pair1 = readPair(2, leftRead);
    Pair pair2 = readPair(2, rightRead);
    Pair pair3;

    //check if no value has been returned. <--
    if (pair1.p1.x != FLT_MAX && pair2.p1.x != FLT_MAX) pair3 = newPairFromTwoPairs(pair1, pair2);
    if (pair1.p1.x == FLT_MAX && pair2.p1.x == FLT_MAX) pair3 = newPair(smaller[0], bigger[0]);
    if (pair1.p1.x == FLT_MAX) pair3 = newPairFromOnePairAndOnePoint(pair2, smaller[0]);
    if (pair2.p1.x == FLT_MAX) pair3 = newPairFromOnePairAndOnePoint(pair1, bigger[0]);


    //calculate nearest pair and print it:
    printPair(nearestPair(pair1, pair2, pair3));

    //flush
    fflush(stdout);
    fflush(leftRead);
    fflush(rightRead);


    //close pipes
    fclose(leftRead);
    fclose(rightRead);

//    printPair(calculateNearestPointsBruteForce(points, numberOfElements));


    //TODO: Close all pipes all the time!
    //TODO: keine exit() beutzen. Nur in main(); Sonst is es sehr einfach zu vergessen die pipes zu schlißen
    //^-->  Methode von void in boolean umwandeln und dann alle pipes schließen. bzw eine Funktion erstellen.

    return true;
}


