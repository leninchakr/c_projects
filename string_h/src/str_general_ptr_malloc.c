#define _POSIX_C_SOURCE 200809L

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define BUFFER_SIZE 6

void myFree(void *ptr);

int main(void) {

    // ============================================================
    // C STRING BASICS - Pointer + Malloc()
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
    //     char *ptrstr[6] = malloc(6);
    //     strpcy(ptrstr, "Lenin");
    //
    // Memory:
    //
    //     [ L ][ e ][ n ][ i ][ n ][\0]
    //       0    1    2    3    4    5
    //
    // ============================================================

    // To Store "Lenin" (5-characters) , we need 6-bytes (BUFFER_SIZE+1)
    char *ptrstr __attribute__((cleanup(myFree))) = NULL;
    ptrstr = (char *) malloc(BUFFER_SIZE);
    if(ptrstr == NULL) {
        perror("Memory Allocation Failed!...\n");
    }
    strncpy(ptrstr, "InitV", BUFFER_SIZE);
    char temp[] = "Lenin";  

    printf("Initial Value : %s\n", ptrstr);

    // Way-1: strcpy()
    strcpy(ptrstr, temp);
    printf("After Re-assignment WAY-1 strcpy() : %s\n", ptrstr);

    strcpy(ptrstr, "Lenin Chakravarthy");
    printf("After Re-assignment WAY-1 strcpy() on Steroid: %s\n", ptrstr);

    printf("\n\n");

    // Way-2: 
    snprintf(ptrstr, BUFFER_SIZE, "%s", temp);
    printf("After Re-assignment WAY-2 snprintf() : %s\n",ptrstr);

    printf("\n\n");
   
    snprintf(ptrstr, BUFFER_SIZE, "%s", "Lenin Chakravarthy");
    printf("After Re-assignment WAY-2 snprintf() on Steroid: %s\n", ptrstr);

    printf("\n\n");

    // Way-3: memcpy()
    memcpy(ptrstr, temp, BUFFER_SIZE);
    printf("After Re-assignment WAY-3 memcpy(): %s\n", ptrstr);

    printf("\n\n");
   
    memcpy(ptrstr, "Lenin Chakravarthy", BUFFER_SIZE);
    printf("After Re-assignment WAY-3 mempcy() on Steroid: %s\n", ptrstr);

    printf("\n\n");
   
    memcpy(ptrstr, "Lenin Chakravarthy", BUFFER_SIZE + 3);
    printf("After Re-assignment WAY-3 mempcy() on Steroid: %s\n", ptrstr);

    printf("\n\n");

    // Way-4: Character Assignemnt
    for(int i=0; i<BUFFER_SIZE; i++){
        *(ptrstr + i) = temp[i];
    }
    printf("After Re-assignment WAY-4 Character-Assignment: %s\n", ptrstr);

    printf("\n\n");

    // Way-5: Pointer assingment
    char *dup_var __attribute__((cleanup(myFree)))= (char *) malloc(BUFFER_SIZE);
    if(dup_var == NULL){
        perror("Meory Allocation Failed....\n");
    }
    snprintf(dup_var, BUFFER_SIZE, "%s", "Apple");

    /* This creates 'Memory Leak'. Coz Original Address was lost + not freed! */
    ptrstr = dup_var;
    printf("After Re-assignment WAY-5 Pointer-Assignment: %s\n", ptrstr);

    printf("\n\n");
    
    // Way-6: calloc() + snprintf()
    char *ptr_with_init __attribute__((cleanup(myFree))) = calloc(BUFFER_SIZE, sizeof(char));
    if(ptr_with_init == NULL){
        perror("Memory Allocation failed");
    }
    snprintf(ptr_with_init, BUFFER_SIZE,"%s", "Very Long String, But Null Termination is guarintied");

    printf("After Re-assignment WAY-6 calloc(): %s\n", ptr_with_init);

    printf("\n\n");

    // Way-7: realloc() + snprintf()
    char *ptr_realloc = (char *) realloc(ptrstr, BUFFER_SIZE + 10);
    if(ptr_realloc == NULL) {
        fprintf(stderr, "Unable to re-allocate memory\n");
    }
    snprintf(ptr_realloc, BUFFER_SIZE+10, "%s", "LENINCHAKRAVART");

    printf("After Re-assignment WAY-7 re-alloc(): %s\n", ptr_realloc);
    
    printf("\n\n");
    
    // Way-8: strdup()
    char *ptr_dup __attribute__((cleanup(myFree))) = strdup("Some String with unknow size");
    if(ptr_dup == NULL) {
        fprintf(stderr, "Memory allocation failed for strdup()\n");
    }

    printf("After string duplication WAY-8 strdup(): %s\n", ptr_dup);

    printf("\n\n");

    // Way-9: memmove()
    char *ptr_mem_move = memmove(temp, ptr_dup, strlen(ptr_dup));
    printf("After memory-move WAY-9 memmove(): %s\n", ptr_mem_move);

    return 0;
}


void myFree(void *ptr) {

    void **pp = (void **)ptr;

    free(*pp);
    *pp = NULL;
}
