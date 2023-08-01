#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

static void do_head(FILE* f, long n);

#define DEFAULT_N_LINES 10

static struct option longopts[] = {
    {"lines", required_argument, NULL, 'n'},
    {"help",  no_argument,       NULL, 'h'},
    {0,       0,                 0,     0}
};

int main(int argc, char* argv[]) {

    int opt;
    long n = DEFAULT_N_LINES;

    while ((opt = getopt_long(argc, argv, "hn", longopts, NULL)) != -1) {
        switch (opt) {
            case 'n':
                n = atol(optarg);
                break;
            case 'h':
                fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
                exit(0);
            case '?':
                fprintf(stdout, "Usage: %s [-n LINES] [FILE ...]\n", argv[0]);
                exit(1);
        }
    }

    if (optind == argc) {
        do_head(stdin, n);
    } else {
        for (int i = optind; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_head(f, n);
            fclose(f);
        }
    }

    exit(0);
}

static void do_head(FILE* f, long n) {

    int c;

    if (n <= 0)
        return;

    while ((c = getc(f)) != EOF) {
        if (putchar(c) < 0)
            exit(1);
        if (c == '\n') {
            n--;
            if (n == 0)
                return;
        }
    }
}
