
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


#define PORT "9999"
#define BUFFER_SIZE 2048

// Task declarations for server
void server_task_1();

void server_task_2();

void server_task_3();

void server_task_4();

// Task declarations for client
void client_task_1();

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
 * Nachrichten. Der Client verbindet sich mit dem Server und sendet eine Nachricht "Nachricht123!".
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


static void handle_signal(int signal) {
    TERMINATE = !TERMINATE;
    printf("Terminate signal recieved\n");
}

void err(char *str) {
    printf("%s: ", str);
    perror("");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {

    int choice = 0;
    printf("Wählen Sie den Modus: 1 für Server, 2 für Client: ");
    scanf("%d", &choice);


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
    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGKILL, handle_signal);

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

    s = getaddrinfo(NULL, PORT, &hints, &ai);
    if (s != 0) err("info");

    sfd = socket(ai->ai_family, ai->ai_socktype,
                 ai->ai_protocol);
    if (sfd == -1) err("socket");

//    if (bind(sfd, ai->ai_addr, ai->ai_addrlen) != 0) err("bind");

    if (connect(sfd, ai->ai_addr, ai->ai_addrlen) < 0) err("connect");

    FILE *file = fdopen(sfd, "w+");
    fprintf(file, "Nachricht123!");
    fflush(file);

    char buffer[BUFFER_SIZE];
    fgets(buffer, BUFFER_SIZE, file);
    printf("Recieved: %s", buffer);

    fclose(file);
}


/******************************************************************************
 * Server-Side Tasks
 *****************************************************************************/
void start_server() {
    struct addrinfo hints;
    struct addrinfo *ai;

    signal(SIGTERM, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(SIGKILL, handle_signal);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int s = getaddrinfo(NULL, PORT, &hints, &ai);
    if (s != 0) err("addr info");

    int sfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sfd == -1) err("socket");

    if (bind(sfd, ai->ai_addr, ai->ai_addrlen) != 0) err("bind");
    if (listen(sfd, 0) < 0) err("listen");


    while (TERMINATE == EXIT_SUCCESS) {

        printf("Waiting for connection...\n");
        int afd = accept(sfd, NULL, NULL);
        if (afd < 0) continue;

        printf("connection accepted\n");
        char buffer[BUFFER_SIZE];
        size_t r = read(afd, buffer, BUFFER_SIZE);
        if (r < 0) continue;
        buffer[r] = '\0';

        printf("Received message: ");
        printf("%s\n", buffer);

        send(afd, buffer, BUFFER_SIZE, 0);
        close(afd);
    }

    close(sfd);
}
