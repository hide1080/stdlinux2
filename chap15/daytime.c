#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int open_connection(char* host, char* service);

int main(int argc, char* argv[]) {

    int sock = open_connection((argc > 1 ? argv[1] : "localhost"), "daytime");
    FILE* f = fdopen(sock, "r");

    if (!f) {
        perror("fdopen(3)");
        exit(1);
    }

    char buf[1024];

    fgets(buf, sizeof(buf), f);
    fclose(f);
    fputs(buf, stdout);
    exit(0);
}

static int open_connection(char* host, char* service) {

    struct addrinfo hints, * res;
    int err;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((err = getaddrinfo(host, service, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo(3): %s\n", gai_strerror(err));
        exit(1);
    }

    struct addrinfo* ai;
    int sock;

    for (ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);

        if (sock < 0) {
            continue;
        }

        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            close(sock);
            continue;
        }

        // success
        freeaddrinfo(res);
        return sock;
    }

    fprintf(stderr, "socket(2)/connect(2) failed");
    freeaddrinfo(res);
    exit(1);
}

