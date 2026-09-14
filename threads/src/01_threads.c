#include<stdio.h>

void myTurn(){
    while(1){
        sleep(5);
        printf("My Turn..\n");
    }
}

void yourTurn() {
    while(1){
        sleep(5);
        printf("Your Turn..\n");
    }
}

int main(void) {

    pthread_t newThread;

    pthread_create(&newThread, NULL, myTurn, NULL);

    printf("Running App...!\n");

    return 0;
}
