/**
 * @file cpair.c
 * @brief A program which searches for the closest pair of points1 in a set of 2D-points.
 * @details The program takes an array of 2D points as input.
 *          The input is read from stdin. Each line consists of two floating point numbers,
 *          separated by whitespace, which represent the x and y coordinates of one point.
 * @author Christopher Scherling 12119060
 * @date 11.11.2023
 */

#include "cpair.h"
#include "helperFunctions.c" //TODO: ABSOLUTELY REMOVE THIS LINE!!!!!!

int main(int argc, char *argv[]) {
    programName = argv[0];
    if (argc != 1) usage();

    size_t myNumberOfElements; //Don't question this. Never touch a running system :)
    size_t *ptr_numberOfElements = &myNumberOfElements;

    Point *points = loadData(ptr_numberOfElements);


    if (findClosestPair(points, &myNumberOfElements) == false) {
        free(points);
        exit(EXIT_FAILURE);
    }

    free(points);
    exit(EXIT_SUCCESS);
}


static bool findClosestPair(Point *points, const size_t *numberOfElements) {
    switch (*numberOfElements) {
        case 0:
            return false;
            break;
        case 1:
            return true;
            break;
        case 2: {
            Pair p = newPair(points[0], points[1]);
            printPair(stdout, p);
            return true;
            break;
        }
        default:
            break;
    }

    if (checkIfAllCoordinatesAreTheSame(points, *numberOfElements) == true) {
        Pair p;
        p.p1 = points[0];
        p.p2 = points[0];
        p.dist = 0;

        printPair(stdout, p);
        return true;
    }

    Process processLeft;
    Process processRight;
    if (initProcess(&processLeft) == false) return false;
    if (initProcess(&processRight) == false) return false;


    // ///////////////////////////////// //
    //          fork program             //
    // ///////////////////////////////// //


    switch (processLeft.pid = fork()) {
        case -1:
            fprintf(stderr, "[%s]Cannot fork!\n", programName);
            return false;
            break;
        case 0: //We are now in the first child element
            if (dup2(processLeft.readPipe[1], STDOUT_FILENO) == -1 ||
                dup2(processLeft.writePipe[0], STDIN_FILENO) == -1) {

                cleanupProcess(&processRight);
                cleanupProcess(&processLeft);

                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            cleanupProcess(&processLeft);
            cleanupProcess(&processRight);

            execlp(programName, programName, NULL);

            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            return false;
            break;
        default:
            break;
    }

    switch (processRight.pid = fork()) {
        case -1:
            fprintf(stderr, "[%s]Cannot fork!\n", programName);
            return false;
            break;
        case 0: //We are now in the first child element
            if (dup2(processRight.readPipe[1], STDOUT_FILENO) == -1 ||
                dup2(processRight.writePipe[0], STDIN_FILENO) == -1) {

                cleanupProcess(&processRight);
                cleanupProcess(&processLeft);

                fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
                return false;
            }
            cleanupProcess(&processLeft);
            cleanupProcess(&processRight);

            execlp(programName, programName, NULL);

            fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
            return false;
            break;
        default:
            break;
    }



    // ////////////////////////////////////// //
    //   Open pipes to read and write from    //
    // ////////////////////////////////////// //

    int status = EXIT_SUCCESS;
    FILE *leftWrite = fdopen(processLeft.writePipe[1], "w");
    FILE *rightWrite = fdopen(processRight.writePipe[1], "w");

    FILE *leftRead = fdopen(processLeft.readPipe[0], "r");
    FILE *rightRead = fdopen(processRight.readPipe[0], "r");

    checkFile(leftWrite, &status, "Error opening leftWrite");
    checkFile(rightWrite, &status, "Error opening rightWrite");
    checkFile(leftRead, &status, "Error opening leftRead");
    checkFile(rightRead, &status, "Error opening rightRead");



    // /////////////////////////////////////////////////////////////////////// //
    // Calculate Mean, sort points, divide them and write them to the children //
    // /////////////////////////////////////////////////////////////////////// //

    //Check if all x-coordinates have the same Value. If yes, then use y-coordinates to proceed
    char useXorY = checkIfAllXValuesAreTheSame(points, *numberOfElements) == true ? 'y' : 'x';

    //sort, then calculate mean and index:
    qsort(points, (size_t) *numberOfElements, sizeof(Point), useXorY == 'x' ? compareX : compareY);
    double mean = calculateArithmeticMean(points, useXorY, *numberOfElements);
    size_t index = getIndexOfMean(points, mean, *numberOfElements, useXorY);


    Point *smaller = dividePoints(points, &status, 0, index); //All points that are smaller than mean
    Point *bigger = dividePoints(points, &status, index, *numberOfElements); //All points that are bigger than mean

    //If an error occurred, the free and close everything
    if (status != EXIT_SUCCESS) {
        fflush(leftWrite);
        fflush(rightWrite);
        fclose(leftWrite);
        fclose(rightWrite);
        cleanupProcess(&processRight);
        cleanupProcess(&processLeft);

        free(smaller);
        free(bigger);
        return false;
    }

    //give all points (smaller and bigger than mean) to their respective children
    printPointPointer(leftWrite, smaller, index);
    printPointPointer(rightWrite, bigger, *numberOfElements - index);



    // ////////////////////////////////////// //
    //      Close and flush unused pipes      //
    // ////////////////////////////////////// //

    fflush(leftWrite);
    fflush(rightWrite);
    fclose(leftWrite);
    fclose(rightWrite);
    close(processLeft.writePipe[0]);
    close(processRight.writePipe[0]);


    waitpid(processLeft.pid, &status, 0);
    waitpid(processRight.pid, &status, 0);

    close(processLeft.readPipe[1]);
    close(processRight.readPipe[1]);

    if (status != EXIT_SUCCESS) {
        fprintf(stderr, "[%s]dead child\n", programName);
        fclose(leftRead);
        fclose(rightRead);
        return false;
    }



    // ////////////////////////////////// //
    // Read the answers from the children //
    // ////////////////////////////////// //

    Pair pair1, pair2;
    ssize_t leftReadAmount = readPair(leftRead, &pair1);
    ssize_t rightReadAmount = readPair(rightRead, &pair2);

    fclose(leftRead);
    fclose(rightRead);




    // /////////////////////////////////// //
    // Validate the answers and compute P3 //
    // /////////////////////////////////// //

    Pair pair3;

    if (leftReadAmount == (size_t) -2 || rightReadAmount == (size_t) -2) return false; //Something went wrong while reading from the children

    Pair pairNearest;
    if (leftReadAmount != (size_t) -1 && rightReadAmount != (size_t) -1) { //Both children returned something = There was a pair for left and right
        pair3 = newPairFromTwoPairs(pair1, pair2);
        pairNearest = nearestPair(pair1, pair2, pair3);
    } else if (leftReadAmount != (size_t) -1 && rightReadAmount == (size_t) -1) pairNearest = newPairFromOnePairAndOnePoint(pair1, bigger[0]);//Only one child returned something = There was only a pair
    else if (leftReadAmount == (size_t) -1 && rightReadAmount != (size_t) -1) pairNearest = newPairFromOnePairAndOnePoint(pair2, smaller[0]);//Only one child returned something = There was only a pair
    else {//If there was no pair. This case should normally not be possible because 2 points shouldn't normally be split anymore
        fprintf(stderr, "[%s]A pair got split up", programName);
        free(smaller);
        free(bigger);
        free(points);
        assert(0);
    }




    // /////////////////////////////////////////////////////// //
    // Compute the rare case that the recursion is not perfect //
    //             and then print nearest pair                 //
    // /////////////////////////////////////////////////////// //


    pairNearest = closestPairIncludingMeanProblem(points, *numberOfElements, pairNearest, mean, 'x');
    printPair(stdout, pairNearest);

    free(smaller);
    free(bigger);

    //Everything (except points) SHOULD be closed or freed when coming here...
    return true;
}
