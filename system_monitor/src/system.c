#include<stdio.h>

#include "system.h"

// buffer, hostname both points to same memory location.
// with hostname varaible, we can modify the values
// but, with const, we cant modify the value VIA 'Buffer' in same memory.
// Important: Cont prevent this vairable to modify it. It will not make actual memory location Immutable !

void print_buffer_info(const char *buffer, size_t size) {

    printf("Inside test() => Buffer Address/Pointer :  %p\n", (void *)buffer);
    printf("Inside test() => Buffer size :  %zu\n", size);
    printf("Inside test() => Buffer content %s\n", buffer);

    // testing 'const' keyword !
    //  *(buffer + 1) = 'R';

    printf("Inside test() => Modified 2nd Position value %s\n", buffer);

    // For Testing ! => Compiler Warning. Bypassing 'const' Security !!!
    // Give writable-pointer :|
// EXPERIMENT:
// buffer is const char *, so modification through buffer is prohibited.
//
// char *ptr = buffer;
// This discards the const qualifier and GCC warns about it.
//
// We are intentionally doing this here for learning.
    char *ptr = buffer;

    *(ptr+2) = '4';

    printf("Inside test() => Modified 2nd Position value %s\n", ptr);
}

// Get Memeory Related Information
int get_memory_infor(MemoryInfo * meminfo){

    File *file = fopen("proc/meminfor", "r");

    if(file == NULL) {
        
            return -1;

    }

    char line[256];

    unsigned long total = 0;
    unsigned long avaialbe = 0;
    
}
