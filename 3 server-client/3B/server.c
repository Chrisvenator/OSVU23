// Header files
#include "helper.c"

static int start_socket(arguments args);

static arguments parse_args(int argc, char *argv[]);


// Main driver function.
int main(int argc, char *argv[]) {
    arguments args = parse_args(argc, argv);

    // Use the parsed arguments as needed
    printf("PORT: %d\n", args.PORT);
    printf("INDEX: %s\n", args.INDEX);
    printf("DOC_ROOT: %s\n", args.DOC_ROOT);

//    setup_signal_handling();

    start_socket(args);


    exit(EXIT_SUCCESS);
}

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

                if (file_accessible(args.INDEX) != 1) {
                    fprintf(stderr, "[%s] Cannot open file: [%s]", PROGRAM_NAME, args.INDEX);
                    usage();
                }

                break;
            case '?':
                usage();
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

    if (snprintf(args.full_path, sizeof(args.full_path), "%s/%s", args.DOC_ROOT, args.INDEX) <= 0) {
        fprintf(stderr, "[%s] Something went wrong while calculating the path for the index...", PROGRAM_NAME);
        usage();
    }


    return args;
}


// Main function to handle incoming connections
static int start_socket(arguments args) {
    int sfd, cfd;
    struct sockaddr_in my_addr;
    char buffer[BUFFER_SIZE], method[10], path[1024], protocol[10];


    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(args.PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
        perror("Bind failed");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, LISTEN_BACKLOG) == -1) {
        perror("Listen failed");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    // Register signal handler for graceful shutdown
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);

    fd_set readfds;
    struct timeval timeout;

    while (!TERMINATE) {
        FD_ZERO(&readfds);
        FD_SET(sfd, &readfds);

        // Set timeout to 1 second
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int retval = select(sfd, &readfds, NULL, NULL, &timeout);


        if (retval == -1) {
            if (TERMINATE) break; // Break if termination signal received
            perror("Select failed");
            continue;
        }

//        if (FD_ISSET(sfd, &readfds) > 0) {

        cfd = accept(sfd, NULL, NULL); // Accept the incoming connection
        if (cfd == -1) {
            perror("Accept failed");
            continue;
        }

        printf("Reading from client...\n");
        ssize_t read_size = read(cfd, buffer, BUFFER_SIZE - 1);

        if (read_size == -1) {
            perror("Read failed");
            close(cfd);
            continue;
        }

        buffer[read_size] = '\0';


        // Parse the request
        if (sscanf(buffer, "%s %s %s", method, path, protocol) != 3) send_response(cfd, 400, "Bad Request", NULL, NULL);
        else if (strcmp(method, "GET") != 0) send_response(cfd, 501, "Not Implemented", NULL, NULL);
        else {
//            char full_path[PATH_MAX]; //There is no Filepath longer than PATH_MAX
//            snprintf(full_path, sizeof(full_path), "%s/%s", args.DOC_ROOT, args.INDEX);
            printf("Sending %s req to client...\n", args.full_path);
            if (access(args.full_path, F_OK) != -1) {
                send_response(cfd, 200, "OK", NULL, args.full_path);
            } else {
                send_response(cfd, 404, "Not Found", NULL, NULL);
            }
        }
        close(cfd);
    }
//    }

//TODO: Auf -1 überprüfen
    close(cfd);
    close(sfd);
//    unlink()
    return 0;
}