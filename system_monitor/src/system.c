#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

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
    printf("====================================================\n\n");
}

// Get Memeory Related Information
int get_memory_info(MemoryInfo * meminfo){

    FILE *file = fopen("/proc/meminfo", "r");

    if(file == NULL) {
        fprintf(stderr, "Failed to opne /proc/meminfo : %s \n", strerror(errno));
        return -1;
    }

    char line[256];

    unsigned long total = 0;
    unsigned long  available = 0;

    while(fgets(line, sizeof(line), file) != NULL) {

        if(sscanf(line, "MemTotal: %lu kB", &total) == 1){
            continue;
        }

        if(sscanf(line, "MemAvailable: %lu kB", &available) == 1) {
            continue;
        }
    }
    
    fclose(file);

    meminfo->total_kb = total;
    meminfo->available_kb = available ;
    meminfo->used_kb = total - available;

    return 0;
    
}

int get_memory_info_fd(MemoryInfo * meminfo){

    char buffer[4096];

    int fd = open("/proc/meminfo", O_RDONLY);

    printf("File Descriptor for /proc/meminfo : %d \n", fd);

    if(fd == -1) {
        fprintf(stderr, "Failed to open /proc/meminfo : %s \n", strerror(errno));
        return -1;
    }

    unsigned long total = 0;
    unsigned long  available = 0;
    
    ssize_t read_bytes = read(fd, buffer, sizeof(buffer)-1);
    
    if(read_bytes > 0){
        
        buffer[read_bytes] = '\0';

        printf("Bytes Read : %zd\n", read_bytes);

        char *result = strstr(buffer, "MemTotal:");

        if(result != NULL) {
            if(sscanf(result, "MemTotal: %lu", &total) == 1){
                fprintf(stdout, "We have received the total memeory : %lu kB \n", total);
            }
        }

       result = strstr(buffer, "MemAvailable:");

       if(result != NULL){
            if(sscanf(result, "MemAvailable: %lu", &available) == 1) {

                fprintf(stdout, "We have received the Available Memroryi %lu kB \n", available);
            }
       }

       meminfo->total_kb = total;
       meminfo->available_kb = available;
       meminfo->used_kb = total - available;

    } else if(read_bytes == 0) {
    
        printf("No data available.\n");
        
        close(fd);

        return -1;

    } else {
        
        fprintf(stderr, "Failed to read file : %s \n", strerror(errno));

        close(fd);

        return -1;
    }
    
    if(total == 0 || available == 0) {

        fprintf(stderr, "Unable to get Total & Available Memory...\n");

        close(fd);

        return -1;

    }

    close(fd);
    
    return 0;
}
