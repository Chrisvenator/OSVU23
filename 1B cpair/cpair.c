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
 * @author Christopher Scherling 12119060 :)
 */
int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc != 1) usage(); //If the wrong number of arguments have been passed, throw an error

    size_t myNumberOfElements;
    size_t *ptr_numberOfElements = &myNumberOfElements;

    Point *points = loadData(ptr_numberOfElements);
//    fprintf(stderr, "\n");
//
//
//    fprintf(stderr, "All points:\n");
//    for (int i = 0; i < 4; ++i) {
//        printPointToFile(stderr, &points[i]);
//    }


    int leftPipe[2];
    int rightPipe[2];


    //TODO: THIS:
    if (findClosestPair(points, &myNumberOfElements, leftPipe, rightPipe) == false) {
//        fprintf(stderr, "failed\n");
        exit(EXIT_FAILURE);
    }

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
            Pair p = newPair(points[0], points[1]);
            printPair(stdout, p);
            free(points);
            return true;
            break;
        }
        default:
            break;
    }

    if (checkIfAllCoordinatesAreTheSame(points, numberOfElements) == true) {
        Pair p;
        p.p1 = points[0];
        p.p2 = points[0];
        p.dist = 0;

        printPair(stdout, p);
        free(points);
        return true;
    }

    Process processLeft;
    Process processRight;
    initProcess(&processLeft);
    initProcess(&processRight);

    //TODO: add checks for cases when pipes failed to open

    if (pipe(leftPipe) == -1 || pipe(rightPipe) == -1) {
        fprintf(stderr, "[%s] Error creating pipe!\n", programName);
        return false;
    }

    switch (processLeft.pid = fork()) {
        case -1:
            fprintf(stderr, "Cannot fork!\n");
            return false;
            break;
        case 0: //We are now in the first child element
            if (dup2(processLeft.readPipe[1], STDOUT_FILENO) == -1 ||
                dup2(processLeft.writePipe[0], STDIN_FILENO) == -1) {

                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            cleanupProcess(&processLeft);
            cleanupProcess(&processRight);

            execlp(programName, programName, NULL);

            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            free(points);
            return false;
            break;
        default:
            break;
    }

    switch (processRight.pid = fork()) {
        case -1:
            fprintf(stderr, "Cannot fork!\n");
            return false;
            break;
        case 0: //We are now in the first child element
            if (dup2(processRight.readPipe[1], STDOUT_FILENO) == -1 ||
                dup2(processRight.writePipe[0], STDIN_FILENO) == -1) {

                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            cleanupProcess(&processLeft);
            cleanupProcess(&processRight);

            execlp(programName, programName, NULL);

            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            free(points);
            return false;
            break;
        default:
            break;
    }

    // TODO: close unused pipe ends by parent process
    FILE *leftWrite = fdopen(processLeft.writePipe[1], "w");
    FILE *rightWrite = fdopen(processRight.writePipe[1], "w");

    FILE *leftRead = fdopen(processLeft.readPipe[0], "r");
    FILE *rightRead = fdopen(processRight.readPipe[0], "r");

    checkFile(leftWrite, "Error opening leftWrite");
    checkFile(rightWrite, "Error opening rightWrite");
    checkFile(leftRead, "Error opening leftRead");
    checkFile(rightRead, "Error opening rightRead");

    //Ivan's mean:
//    float mean;
//    float sum = 0;
//    for (int i = 0; i < numberOfElements; ++i) {
//        sum += points[i].x;
//    }
//    mean = sum / (float) numberOfElements;
//    fprintf(stderr, "mean: %f\n", mean); //TODO: calculate mean for x AND y

    //Check if all x-coordinates have the same Value. If yes, then use y-coordinates to proceed
    char useXorY = checkIfAllXValuesAreTheSame(points, numberOfElements) == true ? 'y' : 'x';

    qsort(points, (size_t) numberOfElements, sizeof(Point), useXorY == 'x' ? compareX : compareY);
    double mean = calculateArithmeticMean(points, useXorY, numberOfElements);
    size_t index = getIndexOfMean(points, mean, numberOfElements, useXorY);


//    fprintf(stderr, "Mean: %.3f\n", mean);
//    fprintf(stderr, "left size: %zu; right size: %zu\n", index, numberOfElements - index);


    Point *smaller = dividePoints(points, 0, index, numberOfElements);
    Point *bigger = dividePoints(points, index, numberOfElements, numberOfElements);

//    fprintf(stderr, "Smaller: \n");
//    printPointPointer(stderr, smaller, index);
//    fprintf(stderr, "Bigger: \n");
//    printPointPointer(stderr, bigger, numberOfElements - index);
    printPointPointer(leftWrite, smaller, index);
    printPointPointer(rightWrite, bigger, numberOfElements - index);
//    printPointPointer(leftWrite, smaller, numberOfElements - index);
//    printPointPointer(rightWrite, bigger, index);


    //Ivan's print to child:
//    for (int i = 0; i < numberOfElements; ++i) {
//        if (points[i].x <= mean) {
//            printPointToFile(leftWrite, &points[i]);
//        }
//    }
//    for (int i = 0; i < numberOfElements; ++i) {
//        if (points[i].x > mean) {
//            printPointToFile(rightWrite, &points[i]);
//        }
//    }


    fflush(leftWrite);
    fflush(rightWrite);
    fclose(leftWrite);
    fclose(rightWrite);
    close(processLeft.writePipe[0]);
    close(processRight.writePipe[0]);


    int status;
    waitpid(processLeft.pid, &status, 0);
    waitpid(processRight.pid, &status, 0);

    close(processLeft.readPipe[1]);
    close(processRight.readPipe[1]);

    if (status != EXIT_SUCCESS) {
        printf("dead child\n");
        return false;
    }


    Pair pair1, pair2;
    ssize_t leftReadAmount = readPair(leftRead, &pair1);
    ssize_t rightReadAmount = readPair(rightRead, &pair2);


    Pair pair3;

//    fprintf(stderr, "Left amount: %zu, right amount: %zu\n", leftReadAmount, rightReadAmount);
//    fprintf(stderr, "Pair 1: %f\n", pair1.dist);
//    printPair(pair1);
//    fprintf(stderr, "Pair 2: %f\n", pair2.dist);
//    printPair(pair2);

    if (leftReadAmount == (size_t) -2 || rightReadAmount == (size_t) -2) assert(0);

    Pair pairNearest;
    if (leftReadAmount != (size_t) -1 && rightReadAmount != (size_t) -1) {
        pair3 = newPairFromTwoPairs(pair1, pair2);
        pairNearest = nearestPair(pair1, pair2, pair3);
    } else if (leftReadAmount != (size_t) -1 && rightReadAmount == (size_t) -1) pairNearest = newPairFromOnePairAndOnePoint(pair1, bigger[0]);
    else if (leftReadAmount == (size_t) -1 && rightReadAmount != (size_t) -1) pairNearest = newPairFromOnePairAndOnePoint(pair2, smaller[0]);
    else
        assert(0); //TODO: close everything when coming here



//    fprintf(stderr, "\nNearest Pair:\n");
    printPair(stdout, pairNearest);

    //TODO: Close all pipes all the time!
    //TODO: keine exit() beutzen. Nur in main(); Sonst is es sehr einfach zu vergessen die pipes zu schlißen
    //^-->  Methode von void in boolean umwandeln und dann alle pipes schließen. bzw eine Funktion erstellen.


    return true;
}
