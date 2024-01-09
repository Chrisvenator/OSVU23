/**
 * @author Christopher Scherling 12119060
 * @file client_helper_functions.c
 * @date 01.11.2023
 *
 *
 * @brief Helper functions for network client in the OSVU23 project.
 *
 * @details This file contains a collection of helper functions used by the main client program for the OSVU23 project.
 * It includes functions for parsing command-line arguments,
 * extracting hostname and resource path from URLs, managing strings related to file and directory paths,
 * and parsing HTTP response status. These utility functions encapsulate specific tasks related to network
 * communication and string manipulation, providing modular and reusable code segments for the main client application.
 *
 * @note The functions in this file handle dynamic memory allocation.
 * It is crucial for the calling code to manage the allocated memory appropriately to prevent memory leaks.
 * Functions that allocate memory have specific warnings about the necessity of freeing allocated memory.
 **/

#include "client.h"

/**
 * @brief Displays usage information and exits the program.
 * @details The function is designed to be invoked when the user inputs incorrect arguments.
 * It provides a clear, concise message about the correct usage syntax,
 * ensuring that the user is aware of the expected input format.
 * This function not only aids in user guidance but also acts as a key component of the program's error handling mechanism.
 * By clearly defining the expected usage, it helps prevent misinterpretation and misuse of the program.
 * It's important to note that this function terminates the program execution, marking an abnormal exit due to improper usage.
 * <br> So please free everything before invoking this function.S
 */
static void usage(void) {
    fprintf(stderr, "[%s] USAGE: client [-p PORT] [ -o FILE | -d DIR ] URL\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

/**
 * @brief Defines the structure for holding command line arguments.
 * @details This structure serves as a centralized repository for all command-line arguments used by the program.
 * By storing these arguments in a structured manner, it provides an efficient way to access and manage user inputs throughout the program.
 * The structure includes fields for port, file, directory, URL, hostname, and resource, covering all necessary parameters for network operations.
 * This comprehensive approach simplifies the process of passing multiple arguments across functions and enhances code readability and maintainability.
 */
typedef struct arguments {
    char *port;
    char *file;
    char *dir;
    char *url;
    char *hostname;
    char *resource;
} arguments;


/**
 * @brief Extracts the hostname from a given URL.
 * @details This function processes the URL provided in the arguments to isolate and return the hostname part.
 * It handles different URL formats and extracts the hostname correctly,
 * disregarding other components like the protocol, port, or resource path.
 * @warning Remember to free the returned string to avoid memory leaks.
 * @param args The arguments structure containing the URL to process.
 * @return A dynamically allocated string containing the hostname. Returns NULL on failure.
 */
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

/**
 * @brief Counts the occurrences of a specified character in a string.
 * @details The function iterates over the given string and counts how many times the specified character appears.
 * This can be used to validate or parse strings based on specific character occurrences.
 * @param str The string to search through.
 * @param c The character to count in the string.
 * @return The count of the specified character in the string.
 */
static int getNumberOfChar(char *str, char c) {
    if (str == NULL || strlen(str) == 0) {
        assert(0);
        return 0;
    }
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] == c) {
            count++;
        }
    }

    return count;
}


/**
 * @brief Extracts the resource path from a URL.
 * @details This function isolates and returns the resource path from a provided URL.
 * It identifies the start of the resource part in the URL and allocates memory to store this substring,
 * which is then returned.
 * @warning Remember to free the returned string to avoid memory leaks.
 * @param args The arguments structure containing the URL to process.
 * @return A dynamically allocated string containing the resource path. Returns NULL on failure or "/" if no resource path is found.
 */
