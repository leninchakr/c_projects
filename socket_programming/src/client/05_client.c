/*
    Refer "01_proto":

CLIENT                                      SERVER
  |                                           |
  | "1-st Message Apple*"                     |
  | "2-nd Message Ball*"                      |
  | "3-rd Message Chocolate*"                 |
  | "4-th Message SkyRoot*"                   |
  |                                           |
  | shutdown(SHUT_WR)                         |
  |------------------------------------------>|
  |                                           |
  |                                  recv() == 0
  |                                  "Peer said SHUT_WR"
  |                                           |
  |                                  traverse MessageList
  |                                           |
  |<--------- message 1 --------------------- |
  |<--------- message 2 --------------------- |
  |<--------- message 3 --------------------- |
  |<--------- message 4 --------------------- |
  |                                           |
  |                                  shutdown(SHUT_WR)
  |<------------------------------------------|
  |                                           |
  | recv() == 0                               |
  | "Peer closed connection..."               |
  |                                           |

*/

#define _DEFAULT_SOURCE

#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>

#define PORT 2200
#define BUFFER_SIZE 256

/* Linked List - Start */
struct MessageNode {
    char *data;
    struct MessageNode *next;
};
typedef struct MessageNode MessageNode;

struct Message_LL {
    MessageNode *head;
    MessageNode *tail;
    int count;
};
typedef struct Message_LL Message_LL;
/* Linked List - Start */

void add_node(char *data, Message_LL *ll);
void print_ll(Message_LL *ll);

bool send_all_2_server(int server_fd, Message_LL *ll);
int recv_from_server(int server_fd, Message_LL * recv_msg_ll);

void myFree(void *ptr);

int main(void) {

   /* Create a socket */
   int server_fd = socket(AF_INET, SOCK_STREAM, 0);
   if(server_fd == -1){
       perror("Client: Socket");
       return -1;
   }

    /* Server Address Config */
    struct sockaddr_in serv_addr = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    /* connect to Server */
    int connect_state = connect(
                                server_fd, 
                                (const struct sockaddr *) &serv_addr, 
                                sizeof(serv_addr)
                                );
    if(connect_state == -1){
        perror("Client-Connect");
        return -1;
    }

    Message_LL ll = {0};

    add_node("Apple*", &ll);
    add_node("Ball*", &ll);
    add_node("Chocolate*", &ll);
    add_node("SkyRoot*", &ll);

    send_all_2_server(server_fd, &ll);

    /* Send SHUT_WR after sending message to Server */
    shutdown(server_fd, SHUT_WR);

    /* Receive Client Message */
    Message_LL recv_msg_ll = {0};

    recv_from_server(server_fd, &recv_msg_ll);

    close(server_fd);

    return 0;
}

bool send_all_2_server(int  server_fd, Message_LL *ll) {

    MessageNode *curr_node = ll->head;

    while(curr_node != NULL) {

        char *temp = curr_node->data;
    
        strcpy(temp, curr_node->data);
        
        ssize_t send_bytes = send(server_fd, temp, strlen(temp), 0);

        if(send_bytes == -1) {
            perror("Clinet: send");
            return false;
        }else if(send_bytes == 0) {
            fprintf(stderr, "Server disconnected connection....\n");
            return false;
        }else {
            curr_node = curr_node->next;
        }
    }

    return true;
}


void add_node(char *data, Message_LL *ll){

    MessageNode *a_node = (MessageNode *) malloc(sizeof(MessageNode));
    
    a_node->data = strdup(data);
    a_node->next = NULL;

    if(ll->count ==0) {
        ll->head = a_node;
        ll->tail = a_node;
        ll->count = 1;
    } else {
        ll->tail->next = a_node;
        ll->tail = a_node;
        ll->count++;
    }

}

int recv_from_server(int server_fd, Message_LL * recv_msg_ll) {

    bool isRecvComp = false;

    char *temp __attribute__((cleanup(myFree))) = malloc(BUFFER_SIZE);
    if(temp == NULL){
        perror("Memeory Allocation Failed :(...");
        return -1;
    }

    while(!isRecvComp) {

        ssize_t recv_bytes = recv(server_fd, temp, BUFFER_SIZE - 1, 0);

        if(recv_bytes == -1){

            if(errno == EINTR){
                continue;
            }

            perror("Client-Recv");
            return -1;
        }

        if(recv_bytes == 0) {
            printf("Server said SHUT_WR\n");
            printf("\n");
            isRecvComp = true;
            print_ll(recv_msg_ll);
            continue;
        }

        temp[recv_bytes] = '\0';

        if(recv_bytes > 0) {

            printf("---------- Token-Start ----------\n");

            char *tok = strtok(temp, "!");
            while(tok != NULL) {

                printf("Client-Token : %s\n", tok); 

                add_node(tok, recv_msg_ll);
                tok = strtok(NULL, "!");
            }

            printf("---------- Token-End ----------\n");
            printf("\n");
        }
    
    }

    return 0;
}

void myFree(void *ptr){

    /* Generic-Void-Pointer to Pointer-to-Pointer */
    void **pp = (void **) ptr;

    free(*pp);
    *pp = NULL;
}

void print_ll(Message_LL *ll) {

    MessageNode *curr_node = ll->head;

    uint32_t lno = 1;

    if(ll->count > 0) {

        printf("---------- Client : Server's Message - Start--------------\n");
        
        while(curr_node != NULL) {
            printf("recv #%d: %s\n", lno, curr_node->data);
            curr_node = curr_node->next;
            lno++;
        }

        printf("---------- Client : Server's Message - End--------------\n");
    }
}
