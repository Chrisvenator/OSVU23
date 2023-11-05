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

    Point *points = loadData();
    if (checkIfAllCoordinatesAreTheSame(points) == true) {
        free(points);
        exit(EXIT_SUCCESS);
    }

    findClosestPair(points, numberOfElements);

    free(points);
    exit(EXIT_SUCCESS);
}

void findClosestPair(Point *points, size_t n) {
    // Base cases
    if (n <= 1) exit(EXIT_FAILURE);
    else if (n == 2) {
        Pair pair;
        pair.p1.x = points[0].x;
        pair.p1.y = points[0].y;
        pair.p2.x = points[1].x;
        pair.p2.y = points[1].y;

        printPair(pair);
        exit(EXIT_SUCCESS);
    }
    // Calculate mean x-coordinate, xm

    Point *smaller; //Values smaller than mean
    Point *bigger; //Values bigger than mean
    double mean;
    size_t index;

    if (checkIfAllCoordinatesAreTheSame(points) == true) {
        Point p0 = points[0];
        Pair p;
        p.p1 = p0;
        p.p2 = p0;
        printPair(p);
        exit(EXIT_SUCCESS);
    } else if (checkIfAllXValuesAreTheSame(points) == true) {
        mean = calculateArithmeticMean(points, 'y');
        qsort(points, (size_t) numberOfElements, sizeof(Point), compareY);
        index = getIndexOfMean(points, mean, numberOfElements, 'y');
    } else {
        mean = calculateArithmeticMean(points, 'x');
        qsort(points, (size_t) numberOfElements, sizeof(Point), compareX);
        index = getIndexOfMean(points, mean, numberOfElements, 'x');
    }

    // /////////////////////// //
    // Divide points around xm //
    // /////////////////////// //

    //Check if the index, where the mean is located, is an odd number. If yes, we need to include the mean as well or else it will be cut out
    index = index % 2 == 0 ? index : index + 1;

    smaller = dividePoints(points, 0, index); //Size of smaller: index
    bigger = dividePoints(points, index, numberOfElements); //Size of bigger: numberOfElements - index



    //These vars will be used later:
//    size_t leftCount = index;
//    size_t rightCount = numberOfElements - index;


//    Pair pair1 = newPair(smaller[0], smaller[1]);
//    Pair pair2 = newPair(bigger[0], bigger[1]);
//    Pair pair3 = newPairFromTwoPairs(pair1, pair2);

    //Print the nearest pair into stdout:
//    printPair(nearestPair(pair1, pair2, pair3));
    printPair(calculateNearestPointsBruteForce(points, numberOfElements));


    free(smaller);
    free(bigger);
    //TODO: Close all pipes all the time!
}


