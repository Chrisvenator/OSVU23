// Header files
#include "helper.c"

//static int start_socket(arguments args);

static arguments parse_args(int argc, char *argv[]);


// Main driver function.
int main(int argc, char *argv[]) {
    arguments args = parse_args(argc, argv);

    // Use the parsed arguments as needed
    printf("PORT: %d\n", args.PORT);
    printf("INDEX: %s\n", args.INDEX);
    printf("DOC_ROOT: %s\n", args.DOC_ROOT);



//    start_socket(args);


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

    if (optind > argc) usage();
    args.DOC_ROOT = argv[optind];

    if (is_directory_accessible(args.DOC_ROOT) != 1) usage();


    return args;
}


//int start_socket(arguments args) {
//    int sfd, cfd;
//    struct sockaddr_in my_addr, peer_addr;
//    socklen_t peer_addr_size;
//    char buffer[BUFFER_SIZE];
//
//    sfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (sfd == -1) {
//        fprintf(stderr, "[%s]", PROGRAM_NAME);
//        perror("Socket creation failed");
//        exit(EXIT_FAILURE);
//    }
//
//    memset(&my_addr, 0, sizeof(my_addr));
//    my_addr.sin_family = AF_INET;
//    my_addr.sin_port = htons(args.PORT);
//    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//
//    if (bind(sfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) == -1) {
//
//    }
//
////    if (listen(sfd, LISTEN_BACKLOG) == -1) {
////
////    }
//
//    peer_addr_size = sizeof(peer_addr);
//    cfd = accept(sfd, (struct sockaddr *) &peer_addr, &peer_addr_size);
//    if (cfd == -1)
//        handle_error("accept");
//
//    // Read the request from the client
//    ssize_t read_size = read(cfd, buffer, BUFFER_SIZE - 1);
//    if (read_size == -1)
//        handle_error("read");
//
//    buffer[read_size] = '\0';
//    printf("Received request:\n%s\n", buffer);
//
//    // Here you can parse buffer to identify the request method and path
//
//    // Code to deal with incoming connection...
//
//    if (close(cfd) == -1)
//        handle_error("close");
//
//    if (close(sfd) == -1)
//        handle_error("close");
//
//    return 0;
//}
