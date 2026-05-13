#ifndef SCANNER_H
#define SCANNER_H

#define PORT_OPEN 1
#define PORT_CLOSED 0
#define PORT_ERROR -1
#define BANNER_SIZE 1024

int check_port(const char *ip, int port, char *banner, int banner_size);

#endif // SCANNER_H