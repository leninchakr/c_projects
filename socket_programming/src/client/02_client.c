#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdbool.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>

#define PORT 2200

bool send_all_msg_to_server(int to_server_fd, const char *full_msg);

int main(void) {

    /* Create a Socket to Server */
    int to_server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(to_server_fd == -1) {
        perror("socket-client");
        return -1;
    }

    /* Create a connection on 'to_server_fd' */
    
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int connect_state = connect(
                                    to_server_fd, 
                                    (const struct sockaddr *) &server_addr, 
                                    (socklen_t) sizeof(server_addr)
                                );
    if(connect_state == -1) {
        perror("Connect-Client");
        return -1;
    }

    /* Send Data to Server */
    const char send_msg_ptr[] = "Hello from Client!!!*";
    /*
    ssize_t sent_bytes = send(to_server_fd, send_msg_ptr, strlen(send_msg_ptr), MSG_NOSIGNAL);
    if(sent_bytes == -1) {
        perror("Send-Client");
        return -1;
    }
    */

    send_all_msg_to_server(to_server_fd, send_msg_ptr);

    close(to_server_fd);

    return 0;
}


bool send_all_msg_to_server(int to_server_fd, const char *full_msg) {

    size_t msg_len = strlen(full_msg);

    bool is_full_msg_sent = false;
    size_t char_sent_so_far = 0;

    while(!is_full_msg_sent) {

        if(char_sent_so_far >= msg_len) {
            is_full_msg_sent = true;
            continue;
        }

        ssize_t sent_bytes = send(
                                    to_server_fd, 
                                    &full_msg[char_sent_so_far], 
                                    (msg_len - char_sent_so_far),
                                    MSG_NOSIGNAL
                                );

        if(sent_bytes == -1) {
        
            if(errno == EINTR) {
                continue;
            }

            perror("Send-Client");
            return -1;
        }

        char_sent_so_far += sent_bytes;
    }

    return true;
}
