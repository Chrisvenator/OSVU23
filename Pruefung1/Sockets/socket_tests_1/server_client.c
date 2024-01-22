
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
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


#define PORT 9997
#define BUFFER_SIZE 2048

// Task declarations for server
void server_task_1(int *socked_fd);

void server_task_2(int socked_fd);

void server_task_3(int socked_fd);

void server_task_4(int socked_fd);

// Task declarations for client
void client_task_1(int *socked_fd);

void client_task_2();

void client_task_3();

void start_server();

void start_client();

/******************************************************************************
 * Prüfungsaufgabe: Einfache Client-Server-Kommunikation mit Sockets
 *
 * Ziel der Aufgabe ist es, grundlegende Netzwerkkommunikation mit Sockets in C
 * zu implementieren. Der Server wartet auf eine eingehende Verbindung und kann
 * Nachrichten empfangen. Der Client stellt eine Verbindung zum Server her und
 * kann Nachrichten senden. Diese Implementierung verwendet das TCP/IP-Protokoll.
 *
 * Task 1: Implementieren Sie die Erstellung und Initialisierung eines Sockets für den Server.
 *         Dies beinhaltet das Erstellen eines Socket-Deskriptors und das Einstellen
 *         von Socket-Optionen, falls notwendig.
 *
 * Task 2: Binden Sie den Server-Socket an eine IP-Adresse und einen Port. Das bedeutet,
 *         Sie weisen dem Server-Socket eine lokale Adresse zu, auf der er auf eingehende
 *         Verbindungen hören wird.
 *
 * Task 3: Konfigurieren Sie den Server-Socket, um auf eingehende Verbindungen zu warten.
 *         Dieser Schritt wandelt den Socket in einen passiven Socket um, der eingehende
 *         Verbindungsanfragen akzeptiert.
 *
 * Task 4: Akzeptieren Sie eine eingehende Verbindung. Warten Sie auf eine Verbindung
 *         von einem Client und akzeptieren Sie diese, um eine bidirektionale
 *         Kommunikation über einen neuen Socket zu ermöglichen.
 *
 * Task 5: Implementieren Sie die Erstellung und Initialisierung eines Sockets für den Client.
 *         Ähnlich wie beim Server müssen Sie hier einen Socket-Deskriptor erstellen.
 *
 * Task 6: Stellen Sie eine Verbindung vom Client-Socket zum Server her. Nutzen Sie hierfür
 *         die IP-Adresse und den Port des Servers, um eine Verbindung zu initiieren.
 *
 * Task 7: Senden Sie eine Nachricht vom Client an den Server. Nachdem die Verbindung
 *         etabliert wurde, kann der Client Daten senden, die der Server empfangen wird.
 *
 * Die Aufgaben sollen nacheinander ausgeführt werden, um eine erfolgreiche
 * Kommunikation zwischen dem Client und dem Server zu gewährleisten. Die genaue
 * Implementierung der einzelnen Tasks hängt von den spezifischen Anforderungen und
 * der Netzwerkkonfiguration ab.
 *
 *
 * Bitte beachten Sie, dass für die vollständige Implementierung der Sockets
 * weitere Funktionen und Fehlerbehandlung notwendig sind.
 *****************************************************************************/


/******************************************************************************
 * Prüfungsaufgabe: Einfache Client-Server-Kommunikation mit Sockets
 *
 * Die Aufgabe besteht darin, grundlegende Netzwerkkommunikation mit Sockets in C
 * zu implementieren. Der Server wartet auf eine eingehende Verbindung und empfängt
 * Nachrichten. Der Client verbindet sich mit dem Server und sendet eine Nachricht.
 *
 * Task 1: Implementieren Sie die Socket-Erstellung und Initialisierung für den Server.
 * Task 2: Binden Sie den Server-Socket an die richtige Adresse und Port.
 * Task 3: Bringen Sie den Server-Socket dazu, auf eingehende Verbindungen zu warten.
 * Task 4: Akzeptieren Sie eine eingehende Verbindung und lesen Sie die Nachricht.
 *
 * Task 5: Implementieren Sie die Socket-Erstellung und Initialisierung für den Client.
 * Task 6: Verbinden Sie den Client-Socket mit dem Server.
 * Task 7: Senden Sie eine Nachricht vom Client zum Server.
 *
 * Siehe auch: socket(2), bind(2), listen(2), accept(2), connect(2), recv(2), send(2)
 *****************************************************************************/
