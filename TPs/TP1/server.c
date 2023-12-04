//
// Created by arnaud on 16/11/2023.
//
#include "server.h"

int main(int argc, char *argv[]) {
    // UDP server
    struct addrinfo hints;
    struct addrinfo *res;
    int err;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, argv[1], &hints, &res);
    assert(err == 0);
    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert(sfd != -1);
    err = bind(sfd, res->ai_addr, res->ai_addrlen);
    assert(err != -1);
    char buf[1024];
    while (1){
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        err = recvfrom(sfd, buf, 1023, 0, (struct sockaddr *) &addr, &addrlen);
        buf[err] = '\0';
        printf("> Message received from %s:%d | Message: %s\n", addr.ss_family == AF_INET6 ? "IPv6" : "IPv4", addr.ss_family == AF_INET6 ? ((struct sockaddr_in6 *) &addr)->sin6_port : ((struct sockaddr_in *) &addr)->sin_port, buf);
        char *msg = "Message received!\0";
        sendto(sfd, msg, strlen(msg), 0, (struct sockaddr *) &addr, addrlen);
    }
    freeaddrinfo(res);
    return 0;
}