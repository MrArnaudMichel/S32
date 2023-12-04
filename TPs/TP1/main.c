//
// Created by arnaud on 23/11/2023.
//

#include "server.h"
#include "client.h"

int main(int argc, char *argv[]) {
    struct addrinfo hints;
    struct addrinfo *res;
    int err;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    err = getaddrinfo(NULL, "8080", &hints, &res);
    assert(err == 0);
    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert(sfd != -1);
    err = bind(sfd, res->ai_addr, res->ai_addrlen);
    assert(err != -1);
    err = listen(sfd, 10);
    assert(err != -1);
    int cfd = accept(sfd, NULL, NULL);
    assert(cfd != -1);
    char buf[1024];
    err = recv(cfd, buf, 1024, 0);
    assert(err != -1);
    printf("%s\n", buf);
    struct addrinfo *res2;
    err = getaddrinfo("localhost", "8080", NULL, &res2);
    assert(err == 0);
    int sfd2 = socket(res2->ai_family, res2->ai_socktype, res2->ai_protocol);
    assert(sfd2 != -1);
    err = connect(sfd2, res2->ai_addr, res2->ai_addrlen);
    assert(err != -1);
    char *msg = "Hello World!";
    err = send(sfd2, msg, strlen(msg), 0);
    assert(err != -1);
    return 0;
}