//
// Created by arnaud on 04/12/23.
//

#include "server.h"

#include <time.h>

int main() {
    int err;
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    err = getaddrinfo(NULL, "5023", &hints, &res);
    assert(err == 0);

    int sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert(sfd != -1);

    err = bind(sfd, res->ai_addr, res->ai_addrlen);
    assert(err != -1);

    listen(sfd, 1);

    int c;
    char buf[1024];

    while (1) {
        struct sockaddr_storage addr;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        c = accept(sfd, (struct sockaddr *) &addr, &addrlen);
        err = recv(c, buf, 1023, 0);
        buf[err] = '\0';
        printf("%s\n", buf);

        char *p = strstr(buf, "/");
        printf("%s\n", p);
        char *q = strstr(p, " ");
        if (q == NULL) {
            exit(EXIT_FAILURE);
        }

        size_t size_file = q - p;
        char filename[size_file + 1];
        memcpy(filename, p, size_file);
        filename[size_file] = '\0';
        printf("%s\n", filename);

        char filepath[1024];
        filepath[0] = '\0';
        strcat(filepath, "./siteTest");
        strcat(filepath, filename);
        printf("%s \n", filepath);

        FILE *fichier = fopen(filepath, "r");
        if (fichier == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        fseek(fichier, 0, SEEK_END);
        long size = ftell(fichier);
        rewind(fichier);

        char buffer[1024];
        sprintf(buffer, "HTTP/1.1 200 OK\nContent-Length: %ld \n\n", size);
        send(c, buffer, strlen(buffer), 0);

        int nbread;
        while ((nbread = fread(buffer, 1, 1023, fichier)) > 0) {
            //usleep(500000);
            ssize_t sent_bytes = send(c, buffer, nbread, 0);
            if (sent_bytes == -1) {
                perror("Erreur lors de l'envoi des données");
                break;
            }
        }
        fclose(fichier);
        close(c);
    }
    return 0;
}
