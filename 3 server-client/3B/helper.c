/**
 * @author Christopher Scherling 12119060
 * @file helper.c
 * @date 10.01.2024
 *
 *
 * @brief
 *
 * @details
 *
 * @note
 */
#include "server.h"

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
    fprintf(stderr, "[%s] USAGE: server [-p PORT] [-i INDEX] DOC_ROOT\n", PROGRAM_NAME);
    fprintf(stderr, "[%s] USAGE: server [-p PORT] [-i INDEX] DOC_ROOT\n", PROGRAM_NAME);
    exit(EXIT_FAILURE);
}

/**
 * @brief Structure to hold server configuration arguments.
 * @details Contains server configuration parameters such as the port number, index file name, and document root
 *          directory, which are parsed from the command line arguments.
 * @param PORT The port number on which the server will listen.
 * @param INDEX The name of the index file (e.g., 'index.html').
 * @param DOC_ROOT The root directory from which files will be served.
 */
typedef struct arguments {
    uint16_t PORT;
    char *INDEX;
    char *DOC_ROOT;
} arguments;

/**
 * @brief Global flag indicating server termination.
 * @details This variable is set to 'true' by the signal handler to indicate that the server should terminate.
 * @note Note that sig_atomic_t is not thread-safe, but is asynchronous-signal safe, making it suitable for
 *          use in this context.
 */
volatile sig_atomic_t TERMINATE = false;

/**
 * @brief Validates the provided port number.
 * @details Checks if the given port number is within the valid range and is usable. This function is typically
 *          called during argument parsing to ensure the port number provided is suitable for socket operations.
 * @param str The port number to validate.
 * @param args Arguments where the port number will be saved
 * @return An integer indicating if the port is valid (1) or invalid (0).
 */
static int is_valid_port(const char *str, arguments *args) {
    char *endptr;
    long port;

    errno = 0;  // To distinguish success/failure after call
    port = strtol(str, &endptr, 10);

    // Check for various possible errors
    if ((errno == ERANGE && (port == LONG_MAX || port == LONG_MIN))
        || (errno != 0 && port == 0)) {
        fprintf(stderr, "[%s]\n", PROGRAM_NAME);
        perror("strtol");
        return 0;
    }

    if (endptr == str) {
        fprintf(stderr, "[%s]No digits were found\n", PROGRAM_NAME);
        return 0;
    }

    // Check for valid port range
    if (port < 0 || port > 65535) { //unit16_t are 4 bits. Max Value: 0xFFFF = 65535
        fprintf(stderr, "[%s] Port number out of range\n", PROGRAM_NAME);
        return 0;
    }

    char string_port[5]; //String can only be 5 chars long
    sprintf(string_port, "%d", (int) port);

    if (strlen(string_port) != strlen(str)) {
        fprintf(stderr, "[%s] Not a valid port!\n", PROGRAM_NAME);
        return 0;
    }

    args->PORT = port;

    // If we're here, str was a valid port number
    return 1;
}

/**
 * @brief Checks file accessibility.
 * @details Verifies if the given file path points to an accessible file. This is used to ensure that the server
 *          can access and serve the requested file.
 * @param filename The path to the file to check.
 * @return An integer indicating if the file is accessible (1) or not (0).
 */
static int file_accessible(const char *filename) {
    // Check if the file exists and has read permission
    if (access(filename, F_OK) != -1 && access(filename, R_OK) != -1) {
        return 1; // File exists and is readable
    } else {
        return 0; // File does not exist or is not readable
    }
}

/**
 * @brief Checks directory accessibility.
 * @details Verifies if the given directory path is accessible and readable. This is important for the server to
 *          validate the document root directory provided in the command line arguments.
 * @param path The path to the directory to check.
 * @return An integer indicating if the directory is accessible (1) or not (0).
 */
//"The stat function allows you to obtain information about the file system object specified by the path,
// and you can use the S_ISDIR macro to check if it's a directory.
// Additionally, the access function can be used to check for read and execute permissions,
// which are typically required for accessing a directory."
static int is_directory_accessible(const char *path) {
    struct stat path_stat;

    // Check if path exists and get its status
    if (stat(path, &path_stat) != 0) {
        fprintf(stderr, "[%s]", PROGRAM_NAME);
        perror("directory path Path does not exist or cannot be accessed");
        return 0; // Path does not exist or cannot be accessed
    }

    // Check if it's a directory
    if (!S_ISDIR(path_stat.st_mode)) {
        fprintf(stderr, "[%s]%s is not a directory\n", PROGRAM_NAME, path);
        return 0;
    }

    // Check for read and execute permissions
    if (access(path, R_OK) != -1 && access(path, X_OK) != -1) {
        return 1; // Directory is accessible
    } else {
        fprintf(stderr, "[%s]", PROGRAM_NAME);
        perror("Directory is not accessible");
        return 0; // Directory is not accessible
    }
}

/**
 * @brief Sends an HTTP response to the client.
 * @details Constructs and sends an HTTP response based on the provided status code, status message, and file path.
 *          For successful requests (status 200), it sends the requested file's content. For unsuccessful requests,
 *          it sends only the status header. Handles file opening, reading, and closing operations internally.
 * @param client_fd Client file descriptor to which the response is sent.
 * @param status HTTP status code (e.g., 200 for OK, 400 for Everything else).
 * @param status_message A brief message corresponding to the HTTP status code.
 * @param file_path The path to the file that needs to be sent in case of a successful request.
 */
static void send_response(int client_fd, int status, const char *status_message, const char *file_path) {
    char header[BUFFER_SIZE];
    int file_fd;
    struct stat file_stat;

    if (status == 200 && (file_fd = open(file_path, O_RDONLY)) != -1) {
        // Get file size
        fstat(file_fd, &file_stat);
        size_t content_length = file_stat.st_size;

        // Send header with content length
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nDate: Thu, 1 Dec 20 12:00:00\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n", status, status_message, content_length);
        write(client_fd, header, strlen(header));

        // Send file content
        while (1) {
            ssize_t read_bytes = read(file_fd, header, BUFFER_SIZE);
            if (read_bytes <= 0) break;
            write(client_fd, header, read_bytes);
        }
        close(file_fd);
    } else {
        // Send response without content length (for errors)
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nConnection: close\r\n\r\n", status, status_message);
        write(client_fd, header, strlen(header));
        fprintf(stderr, "[%s], File: %s not found! ", PROGRAM_NAME, file_path);
        perror("send response to client");
    }
}

/**
 * @brief Signal handler for graceful shutdown.
 * @details Sets a global flag to indicate that the server should terminate. This function is designed to be
 *          registered as a signal handler for signals like SIGINT or SIGTERM, allowing for a graceful shutdown
 *          of the server.
 */
static void handle_signal() {
    TERMINATE = true;
}