static char *extract_resource(arguments args) {
    if (args.url == NULL) {
        return "/";
    }

    // Find the start of the resource (after "://")
    const char *START = strstr(args.url, "://");
    if (START != NULL) {
        START = strchr(START + 3, '/');
    } else {
        START = strchr(args.url, '/');
    }

    // If there is no resource part, return an empty string
    if (START == NULL) {
        return strdup("/");
    }

    // Allocate memory for the resource
    char *resource = malloc(strlen(START) + 1);
    if (resource == NULL) {
        return NULL; // Allocation failed
    }

    // Copy the resource, stopping at any special character
    const char *SPECIAL_CHARS = ";?:@=&";
    size_t i = 0;
    while (START[i] && strchr(SPECIAL_CHARS, START[i]) == NULL) {
        resource[i] = START[i];
        ++i;
    }

    resource[i] = '\0'; // Null-terminate the string

    return resource;
}

/**
 * @brief Appends 'index.html' to a path if it ends with a slash.
 * @details Checks if the provided path string ends with a '/', and if so, appends 'index.html' to it.
 * This is useful for handling directory paths in HTTP requests where a default document is expected.
 * @warning This function dynamically allocates memory for the new path.
 * Ensure to free this memory after usage to avoid memory leaks.
 * @param path A pointer to the string to modify.
 */
static void appendIndexHtmlIfRequired(char **path) {
    size_t pathLen = strlen(*path);
    size_t suffixLen = strlen(DEFAULT_SUFFIX);

    if (pathLen >= suffixLen && strcmp(*path + pathLen - suffixLen, DEFAULT_SUFFIX) == 0) {
        char *newPath = malloc(pathLen + strlen(DEFAULT_FILENAME) + 1);
        if (newPath == NULL) {
            perror("Failed to allocate memory");
            free(newPath);
            exit(EXIT_FAILURE);
        }

        strcpy(newPath, *path);
        strcat(newPath, DEFAULT_FILENAME);

        strcpy(*path, newPath);
        free(newPath);  // Free the old path
    }
}

/**
 * @brief Ensures that a string ends with a slash ('/').
 * @details This function appends a slash to the end of the provided string if it doesn't already end with one.
 * It's particularly useful for normalizing directory paths.
 * @warning This function dynamically allocates memory for the new directory string.
 * Ensure to free this memory after usage to avoid memory leaks.
 * @param str A pointer to the string to modify.
 */
