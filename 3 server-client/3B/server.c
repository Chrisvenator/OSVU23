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
#include "helper.c"
#include <signal.h>
#include <bits/sigaction.h>

static int start_socket(arguments args);

static arguments parse_args(int argc, char *argv[]);


// Main driver function.
int main(int argc, char *argv[]) {
    arguments args = parse_args(argc, argv);

    // Use the parsed arguments as needed
    printf("PORT: %d\n", args.PORT);
    printf("INDEX: %s\n", args.INDEX);
    printf("DOC_ROOT: %s\n", args.DOC_ROOT);

    exit(start_socket(args));
}

/**
 * @brief Parses command line arguments.
 * @details This function parses command line arguments to configure server settings. It extracts the port number,
 *          index file name, and document root directory. It validates the provided arguments and sets defaults
 *          where necessary. Arguments are expected in the format `-p [port] -i [index]`.
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 * @return A structure of type 'arguments' containing the parsed and validated command line arguments.
 */
static arguments parse_args(int argc, char *argv[]) {
    if (argc <= 0) usage();
    PROGRAM_NAME = argv[0];
    if (argc == 1) usage();
    int opt;
    arguments args;

    args.PORT = DEFAULT_PORT;
    args.INDEX = DEFAULT_INDEX;

    bool p_set = false;
    bool i_set = false;

    while ((opt = getopt(argc, argv, "p:i:")) != -1) {
        switch (opt) {
            case 'p':
                if (p_set == true) usage();
                p_set = true;

                if (is_valid_port(optarg, &args) != 1) {
                    fprintf(stderr, "[%s] Port must be an integer: [%d]\n", PROGRAM_NAME, args.PORT);
                    usage();
                }

                break;
            case 'i':
                if (i_set == true) usage();
                i_set = true;

                args.INDEX = optarg;

                break;
            case '?':
                usage();
                break;
            default:
                usage();
                assert(0);
                break;
        }
    }


    if (argc - optind != 1) {
        fprintf(stderr, "[%s] Argument mismatch!\n", PROGRAM_NAME);
        usage();
    }
    args.DOC_ROOT = argv[optind];


    if (is_directory_accessible(args.DOC_ROOT) != 1) usage();
    if (strlen(args.DOC_ROOT) + strlen(args.INDEX) + 1 >= PATH_MAX) {
        fprintf(stderr, "[%s] The maximum length of a Path is %d characters!", PROGRAM_NAME, PATH_MAX);
        usage();
    }

//    char *index_path = malloc(sizeof(char *) * PATH_MAX);
//    snprintf(index_path, PATH_MAX, "%s/%s", args.DOC_ROOT, args.INDEX);
//    if (file_accessible(index_path) != 1) {
//        fprintf(stderr, "[%s] Cannot open file: [%s]\n", PROGRAM_NAME, index_path);
//        free(index_path);
//        usage();
//    }
//    free(index_path);


    return args;
}


/**
 * @brief Initializes and starts the server socket.
 * @details Sets up a TCP socket for the server, binds it to the specified port, and listens for incoming connections.
 *          It handles incoming connections, parses HTTP requests, and sends appropriate responses. Implements a
 *          simple select-based mechanism to handle multiple connections. Also includes error handling and graceful
 *          shutdown on receiving a termination signal.
 * @param args A structure containing the server configuration parameters such as port and document root.
 * @return An integer indicating the success or failure of the operation.
 */
static int start_socket(arguments args) {
    int socket_fd, connection_fd; //called sfd & cfd in the man pages
    int opt = 1;
    struct sockaddr_in address;
    struct timeval timeout;
    fd_set readfds;
    char buffer[BUFFER_SIZE], method[10], path[PATH_MAX], protocol[10];


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    // Set SO_REUSEADDR to allow the local address to be reused
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("Setsockopt SO_REUSEADDR failed");
        close(socket_fd);
        return EXIT_FAILURE;
    }


    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(args.PORT);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        perror("Bind failed");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    if (listen(socket_fd, LISTEN_BACKLOG) == -1) {
        perror("Listen failed");
        close(socket_fd);
        return EXIT_FAILURE;
    }

    // Register signal handler for graceful shutdown
//    signal(SIGINT, handle_signal);
//    signal(SIGTERM, handle_signal);
//    signal(SIGQUIT, handle_signal);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // or SA_RESTART to automatically restart interrupted system calls

    if (sigaction(SIGINT, &sa, NULL) == -1  || sigaction(SIGTERM, &sa, NULL) == -1 || sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction SIGINT failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    while (!TERMINATE) {
        FD_ZERO(&readfds); //Removes file descriptors from pointer
        FD_SET(socket_fd, &readfds); //Adds file descriptors from pointer

        // Set timeout to 1 second
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;


        if (select(socket_fd + 1, &readfds, NULL, NULL, &timeout) == -1) {
            perror("Select failed");
            if (TERMINATE) break; // Break if termination signal received
            continue;
        }


        //Test if the file descriptor is still present
        if (FD_ISSET(socket_fd, &readfds)) {
            connection_fd = accept(socket_fd, NULL, NULL); // Accept the incoming connectio
            if (connection_fd == -1) {
                perror("Accept failed");
                continue;
            } else printf("Connection accepted...\n");


            // We don't need to check the whole request since we only need the first line.
            // If the first line is incorrect, we return some 400 status
            ssize_t read_size = read(connection_fd, buffer, BUFFER_SIZE - 1);


            if (read_size == -1) {
                perror("Read failed");
                close(connection_fd);
                continue;
            } else printf("Reading from client...\n");

            buffer[read_size] = '\0';


            // Parse the request
            if (sscanf(buffer, "%s %s %s", method, path, protocol) != 3 || strcmp(protocol, "HTTP/1.1") != 0) send_response(connection_fd, 400, "(Bad Request)", NULL);
            else if (strcmp(method, "GET") != 0) send_response(connection_fd, 501, "(Not Implemented)", NULL);
            else {
                char *requested_resource = malloc(sizeof(char *) * PATH_MAX);
                snprintf(requested_resource, PATH_MAX, "%s/%s", args.DOC_ROOT, strcmp(path, "/") == 0 ? args.INDEX : path);

                fprintf(stdout, "Client requested: method: %s, protocol: %s, path: %s, Index: %s,  Resource path: %s\n", method, protocol, path, args.INDEX, requested_resource);


                if (access(requested_resource, F_OK) != -1) {
                    printf("Sending %s req to client...\n", requested_resource);
                    send_response(connection_fd, 200, "OK", requested_resource);
                } else {
                    send_response(connection_fd, 404, "(Not Found)", NULL);
                }

                free(requested_resource);
            }
            close(connection_fd);
        }
    }

    if (close(socket_fd) == -1) {
        perror("Closing socket failed!");
    } else printf("Socket closed.\n");
//    unlink() // For TCP/IP sockets (which use the Internet address family, AF_INET), unlink() is not relevant.
    return EXIT_SUCCESS;
}