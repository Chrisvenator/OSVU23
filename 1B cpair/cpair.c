#define _POSIX_C_SOURCE 200809L \

#include "cpair.h"

char *programName;

void usage(void) {
    fprintf(stderr, "USAGE: %s", programName);
    exit(EXIT_FAILURE);
}

/**
 * @brief Function to compute the Euclidean distance between two points
 * @param p1
 * @param p2
 * @return
 */
double distance(struct Point p1, struct Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrtf(dx * dx + dy * dy);
}


Point getCoordinates(char *string) {
    int amountOfSpaces = 0;
    for (int i = 0; i < strlen(string); ++i) {
        if (string[i] == ' ') amountOfSpaces++;
    }
    if (amountOfSpaces != 1){
        fprintf(stderr, "[%s] ERROR: There must only be 2 coordinates. Not more, not less!\n", programName);
        exit(EXIT_FAILURE);
    }



    float x;
    float y;

    char *split;
    char splitAt[] = " ";
    split = strtok(string, splitAt);

    if (is_float(split) == 1){
        x = strtof(split, NULL);
    } else {
        fprintf(stderr, "[%s] ERROR: The first coordinate is not a float!\n", programName);
        exit(EXIT_FAILURE);
    }

    split = strtok(string, splitAt);
    if (is_float(split) == 1){
        y = strtof(split, NULL);
    } else {
        fprintf(stderr, "[%s] ERROR: The first coordinate is not a float!\n", programName);
        exit(EXIT_FAILURE);
    }


    Point point;
    point.x = x;
    point.y = y;

    return point;
}



int main(int argc, char *argv[]) {
    programName = argv[0];

    //If arguments have been passed, then throw an error
    if (argc != 1) usage();

    //TODO: Actual code here

    //read from stdin
    char *line = NULL;
    size_t size = 0;
    FILE *input = fopen("./1B cpair/stdin.txt", "r");

    //TODO: fix this
    //TODO: replace with stdin
    while (getline(&line, &size, input) >= 0) {
        Point p = getCoordinates(line);
        printf("Point:\nx:%f\ny:%f",p.x,p.y);
    }




    //Output:
//    fprintf(stdout, "%.3f %.3f\n%.3f %.3f", x1, y1, x2, y2);
    exit(EXIT_SUCCESS);
}

//Command "cat" on Windows: "tpye .\stdin.txt"