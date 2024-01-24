//
// Created by freyness on 1/23/24.
//

#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <string.h>

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#include <assert.h>

//Namen sollten die hier sein:
//sem_ready, worker, idle
#define SEM_REQUEST "/sem_req"
#define SEM_RESPONSE "/sem_resp"
#define SHM_NAME "/shm_2"

#define SHM_SIZE 8192

#define DEFAULT_PORTNUM     (2017)
#define DEFAULT_PORTNUM_STR "2017"

void error_exit(const char *msg);


void usage();

typedef struct shmp {
// Something
} shmp;

int Setup_connection(const char *port_str) {
    struct addrinfo hints;
    struct addrinfo *ai;
    int sfd, s;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(NULL, DEFAULT_PORTNUM_STR, &hints, &ai);
    if (s < 0) error_exit("address info");

    sfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sfd < 0) error_exit("socket in use");

    if (bind(sfd, ai->ai_addr, ai->ai_addrlen) != 0) error_exit("bind");

    if (listen(sfd, 0) < 0) error_exit("listen");

    freeaddrinfo(ai);

    return sfd;
}

void allocate_resources(size_t *max_chars) {
    int shmemfd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (shmemfd < 0) error_exit("shmem failed");

    sem_t *sem_request = sem_open(SEM_REQUEST, O_CREAT | O_EXCL, 0600, 0);
    sem_t *sem_response = sem_open(SEM_RESPONSE, O_CREAT | O_EXCL, 0600, 0);

    if (sem_request == SEM_FAILED || sem_response == SEM_FAILED) error_exit("sem failed");

    if (ftruncate(shmemfd, SHM_SIZE + 1) != 0) error_exit("truncate failed");

    shmp = mmap(NULL, *max_chars, PROT_READ | PROT_WRITE, MAP_SHARED, shmemfd, 0);
    if (shmp == MAP_FAILED) error_exit("mmap failed");
}

int read_from_socket();

int aufgabe_3(int sfd, char *response) {
    int accept_fd = accept(sfd, NULL, NULL);
    if (accept_fd < 0) error_exit("accept");

    int val = read_from_socket();

    FILE *file = fdopen(accept_fd, "w+");
    if (file == NULL) error_exit("file");

    if (val < 0) fprintf(file, "ERROR\n");
    else {
        fprintf(file, "OK%d\n", val);
    }

    fflush(file);
    fclose(file);
}

// read from conn and write to
int task4(FILE *conn, size_t max) {
    while (sem_wait(sem_idle) == -1) { // interuppt handling, retry
        if (errno != EINTR) {
            error_exit("fail");
        }
    }

    size_t i = 0;
    int c;
    while (i < max && (c = fgetc(conn) != EOF)) {                          // read from conn
        shmp[i] = (char) c;
        i++;
    }

    if (feof(conn)) {
        shmp[i] = '\0'; // important
    } else {
        shmp = '\0'; // important
    }

    if (sem_post(sem_ready) == -1) {
        error_exit("fail");
    }

    return feof(conn) ? i : -1;
}

int main(int argc, char *argv[]) {
    exit(EXIT_SUCCESS);
}