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
void appendIndexHtmlIfRequired(char **path) {
    const char *suffix = "/";
    const char *appendStr = "index.html";
    size_t pathLen = strlen(*path);
    size_t suffixLen = strlen(suffix);

    if (pathLen >= suffixLen && strcmp(*path + pathLen - suffixLen, suffix) == 0) {
        char *newPath = malloc(pathLen + strlen(appendStr) + 1);
        if (newPath == NULL) {
            perror("Failed to allocate memory");
            free(newPath);
            exit(EXIT_FAILURE);
        }
        strcpy(newPath, *path);
        strcat(newPath, appendStr);

        strcpy(*path, newPath);
        free(newPath);  // Free the old path
    }
}

static void addSlashToEnd(char **str) {
    // Check if str ends with '/', if not, append it

    size_t dirLen = strlen(*str);
    if (dirLen == 0 || *str[dirLen - 1] != '/') {
        char *newDir = malloc((sizeof(*str) + sizeof(char *) * 2)); // +1 for '/' and +1 for null terminator
        if (newDir == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        strcpy(newDir, *str);
        newDir[dirLen] = '/';
        newDir[dirLen + 1] = '\0';

        strcpy(*str, newDir);
        free(newDir);
    }
}


static void appendFilenameToDir(arguments *args) {


    // Find substring in args.resource after the last '/'
    const char *lastSlash = strrchr(args->resource, '/');
    if (lastSlash != NULL) {
        // Append everything after the last '/' from args.resource to args.dir
        const char *substring = lastSlash + 1; // Skip the '/'
        char *updatedDir = malloc(sizeof(args->dir) + sizeof(substring) + sizeof(char *)); // +1 for null terminator
        strcpy(updatedDir, args->dir);
        if (updatedDir == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        strcat(updatedDir, substring);
        args->dir = updatedDir;
    }

}

static int getNumberOfChar(char *str, char c) {
    int i;
    for (i = 0; str[i]; str[i] == c ? i++ : *str++);

    return i;
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


    args.hostname = extract_hostname(args);
    args.resource = extract_resource(args);

    if (d_set == true && args.dir != NULL && args.resource != NULL) {

        struct stat st = {0};

        if (stat(args.dir, &st) == -1) {
            printf("Creating directory...\n");
            printf("Creating directory...\n");
            mkdir(args.dir, 0777);
            printf("Dir created.\n");
        } else {
            printf("Dir exists\n");
        }

        //check if last character is "/". If yes, add "index.html"
        //If no, the append the filename of the resource to the dir path. It will be later written into that file.

        if ((args.url[strlen(args.url) - 1]) == '/' || getNumberOfChar(args.url, '/') <= 2) {
            strcat(args.dir, "/");
            appendIndexHtmlIfRequired(&args.dir);
        } else {
            printf("DIR0: %s\n", args.dir);
            addSlashToEnd(&args.dir);
            printf("DIR1: %s\n", args.dir);
            appendFilenameToDir(&args);
            printf("DIR2: %s\n", args.dir);
        }

        args.file = malloc(sizeof(args.dir));
        strcpy(args.file, args.dir);
    }

    appendIndexHtmlIfRequired(&args.url);

    // print the parsed arguments:
    printf("\nPORT: %s\n", args.port);
    printf("FILE: %s\n", args.file);
    printf("DIR: %s\n", args.dir);
    printf("URL: %s\n", args.url);
    printf("Hostname: %s\n", args.hostname);
    printf("Resource: %s\n\n", args.resource);


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





