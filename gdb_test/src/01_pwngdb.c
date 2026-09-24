#include<stdio.h>

int add(int a, int b);

int main(void) {

    int m = 7;
    int n = 5;

    int p = add(m,n);

    printf("Add : %d\n", p);

    return 0;
}


int add(int a, int b) {

    int c = a+b;

    return c;

}
