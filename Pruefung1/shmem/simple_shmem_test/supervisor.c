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
#include <signal.h>
#include <bits/sigaction.h>
#include <semaphore.h>
#include <time.h>


typedef struct shmem {
    bool stop;
    char c;
} shmem;

static const char *PROGNAME;
static struct shmem *myshmem;
static sem_t *sem_reader;

#define ERROR(...)  \
        do{ \
            fprintf(stderr, "%s ERROR: ", PROGNAME);    \
            fprintf(stderr, __VA_ARGS__);   \
            fprintf(stderr, "\n");  \
            exit(EXIT_FAILURE); \
        }  while(0)


void handle_signal(int signal) {
    myshmem->stop = true;
    sem_post(sem_reader);
}

#define SHM_NAME "/unique_name"
#define SEM_NAME "/sem_read"
#define SEM_NAME2 "/sem_write"

int main(int argc, char *argv[]) {
    FILE *outfile = stdout;

    bool o_set = false;


    int opt;
    while ((opt = getopt(argc, argv, "o:")) != -1) {
        switch (opt) {
            case 'o':
                if (o_set == true) ERROR("o set");
                o_set = true;
                outfile = fopen(optarg, "w+");
                break;
            default: /* '?' */
                ERROR("Usage: %s [-o outputfile]", argv[0]);
                break;
        }
    }
    PROGNAME = argv[0];

    unlink(SEM_NAME);
    unlink(SEM_NAME2);

    int shmfd = shm_open(SHM_NAME, O_RDWR | O_CREAT, 0660);

    if (shmfd == -1) {
        fclose(outfile);
        ERROR("shm open failed");
    }

    if (ftruncate(shmfd, sizeof(struct shmem)) < 0) {
        fclose(outfile);
        perror(":");
        ERROR("ftruncate fail");
    }

    myshmem = mmap(NULL, sizeof(*myshmem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);

    if (myshmem == MAP_FAILED) {
        fclose(outfile);
        close(shmfd);
        ERROR("ftruncate fail");
    }


    sem_reader = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 0);

    if (sem_reader == SEM_FAILED) {
        fclose(outfile);
        close(shmfd);
        munmap(myshmem, sizeof(*myshmem));
        ERROR("sem failed");

    }


    sem_t *sem_writer = sem_open(SEM_NAME2, O_CREAT | O_EXCL, 0600, 1);

    if (sem_writer == SEM_FAILED) {
        fclose(outfile);
        close(shmfd);
        sem_close(sem_reader);
        sem_unlink(SEM_NAME);
        sem_unlink(SEM_NAME2);
        munmap(myshmem, sizeof(*myshmem));
        ERROR("sem failed");
    }



    //______________________//
    myshmem->stop = false;
    signal(SIGQUIT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    while (!(myshmem->stop)) {


        if (sem_wait(sem_reader) == -1) {
            myshmem->stop = true;
            fclose(outfile);
            close(shmfd);
            sem_close(sem_reader);
            sem_unlink(SEM_NAME);
            sem_unlink(SEM_NAME2);
            munmap(myshmem, sizeof(*myshmem));
            ERROR("wait failed");
        }

        if (myshmem->stop) {
            break;
        }


        fprintf(outfile, "%c", myshmem->c);

        if (sem_post(sem_writer) == -1) {
            myshmem->stop = true;
            fclose(outfile);
            close(shmfd);
            sem_close(sem_reader);
            sem_unlink(SEM_NAME);
            sem_unlink(SEM_NAME2);
            munmap(myshmem, sizeof(*myshmem));
            ERROR("wait failed");
        }

    }

    //_____________________//
    //Close everything:

    bool unmap_success = true;
    bool unlink_success = true;
    bool success = true;

    if ((munmap(myshmem, sizeof(*myshmem)) == -1)) unmap_success = false;

    if (shm_unlink((SHM_NAME)) == -1) unlink_success = false;

    if (sem_close(sem_reader) == -1) success = false;
    if (sem_unlink(SEM_NAME) == -1) success = false;
    if (sem_close(sem_writer) == -1) success = false;
    if (sem_unlink(SEM_NAME2) == -1) success = false;


    close(shmfd);
    fclose(outfile);

    if (success == false) ERROR("sem failed close/unlink");
    if (unlink_success == false) ERROR("unlink failed");
    if (unmap_success == false) ERROR("unmap failed");


    exit(EXIT_SUCCESS);
}

















