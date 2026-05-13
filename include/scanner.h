#ifndef SCANNER_H
#define SCANNER_H

#define PORT_OPEN 1
#define PORT_CLOSED 0
#define PORT_ERROR -1

int check_port(const char *ip, int port, int timeout);

#endif // SCANNER_H