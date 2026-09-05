#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 2200
#define BUFFER_SIZE 256

int main(void) {

   /* Create a socket */
   int server_fd = socket(AF_INET, SOCK_STREAM, 0);
   if(listen_fd == -1){
       perror("Sever: Socket");
       return -1;
   }

    /* Server Address Config */
    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr);

    int conncet_state = connect(server_fd, &serv_addr, sizeof(serv_addr));
    if(connect_state == -1){
        perror("Client-Connect");
        return -1;
    }

    close(server_fd);

    return 0;
}
