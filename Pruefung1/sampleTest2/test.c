#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct args {
    char *pattern;
    char *seq;
    int num_children;
} args_t;

void usage(char *msg) {
    fprintf(stdout, "%s\n", msg);
    exit(EXIT_FAILURE);
}


/*******************************************************************************
* Task 1 - read_arguments()
*
* read the pattern, the sequence and the max number of children with getopt()
* The pattern is given via -p and an argument
 * The pattern can be at most 5 characters long
* The sequence is given via -s and an argument
 * The sequence can at most be 35 characters long
* The number of children is given via -n and an argument
 * num_children must be between [1, 7]
* All these options must be present when calling main and can at most appear one time each
******************************************************************************/
void read_arguments(int argc, char **argv, args_t *args) {
    int c;
    int p_flag = 0;
    int s_flag = 0;
    int n_flag = 0;

    while ((c = getopt(argc, argv, "p:s:n:")) != -1) {
        switch (c) {
            case 'p':
                if (p_flag == 1) {
                    usage("too many -p options");
                }
                if (optarg != NULL && optarg[0] == '-') {
                    usage("option -p requires argument");
                }
                if (strlen(optarg) > 5) {
                    usage("pattern too long");
                }
                p_flag = 1;
                args->pattern = optarg;
                break;
            case 's':
                if (s_flag == 1) {
                    usage("too many -s options");
                }
                if (optarg != NULL && optarg[0] == '-') {
                    usage("option -s requires argument");
                }
                if (strlen(optarg) > 35) {
                    usage("sequence too long");
                }
                s_flag = 1;
                args->seq = optarg;
                break;
            case 'n':
                if (n_flag == 1) {
                    usage("too many -n options");
                }
                if (optarg != NULL && optarg[0] == '-') {
                    usage("option -n requires argument");
                }
                n_flag = 1;
                int num = (int) strtol(optarg, NULL, 10);
                if (num > 7 || num < 1) {
                    usage("num of children too long/short");
                }
                args->num_children = num;
                break;
            default:
                usage("illegal option");
        }
    }

    if (argc - optind > 0) {
        usage("too many positional arguments");
    }
}


/*******************************************************************************
* Task 2 - fork_children()
 *
 * CHILDREN FUNCTIONALITY:
* Initialize num_children pipes
* Fork num_children children and scan the sequence for the pattern
 * Each child starts from its from the index (child_index * pattern_length) and scans pattern_length chars
 * e.g. seq = 0123456789,  pattern = 01234, num_children = 3
 *      child 1 starts at seq[0] and scans up to seq[4]
 *      child 2 starts at seq[5] and scans up to seq[9]
 *      child 3 starts at seq[10] but seq is only 10 chars long, so it does nothing and writes -1 to the pipe
 *
 * write the starting and ending index of the scanned chars to the pipe if the pattern was found
 * else write -1 to the pipe
 *
 * PARENT FUNCTIONALITY:
 * read from the read ends of the pipes and write the read indices to stdout like this:
 *        (s, e)\n where s is the starting index and e is the ending index
 *
******************************************************************************/
void fork_children(args_t args) {
    int children_to_parent[args.num_children][2];
    pid_t children_pids[args.num_children];

    for (int i = 0; i < args.num_children; i++) {
        if (pipe(children_to_parent[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        children_pids[i] = fork();

        if (children_pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // CHILD
        if (children_pids[i] == 0) {
            close(children_to_parent[i][0]);

            char found[10];
            int child_ind = i*strlen(args.pattern);

            for (int j = child_ind; j < child_ind + strlen(args.pattern); j++) {
                if (j >= strlen(args.seq) || args.seq[j] != args.pattern[j - child_ind]) {
                    sprintf(found, "-1");
                    write(children_to_parent[i][1], &found, sizeof(found));
                    close(children_to_parent[i][1]);
                    exit(EXIT_SUCCESS);
                }
            }

            int end_ind = child_ind + (int) strlen(args.pattern);
            sprintf(found, "(%d, %d)", child_ind, end_ind);
            write(children_to_parent[i][1], &found, sizeof(found));
            close(children_to_parent[i][1]);
            exit(EXIT_SUCCESS);

            // PARENT
        } else {
            int status;
            if (waitpid(children_pids[i], &status, 0) == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            }

            if (WEXITSTATUS(status) != EXIT_SUCCESS) {
                fprintf(stderr, "Child terminated with error");
                exit(EXIT_FAILURE);
            }

            char result[10];
            read(children_to_parent[i][0], &result, sizeof(result));
            close(children_to_parent[i][0]);
            fprintf(stdout, "%s\n", result);
        }
    }

    exit(EXIT_SUCCESS);

}


int main(int argc, char **argv) {
    args_t args;
    read_arguments(argc, argv, &args);

    fork_children(args);
}
