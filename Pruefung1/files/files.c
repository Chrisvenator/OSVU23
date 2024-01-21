/******************************************************************************
 * Prüfungsaufgabe: Manipulation von Textdateien
 *
 * Die Aufgabe besteht darin, eine Textdatei zu lesen, ihren Inhalt zu bearbeiten
 * und das Ergebnis in eine neue Datei zu schreiben.
 *
 * Task 1: Lesen Sie den Inhalt einer Textdatei.
 * Task 2: Konvertieren Sie alle Buchstaben zu Großbuchstaben.
 * Task 3: Schreiben Sie den bearbeiteten Inhalt in eine neue Datei.
 *
 * Siehe auch: fopen(3), fgetc(3), fputc(3), fclose(3), toupper(3)
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <math.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include "signal.h"
#include <signal.h>
#include <bits/sigaction.h>
#include <semaphore.h>
#include <time.h>

#define MAX_TEXTLEN 1024

// Task declarations
void task_1(const char *input_filename, char text[MAX_TEXTLEN]);

void task_2(char text[MAX_TEXTLEN]);

void task_3(const char *output_filename, const char text[MAX_TEXTLEN]);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <inputfile> <outputfile>\n", argv[0]);
        return 1;
    }

    bool i_set = false;
    bool o_set = false;

    char *infile;
    char *outfile;


    int opt;
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'o':
                if (o_set == true) exit(EXIT_FAILURE);
                o_set = true;
                outfile = optarg;
                break;
            case 'i':
                if (i_set == true) exit(EXIT_FAILURE);
                i_set = true;
                infile = optarg;
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    char text[MAX_TEXTLEN];

    // Task 1: Lesen Sie den Inhalt der Datei.
    task_1(infile, text);
    printf("File content: %s\n", text);

    // Task 2: Konvertieren Sie den Text zu Großbuchstaben.
    task_2(text);

    // Task 3: Schreiben Sie den bearbeiteten Text in eine neue Datei.
    task_3(outfile, text);

    return 0;
}

/******************************************************************************
 * Task 1: Lesen des Dateiinhalts.
 *****************************************************************************/
void task_1(const char *input_filename, char text[MAX_TEXTLEN]) {
    // Implement your solution here
    FILE *infile = fopen(input_filename, "r");

    if (infile == NULL) exit(EXIT_FAILURE);

    char c;

    for (int i = 0; i < MAX_TEXTLEN - 1; ++i) {
        c = getc(infile);
        if (c == EOF || c < 0) {
            text[i] = '\0';
            break;
        }
        text[i] = c;
    }

    if (fclose(infile) != 0) exit(EXIT_FAILURE);
}

/******************************************************************************
 * Task 2: Konvertieren des Textes zu Großbuchstaben.
 *****************************************************************************/
void task_2(char text[MAX_TEXTLEN]) {
    for (int i = 0; i < MAX_TEXTLEN; ++i) {
        text[i] = toupper(text[i]);
    }
}

/******************************************************************************
 * Task 3: Schreiben des bearbeiteten Textes in eine neue Datei.
 *****************************************************************************/
void task_3(const char *output_filename, const char text[MAX_TEXTLEN]) {
    FILE *outfile = fopen(output_filename, "w+");
    if (outfile == NULL) exit(EXIT_FAILURE);

    fprintf(outfile, "%s", text);
    if (fclose(outfile) < 0) exit(EXIT_FAILURE);
}