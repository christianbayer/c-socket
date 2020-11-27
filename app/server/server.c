#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define BUFFER 1024

void openImage(char *filename) {
    char str[100];
    strcpy(str, "tiv ");
    strcpy(str, (strcat(str, filename)));
    system(str);
}

void saveFile(int sockfd) {
    int bytecount = 0;
    char buffer[BUFFER];
    char *filename;

    read(sockfd, buffer, BUFFER);
    filename = strdup(buffer);

    FILE *file = fopen(filename, "wb+");
    while ((bytecount = read(sockfd, buffer, BUFFER)) > 0) {
        fwrite(buffer, 1, bytecount, file);
    }
    fclose(file);
    openImage(filename);
}

void receiveFile() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nCould not open the socket!\n");
        exit(1);
    }
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("\nCould not bind!\n");
        exit(1);
    }
    if (listen(server_fd, 3) < 0) {
        perror("\nCould not listen!\n");
        exit(1);
    }
    if ((client_fd = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
        perror("\nCould not accept!\n");
        exit(1);
    }
    saveFile(client_fd);
    close(client_fd);
    close(server_fd);

    printf("\nFile received!\n");
}

int main() {
    receiveFile();
    exit(0);
}