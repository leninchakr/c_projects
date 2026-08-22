#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 2200

int main(void) {

    /* Create a Socket to Server */
    int to_server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* Create a connection on 'to_server_fd' */
    
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    int connect_state = connect(to_server_fd, (const struct sockaddr *) &server_addr, (socklen_t) sizeof(server_addr));
    if(connect_state == -1) {
        perror("Connect-Client");
        return -1;
    }

    /* Send Data to Server */
    const char *send_msg_ptr = "Hello from Client!!!*";
    ssize_t sent_bytes = send(to_server_fd, send_msg_ptr, sizeof(*send_msg_ptr), MSG_NOSIGNAL);
    if(sent_bytes == -1) {
        perror("Send-Client");
        return -1;
    }

    return 0;
}
