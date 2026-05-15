#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "scanner.h"
#include "utils.h"
int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: %s <IP_ADDRESS> <START_PORT> <END_PORT> [--udp]\n", argv[0]);
        printf("Example: %s 192.168.1.1 1 1000\n", argv[0]);
        printf("Example UDP: %s 192.168.1.1 1 1000 --udp\n", argv[0]);
        return 1;
    }

    char target_ip[16];
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    bool use_udp = false;

    if (argc == 5 && strcmp(argv[4], "--udp") == 0) {
        use_udp = true;    
    }
    //DNS resolution
    printf("Resolving hostname %s...\n", argv[1]);
    if (hostname_to_ip(argv[1], target_ip) < 0) {
        fprintf(stderr, "Failed to resolve hostname.\n");
        return 1;
    }

    int total_ports = end - start + 1;

    pthread_t *threads = malloc(sizeof(pthread_t) * total_ports);
    struct MultiThreadingArgs *mt_args = calloc(total_ports, sizeof(struct MultiThreadingArgs));

     if (threads == NULL || mt_args == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }


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
        mt_args[idx].is_udp = use_udp;
        pthread_create(&threads[idx], NULL, scan_worker, &mt_args[idx]);
    }
        
    for (int i = 0; i < total_ports; i++) {
        pthread_join(threads[i], NULL);
    }\

    //CSV output

    FILE *csv_file = fopen("scan_results.csv", "w");
    if (csv_file == NULL) {
        fprintf(stderr, "Could not open file for writing.\n");
        return 1;
    }
    fprintf(csv_file, "Port,Status,Banner\n");
    for (int i = 0; i < total_ports; i++) {
        if (mt_args[i].port_status){
            int len = strlen(mt_args[i].banner);
            while (len > 0 && (mt_args[i].banner[len - 1] == '\n' || mt_args[i].banner[len - 1] == '\r')) {
                mt_args[i].banner[--len] = '\0';
            }
            fprintf(csv_file, "%d,OPEN,\"%s\"\n", mt_args[i].port, mt_args[i].banner);
        } else {
            fprintf(csv_file, "%d,CLOSED,\"%s\"\n", mt_args[i].port, mt_args[i].banner);
        
        }
    }
    fclose(csv_file);

    free(threads);
    free(mt_args);

    printf("--------------------------------------------------\n");
    printf("Scan finished.\n");
    return 0;
}