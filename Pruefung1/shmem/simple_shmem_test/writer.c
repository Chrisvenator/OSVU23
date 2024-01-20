#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <math.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include "signal.h"


typedef struct shmem {
    bool stop;
    char c;
} shmem;

static const char *PROGNAME;
static struct shmem *myshmem;

#define ERROR(...)  \
        do{ \
            fprintf(stderr, "%s ERROR: ", PROGNAME);    \
            fprintf(stderr, __VA_ARGS__);   \
            fprintf(stderr, "\n");  \
            exit(EXIT_FAILURE); \
        }  while(0)


#define SHM_NAME "/unique_name"
#define SEM_NAME "/sem_read"
#define SEM_NAME2 "/sem_write"

#define ERROR(...)  \
        do{ \
            fprintf(stderr, "%s ERROR: ", PROGNAME);    \
            fprintf(stderr, __VA_ARGS__);   \
            fprintf(stderr, "\n");  \
            exit(EXIT_FAILURE); \
        }  while(0)

int main(int argc, char *argv[]) {
    if (argc != 3) ERROR("Args");

    FILE *infile;

    bool i_set = false;

    int opt;
    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {
            case 'i':
                if (i_set == true) ERROR("i hase been set");
                i_set = true;
                infile = fopen(optarg, "r");
                if (infile == NULL) {
                    myshmem->stop = true;
                    ERROR("fopen failed");
                }
                break;
            default: /* '?' */
                ERROR("getopt");
                break;
        }
    }

    int shmfd = shm_open(SHM_NAME, O_RDWR, 0660);

    if (shmfd == 1) {

        myshmem->stop = true;
        fclose(infile);
        ERROR("shmfd failed");
    }

    myshmem = mmap(NULL, sizeof(*myshmem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (myshmem == MAP_FAILED) {
        fclose(infile);
        close(shmfd);
        myshmem->stop = true;
        ERROR("mmap failed");
    }

    sem_t *sem_reader = sem_open(SEM_NAME, 0);

    if (sem_reader == SEM_FAILED) {
        fclose(infile);
        close(shmfd);
        munmap(myshmem, sizeof(*myshmem));

        myshmem->stop = true;
        ERROR("sem creation failed");
    }
    sem_t *sem_writer = sem_open(SEM_NAME2, 0);

    if (sem_writer == SEM_FAILED) {
        fclose(infile);
        close(shmfd);
        munmap(myshmem, sizeof(*myshmem));
        sem_close(sem_reader);
        sem_close(sem_writer);
        myshmem->stop = true;
        ERROR("sem writer open failed");
    }

    //_________________________//

    while (!(myshmem->stop)) {
        char currC;

        sem_wait(sem_writer);

        currC = fgetc(infile);
        if ((currC) == EOF)myshmem->stop = true;
        if (currC == -1) {
            myshmem->stop = true;
            sem_post(sem_reader);
            break;
        }

        myshmem->c = currC;

        sem_post(sem_reader);

    }

    //_________________________//

    bool error = false;

    if (munmap(myshmem, sizeof(*myshmem)) == -1) error = true;
    if (close(shmfd) == -1) error = true;
    if (sem_close(sem_reader) == -1) error = true;
    if (sem_close(sem_writer) == -1) error = true;

    exit(error);


}










