#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>

#define PORT 2200

int main(void) {

    // create a socket to connected with server
    int to_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(to_server_fd == -1){
        perror("socket-client");
        return -1;
    }

    // Define which server to connect with
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Connect with Server
    int connect_state = connect(
                            to_server_fd, 
                            (const struct sockaddr *) &server_addr, 
                            sizeof(server_addr)
                            );

    if(connect_state == -1) {
        perror("connect");
        return -1;
    }
    
    char server_msg[256] = {0};
    ssize_t recv_bytes = recv(to_server_fd, server_msg, sizeof(server_msg)-1, 0);


    if(recv_bytes == -1) {

        if(errno == EINTR){
            continue;
        }

        perror("Receive-Client");
        return -1;

    } else if(recv_bytes == 0) {

        printf("Connection is closed by Server..!!\n");
        return -1;

    } else {
        server_msg[recv_bytes] = '\0';
        printf("Received Message: %s\n", server_msg);
    }
    
    // close the socket
    close(to_server_fd);

    return 0;
}
