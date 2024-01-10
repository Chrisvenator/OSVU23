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
 * @brief
 * @details
 */
typedef struct arguments {
    uint16_t PORT;
    char *INDEX;
    char *DOC_ROOT;
} arguments;

/**
 * @brief
 * @details
 */
//Note that sig_atomic_t is not thread-safe, only async-signal safe.
volatile sig_atomic_t TERMINATE = false;

/**
 * @brief
 * @details
 * @param str
 * @param args
 * @return
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
 * @brief
 * @details
 * @param filename
 * @return
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
 * @brief
 * @details
 * @param path
 * @return
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
 * @brief
 * @details
 * @param cfd
 * @param status
 * @param status_message
 * @param file_path
 */
// Function to send a response
static void send_response(int cfd, int status, const char *status_message, const char *file_path) {
    char header[BUFFER_SIZE];
    int file_fd;
    struct stat file_stat;

    if (status == 200 && (file_fd = open(file_path, O_RDONLY)) != -1) {
        // Get file size
        fstat(file_fd, &file_stat);
        size_t content_length = file_stat.st_size;

        // Send header with content length
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nDate: Thu, 1 Dec 20 12:00:00\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n", status, status_message, content_length);
        write(cfd, header, strlen(header));

        // Send file content
        while (1) {
            ssize_t read_bytes = read(file_fd, header, BUFFER_SIZE);
            if (read_bytes <= 0) break;
            write(cfd, header, read_bytes);
        }
        close(file_fd);
    } else {
        // Send response without content length (for errors)
        snprintf(header, sizeof(header), "HTTP/1.1 %d %s\r\nConnection: close\r\n\r\n", status, status_message);
        write(cfd, header, strlen(header));
        fprintf(stderr, "[%s], File: %s not found! ", PROGRAM_NAME, file_path);
        perror("send response to client");
    }
}

/**
 * @brief
 * @details
 */
// Signal handler for graceful shutdown
static void handle_signal() {
    TERMINATE = true;
}