#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

int hostname_to_ip(const char *hostname, char *ip) {
    struct addrinfo hints, *res;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // Use IPv4
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "DNS error: %s\n", gai_strerror(status));
        return -1; // Error
    }

    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    void *addr = &(ipv4->sin_addr);

    inet_ntop(res->ai_family, addr, ip, INET_ADDRSTRLEN);

    freeaddrinfo(res); // Free the linked list
    return 0; // Success
}