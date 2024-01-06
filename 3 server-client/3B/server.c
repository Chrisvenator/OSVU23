// Header files
#include "headers.h"

#define ADDRESS_STRUCTURE struct sockaddr

// Chat functionality for server-client communication.
static void chatFunction(int socketDescriptor) {
    char buffer[MAX_BUFFER_SIZE];
    int count;
    // Continuous loop for message exchange.
    while (1) {
        memset(buffer, 0, MAX_BUFFER_SIZE);

        // Receive client message and store in buffer.
        read(socketDescriptor, buffer, sizeof(buffer));
        // Display client message.
        printf("Client says: %s\nReply: ", buffer);
        memset(buffer, 0, MAX_BUFFER_SIZE);
        count = 0;
        // Get server response.
        while ((buffer[count++] = getchar()) != '\n');

        // Send response back to client.
        write(socketDescriptor, buffer, sizeof(buffer));

        // "Exit" message ends the server process.
        if (strncmp("exit", buffer, 4) == 0) {
            printf("Closing server...\n");
            break;
        }
    }
}

// Main driver function.
int main() {
    int serverFd, clientFd, size;
    struct sockaddr_in serverAddr, clientAddr;

    // Create socket and check for errors.
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        printf("Failed to create socket...\n");
        exit(EXIT_FAILURE);
    } else
        printf("Socket created successfully.\n");
    memset(&serverAddr, 0, sizeof(serverAddr));

    // Set IP and PORT for the server.
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(DEFAULT_PORT);

    // Bind the socket and check for errors.
    if (bind(serverFd, (ADDRESS_STRUCTURE *) &serverAddr, sizeof(serverAddr)) != 0) {
        printf("Failed to bind socket...\n");
        exit(EXIT_FAILURE);
    } else
        printf("Socket successfully binded.\n");

    // Start listening for client connections.
    if (listen(serverFd, 5) != 0) {
        printf("Failed to listen...\n");
        exit(EXIT_FAILURE);
    } else
        printf("Listening for connections...\n");
    size = sizeof(clientAddr);

    // Accept client connections.
    clientFd = accept(serverFd, (ADDRESS_STRUCTURE *) &clientAddr, &size);
    if (clientFd < 0) {
        printf("Failed to accept client...\n");
        exit(EXIT_FAILURE);
    } else
        printf("Client connected.\n");

    // Initiate chat between server and client.
    chatFunction(clientFd);

    // Close the server socket.
    close(serverFd);
}
