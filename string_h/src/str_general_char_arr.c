#include<stdio.h>
#include<string.h>

int main(void) {
    
    // ============================================================
    // C STRING BASICS
    // ============================================================
    //
    // String = An array of characters ending with '\0'
    //
    // Example:
    // "Lenin" → 'L', 'e', 'n', 'i', 'n', '\0'
    //
    // ------------------------------------------------------------
    // IMPORTANT
    // ------------------------------------------------------------
    //
    // A C string MUST have '\0' (NULL character) at the end.
    //
    // "Lenin" contains:
    //
    //     5 characters
    //   + 1 byte for '\0'
    //   -----------------
    //     6 bytes
    //
    // Therefore:
    //
    //     BUFFER_SIZE = CHARACTER_COUNT + 1 = 5+1 = 6
    //
    // Example:
    //
    //     char strarr[6] = "Lenin";
    //
    // Memory:
    //
    //     [ L ][ e ][ n ][ i ][ n ][\0]
    //       0    1    2    3    4    5
    //
    // ============================================================
    
    // To Store "Lenin" (5-characters) , we need 6-bytes (BUFFER_SIZE+1)
    //  [ 0 ][ 0 ][ 0 ][ 0 ][ 0 ][ 0 ] 
    char strarr[6] = {0};
    char temp[] = "Apple";

    printf("Before initialization : %s\n", strarr);

    // ❌ invalid
    // strarr = temp;

    printf("----- strcpy() -----\n");
    // Way-1: strcpy()
    strcpy(strarr, temp);
    printf("After Re-assignment WAY-1 strcpy() : %s\n", strarr);

    strcpy(strarr, "Lenin Chakravarthy");
    printf("After Re-assignment WAY-1 strcpy() on Steroid: %s\n", strarr);

    printf("\n\n");
   
    // Way-2: snprintf()
    snprintf(strarr, sizeof(strarr), "%s", temp);
    printf("After Re-assignment WAY-2 snprintf() : %s\n", strarr);

    printf("\n\n");
   
    snprintf(strarr, sizeof(strarr), "%s", "Lenin Chakravarthy");
    printf("After Re-assignment WAY-2 snprintf() on Steroid: %s\n", strarr);

    printf("\n\n");
   
    // Way-3: memcpy()
    memcpy(strarr, temp, 6);
    printf("After Re-assignment WAY-3 memcpy(): %s\n", strarr);

    printf("\n\n");
   
    memcpy(strarr, "Lenin Chakravarthy", 6);
    printf("After Re-assignment WAY-3 mempcy() on Steroid: %s\n", strarr);

    printf("\n\n");
   
    memcpy(strarr, "Lenin Chakravarthy", 19);
    printf("After Re-assignment WAY-3 mempcy() on Steroid: %s\n", strarr);

    printf("\n\n");
   
    // Way-4: Character-Assignment
    strarr[0] = 'A';
    strarr[1] = 'p';
    strarr[2] = 'p';
    strarr[3] = 'l';
    strarr[4] = 'e';
    strarr[5] = '\0';
    printf("After Re-assignment WAY-4 Character-Assignmen): %s\n", strarr);


    return 0;
}
