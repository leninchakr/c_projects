#ifndef SERVER_H
#define SERVER_H

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

// State Machine for Receive
typedef enum {
    RECV_DATA,
    RECV_PEER_CLOSED,
    RECV_RETRY,
    RECV_ERROR
} RecvState;

// Result Set of Recv Function
typedef struct {
    RecvState state;
    ssize_t bytes;
} Recv_ResultSet;

void init_list(MessageList *list);
int add_node(MessageList *list, const char *msg);
void print_all_msg(const MessageList *list);

int recv_all(int conn_fd, MessageList *ll);
int send_all(int conn_fd, MessageList *msgList);
void myFree(void *p);

int create_listen_socket(void);
int accept_client(int listen_fd);
Recv_ResultSet one_read_recv(int conn_fd, char *temp);
int prepare_bal_data(char **bal_msg, char *temp, ssize_t bytes);
int tokenize_add_node(char *bal_msg, MessageList *ll, char **bound);
void recalculate_bal_data(char *bal_msg, char *bound);

#endif
