#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 2200

int main(void) {

    // 1. Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd == -1) {
        perror("socket");
        return -1;
    }
    
    // 2. bind the socket
    struct sockaddr_in server_in = {0};

    server_in.sin_family = AF_INET;
    server_in.sin_port = htons(PORT);
    server_in.sin_addr.s_addr = inet_addr("127.0.0.1");

    int bind_state = bind(server_fd, (const struct sockaddr *) &server_in, sizeof(server_in));

    if(bind_state == -1) {
        perror("bind");
        return -1;
    }

    // 3. List on the Binding
    int listen_state = listen(server_fd, 1024);
    if(listen_state == -1) {
        perror("listen");
        return -1;
    }

    // 4. Accept the connection from client
    socklen_t sockLen = sizeof(server_in);

    int accept_fd = accept(server_fd, (struct sockaddr *) &server_in, &sockLen);
    if(accept_fd == -1) {
        perror("accept");
        return -1;
    }

    printf("Accepted the connection successfully. Now it is not blocked !\n");

    sleep(60);
    
    // Close all FD
    close(server_fd);
    close(accept_fd);

    return 0;
}
