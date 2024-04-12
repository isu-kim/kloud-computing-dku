#include "server.h"

/**
 * A function that starts listening on designated port and address
 * @return
 */
int serve() {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUM);
    server_addr.
}