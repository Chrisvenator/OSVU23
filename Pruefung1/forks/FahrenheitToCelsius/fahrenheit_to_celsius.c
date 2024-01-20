//
// Created by junioradmin on 28.11.23.
//

#include "getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static void usage(char *name) {
    fprintf(stderr, "[%s]: Usage: fahrenheit -c <temp> | -f <temp>\n", name);
}

static float FtoC(float temp){
	return (temp -32) *5 /9;
}

static float CtoF(float temp){
	return temp *9 /5 +32;
}

int main(int argc, char **argv) {
    if (argc != 3) usage(argv[0]);

    int c = 0;
    int f = 0;
    const char *temperature = NULL;


    int option;
    while ((option = getopt(argc, argv, "c:f:")) != -1) {
        switch (option) {
            case 'c':
                if (c != 0 || f != 0) usage(argv[0]);
                c = 1;
                temperature = optarg;
                break;
            case 'f':
                if (c != 0 || f != 0) usage(argv[0]);
                f = 1;
                temperature = optarg;
                break;
            default: /* '?' */
                usage(argv[0]);
		break;
        }
    }

    
    printf("Temperature argument = %s\n", temperature);

    float temp = atof(temperature);
    if (f == 1) temp = FtoC(temp);
    if (c == 1) temp = CtoF(temp);
    

    fprintf(stdout, "[%s]: %s is %.3f", argv[0], temperature, temp);

    /* Other code omitted */

    exit(EXIT_SUCCESS);
}
