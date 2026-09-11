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

/* Linked List */
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

void add_node(char *data, Message_LL *ll);

bool send_all_2_server(int server_fd, Message_LL *ll);

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

    add_node("1-st Messge Apple*", &ll);
    add_node("2-nd Messge Ball*", &ll);
    add_node("3-rd Messge Chocolate*", &ll);
    add_node("4-th Messge SkyRoot*", &ll);

    send_all_2_server(server_fd, &ll);

    /* Send SHUT_WR after sending message to Server */
    shutdown(server_fd, SHUT_WR);

    //close(server_fd);
    
    return 0;
}

bool send_all_2_server(int  server_fd, Message_LL *ll) {

    MessageNode *curr_node = ll->head;

    while(curr_node != NULL) {

        char *temp = curr_node->data;
    
        // temp = &curr_node->data;
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
