#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>

struct Point {
    double x, y;
};

double distance(struct Point p1, struct Point p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

void findClosestPair(struct Point points[], int n) {
    if (n <= 1) {

        exit(EXIT_SUCCESS);
    } else if (n == 2) {
        if (points[0].x < points[1].x || (points[0].x == points[1].x && points[0].y < points[1].y)) {
            printf("%.3lf %.3lf\n%.3lf %.3lf\n", points[0].x, points[0].y, points[1].x, points[1].y);
        } else {
            printf("%.3lf %.3lf\n%.3lf %.3lf\n", points[1].x, points[1].y, points[0].x, points[0].y);
        }
        exit(EXIT_SUCCESS);
    } else {
        double xm = 0.0;
        for (int i = 0; i < n; i++) {
            xm += points[i].x;
        }
        xm /= n;

        struct Point *leftPoints = (struct Point *) malloc(n * sizeof(struct Point));
        struct Point *rightPoints = (struct Point *) malloc(n * sizeof(struct Point));

        int leftCount = 0, rightCount = 0;
        for (int i = 0; i < n; i++) {
            if (points[i].x <= xm) {
                leftPoints[leftCount++] = points[i];
            } else {
                rightPoints[rightCount++] = points[i];
            }
        }

        int leftPipe[2], rightPipe[2];
        if (pipe(leftPipe) == -1 || pipe(rightPipe) == -1) {
            perror("Pipe creation error");
            exit(EXIT_FAILURE);
        }

        pid_t leftChild, rightChild;
        leftChild = fork();
        if (leftChild == 0) {
            close(leftPipe[1]);
            close(rightPipe[0]);

            dup2(leftPipe[0], STDIN_FILENO);
            close(leftPipe[0]);

            dup2(rightPipe[1], STDOUT_FILENO);
            close(rightPipe[1]);

            execlp("./cpair", "cpair", (char *) NULL);
            perror("execlp failed");
            exit(EXIT_FAILURE);
        } else if (leftChild > 0) {
            rightChild = fork();
            if (rightChild == 0) {
                close(leftPipe[0]);
                close(rightPipe[1]);

                dup2(rightPipe[0], STDIN_FILENO);
                close(rightPipe[0]);

                dup2(leftPipe[1], STDOUT_FILENO);
                close(leftPipe[1]);

                execlp("./cpair", "cpair", (char *) NULL);
                perror("execlp failed");
                exit(EXIT_FAILURE);
            } else if (rightChild < 0) {
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        close(leftPipe[0]);
        close(leftPipe[1]);
        close(rightPipe[0]);
        close(rightPipe[1]);

        for (int i = 0; i < leftCount; i++) {
            printf("%.3lf %.3lf\n", leftPoints[i].x, leftPoints[i].y);
        }
        fflush(stdout);

        for (int i = 0; i < rightCount; i++) {
            printf("%.3lf %.3lf\n", rightPoints[i].x, rightPoints[i].y);
        }
        fflush(stdout);

        int leftStatus, rightStatus;
        waitpid(leftChild, &leftStatus, 0);
        waitpid(rightChild, &rightStatus, 0);

        if (WEXITSTATUS(leftStatus) != EXIT_SUCCESS || WEXITSTATUS(rightStatus) != EXIT_SUCCESS) {
            exit(EXIT_FAILURE);
        }

        struct Point leftClosestPair[2], rightClosestPair[2], closestPair[2];

        if (scanf("%lf %lf\n%lf %lf\n", &leftClosestPair[0].x, &leftClosestPair[0].y, &leftClosestPair[1].x,
                  &leftClosestPair[1].y) == 4) {
            closestPair[0] = leftClosestPair[0];
            closestPair[1] = leftClosestPair[1];
        } else {
            perror("Failed to read closest pair from left child");
            exit(EXIT_FAILURE);
        }

        if (scanf("%lf %lf\n%lf %lf\n", &rightClosestPair[0].x, &rightClosestPair[0].y, &rightClosestPair[1].x,
                  &rightClosestPair[1].y) == 4) {
            if (distance(rightClosestPair[0], rightClosestPair[1]) < distance(closestPair[0], closestPair[1])) {
                closestPair[0] = rightClosestPair[0];
                closestPair[1] = rightClosestPair[1];
            }
        } else {
            perror("Failed to read closest pair from right child");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < leftCount; i++) {
            for (int j = 0; j < rightCount; j++) {
                double d = distance(leftPoints[i], rightPoints[j]);
                if (d < distance(closestPair[0], closestPair[1])) {
                    closestPair[0] = leftPoints[i];
                    closestPair[1] = rightPoints[j];
                }
            }
        }

        printf("%.3lf %.3lf\n%.3lf %.3lf\n", closestPair[0].x, closestPair[0].y, closestPair[1].x, closestPair[1].y);
        exit(EXIT_SUCCESS);
    }
}

#define MSG_SIZE 256

int main() {
    int pipefd[2];
    pid_t child_pid;
    char message[MSG_SIZE];
    char buffer[MSG_SIZE];

    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        close(pipefd[1]);
        read(pipefd[0], buffer, MSG_SIZE);
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
    } else {
        close(pipefd[0]);

        printf("Enter a message to send to the child: ");
        fgets(message, MSG_SIZE, stdin);
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
    }

    return 0;
}
