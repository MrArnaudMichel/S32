//
// Created by arnaud on 04/12/23.
//

#include "server.h"

int main() {
    int err;
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, "8458", &hints, &res);
    assert(err == 0);
    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert(sfd != -1);
    err = bind(sfd, res->ai_addr, res->ai_addrlen);
    assert(err != -1);
    listen(sfd, 5);
    int c;
    char buf[1024];
    while (1) {
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        c = accept(sfd, (struct sockaddr *) &addr, &addrlen);
        err = recv(c, buf, 1023, 0);
        buf[err] = '\0';
        printf("%s\n", buf);
        char *msg = "Anto est pas bo";
        send(c, msg, strlen(msg), 0);
        close(c);
    }

}