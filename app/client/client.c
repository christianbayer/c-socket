#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <libgen.h>

#define ADDRESS "127.0.0.1"
#define PORT 8080
#define BUFFER 1024

void sendFile(FILE *fp, char filePath[100]) {
    int client_fd, byteCount = 0;
    char buffer[BUFFER];
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nCould not open the socket!\n");
        exit(1);
    }
    if (inet_pton(AF_INET, ADDRESS, &serv_addr.sin_addr) < 0) {
        printf("\nInvalid server address!\n");
        exit(1);
    }
    if (connect(client_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nCould not connect to the server.\n");
        exit(1);
    }

    strcpy(buffer, basename(strdup(filePath)));
    send(client_fd, buffer, BUFFER, 0);

    while ((byteCount = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        send(client_fd, buffer, byteCount, 0);
    }

    fclose(fp);
    close(client_fd);

    printf("\nFile sent!\n");
}

int main(void) {
    FILE *fp;
    char filePath[100];

    printf("Enter the file: ");
    gets(filePath);

    if ((fp = fopen(filePath, "rb+")) == NULL) {
        printf("Could not send the file %s\n", filePath);
        exit(1);
    }

    sendFile(fp, filePath);
    exit(0);
}