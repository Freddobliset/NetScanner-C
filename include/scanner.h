#ifndef SCANNER_H
#define SCANNER_H

#define PORT_OPEN 1
#define PORT_CLOSED 0
#define PORT_ERROR -1
#define BANNER_SIZE 1024
#include <stdbool.h>
#include <pthread.h>
int check_port(const char *ip, int port, char *banner, int banner_size);
void* scan_worker(void *args);
int check_udp_port(const char *ip, int port);

typedef struct {
    int port;
    bool is_open;
    char banner[BANNER_SIZE];
} PortResult;

typedef struct {
    char target_ip[16];
    int start_port;
    int end_port;
    int current_port;
    bool is_udp;
    pthread_mutex_t *mutex;
    PortResult *results; // Array to store results for each port
    int results_count;
} PortQueue;

typedef struct {
    PortQueue *queue;
    char banner[BANNER_SIZE];
} WorkerArgs;

void* ts_worker_pool(void *args);
#endif // SCANNER_H