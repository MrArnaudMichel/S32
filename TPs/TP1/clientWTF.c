//
// Created by arnaud on 23/11/2023.
//

#include "server.h"

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
        // receive a file
        FILE *fp = fopen("received.txt", "w");
        if(fp == NULL){
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        while(1){
            err = recvfrom(sfd, buf, 1023, 0, (struct sockaddr *) &addr, &addrlen);
            buf[err] = '\0';
            if(strcmp(buf, "EOF") == 0){
                break;
            }
            fprintf(fp, "%s", buf);
        }
        fclose(fp);
        alarm(5);

        freeaddrinfo(res);
        close(sfd);
    }
}