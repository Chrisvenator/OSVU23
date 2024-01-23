
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <getopt.h>

#include "client.h"
#include "common.h"

/** name of the executable (for printing messages) */
char *program_name = "client";

void usage();

/** program entry point */
int main(int argc, char **argv) {
    struct args args;

    /* parse program arguments and fill 'arguments' [given] */
    parse_arguments(argc, argv, &arguments);

    bool p_set = false;
    bool g_set = false;
    bool s_set = false;
    args.portnum = 2017;
    args.portstr = "2017";

    int opt;
    while ((opt = getopt(argc, argv, "p:g:s:")) != -1) {
        switch (opt) {
            case 'p':
                if (p_set == true) error_exit("p is set");
                p_set = true;

                args.portstr = optarg;
                args.portnum = parse_number(optarg);
                if (args.portnum<1024 || args.portnum>UINT16_MAX) {
                    error_exit("portnumber not in range");
                }
                break;
            case 'g':
                if (g_set == true || s_set == true) usage();
                args.value = parse_number(optarg);
                g_set = true;
                args.cmd = 0;
                break;
            case 's':
                if (g_set == true || s_set == true) usage();
                args.value = parse_number(optarg);
                s_set = true;
                args.cmd = 1;
                break;
            default: /* '?' */
                usage();
                exit(EXIT_FAILURE);
        }
    }

    if (s_set == false && g_set == false) usage();
    if (argc - optind != 1) usage();
    args.id = parse_number(argv[optind]);
    if (args.id < 0 || args.id > 63) usage();


    /*******************************************************************
     * Task 1
     * ------
     * Connect to server.
     *
     * - Resolve host address, set port. Macros exist (SERVER_IPADDR_STR) and
     *   variables exist (arguments).
     * - Create socket. Use variable 'sockfd' for creation. Socket type
     *   is SOCK_STREAM.
     * - Connect.
     *
     * See also: getaddrinfo(3), socket(2), connect(2), ip(7),
     * error_exit (common.h)
     *******************************************************************/

    /* file descriptor of socket */
    int sockfd;

    struct addrinfo hints;
    struct addrinfo *ai;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int s = getaddrinfo(NULL, args.portstr, &hints, &ai);
    if (s != 0) error_exit("addr");

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd == -1) error_exit("socket");

    if (connect(sockfd, ai->ai_address, ai->ai_addresslen) < 0) error_exit("connect");


    /*******************************************************************
     * Task 2
     * ------
     * Pack and send command to server and receive acknowledge.
     *
     * - Pack the command from the arguments into a buffer.
     * - Send the buffer to the server.
     * - Receive response from the server.
     *   Save response to variables 'nok' and 'value' accordingly.
     *
     * See also: send(2), recv(2)
     *******************************************************************/

    uint8_t nok;
    uint8_t value;

    union {
        struct {
            uint8_t cmd : 2;
            uint8_t id : 6;
        } fields;
        uint8_t all;
    } fst;
    fst.fields.cmd = args.cmd;
    fst.fields.id = args.id;
    printf("first byte: 0x%x\n", fst.all);

    union {
        struct {
            uint8_t value : 7;
            uint8_t empty : 1;
        } fields;
        uint8_t all;
    } snd;
    snd.fields.value = args.value;
    snd.fields.empty = 0x00;
    printf("second byte: 0x%x\n\n", snd.all);

    union {
        struct {
            uint8_t fst : 8;
            uint8_t snd : 8;
        } fields;
        uint16_t all;
    } request;
    request.fields.fst = fst.all;
    request.fields.snd = snd.all;


    task_2_demo(&sockfd, &args, &nok, &value);



    /* DO NOT CHANGE THE FOLLOWING LINES */
    /* print server response */
    puts((nok) ? "NOK" : "OK");

    if (args.cmd == GET && !nok)
        printf("%d\n", value);

    /* cleanup: close socket */
    close(sockfd);

    exit(EXIT_SUCCESS);
}

void usage() {
    perror("somewhere Error");
    exit(EXIT_FAILURE);
}
