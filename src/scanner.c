#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

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