static void addSlashToEnd(char **str) {
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


/**
 * @brief Appends the filename from a resource URL to a directory path.
 * @details Extracts the filename from the resource part of a URL and appends it to a given directory path.
 * This is used when saving resources to a specific directory.
 * @warning This function dynamically allocates memory for the updated directory.
 * Ensure to free this memory after usage to avoid memory leaks.
 * @param args A pointer to the arguments structure containing the resource URL and directory path.
 */
static void appendFilenameToDir(arguments *args) {


    // Find substring in args.resource after the last '/'
    const char *LAST_SLASH = strrchr(args->resource, '/');
    if (LAST_SLASH != NULL) {
        // Append everything after the last '/' from args.resource to args.dir:
        const char *SUBSTRING = LAST_SLASH + 1; // Skip the '/'
        char *updatedDir = malloc(sizeof(args->dir) + sizeof(SUBSTRING) + sizeof(char *)); // +1 for null terminator
        strcpy(updatedDir, args->dir);
        if (updatedDir == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        strcat(updatedDir, SUBSTRING);
        args->dir = updatedDir;
    }

}

/**
 * @brief Parses command line arguments and populates an "arguments" structure.
 * @details This function processes the command line arguments provided to the program,
 * sets the appropriate fields in the arguments structure,
 * and handles any invalid inputs or combinations of arguments.
 * @warning The function dynamically allocates memory for args.hostname and args.resource.
 * Ensure to free these resources after usage to avoid memory leaks.
 * @param argc The number of commandline arguments.
 * @param argv The array of commandline argument strings.
 * @return An arguments structure populated with the parsed data.
 */
static arguments parse_arguments(int argc, char *argv[]) {
    if (argc == 0) usage();
    PROGRAM_NAME = argv[0];

    int opt;
    arguments args;
    args.port = DEFAULT_PORT;
    args.file = NULL;
    args.dir = NULL;
    args.url = NULL;
    args.hostname = NULL;
    args.resource = NULL;

    bool p_set = false;
    bool o_set = false;
    bool d_set = false;

    opterr = 0; //Silence the "Invalid option -- x" error

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

                //Check if the string has some bad characters in it
                const char *SPECIAL_CHARS = "!\"§$%&=?²³¼½¬{[]}\\,;:<>|´`+*~#'@€ ";

                for (int j = 0; j < strlen(SPECIAL_CHARS); ++j) {
                    if (getNumberOfChar(args.dir, SPECIAL_CHARS[j]) != 0) {
                        free(args.hostname);
                        free(args.resource);
                        fprintf(stderr, "[%s] Invalid Filename: %c!\n", PROGRAM_NAME, SPECIAL_CHARS[j]);

                        exit(EXIT_FAILURE);
                    }
                }

                //Make it so that a directory can never have a "."
                if (args.dir != NULL && getNumberOfChar(args.dir, '.') != 0) {
                    free(args.hostname);
                    free(args.resource);
                    fprintf(stderr, "[%s] Invalid Filename!\n", PROGRAM_NAME);

                    exit(EXIT_FAILURE);
                }


                break;

            case '?': // Unrecognized option
                usage();
                break;
            default: //unreachable option
                usage();
                assert(0);
                break;
        }
    }

    // Checking for mandatory URL argument
    if (optind >= argc) usage();

    args.url = argv[optind];
    if (optind + 1 != argc) usage();


    args.hostname = extract_hostname(args);
    args.resource = extract_resource(args);

    if (d_set == true && args.dir != NULL && args.resource != NULL) {

        struct stat st = {0};

        if (stat(args.dir, &st) == -1) {
            if(mkdir(args.dir, 0777) == -1){
                free(args.hostname);
                free(args.resource);
                fprintf(stderr, "[%s] Failed to create directory!\n", PROGRAM_NAME);

                exit(EXIT_FAILURE);
            }
        }

        //check if last character is "/". If yes, add "index.html"
        //If no, the append the filename of the resource to the dir path. It will be later written into that file.

        if ((args.url[strlen(args.url) - 1]) == '/' || getNumberOfChar(args.url, '/') <= 2) {
            strcat(args.dir, "/");
            appendIndexHtmlIfRequired(&args.dir);
        } else {
            addSlashToEnd(&args.dir);
            appendFilenameToDir(&args);
        }

        args.file = malloc(strlen(args.dir) + 1);
        if (args.file == NULL) {
            free(args.hostname);
            free(args.resource);
            fprintf(stderr, "[%s] Failed to allocate memory!\n", PROGRAM_NAME);

            exit(EXIT_FAILURE);

        } else {
            strcpy(args.file, args.dir);
        }
    }

    appendIndexHtmlIfRequired(&args.url);

    return args;
}

/**
 * @brief Parses the status line of an HTTP response.
 * @details Analyzes the HTTP response line to extract the status code and message.
 * It validates the HTTP version and checks if the status code indicates a successful response.
 * @param RESPONSE_LINE The HTTP response line to parse as a String.
 * @return An integer representing the status of the parsing: 0 for success, non-zero for various errors.
 */
static int parseHttpResponseStatus(const char *RESPONSE_LINE) {
    char httpVersion[9]; // Enough to hold "HTTP/1.1"
    int statusCode;
    char statusMessage[BUFFER_SIZE];

    //Checking if we even have the header present in the string.
    if (strncmp(RESPONSE_LINE, "HTTP/", 5) != 0) {
        return 2;
    }

    // Parse the response line
    if (sscanf(RESPONSE_LINE, "%8s %d %[^\r\n]", httpVersion, &statusCode, statusMessage) < 3) {
        fprintf(stderr, "[%s]Protocol error!\n", PROGRAM_NAME);
        return 2;
    }

    if (strcmp(httpVersion, "HTTP/1.1") != 0) {
        fprintf(stderr, "[%s]Protocol error!\n", PROGRAM_NAME);
        return 2;
    }

    if (statusCode != 200) {
        fprintf(stderr, "%d %s\n", statusCode, statusMessage);
        return 3;
    }

    return 0;
}





