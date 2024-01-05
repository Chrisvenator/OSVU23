#include "client_helper_functions.c"
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <string.h>

static void performHttpGet(arguments args);

int main(int argc, char *argv[]) {
    arguments args = parse_arguments(argc, argv);
    performHttpGet(args);
}

static void performHttpGet(arguments args) {
    struct addrinfo hints, *res;
    int sockfd;


    // Setting up hints structure for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Resolve the domain name into a list of addresses
    if (getaddrinfo(args.hostname, (char *) (args.port), &hints, &res) != 0) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        //With perror, the errormessage AND the cause gets thrown automatically
        perror("getaddrinfo failed\n");
        free(args.hostname);
        if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    // Create a socket and connect
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        perror("socket creation failed\n");
        freeaddrinfo(res);
        free(args.hostname);
        if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        perror("connect failed\n");
        close(sockfd);
        freeaddrinfo(res);
        free(args.hostname);
        if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    // Convert socket to file pointer for easier handling
    FILE *sockfile = fdopen(sockfd, "r+");
    if (sockfile == NULL) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        perror("fdopen failed\n");
        close(sockfd);
        free(args.hostname);
        if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    // Send the HTTP GET request
    fprintf(sockfile, "GET %s HTTP/1.1\r\n", args.resource);
    fprintf(sockfile, "Host: %s\r\n", args.hostname);
    fprintf(sockfile, "Connection: close\r\n\r\n");
    fflush(sockfile);
    free(args.hostname);
    free(args.resource);



    // Skip header fields
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), sockfile) != NULL) {
        //check if the response was not a success
        int response = parseHttpResponseStatus(buffer);
        if (response != 0) {
            fclose(sockfile);
            close(sockfd);
            freeaddrinfo(res);
            if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);

            exit(response);
        }

        // Check for empty line (end of headers)
        if (strcmp(buffer, "\r\n") == 0) {
            break;
        }
    }

//    if (args.file != NULL) {
//        if (access(args.file, R_OK) != 0 || access(args.file, W_OK) != 0) {
//            fprintf(stderr, "[%s] ERROR: Opening file: %s. %s\n", PROGRAM_NAME, args.file, strerror(errno));
//            fclose(sockfile);
//            close(sockfd);
//            freeaddrinfo(res);
//
//            exit(EXIT_FAILURE);
//        }
//    }

    // Read the response body
    FILE *outputFile = args.file == NULL ? stdout : fopen(args.file, "w");



    //This handles the File pointer. The errormessage "Error opening output file: Permission denied" is being thrown by perror.
    if (outputFile == NULL) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        perror("Error opening output file");
        //TODO: close everything here
        fclose(sockfile);
        close(sockfd);
        freeaddrinfo(res);

        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), sockfile) != NULL) {
        fputs(buffer, outputFile);
    }

    // Clean up
    if (args.file != NULL) {
        fclose(outputFile);
    }
    fclose(sockfile);
    close(sockfd);
    if (args.dir != NULL && strcmp(args.dir, args.file) == 0) free(args.file);

    freeaddrinfo(res);
}
