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

    int result_hostname =  gethostname(hostname, sizeof(hostname));

    if(result_hostname == 0) {

        snprintf(hostname_disp, sizeof(hostname_disp), "%s", hostname);

    } else {

        snprintf(hostname_disp, sizeof(hostname_disp), "Failed to get Hostname. Error : %s ",  strerror(errno));

    }

    print_buffer_info(hostname, sizeof(hostname));

    printf("System Monitor\n");

    printf("----------------------\n");
    printf("Hostname \t\t: %s \n", hostname_disp);

	return 0;

}
