#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "scanner.h"
#include "utils.h"
#define  POOL_SIZE 100
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

    pthread_mutex_t my_lock;
    if (pthread_mutex_init(&my_lock, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    int total_ports = end - start + 1;
    PortResult *results = malloc(sizeof(PortResult) * total_ports);
    if (!results) {
        perror("Failed to allocate memory for results");
        return 1;
    }


    PortQueue queue;
    strncpy(queue.target_ip, target_ip, sizeof(queue.target_ip) - 1);
    queue.start_port = start;
    queue.end_port = end;
    queue.current_port = start;
    queue.is_udp = use_udp;
    queue.mutex = &my_lock;
    queue.results = results;
    queue.results_count = 0;
    if (pthread_mutex_init(&my_lock, NULL) != 0) {
        perror("Mutex init failed");
        return 1;
    }

    printf("Starting scan on %s from port %d to %d (%s)\n", target_ip, start, end, use_udp ? "UDP" : "TCP");
    printf("%-7s | %-8s | %s\n", "Port", "Status", "Banner");
    printf("--------------------------------------------------\n");

    pthread_t pool_threads[POOL_SIZE];

    for (int i = 0; i < POOL_SIZE; i++) {
        WorkerArgs *worker_args = malloc(sizeof(WorkerArgs));
        if (worker_args == NULL) {
            perror("Failed to allocate memory for worker arguments");
            return 1;
        }
        worker_args->queue = &queue;

        if (pthread_create(&pool_threads[i], NULL, ts_worker_pool, worker_args) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < POOL_SIZE; i++) {
        pthread_join(pool_threads[i], NULL);
    }

    //CSV output

   FILE *csv_file = fopen("scan_results.csv", "w");
    if (csv_file == NULL) {
        fprintf(stderr, "Could not open file for writing.\n");
        return 1;
    }
    fprintf(csv_file, "Port,Status,Banner\n");
    for (int i = 0; i < queue.results_count; i++) {
        PortResult *res = &queue.results[i];
        int len = strlen(res->banner);
        while (len > 0 && (res->banner[len - 1] == '\n' || res->banner[len - 1] == '\r')) {
            res->banner[--len] = '\0';
        }
        fprintf(csv_file, "%d,%s,%s\n", res->port, res->is_open ? "OPEN" : "CLOSED", res->banner);

    }
    fclose(csv_file); 
    free(results);
    pthread_mutex_destroy(&my_lock);
    printf("--------------------------------------------------\n");
    printf("Scan finished.\n");
    return 0;
}