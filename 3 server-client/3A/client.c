/**
 * @author Christopher Scherling
 * @file client.c
 * @date 01.11.2023
 *
 *
 * @brief Main client file for the OSVU23 project.
 *
 * @details This file is the core of the client-side application for the OSVU23 project.
 * It contains the main function, which is the entry point of the program,
 * and the performHttpGet function, which is responsible for executing HTTP GET requests.
 * The main function parses command-line arguments using the helper functions defined in
 * client_helper_functions.c and then proceeds to initiate the HTTP request process.
 * The performHttpGet function takes care of establishing network connections,
 * sending the HTTP request, receiving the response, and handling the output,
 * either by saving it to a file or displaying it on the console.
 *
 * @note This file works in conjunction with client_helper_functions.c,
 * which provides necessary utility functions.
 * Proper memory management is essential, especially in handling dynamically allocated
 * resources within performHttpGet and the functions it calls from client_helper_functions.c.
 **/


#include "client_helper_functions.c"

static void performHttpGet(arguments args);

/**
 * @brief Entry point of the program, sets up and executes HTTP GET requests.
 * @details This function serves as the main entry point.
 * It parses command line arguments, sets up necessary structures for network communication,
 * and initiates the HTTP GET request process.
 * @param argc The number of commandline arguments.
 * @param argv The array of commandline argument strings.
 * @return The exit status of the program.
 */
int main(int argc, char *argv[]) {
    arguments args = parse_arguments(argc, argv);
    performHttpGet(args);

    exit(EXIT_SUCCESS);
}

/**
 * @brief Performs an HTTP GET request based on the provided arguments.
 * @details Establishes a socket connection to the specified host and
 * sends an HTTP GET request for the desired resource.
 * It handles the reception of the response and saves the data to a file or standard output as specified.
 * @param args The arguments structure containing details for the HTTP request.
 */
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


    int i = 0;
    // Skip header fields
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), sockfile) != NULL) {
        //check if the response was not a success
        int response = parseHttpResponseStatus(buffer);
        if (response != 0 && i == 0) {
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

        i++;
    }


    FILE *outputFile = args.file == NULL ? stdout : fopen(args.file, "w");

    //This handles the File pointer. The errormessage "Error opening output file: Permission denied" is being thrown by perror.
    if (outputFile == NULL) {
        fprintf(stderr, "[%s] ", PROGRAM_NAME);
        perror("Error opening output file");
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
