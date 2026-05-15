#ifndef SCANNER_H
#define SCANNER_H

#define PORT_OPEN 1
#define PORT_CLOSED 0
#define PORT_ERROR -1
#define BANNER_SIZE 1024
#include <stdbool.h>
int check_port(const char *ip, int port, char *banner, int banner_size);

struct  MultiThreadingArgs {
    char ip[16];
    int port;
    char banner[BANNER_SIZE];
    int banner_size;
    bool port_status;
    bool is_udp;
};
void* scan_worker(void *args);

int check_udp_port(const char *ip, int port);
#endif // SCANNER_H