#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/ip.h>
#include<arpa/inet.h>

#define PORT 2200

// POSIX Socket Lifecyle : Socket -> Bind -> Listen -> Accept -> Recv -> Close

int main(void) {
    
    // 1. create Socket
    int server_fd = socket(AF_INET, SOCK_STREAM,0);

    if(server_fd == -1) {
        perror("socket");
        return -1;
    }

    // 2. Bind the address & port to Socker
    struct sockaddr_in in_addr = {0};

    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(PORT);
    // in_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ip_addr_coversion_state = inet_pton(AF_INET, "127.0.0.1", &in_addr.sin_addr);

    if(ip_addr_coversion_state != 1) {
        perror("ip conversion");
        return -1;
    }

    int bind_state = bind(server_fd, (const struct sockaddr *) &in_addr, sizeof(in_addr));

    if(bind_state == -1) {
        perror("bind");
        return -1;
    }

    // 3. Make it listening Socket
    int listen_state = listen(server_fd, 1024);

    if(listen_state == -1) {
        perror("listen");
        return -1;
    }

    //  4. Accept Connection
    int size_of_in_addr = sizeof(in_addr);

    int accept_fd = accept(server_fd, (struct sockaddr *) &in_addr, (socklen_t *) &size_of_in_addr);

    if(accept_fd == -1) {
        perror("accept");
        return -1;
    }

    printf("accept() block is released...\n");
    
    // 5. Receive msg from the accepted socket
    char buffer[256];

    ssize_t received_bytes = recv(accept_fd, buffer, sizeof(buffer)-1, 0);

    if(received_bytes > 0 ) {
        buffer[received_bytes] = '\0';
        printf("The Received Data is %s \n", buffer);
    }else if(received_bytes == 0) {
        fprintf(stderr, "Client Closed Connection...\n");
    } else {
        perror("recv");
    }

    close(accept_fd);
    close(server_fd);

    return 0;

}


