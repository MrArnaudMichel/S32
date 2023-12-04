//
// Created by arnaud on 16/11/2023.
//


#include "client.h"


int main(int argc,char* argv[]){
    if (argc != 4) {
        printf("Usage: %s <hostname> <port> <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct addrinfo* res;
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;


    getaddrinfo(argv[1], argv[2], &hints, &res);

    if(res!=NULL) {
        int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sfd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        char* msg = argv[3];
        int err = sendto(sfd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen);
        if (err == -1) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        alarm(5);
        char buf[1024];
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        err = recvfrom(sfd, buf, 1023, 0, (struct sockaddr *) &addr, &addrlen);
        buf[err] = '\0';
        alarm(5);
        printf("> Message received from %s:%d | Message: %s\n", addr.ss_family == AF_INET6 ? "IPv6" : "IPv4", addr.ss_family == AF_INET6 ? ((struct sockaddr_in6 *) &addr)->sin6_port : ((struct sockaddr_in *) &addr)->sin_port, buf);

        freeaddrinfo(res);
        close(sfd);
    }
}