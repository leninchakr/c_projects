#ifndef SYSTEM_H
#define SYSTEM_H

#include<stddef.h>

typedef struct {
    
    unsigned long total_kb;
    unsigned long available_kb;
    unsigned long used_kb;

}MemoryInfo;

void print_buffer_info(const char *buffer, size_t size);

int get_memory_info(MemoryInfo * memInfo);

/* File Discriptor Version */
int get_memory_info_fd(MemoryInfo * memInfo);

#endif
