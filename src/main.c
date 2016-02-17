#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "http2.h"

#define MYPORT "80"
#define BACKLOG 10

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, client_fd;
    int rv;

    memset(&hints, 0, sizeof hints);
    // IP version not specified
    hints.ai_family = AF_UNSPEC;
    // Stream socket
    hints.ai_socktype = SOCK_STREAM;
    // Fill up server IP automatically
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // Create socket
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    // Bind
    bind(sockfd, res->ai_addr, res->ai_addrlen);

    // Free memory
    freeaddrinfo(res);
   
    // Start listening
    listen(sockfd, BACKLOG);

    printf("Listenting.\n");

    // now accept an incoming connection:
    addr_size = sizeof their_addr;
    client_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
}