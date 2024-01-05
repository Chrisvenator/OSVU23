//
// Created by junioradmin on 04.01.24.
//
#include "headers.h"

static void usage(void) {
    fprintf(stderr, "[%s]USAGE: client [-p PORT] [ -o FILE | -d DIR ] URL\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

struct arguments {
    char *port;
    char *file;
    char *dir;
    char *url;
    char *hostname;
    char *resource;
};


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

static char *extract_resource(arguments args) {
    if (args.url == NULL) {
        return "/";
    }

    // Find the start of the resource (after "://")
    const char *start = strstr(args.url, "://");
    if (start != NULL) {
        start = strchr(start + 3, '/');
    } else {
        start = strchr(args.url, '/');
    }

    // If there is no resource part, return an empty string
    if (start == NULL) {
        return strdup("/");
    }

    // Allocate memory for the resource
    char *resource = malloc(strlen(start) + 1);
    if (resource == NULL) {
        return NULL; // Allocation failed
    }

    // Copy the resource, stopping at any special character
    const char *special_chars = ";?:@=&";
    size_t i = 0;
    while (start[i] && strchr(special_chars, start[i]) == NULL) {
        resource[i] = start[i];
        ++i;
    }

    resource[i] = '\0'; // Null-terminate the string

    return resource;
}

//TODO: free URL everywhere where needed
void appendIndexIfRequired(arguments *args) {
    const char *suffix = "/";
    const char *appendStr = "index.html";
    size_t urlLen = strlen(args->url);
    size_t suffixLen = strlen(suffix);

    // Check if the URL ends with '/'
    if (urlLen >= suffixLen && strcmp(args->url + urlLen - suffixLen, suffix) == 0) {
        // Allocate new memory for the modified URL
        char *newUrl = malloc(urlLen + strlen(appendStr) + 1); // +1 for null-terminator
        if (newUrl == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        strcpy(newUrl, args->url);
        strcat(newUrl, appendStr);

        // Free the old URL if it was dynamically allocated
        strcpy(args->url, newUrl);
        free(newUrl);
    }
}

static arguments parse_arguments(int argc, char *argv[]) {
    if (argc == 0) exit(EXIT_FAILURE);
    PROGRAM_NAME = argv[0];

    int opt;
    arguments args;
    args.port = "80";
    args.file = NULL;
    args.dir = NULL;
    args.url = NULL;
    args.hostname = NULL;
    args.resource = NULL;

    bool p_set = false;
    bool o_set = false;
    bool d_set = false;

    // Using getopt to parse command-line arguments
    while ((opt = getopt(argc, argv, "p:o:d:")) != -1) {
        switch (opt) {
            case 'p':
                if (p_set == true) usage();
                p_set = true;
                args.port = optarg;
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
    appendIndexIfRequired(&args);


    // print the parsed arguments:
    printf("PORT: %s\n", args.port);
    printf("FILE: %s\n", args.file);
    printf("DIR: %s\n", args.dir);
    printf("URL: %s\n", args.url);

    args.hostname = extract_hostname(args);
    args.resource = extract_resource(args);

    if (d_set == true && args.file != NULL && args.resource != NULL) {
        strcat(args.file, "/");
    }


    return args;
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





