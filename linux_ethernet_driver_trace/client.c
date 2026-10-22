#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from VFS socket write!\n";

    // 1. Create socket file descriptor (Returns an integer FD handled by VFS)
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { printf("\n Socket creation error \n"); return -1; }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // 2. Connect to the local server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n"); return -1;
    }

    // 3. THE CRITICAL CALL: write() hitting the VFS socket layer
    write(sock, hello, strlen(hello));
    printf("Client sent data using write() syscall!\n");

    // 4. Close file descriptor
    close(sock);
    return 0;
}
