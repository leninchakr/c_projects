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
#include<netinet/in.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>

#define PORT 2200
#define BUFFER_SIZE 256

// 1.Define Node
typedef struct MessageNode {
    char *data;
    struct MessageNode *next;
} MessageNode;

// 2.Define LL
typedef struct MessageList {
    MessageNode *head;
    MessageNode *tail;
    size_t count;
} MessageList;

void init_list(MessageList *list);
int add_node(MessageList *list, const char *msg);
void print_all_msg(const MessageList *list);

int recv_all(int conn_fd, MessageList *msgList);
int recv_own(int conn_fd, MessageList *ll);
int send_all(int conn_fd, MessageList *msgList);
void myFree(void *p);

int create_listen_socket(void);
int accept_client(int listen_fd);

int main(void) {

    int listen_fd = create_listen_socket();
    if(listen_fd == -1) {
        fprintf(stderr, "Unable to create listen socket...\n");
        return -1;
    }

    int connected_fd = accept_client(listen_fd);
    if(connected_fd == -1) {
        fprintf(stderr, "Unabled to hold on client connection...\n");
        return -1;
    }
        
    MessageList msgList;
    init_list(&msgList);

    //recv_all(connected_fd, &msgList);
    recv_own(connected_fd, &msgList);

    print_all_msg(&msgList);

    return 0;
}

int create_listen_socket(void) {

    // 1.Create Listen Socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1) {
        perror("Server: Unable to create Listen-Socket");
        return -1;
    }

    // 2.Bind the listen socket with Server Address
    struct sockaddr_in servAddr = {0};

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
        close(listen_fd);
        return -1;
    }
    
    // 3.Make it listen
    int listen_state = listen(listen_fd, 5);
    if(listen_state == -1) {
        perror("Sever: Unable to make the socket listen");
        close(listen_fd);
        return -1;
    }
    
    printf("Sever listening on port : %d....\n", PORT);

    return listen_fd;
}

int accept_client(int listen_fd) {

    // Wait for the clinet-socket connection + Accept it
    //  4.Accept the first connection
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &clientAddr.sin_addr);

    socklen_t clientAddr_len = sizeof(clientAddr);

    int connected_fd = accept(
                                listen_fd, 
                                (struct sockaddr *) &clientAddr, 
                                &clientAddr_len
                             );
    if(connected_fd == -1) {
        perror("Server: Unable to accept client socket");
        close(listen_fd);
        return -1;
    }

    return connected_fd;
}

int recv_all(int conn_fd, MessageList *msgList) {

    char *buf = malloc(BUFFER_SIZE);
    if(buf == NULL){
        perror("Malloc Failed");
        return -1;
    }

    bool isRecvComp = false;
    
    char *full_part_boundary = NULL;
    char *bal_part = NULL;

    while(!isRecvComp) {

        ssize_t recv_bytes = recv(conn_fd, buf, BUFFER_SIZE-1, 0);

        if(recv_bytes == -1) {
            perror("Server: Received Fialed");
            return -1;
        }

        if(recv_bytes == 0) {

            fprintf(stdout, "Server : Client said SHUT_WR\n");

            send_all(conn_fd, msgList);
            shutdown(conn_fd, SHUT_WR);

            isRecvComp = true;
        }

        /*
        recv()
        ↓
        append bytes to pending data
        ↓
        look for '*'
        ↓
        complete message?
        │
        ├── YES → add_node()
        │
        └── NO  → keep it for next recv()
        */

        if(recv_bytes > 0) {
            
            buf[recv_bytes] = '\0';

            /* Juicy Part - Start */
            size_t old_len = bal_part != NULL ? strlen(bal_part) : 0;
            size_t new_len = old_len + recv_bytes;

            char *temp_ptr  = realloc(bal_part, new_len+1);
            if(temp_ptr == NULL){
                perror("Memory Allocation Failed");
                return -1;
            }
            bal_part = temp_ptr;

            memcpy(bal_part + old_len, buf, recv_bytes);

            bal_part[new_len] = '\0';
            /* Juicy Part - End */

            // Find last Occurance of '*' and Split the buffer.
            full_part_boundary = strrchr(bal_part, '*');
            if(full_part_boundary != NULL) {

                *full_part_boundary = '\0';

                // Get the First-Token Address. Split till '\0'
                char *token = strtok(bal_part, "*");

                printf("---------- Token-Start ----------\n");
                while(token != NULL){
                    printf("Server-Token : %s\n", token);

                    // Add New node to linked list
                    add_node(msgList, token);

                    // Pass NULL to continue scannig the same string!
                    token = strtok(NULL, "*");
                }
                printf("---------- Token-End ----------\n");

                char *remaining = full_part_boundary+1;
                memmove(bal_part, remaining, strlen(remaining)+1);
            }

        }
    }

    free(buf);

    return 0;
}

