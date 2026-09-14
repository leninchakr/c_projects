#include<stdio.h>
#include<string.h>

int main() {


    // C creates a string literal:For => char *init = "Lenin";
    /*
        Memory (Read-Only) somewhere in the program
              |
              v
         +---+---+---+---+---+----+
         | L | e | n | i | n | \0 |
         +---+---+---+---+---+----+
           5000 5001 5002 5003 5004 5005
    */
    /*
         init
            +-------------+
            | 0x5000 (ro) |
            +-------------+
    */
    /*
        "Lenin"
           |
           | address (Read-only) of first character
           v
        0x5000
     */

    char *ptrarr = NULL; 
    char *init = "Lenin";
    char temp[] = "Apple-Ball";
    
    // Way-1: Pointer-Assignment
    // ptrarr -> Read-only Memory Location
    ptrarr = init;
    printf("After Re-assignment WAY-1 Assignment Operator : %s\n", ptrarr);

    // Way-2: strcpy()
    // We cannot modify Read-only Memory Location (Segmentation Fault!)
    printf("Now Segmentation Fault will occur!...\n");
    strcpy(ptrarr, temp);
    printf("After Re-assignment WAY-1 strcpy() : %s\n", ptrarr);

    return 0;
}
