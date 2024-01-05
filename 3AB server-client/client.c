#include "client_helper_functions.c"
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netdb.h>
//#include <string.h>

static void performHttpGet(arguments args);

int main(int argc, char *argv[]) {
    arguments args = parse_arguments(argc, argv);
    args.dir = "/";

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
//        perror("getaddrinfo failed\n");
        fprintf(stderr, "getaddrinfo failed\n");
        free(args.hostname);
        free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    // Create a socket and connect
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket creation failed\n");
        freeaddrinfo(res);
        free(args.hostname);
        free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect failed\n");
        close(sockfd);
        freeaddrinfo(res);
        free(args.hostname);
        free(args.file);
        free(args.resource);
        exit(EXIT_FAILURE);
    }

    // Convert socket to file pointer for easier handling
    FILE *sockfile = fdopen(sockfd, "r+");
    if (sockfile == NULL) {
        perror("fdopen failed\n");
        close(sockfd);
        free(args.hostname);
        free(args.file);
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
            //TODO: close everything here
            fclose(sockfile);
            close(sockfd);
            free(args.file);
            freeaddrinfo(res);

            exit(response);
        }

        // Check for empty line (end of headers)
        if (strcmp(buffer, "\r\n") == 0) {
            break;
        }
    }

    // Read the response body
    FILE *outputFile = args.file == NULL ? stdout : fopen(args.file, "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        //TODO: close everything here
        fclose(sockfile);
        close(sockfd);
        free(args.file);
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
    free(args.file);
    freeaddrinfo(res);
}
