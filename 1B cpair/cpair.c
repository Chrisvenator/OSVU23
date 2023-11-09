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

    for (int i = 0; i < myNumberOfElements; i++) {
        printf("%f %f\n", points[i].x, points[i].y);
    }
    printf("===========================\n");

    int leftPipe[2];
    int rightPipe[2];


    //TODO: THIS:
    if (findClosestPair(points, &myNumberOfElements, leftPipe, rightPipe) == false) {
        exit(EXIT_FAILURE);
    }
    if (close(leftPipe[0]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(leftPipe[1]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(rightPipe[0]) != 0) fprintf(stderr, "Error closing pipes!\n");
    if (close(rightPipe[1]) != 0) fprintf(stderr, "Error closing pipes!\n");

    free(points);

    exit(EXIT_SUCCESS);
}

bool findClosestPair(Point *points, const size_t *n, int leftPipe[2], int rightPipe[2]) {
    size_t numberOfElements = *n;

    switch (numberOfElements) {
        case 0:
            free(points);
            return false;
            break;
        case 1:
            free(points);
            return true;
            break;
        case 2: {
            struct Pair pair;
            pair.p1.x = points[0].x;
            pair.p1.y = points[0].y;
            pair.p2.x = points[1].x;
            pair.p2.y = points[1].y;
            printPair(pair);
            free(points);
            return true;
            break;
        }
        default:
            break;
    }

    struct Process processLeft;
    struct Process processRight;
    initProcess(&processLeft);
    initProcess(&processRight);


    if (pipe(leftPipe) == -1 || pipe(rightPipe) == -1) {
        fprintf(stderr, "[%s] Error creating pipe!", programName);
        return false;
    }


    switch (processLeft.pid = fork()) {
        case -1:
            fprintf(stderr, "Cannot fork!\n");
            return false;
        case 0: //We are now in the first child element
            if (dup2(processLeft.readPipe[1], STDOUT_FILENO) == -1 ||
                dup2(processLeft.writePipe[0], STDIN_FILENO) == -1) {

                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            cleanupProcess(&processLeft);
            cleanupProcess(&processRight);

            execlp(programName, programName, NULL);


            //We shouldn't get here
            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            free(points);
            return false;
        default:
            switch (processRight.pid = fork()) {
                case -1:
                    fprintf(stderr, "[%s]Cannot fork!", programName);
                    return false;
                case 0:
                    if (dup2(processRight.readPipe[1], STDOUT_FILENO) == -1 ||
                        dup2(processRight.writePipe[0], STDIN_FILENO) == -1) {
                        fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                        return false;
                    }

                    cleanupProcess(&processLeft);
                    cleanupProcess(&processRight);

                    //dup2 (r&w) here
                    execlp(programName, programName, NULL);

                    fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
                    free(points);
                    return false;
                default:
                    break;
            }
            break;
    }

    // TODO: close unused pipe ends by parent process
    // TODO: add leftWrite and rightWrite

    FILE *leftRead = fdopen(processLeft.readPipe[0], "r");
    FILE *rightRead = fdopen(processRight.readPipe[0], "r");

    float mean;
    float sum = 0;
    for (int i = 0; i < numberOfElements; ++i) {
        sum += points[i].x;
    }
    mean = sum / (float) numberOfElements;

    for (int i = 0; i < numberOfElements; ++i) {
        if (points[i].x <= mean) {
            printPointToFile(leftRead, points[i]);
        }
        else {
            printPointToFile(rightRead, points[i]);
        }
    }







    //TODO: Fix if no value is present
    //mit getLine lesen und parsen. Es sind IMMER 2 Punkte vorhanden
    Pair pair1 = readPair(2, leftRead);
    Pair pair2 = readPair(2, rightRead);
    Pair pair3;

     // What?
//    //check if no value has been returned. <--
//    if (pair1.p1.x != FLT_MAX && pair2.p1.x != FLT_MAX) pair3 = newPairFromTwoPairs(pair1, pair2);
//    if (pair1.p1.x == FLT_MAX && pair2.p1.x == FLT_MAX) pair3 = newPair(smaller[0], bigger[0]);
//    if (pair1.p1.x == FLT_MAX) pair3 = newPairFromOnePairAndOnePoint(pair2, smaller[0]);
//    if (pair2.p1.x == FLT_MAX) pair3 = newPairFromOnePairAndOnePoint(pair1, bigger[0]);


    //calculate nearest pair and print it:
    printPair(nearestPair(pair1, pair2, pair3));

    //close pipes


//    printPair(calculateNearestPointsBruteForce(points, numberOfElements));


    //TODO: Close all pipes all the time!
    //TODO: keine exit() beutzen. Nur in main(); Sonst is es sehr einfach zu vergessen die pipes zu schlißen
    //^-->  Methode von void in boolean umwandeln und dann alle pipes schließen. bzw eine Funktion erstellen.

    return true;
}


