#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int check_port(const char *ip, int port,char *banner, int banner_size){
    int sock;
    struct sockaddr_in server; // struct for server address
    struct timeval tv; // struct for timeout

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0); // create socket

    if (sock < 0) {
        return PORT_ERROR; 
    }

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port); 
    inet_pton(AF_INET, ip, &server.sin_addr); 

    // Set socket timeout
    if (connect (sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return PORT_CLOSED; 
    }

    // --- BANNER GRABBING LOGIC ---

    const char *http = "GET / HTTP/1.0\r\n\r\n";
    send(sock, http, strlen(http), 0); 
    memset(banner, 0, banner_size);

    int bytes = recv(sock, banner, banner_size - 1, 0);

    if (bytes > 0) {
        for (int i = 0; i < bytes; i++) {
            if (banner[i] < 32 || banner[i] > 126) {
                banner[i] = '.';
            }
        }
    } else {
        strncpy(banner, "No banner received", banner_size - 1);
    }
    close(sock);
    return PORT_OPEN;
}

int check_udp_port(const char *ip, int port) {
    int sock;
    struct sockaddr_in server;
    struct timeval tv;

    tv.tv_sec = 1;
    tv.tv_usec = 0;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        return PORT_ERROR; 
    }

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return PORT_CLOSED; 
    }
    const char *probe = "Hello";
    if (send(sock, probe, strlen(probe), 0) < 0) {
        close(sock);
        return PORT_CLOSED; 
    }
    char buffer[1024];
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

    close(sock);
    if (bytes > 0) {
        return PORT_OPEN;
    } else {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            return PORT_OPEN;
        }
        return PORT_CLOSED;
    }
}

/* void* scan_worker(void *args) {
    struct MultiThreadingArgs *mt_args = (struct MultiThreadingArgs *)args; // cast void* to struct pointer
    int status;
    if(mt_args->is_udp){
        status = check_udp_port(mt_args->ip, mt_args->port);
    } else {
        status = check_port(mt_args->ip, mt_args->port, mt_args->banner, mt_args->banner_size);
    }

    if (status == PORT_OPEN) {
        mt_args->port_status = true;
        if(!mt_args->is_udp && strlen(mt_args->banner) > 0){
        printf("%-7d | OPEN     | %s\n", mt_args->port, mt_args->banner);
        } else {
            printf("%-7d | OPEN     | No banner\n", mt_args->port);
        }
    } else {
        mt_args->port_status = false;
        printf("%-7d | CLOSED   | %s\n", mt_args->port, mt_args->banner);
    }
    return NULL;
} */

void* ts_worker_pool(void *args) {
    WorkerArgs *worker_args = (WorkerArgs *)args;
    PortQueue *queue = worker_args->queue;
    char banner[BANNER_SIZE];
    int status;
    PortResult res;
    while (1){
        int port_to_scan = -1;

        pthread_mutex_lock(queue->mutex);

        if (queue->current_port > queue->end_port) {
            pthread_mutex_unlock(queue->mutex);
            break; 
        }
        port_to_scan = queue->current_port;
        queue->current_port++;
        pthread_mutex_unlock(queue->mutex);

        if(queue->is_udp){
            status = check_udp_port(queue->target_ip, port_to_scan);
            if(status == PORT_OPEN){
                printf("%-7d | OPEN     | No banner\n", port_to_scan);
            }
        } else {
            banner[0] = '\0';
            status = check_port(queue->target_ip, port_to_scan, banner, BANNER_SIZE);
            if(status == PORT_OPEN){
                if(strlen(banner) > 0){
                    printf("%-7d | OPEN     | %s\n", port_to_scan, banner);
                } else {
                    printf("%-7d | OPEN     | No banner\n", port_to_scan);
                }
            }
        }

        res.port = port_to_scan;
        res.is_open = (status == PORT_OPEN);
        strncpy(res.banner, banner, BANNER_SIZE - 1);
        res.banner[BANNER_SIZE - 1] = '\0';

        pthread_mutex_lock(queue->mutex);
        queue->results[queue->results_count++] = res;
        pthread_mutex_unlock(queue->mutex);

    }
    free(worker_args);
    return NULL;
}