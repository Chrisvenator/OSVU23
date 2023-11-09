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


//    fprintf(stderr, "All points:\n");
//    for (int i = 0; i < 4; ++i) {
//        printPointToFile(stderr, &points[i]);
//    }


    int leftPipe[2];
    int rightPipe[2];


    //TODO: THIS:
    if (findClosestPair(points, &myNumberOfElements, leftPipe, rightPipe) == false) {
        printf("failed\n");
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
            printPointToFile(stdout, &points[0]);
            printPointToFile(stdout, &points[1]);
            free(points);
            return true;
            break;
        }
        default:
            break;
    }

    Process processLeft;
    Process processRight;
    initProcess(&processLeft);
    initProcess(&processRight);

    //TODO: add checks for cases when pipes failed to open

    if (pipe(leftPipe) == -1 || pipe(rightPipe) == -1) {
        fprintf(stderr, "[%s] Error creating pipe!", programName);
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

    float mean;
    float sum = 0;
    for (int i = 0; i < numberOfElements; ++i) {
        sum += points[i].x;
    }
    mean = sum / (float) numberOfElements;
//    fprintf(stderr, "mean: %f\n", mean);



    for (int i = 0; i < numberOfElements; ++i) {
        if (points[i].x <= mean) {
            printPointToFile(leftWrite, &points[i]);
        }
    }

    for (int i = 0; i < numberOfElements; ++i) {
        if (points[i].x > mean) {
            printPointToFile(rightWrite, &points[i]);
        }
    }


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

//    printf("Left amount: %zu, right amount: %zu\n", leftReadAmount, rightReadAmount);

//    printf("Pair 1: %f\n", pair1.dist);
//    printPair(pair1);
//    printf("Pair 2: %f\n", pair2.dist);
//    printPair(pair2);


    if (leftReadAmount != (size_t) -1 && rightReadAmount != (size_t) -1) pair3 = newPairFromTwoPairs(pair1, pair2);
    else if (leftReadAmount != (size_t) -1 && rightReadAmount == (size_t) -1) pair3 = newPairFromOnePairAndOnePoint(pair1, pair2.p1);
    else if (leftReadAmount == (size_t) -1 && rightReadAmount != (size_t) -1) pair3 = newPairFromOnePairAndOnePoint(pair2, pair1.p1);
    else
        assert(0);

//    printf("Pair 3: %f\n", pair3.dist);
//    printPair(pair3);


    Pair pairNearest = nearestPair(pair1, pair2, pair3);
//    printf("Nearest Pair:\n");
    printPair(pairNearest);

    //TODO: Close all pipes all the time!
    //TODO: keine exit() beutzen. Nur in main(); Sonst is es sehr einfach zu vergessen die pipes zu schlißen
    //^-->  Methode von void in boolean umwandeln und dann alle pipes schließen. bzw eine Funktion erstellen.


    return true;
}
