//
// Created by arnaud on 04/12/23.
//

#include "server.h"

#include <time.h>

int main() {
    int err;
    struct addrinfo hints;
    struct addrinfo *res;
    char* path = "/siteTest";

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    err = getaddrinfo(NULL, "5002", &hints, &res);
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

        char* params = strtok(buf, "\n");


        FILE *fichier = fopen("file.html", "r");
        if (fichier == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            exit(EXIT_FAILURE);
        }

        fseek(fichier, 0, SEEK_END);
        long size = ftell(fichier);
        rewind(fichier);

        char entete[1024] = "\0";
        sprintf(entete, "HTTP/1.1 200 OK\nContent-Length: %ld \n\n", size);

        char *contenu_fichier = (char *)malloc(size + 1);
        if (contenu_fichier == NULL) {
            perror("Erreur d'allocation de mémoire");
            exit(EXIT_FAILURE);
        }

        fread(contenu_fichier, 1, size, fichier);
        contenu_fichier[size] = '\0';

        send(c, entete, strlen(entete), 0);

        ssize_t sent_bytes = send(c, contenu_fichier, size, 0);
        if (sent_bytes == -1) {
            perror("Erreur lors de l'envoi des données");
            exit(EXIT_FAILURE);
        }

        fclose(fichier);
        close(c);

        free(contenu_fichier);
    }
}
