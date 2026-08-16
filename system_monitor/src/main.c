#define _POSIX_C_SOURCE 200809L

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

#include "system.h"


int main(void) {
   
    // Memory Address (Pointer Data Type) type -> char *
	char hostname[256];
    char hostname_disp[256];
    
    MemoryInfo meminfo;

    int result_hostname =  gethostname(hostname, sizeof(hostname));

    if(result_hostname == 0) {

        snprintf(hostname_disp, sizeof(hostname_disp), "%s", hostname);

    } else {

        snprintf(hostname_disp, sizeof(hostname_disp), "Failed to get Hostname. Error : %s ",  strerror(errno));

    }

//    print_buffer_info(hostname, sizeof(hostname));

    // get_memory_info(&meminfo);

    get_memory_info_fd(&meminfo);

    printf("System Monitor\n");

    printf("----------------------\n");
    
    printf("Hostname \t\t\t: %s \n", hostname_disp);
    
    printf("Total Memeory \t\t\t: %lu kB \n", meminfo.total_kb);

    printf("Available  Memeory \t\t: %lu kB \n", meminfo.available_kb);

    printf("Used Memeory \t\t\t: %lu kB \n", meminfo.used_kb);


	return 0;

}
