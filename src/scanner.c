#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int check_port(const char *ip, int port, int timeout) {
    int sock;
    struct sockaddr_in server; // struct for server address

    sock = socket(AF_INET, SOCK_STREAM, 0); // create socket

    if (sock < 0) {
        return PORT_ERROR; 
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port); 
    inet_pton(AF_INET, ip, &server.sin_addr); 

    // Set socket timeout
    if (connect (sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        close(sock);
        return PORT_CLOSED; 
    }
    close(sock);
    return PORT_OPEN;
}