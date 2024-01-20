//
// Created by junioradmin on 29.11.23.
//

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
       #include <sys/types.h>
       #include <sys/wait.h>



char *program_name = "<not yet set>";

void usage(const char *message);

int main(int argc, char **argv){
    /* set program_name */
    if (argc > 0){
        program_name = argv[0];
    }



    size_t capacity = 200;
    char * input = malloc(capacity * sizeof(char));

    if (input == NULL){
        free(input);
        perror("input");
        exit(EXIT_FAILURE);
    }

    char * line = NULL;
    size_t size = 0;
    size_t i = 0;

    while (getline(&line, &size, stdin) != -1){
        while (size >= capacity){
            capacity = capacity * 2;
            char * tmp = realloc(input, capacity);
            if (tmp == NULL){
                perror("realloc failed!");
                free(line);
                free(input);
                tmp = NULL;
                exit(EXIT_FAILURE);
            }
        }

        strcat(input, line);

        i++;
    }
    free(line);
    
    fprintf(stdout, "input: %s\n", input);

    //Abbruchbedingung:
    if (strlen(input) < 2){
        free(input);
        exit(EXIT_SUCCESS);
    }
    
    int readPipe [2];
    int writePipe [2];

    if (pipe(readPipe) == -1 || pipe(writePipe) == -1){
        perror("faild to create pipe");
        free(input);
        exit(EXIT_FAILURE);
    }
    

    pid_t pid = fork();
    
    switch (pid){
    case -1:
        perror("fork failed");
        free(input);
        exit(EXIT_FAILURE);
        break;
    case 0:
        if(dup2(readPipe[1], STDOUT_FILENO) == -1 || dup2(writePipe[0], STDIN_FILENO) == -1){
            perror("failed to dup2");
            free(input);
            exit(EXIT_FAILURE);
        }        

        close(readPipe[0]);
        close(readPipe[1]);
        close(writePipe[0]);
        close(writePipe[1]);
        free(input);  

        execlp(program_name, program_name, NULL);      
        assert(0);
        break;
    default:
        break;
    }

    bool is_same = true;

    size_t amount = strlen(input);
    if (input[0] == input[amount-1]) is_same = false;
    if (is_same == false){
        exit(-1);
    }
    

    FILE * write = fdopen(writePipe[1], "w");
    FILE * read = fdopen(readPipe[0], "r");

    if (write == NULL || read == NULL){
        perror("error creating File reader/writer");
        free(input);
        close(readPipe[0]);
        close(readPipe[1]);
        close(writePipe[0]);
        close(writePipe[1]);
        fclose(read);
        fclose(write);
    }

    char* childIn = malloc((amount-1) * sizeof(char));
    strncpy(childIn, input + 1, amount - 2);

    fprintf(write, "%s", childIn);
    fprintf(stderr, "%s - %d - %d\n", childIn, amount, is_same);


    fflush(write);
    fclose(write);
    close(writePipe[0]);

    int status = EXIT_SUCCESS;
    if(waitpid(pid, &status, 0) == -1){
        fclose(read);
        free(input);
        exit(0);
    }

    if(status == -1) is_same = false;

    fclose(read);



    free(input);
    exit(is_same ? 0 : -1);
}

void usage(const char *message){
    fprintf(stderr, "%s\n", message);
    fprintf(stderr,
            "%s: Usage: ja mann \n",
            program_name);
    exit(EXIT_FAILURE);
}