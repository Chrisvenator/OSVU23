#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

#define READ_END 0
#define WRITE_END 1

/*
 * Implement a C program that copies the contents of one file to another using parent and child processes.
 * Utilize fork to create a child process that performs the file copying.
 * Requirements:
 *
 * The program should take two command-line arguments: the source file name and the destination file name.
 * Use fork to create a child process.
 * In the child process, open the source file for reading.
 * In the parent process, open the destination file for writing.
 * Use appropriate system calls (e.g., open, read, write, close) to copy the contents of the source file to the destination file.
 * Ensure proper error handling for file operations.
 * The parent process should wait for the child process to complete before terminating.
 * Hints:
 *
 * You can use open, read, write, and close system calls for file operations. Check their man pages (man 2 open, etc.) for details.
 * Don't forget to handle errors appropriately, especially when opening files or creating the child process.
 * Example:
 *
 * bash
 * Copy code
 * ./copy_program source.txt destination.txt
 * Note: This task involves basic file I/O and process creation, similar to the previous task you provided. It allows you to practice using fork to create a child process and coordinating file operations between parent and child processes.
 */
/******************************************************************************
 * Function declarations
 *****************************************************************************/
void get_arguments(int argc, char *argv[], char **source, char **dest);


void rip(int fd[2], char *msg) {
    fprintf(stderr, "Something went wrong... %s\n", msg);
    close(fd[0]);
    close(fd[1]);
    exit(EXIT_FAILURE);
}

/******************************************************************************/

int main(int argc, char *argv[]) {
    char *source;
    char *dest;
    get_arguments(argc, argv, &source, &dest);
    printf("source:%s\ndest:%s\n", source, dest);

    int fd[2];
    if (pipe(fd) < 0) rip(fd, "pipe");


    pid_t pid = fork();
    if (pid <= -1) rip(fd, "fork");
    if (pid == 0) {
        if (dup2(fd[WRITE_END], STDOUT_FILENO) < 0) rip(fd, "dup2");

        size_t len = 0;
        ssize_t nread;

        FILE *stream;
        char *line = NULL;
        stream = fopen(source, "r");
        if (stream == NULL) {
            fclose(stream);
            rip(fd, "fopen");
        }


        while ((nread = getline(&line, &len, stream)) != -1) {
//            fprintf(stderr, "Retrieved line of length %zd:\n", nread);
            fwrite(line, nread, 1, stdout);
        }

        free(line);
        fclose(stream);
        close(fd[0]);
        close(fd[1]);

        exit(EXIT_SUCCESS);
    }

    int status = EXIT_SUCCESS;
    int waitStat = waitpid(pid, &status, 0);
    if (waitStat <= -1 || status != EXIT_SUCCESS) rip(fd, "waitpid");

    char result[10000] = "";
    read(fd[READ_END], result, 10000);

    FILE *writeFile = fopen(dest, "w");
    fwrite(result, strlen(result), 1, writeFile);

    fclose(writeFile);
    close(fd[0]);
    close(fd[1]);
    return 0;
}

void get_arguments(int argc, char *argv[], char **source, char **dest) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    *source = argv[1];
    *dest = argv[2];
}