static int TERMINATE = EXIT_SUCCESS;
int socked_fd;
static struct addrinfo hints, *ai; //struct hints & struct adress info (ai) = res (result)


static void handle_signal(int signal) {
    printf("Terminate signal recieved\n");

    close(socked_fd);

    freeaddrinfo(ai);
    exit(TERMINATE);
}

int main(int argc, char *argv[]) {

    int choice = 0;
    printf("Wählen Sie den Modus: 1 für Server, 2 für Client: ");
    scanf("%d", &choice);

    struct sigaction sa = {.sa_handler = handle_signal};
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    if (choice == 1) {
        start_server();
    } else if (choice == 2) {
        start_client();
    } else {
        printf("Ungültige Auswahl.\n");
    }

    printf("Auswahl: %d\n", choice);
    return 0;
}


/******************************************************************************
 * Client-Side Tasks
 *****************************************************************************/
void start_client() {
    client_task_1(&socked_fd);
    fprintf(stdout, "task 1 success\n");

    client_task_2(&socked_fd);
    fprintf(stdout, "task 2 success\n");
    client_task_3(&socked_fd);
    fprintf(stdout, "task 3 success\n");
}

void client_task_1(int *socked_fd) {
    // Task 5: Implementieren Sie die Socket-Erstellung und Initialisierung für den Client.

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;


    int res = getaddrinfo(NULL, "9999", &hints, &ai);

    if (res < 0) {
        perror("socket res");
        exit(EXIT_FAILURE);
    }

    *socked_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (*socked_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
}

void client_task_2() {
    // Task 6: Verbinden Sie den Client-Socket mit dem Server.

    if (connect(socked_fd, ai->ai_addr, ai->ai_addrlen) < 0) {
        close(socked_fd);
        perror("connect");
        freeaddrinfo(ai);
        exit(EXIT_FAILURE);

    }
}

void client_task_3() {
    // Task 7: Senden Sie eine Nachricht vom Client zum Server.

    FILE *sockfile = fdopen(socked_fd, "w+");
    if (sockfile == NULL) {
        perror("Error opening filedescriptor");
        close(socked_fd);
        freeaddrinfo(ai);
        fclose(sockfile);
        exit(EXIT_FAILURE);
    }
    fprintf(sockfile, "das ist eine naaaaachricht\n");
    fflush(sockfile);


    close(socked_fd);
    freeaddrinfo(ai);
    fclose(sockfile);
}


/******************************************************************************
 * Server-Side Tasks
 *****************************************************************************/
void start_server() {


    server_task_1(&socked_fd);
    fprintf(stdout, "task 1 success\n");
    server_task_2(socked_fd);
    fprintf(stdout, "task 2 success\n");

    server_task_3(socked_fd);
    fprintf(stdout, "task 3 success\n");


    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);


    server_task_4(socked_fd);
    fprintf(stdout, "task 4 success\n");
}

void server_task_1(int *socked_fd) {
    // Task 1: Implementieren Sie die Socket-Erstellung und Initialisierung für den Server.

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, "9999", &hints, &ai);
    if (res != 0) exit(EXIT_FAILURE);


    *socked_fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (*socked_fd == -1) exit(EXIT_FAILURE);
}

void server_task_2(int socked_fd) {
    // Task 2: Binden Sie den Server-Socket an die richtige Adresse und Port.


    if (bind(socked_fd, ai->ai_addr, ai->ai_addrlen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

void server_task_3(int socked_fd) {
    // Task 3: Bringen Sie den Server-Socket dazu, auf eingehende Verbindungen zu warten.
    if (listen(socked_fd, 1) == -1) exit(EXIT_FAILURE);
}

void server_task_4(int socked_fd) {
    // Task 4: Akzeptieren Sie eine eingehende Verbindung und lesen Sie die Nachricht.
    while (TERMINATE == EXIT_SUCCESS) {

        printf("Waiting for connection...\n");
        int connection_fd = accept(socked_fd, NULL, NULL);
        if (connection_fd < 0) {
            printf("something went wrong... Continuing\n");
            continue;
        }

        printf("connection accepted\n");

        char buffer[BUFFER_SIZE];
        ssize_t read_size = read(connection_fd, buffer, BUFFER_SIZE - 1);
        if (read_size == -1) continue;

        buffer[read_size] = '\0';

        printf("Our message: ");
        printf("%s\n\n", buffer);

        close(connection_fd);
    }

    close(socked_fd);

}

