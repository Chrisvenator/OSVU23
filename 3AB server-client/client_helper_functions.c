//
// Created by junioradmin on 04.01.24.
//
#include "headers.h"

static void usage(void) {
    fprintf(stderr, "[%s]USAGE: client [-p PORT] [ -o FILE | -d DIR ] URL\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

struct arguments {
    int port;
    char *file;
    char *dir;
    char *url;
};

int parsePortNumber(const char *portStr) {
    char *endptr;
    errno = 0; // To distinguish success/failure after the call
    long port = strtol(portStr, &endptr, 10);

    // Check for various possible errors
    if ((errno == ERANGE && (port == LONG_MAX || port == LONG_MIN)) || (errno != 0 && port == 0)) {
        perror("strtol");
        usage();
    }

    if (endptr == portStr) {
        fprintf(stderr, "No digits were found\n");
        usage();
    }

    // If we got here, strtol() successfully parsed a number
    if (*endptr != '\0') { // In case there are any non-numeric characters
        fprintf(stderr, "Further characters after number: %s\n", endptr);
        usage();
    }

    // Check if port number is within the valid range (1-65535)
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Port number out of range\n");
        usage();
    }

    return (int) port;
}


static arguments parse_arguments(int argc, char *argv[]) {
    if (argc == 0) exit(EXIT_FAILURE);
    PROGRAM_NAME = argv[0];

    int opt;
    arguments args;
    args.port = DEFAULT_PORT;
    args.file = NULL;
    args.dir = NULL;
    args.url = NULL;

    bool p_set = false;
    bool o_set = false;
    bool d_set = false;

    // Using getopt to parse command-line arguments
    while ((opt = getopt(argc, argv, "p:o:d:")) != -1) {
        switch (opt) {
            case 'p':
                if (p_set == true) usage();
                p_set = true;
                args.port = parsePortNumber(optarg);
                break;

            case 'o':
                if (o_set || d_set) usage();
                o_set = true;
                args.file = optarg;
                break;

            case 'd':
                if (o_set || d_set) usage();
                d_set = true;
                args.dir = optarg;
                break;

            case '?': // Unrecognized option
                fprintf(stderr, "Usage: %s [-p PORT] [-o FILE | -d DIR] URL\n", argv[0]);
                usage();
            default: //unreachable option
                usage();
                assert(0);
        }
    }

    // Checking for mandatory URL argument
    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

    args.url = argv[optind];
    if (optind + 1 != argc) usage();

    // print the parsed arguments:
    printf("PORT: %d\n", args.port);
    printf("FILE: %s\n", args.file);
    printf("DIR: %s\n", args.dir);
    printf("URL: %s\n", args.url);

    return args;
}

static char *extract_hostname(arguments args) {
    // Copy the URL to a modifiable string
    char *urlCopy = strdup(args.url);
    if (urlCopy == NULL) {
        perror("strdup failed");
        return NULL;
    }

    char *hostname = NULL;
    char *schemePtr = strstr(urlCopy, "://");

    if (schemePtr != NULL) {
        // Move past the "://"
        hostname = schemePtr + 3;
    } else {
        // No scheme, assume the entire URL is the hostname
        hostname = urlCopy;
    }

    // Check if there's a path part and terminate the hostname there
    char *pathPtr = strchr(hostname, '/');
    if (pathPtr != NULL) {
        *pathPtr = '\0';
    }

    // Check for port and remove it
    char *portPtr = strchr(hostname, ':');
    if (portPtr != NULL) {
        *portPtr = '\0';
    }

    // Duplicate the hostname because we will free urlCopy
    char *result = strdup(hostname);
    if (result == NULL) {
        perror("strdup failed");
    }

    free(urlCopy);
    return result;
}

static int parseHttpResponseStatus(const char *responseLine) {
    char httpVersion[9]; // Enough to hold "HTTP/1.1"
    int statusCode;
    char statusMessage[1024];

    //Checking if we even have the header present in the string.
    if (strncmp(responseLine, "HTTP/", 5) != 0) {
        return 0;
    }

    // Parse the response line
    if (sscanf(responseLine, "%8s %d %[^\r\n]", httpVersion, &statusCode, statusMessage) < 3) {
        fprintf(stderr, "Protocol error!\n");
        return 2;
    }

    // Check if response starts with "HTTP/1.1"
    if (strcmp(httpVersion, "HTTP/1.1") != 0) {
        fprintf(stderr, "Protocol error!\n");
        return 2;
    }

    // Check if status code is 200
    if (statusCode != 200) {
//        fprintf(stderr, "Server response: %d %s\n", statusCode, statusMessage);
        fprintf(stderr, "%d %s\n", statusCode, statusMessage);
        return 3;
    }

    return 0;
}





