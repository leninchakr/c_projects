#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<string.h>

#define PORT 2200

int main(void) {

    // 1. Create Listening Socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_fd == -1){
        perror("socket");
        return -1;
    }

    // 2. Bind this listen socket
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int bind_state = bind(listen_fd, (const struct sockaddr *) &serv_addr, (socklen_t) sizeof(serv_addr) );
    
    if(bind_state == -1) {
        perror("bind");
        return -1;
    }

    // 3. Make the socket Listening-Socket
    int listen_state = listen(listen_fd, 3);

    if(listen_state == -1) {
        perror("listen");
        return -1;
    }

    // 4. Accept connection from queue from Listen-socket
    socklen_t serv_addr_size = sizeof(serv_addr);
    
    int connected_fd = accept(listen_fd, (struct sockaddr *) &serv_addr,&serv_addr_size);

    if(connected_fd == -1) {
        perror("accept");
        return -1;
    }

    // 5. Read from Kernal buffer for the connected_fd
   char message[10] = {0};
   char final_msg[256] = {0};
   unsigned int char_pos = 0;

   char end_of_msg = '*';

   bool is_end_of_transaction = false;

   while(!is_end_of_transaction) {

       ssize_t n_recv_bytes = recv(connected_fd, message, sizeof(message)-1, 0);

        /* Assume Kernal Receive Buffer : Some* */
       if(n_recv_bytes > 0) {

            for(int i=0; i < (int) n_recv_bytes; i++){
                
                if(message[i] == end_of_msg){
                    final_msg[char_pos] = '\0';
                    is_end_of_transaction = true;
                    break;
                } else {
                    final_msg[char_pos] = message[i];
                    char_pos++;
                }

            }

       } else if(n_recv_bytes == 0) {
           printf("Client closed the connection...\n");
           break;
       } else {
           perror("recv");
           break;
       }

   }

    printf("Received Message From Client : %s \n", final_msg);

    return 0;
}
