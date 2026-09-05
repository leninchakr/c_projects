/*
    Refer "01_proto"
*/
#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#define PORT 2200
#define BUFFER_SIZE 256

// 1.Define Node
typedef struct MessageNode {
    char data[BUFFER_SIZE];
    struct MessageNode *next;
} MessageNode;

// 2.Define LL
typedef struct MessageList {
    MessageNode *head;
    MessageNode *tail;
    size_t count;
} MessageList;

// 3.Initialize List
void init_list(MessageList *list){
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

// 4.Add New Message-Node to the LL
int add_node(MessageList *list, const char *msg){
    
    MessageNode *new_node = (MessageNode *)malloc(sizeof(MessageNode));
    if(!new_node){
        perror("Memory allocation filed\n");
        return -1;
    }

    strncpy(new_node->data, msg, BUFFER_SIZE-1);
    new_node->data[BUFFER_SIZE] = '\0';
    new_node->next = NULL;

    if(list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->count++;

    return 0;
}

// 5.Print LL
void print_all_msg(MessageList *list){

    printf("---- Full Messages ------\n");

    MessageNode *curr_node = list->head;
    
    uint32_t line_no = 0;

    while(curr_node->next != NULL){
        line_no++;
        printf("%d. %s\n", line_no, curr_node->data);
        curr_node = curr_node->next;
    }
    printf("------ Done ----------\n");
}

int recv_all(int conn_fd, MessageList *msgList);
int send_all(int conn_fd, MessageList *msgList);

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
        
    MessageList msgList = {0};

    recv_all(connected_fd, &msgList);

    print_all_msg(&msgList);

    return 0;
}

int recv_all(int conn_fd, MessageList *msgList) {

    char end_of_each_msg = '*';

    char temp[BUFFER_SIZE] = {0};
    
    bool isRecvComp = false;

    while(!isRecvComp) {

        ssize_t recv_bytes = recv(conn_fd, temp, BUFFER_SIZE-1, 0);

        if(recv_bytes == -1) {
            perror("Server: Received Fialed");
            return -1;
        }

        if(recv_bytes == 0) {
            send_all(conn_fd, msgList);
            fprintf(stdout, "Peer said SHUT_WR\n");
            isRecvComp = true;
            return 0;
        }

        for(int i=0; i < recv_bytes; i++) {
            if(temp[i] == end_of_each_msg) 
                temp[recv_bytes] = '\0';
        }

        add_node(msgList, temp);

        memset(temp, 0, sizeof(temp));

    }


    return 0;
}


int send_all(int conn_fd, MessageList *msgList) {

    MessageNode *curr_node = msgList->head;

    while(curr_node->next != NULL) {
    
        char temp[BUFFER_SIZE] = curr_node->data;
        
        send(conn_fd, temp, BUFFER_SIZE, 0);

        curr_node = curr_node->next;
    }

    return 0;
}