// 3.Initialize List
void init_list(MessageList *list){
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

// 4.Add New Message-Node to the LL
int add_node(MessageList *list, const char *msg){
    
    MessageNode *new_node = malloc(sizeof(*new_node));
    if(new_node == NULL) {
        perror("Memory allocation filed\n");
        return -1;
    }

    new_node->data= strdup(msg);
    if(new_node->data == NULL) {
        perror("strdup");
        free(new_node);
        return -1;
    }

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
void print_all_msg(const MessageList *list){

    printf("---------- Server : Client's Message - Start--------------\n");

    MessageNode *curr_node = list->head;
    
    uint32_t line_no = 0;

    while(curr_node != NULL){
        line_no++;
        printf("recv #%d: %s\n", line_no, curr_node->data);
        curr_node = curr_node->next;
    }
    printf("---------- Server : Client's Message - End--------------\n");
}

int send_all(int conn_fd, MessageList *msgList) {

    MessageNode *curr_node = msgList->head;

    while(curr_node != NULL) {
    
        char *temp = curr_node->data;
        send(conn_fd, temp, strlen(temp), 0);

        char test = '$';
        send(conn_fd, &test, 1, 0);

        char eom = '!';
        send(conn_fd, &eom, 1, 0);

        curr_node = curr_node->next;
    }

    return 0;
}

/*
    recv_own()
 ├── receive bytes from socket
 ├── manage growing buffer
 ├── find message boundary '*'
 ├── tokenize messages
 ├── add messages to linked list
 └── manage memory
*/
int recv_own(int conn_fd, MessageList *ll) {

    // Step-3
    char *bal_msg __attribute__((cleanup(myFree))) = NULL;

    // Step-2
    char *temp __attribute__((cleanup(myFree))) = malloc(BUFFER_SIZE);

    //int part = 0;
    bool isRcom = false;

    while(!isRcom) {

        // Step-1
        ssize_t recv_bytes = recv(conn_fd, temp, BUFFER_SIZE, 0);

        /* For Test - Start */
        /*
        part++;

        if(part==1) {
            strcpy(temp, "Apple*Ball*Choco");
            recv_bytes = strlen(temp);
        }

        if(part==2) {
            strcpy(temp, "late*SkyROOT*");
            recv_bytes = strlen(temp);
            isRcom = true;
        }
        */
        /* For Test - End */

        if(recv_bytes == 0) {
            fprintf(stdout, "Server : Client said SHUT_WR\n");
            isRcom = true;
            continue;
        }

        if(recv_bytes == -1) {

            if(errno == EINTR){
                continue;
            }

            perror("Recv");
            return -1;
        }

        // Step-4
        ssize_t bal_len = bal_msg == NULL ? 0 : strlen(bal_msg);
        ssize_t new_len = bal_len + recv_bytes;

        // Step-5
        char *temp_loc = realloc(bal_msg, new_len+1);
        if(temp_loc == NULL) {
            perror("Memory Reallocation failed...\n");
            return -1;
        }
        bal_msg = temp_loc;
        if(bal_len == 0) {
            *bal_msg = '\0';
        }

        // Step-6
        //strcat(bal_msg, temp);
        memcpy(bal_msg+bal_len, temp, recv_bytes);
        *(bal_msg+new_len) = '\0';

        // Step-7
        char *bound = strrchr(bal_msg, '*');
        if(bound != NULL) {
            *bound = '\0';
        }

        // Step-8
        if(strlen(bal_msg) > 0) {

            char *token = strtok(bal_msg, "*");

            while(token != NULL) {
                add_node(ll, token);
                token = strtok(NULL, "*");
            }
        }

        // Step-9
        //strncpy(bal_msg, bound+1, strlen(bound+1));
        if(bound != NULL) {
            ssize_t remain_size = strlen(bound+1);
            memmove(bal_msg, bound+1, remain_size+1);
        }

        //printf("Final Balance : %s\n", bal_msg);
    }

    return 0;
}

void myFree(void *p) {
    void **pp = (void **)p;
    free(*pp);
    *pp = NULL;
}
