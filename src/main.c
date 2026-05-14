#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "utils.h"
int main(int argc, char *argv[]) {
    if (argc < 2){
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        printf ("Example: %s 192.168.1.1\n", argv[0]);
        return 1;
    }

    char target_ip[16];
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    char banner[BANNER_SIZE];
    

    //DNS resolution
    printf("Resolving hostname %s...\n", argv[1]);
    if (hostname_to_ip(argv[1], target_ip) < 0) {
        fprintf(stderr, "Failed to resolve hostname.\n");
        return 1;
    }

    printf("Scanning target : %s ...\n", target_ip);
    printf("%-7s | %-8s | %s\n", "PORT", "STATUS", "SERVICE/BANNER");
    printf("--------------------------------------------------\n");


    for (int p = start; p <= end; p++) {
        int status = check_port(target_ip, p, banner, BANNER_SIZE);
        
        if (status == PORT_OPEN) {
            printf("%-7d | OPEN     | %s\n", p, banner);
        }
    }

    printf("--------------------------------------------------\n");
    printf("Scan finished.\n");
    return 0;
}