#ifndef SYSTEM_H
#define SYSTEM_H

#include<stddef.h>

typedef struct {
    
    unsigned long total_kb;
    unsigned long avaiale_kb;
    unsigned long used_kb;

}MemoryInfo;

void print_buffer_info(const char *buffer, size_t size);

int get_memory_info(MemoryInfo * memInfo);

#endif
