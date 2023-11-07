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
    if (n <= 1) exit(EXIT_SUCCESS);
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


    int leftPipe[2];
    int rightPipe[2];

    if(pipe(leftPipe) == -1 || pipe(rightPipe) == -1){
	    fprintf(stderr, "[%s] Error creating pipe!", programName);
	    exit(EXIT_FAILURE);
    }

    pid_t leftChild, rightChild;
    switch (leftChild = fork()){
	    case -1:
		    fprintf(stderr, "Cannot fork!\n");
	            close(leftPipe[0]);
        	    close(leftPipe[1]);
	            close(rightPipe[0]);
        	    close(rightPipe[1]);
	            exit(EXIT_FAILURE);
		case 0: //We are now in the first child element
		     if (dup2(leftPipe[1], STDOUT_FILENO) == -1 || dup2(leftPipe[0], STDIN_FILENO) == -1) { //TODO: stimmt das?
			     fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n", programName, strerror(errno));
			     free(points); //TODO: close pipes
			     exit(EXIT_FAILURE);
		     }
		     close(rightPipe[0]);
		     close(rightPipe[1]);
		     close(leftPipe[1]); //Close left read pipe since we don't need that one
		     //clos all pipes?

		     //communicate with child
		     execlp(programName, programName, NULL);


		     //We shouldn't get here
		     fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
		     free(points);
		     assert(1);
		     exit(EXIT_FAILURE);
		default:
		     switch(rightChild = fork()){
				case -1:
				      fprintf(stderr, "[%s]Cannot fork!", programName);
				      close(leftPipe[0]);
				      close(leftPipe[1]);
				      close(rightPipe[0]);
				      close(rightPipe[1]);
				      exit(EXIT_FAILURE);
				case 0:
				      if (dup2(rightPipe[1], STDOUT_FILENO) == -1 || dup2(rightPipe[0], STDIN_FILENO) == -1) {
					      fprintf(stderr, "[%s] ERROR: Cannot dup2: %s\n",programName, strerror(errno));
					      free(points); //TODO: Close pipes
					      exit(EXIT_FAILURE);
				      }
				      close(leftPipe[0]);
				      close(leftPipe[1]);
				      close(rightPipe[1]); //Close left read pipe since we don't need that one

				      //communicate with child
				      //dup2 (r&w) here
				      execlp(programName, programName, NULL);

				      //We shouldn't get here
				      fprintf(stderr, "[%s] ERROR: Cannot execute: %s\n", programName, strerror(errno));
				      free(points);
				      assert(1);
				      exit(EXIT_FAILURE);				     			     
				default:
				      break;
		     }
		     break;
    }

    //HIER:
    //
    //in Prozesse schreiben (so wie lesen)
    FILE *leftWrite = fdopen(leftPipe[1], "w"); //Wenn w nicht funktioniert, dann a benuten
    FILE *rightWrite = fdopen(rightPipe[1], "w");

    //
    //waitpit
    //
    //prozesse lesen:
    FILE *leftRead = fdopen(leftPipe[0], "r");
    FILE *rightRead = fdopen(rightPipe[0], "r");

    //mit getLine lesen und parsen. Es sind IMMER 2 Punkte vorhanden
    //
    //calculate nearest pair
    //
    //nearest pair printen


    //close pipes


    printPair(calculateNearestPointsBruteForce(points, numberOfElements));


    free(smaller);
    free(bigger);
    //TODO: Close all pipes all the time!
    //TODO: keine exit() beutzen. Nur in main(); Sonst is es sehr einfach zu vergessen die pipes zu schlißen
    //^-->  Methode von void in boolean umwandeln und dann alle pipes schließen. bzw eine Funktion erstellen.
}


