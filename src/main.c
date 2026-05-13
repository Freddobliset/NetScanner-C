#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char *argv[]) {
    if (argc < 2){
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        printf ("Example: %s 192.168.1.1\n", argv[0]);
        return 1;
    }

    char *target_ip = argv[1];

    int common_ports[] = {21, 22, 23, 25, 53, 80, 110, 143, 443, 3306};
    int num_ports = sizeof(common_ports) / sizeof(common_ports[0]);

    printf("Scanning %s for common ports...\n", target_ip);
    for (int i = 0; i < num_ports; i++) {
        int port = common_ports[i];
        int result = check_port(target_ip, port, 1000); 
        if (result == PORT_OPEN) {
            printf("Port %d is OPEN\n", port);
        } else if (result == PORT_CLOSED) {
            printf("Port %d is CLOSED\n", port);
        } else {
            printf("Error checking port %d\n", port);
        }
    }
    printf("Scan complete.\n");
    return 0;
}