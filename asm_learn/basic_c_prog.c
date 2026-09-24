#include<stdio.h>

int add(int a, int b);

int main(void) {

    int A = 5;
    int B = 7;

    int result = add(A,B);

    int result_sq = result * result;

    return 0;
}

int add(int a, int b) {

    int x = a;
    int y = b;

    int z = x+y;

    return z;
}
