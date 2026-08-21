#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>

#define PORT 2200

int main(void) {

    // Create Listen-Socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1){
        perror("Socket");
        return -1;
    }

    // Bind the Listen Socket to Server
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", (struct sockaddr_in *) &serv_addr.sin_addr);

    int bind_state = bind(listen_fd, (const struct sockaddr *) &serv_addr, (socklen_t) sizeof(serv_addr));
    if(bind_state == -1) {
        perror("Bind");
        return -1;
    }

    // Make It Listen
    int listen_state = listen(listen_fd, 2);
    if(listen_state == -1) {
        perror("Listen");
        return -1;
    }

    // Accept a incoming clinet connection
    socklen_t size_of_serv_addr = sizeof(serv_addr);
    int connected_fd = accept(listen_fd, (struct sockaddr *) &serv_addr, &size_of_serv_addr);
    if(connected_fd == -1) {
        perror("Accept");
        return -1;
    }

    printf("New Client-Request discovered...\n");

    char *msg_from_server = "Hello from Server!";

    ssize_t sent_bytes = send(connected_fd, msg_from_server, strlen(msg_from_server),0);
    if(sent_bytes == -1) {
        perror("Send");
        return -1;
    }

    char final_msg[256] = {0};
    uint32_t char_pos = 0;

    bool is_full_client_msg = false;

    char end_of_msg = '*';

    while(!is_full_client_msg) {
    
        char temp_msg[9] = {0};

        ssize_t recv_bytes = recv(connected_fd, temp_msg, sizeof(temp_msg), 0);

        if(recv_bytes > 0) {

            for(int i = 0; i < recv_bytes; i++) {
            
                if(temp_msg[i] == end_of_msg){
                    
                    final_msg[char_pos] = '\0';

                    is_full_client_msg = true;
                    break;
                } else {

                    if(char_pos >= sizeof(final_msg)-1) {
                        is_full_client_msg = true;
                        break;
                    }

                    final_msg[char_pos] = temp_msg[i];
                    char_pos++;
                }
            }

        } else if(recv_bytes == 0) {
            printf("Client disconnected the connection...\n");
            break;
        } else {
            perror("Recv");
            break;
        }

    }

    printf("\nClient Message :\n");
    printf("%s\n", final_msg);

    close(connected_fd);
    close(listen_fd);

    return 0;
}
