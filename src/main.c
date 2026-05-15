#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "scanner.h"
#include "utils.h"
int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: %s <IP_ADDRESS> <START_PORT> <END_PORT>\n", argv[0]);
        printf ("Example: %s 192.168.1.1 1 1000\n", argv[0]);
        return 1;
    }

    char target_ip[16];
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    //DNS resolution
    printf("Resolving hostname %s...\n", argv[1]);
    if (hostname_to_ip(argv[1], target_ip) < 0) {
        fprintf(stderr, "Failed to resolve hostname.\n");
        return 1;
    }

    int total_ports = end - start + 1;

    pthread_t *threads = malloc(sizeof(pthread_t) * total_ports);
    struct MultiThreadingArgs *mt_args = malloc(sizeof(struct MultiThreadingArgs) * total_ports);


    if (threads == NULL || mt_args == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    printf("Scanning target : %s ...\n", target_ip);
    printf("%-7s | %-8s | %s\n", "PORT", "STATUS", "SERVICE/BANNER");
    printf("--------------------------------------------------\n");

    for (int p = start; p <= end; p++) {
        int idx = p - start;
        strncpy(mt_args[idx].ip, target_ip, sizeof(mt_args[idx].ip) - 1);
        mt_args[idx].port = p;
        mt_args[idx].banner_size = BANNER_SIZE;
        pthread_create(&threads[idx], NULL, scan_worker, &mt_args[idx]);
    }
        
    for (int i = 0; i < total_ports; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(mt_args);

    printf("--------------------------------------------------\n");
    printf("Scan finished.\n");
    return 0;
}