#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdbool.h>

#define PORT 2200

bool recv_full(int to_server_fd, char *server_msg, ssize_t server_msg_size);

int main(void) {

    // Socket
    int to_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(to_server_fd == -1){
        perror("client-socket");
        return -1;
    }

    // Create Server Address Config
   struct sockaddr_in server_addr = {0};

   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(PORT);

   int conversion_state = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); 

   if(conversion_state == -1) {
       perror("conversion failed - Invalid Address Family");
       return -1;
   } else if(conversion_state == 0) {
       fprintf(stderr, "Invalid Source Address");
       return -1;
   } else if(conversion_state == 1) {
       fprintf(stdout, "Coversion is successful..\n");
   }

   // Sent this socket to Server
   int connect_state = connect(
                            to_server_fd, 
                            (const struct sockaddr *) &server_addr,
                            (socklen_t) sizeof(server_addr)
                            );

   if(connect_state == -1){
       perror("connect()");
       return -1;
   }

   char server_msg[256] = {0};

   recv_full(to_server_fd, server_msg, sizeof(server_msg));

   printf("Received Message : %s\n", server_msg);  

   return 0;
}

/*
1. If buffer has no room:
       return false

2. Call recv()

3. If recv() == -1:
       if errno == EINTR:
           continue
       otherwise:
           return false

4. If recv() == 0:
       server closed connection
       return false

5. Scan all received bytes for '!'

6. If '!' found:
       replace '!' with '\0'
       return true

7. Otherwise:
       increase received-byte count
       continue
*/
bool recv_full(int to_server_fd, char *server_msg, ssize_t server_msg_size) {

    char end_of_msg = '!';

    ssize_t temp_recv_bCount = 0;

    bool isRecvComplete = false;

    while(!isRecvComplete) {

        /* Buffer Size Verification */
        /* Exit Condition-1 */
        if(temp_recv_bCount >= server_msg_size-1) {
            fprintf(stderr, "Receive Buffer Size is exceeded...\n");
            return false;
        }

        ssize_t recv_bytes = recv(
                                    to_server_fd, 
                                    (server_msg + temp_recv_bCount), 
                                    (server_msg_size - temp_recv_bCount - 1),
                                    0
                                );

        /* Exit Condition-1 */
        if(recv_bytes == -1) {

            if(errno == EINTR){
                continue;
            }

            isRecvComplete = true;
            
            perror("Recv-Client");
            return false;
        }

        if(recv_bytes == 0) {

            fprintf(stderr, "Server closed the connection...\n");
            return false;
        }

        for(int i=0; i < recv_bytes; i++) {

           char curr_char = *(server_msg + temp_recv_bCount + i);

            if(curr_char == end_of_msg){
                *(server_msg + temp_recv_bCount + i) = '\0';

                /* Exit Condition-1 */
                isRecvComplete = true;
                break;
            }
        }

        temp_recv_bCount += recv_bytes;

    }

    return true;
}
