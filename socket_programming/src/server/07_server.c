/*
    Refer "01_proto"
*/
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>

#define PORT 2200

int recv_all(int conn_fd, char *recv_buf, size_t recv_buf_len);

int main(void) {

    // 1.Create Listen Socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        perror("Server: Unable to create Listen-Socket");
        return -1;
    }

    // 2.Bind the listen socket with Server Address
    struct sockaddr_in servAddr;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);
    
    int bind_status = bind(
                            listen_fd, 
                            (const struct sockaddr *) &servAddr, 
                            sizeof(servAddr)
                           );
    if(bind_status == -1) {
        perror("Server: Uable to bind the listen socket to server");
        return -1;
    }
    
    // 3.Make it listen
    int listen_state = listen(listen_fd, 5);
    if(listen_state == -1) {
        perror("Sever: Unable to make the socket listen");
        return -1;
    }
    
    printf("Sever listening on port : %d....\n", PORT);

    // Wait for the clinet-socket connection + Accept it
    //  4.Accept the first connection
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "1270.0.0.1", &clientAddr.sin_addr);

    socklen_t clientAddr_len = sizeof(clientAddr);

    int connected_fd = accept(
                                listen_fd, 
                                (struct sockaddr *) &clientAddr, 
                                &clientAddr_len
                             );
    if(connected_fd == -1) {
        perror("Server: Unable to accept client socket");
        return -1;
    }

    char recv_msg[256] = {0};

    recv_all(connected_fd, recv_msg, sizeof(recv_msg));

    printf("Client Message: %s\n", recv_msg);

    return 0;
}




int recv_all(int conn_fd, char *recv_buf, size_t recv_buf_len) {

    char end_of_client_msg = '*';

    ssize_t recv_bytes = recv(conn_fd, recv_buf, recv_buf_len, 0);

    if(recv_bytes == -1) {
        perror("Server: Received Fialed");
        return -1;
    }

    if(recv_bytes == 0) {
        fprintf(stdout, "Peer said SHUT_WR\n");
        return 0;
    }

    for(int i=0; i < recv_bytes; i++) {
    
      if(recv_buf[i] == end_of_client_msg) 
          recv_buf[recv_bytes] = '\0';
    
    }

    return 0;
}
