#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_cat(FILE* f, int escape);
static void put_char(int c);

int main(int argc, char* argv[]) {

    int opt;
    int escape = 0;

    while ((opt = getopt(argc, argv, "eh")) != -1) {
        switch (opt) {
            case 'e':
                escape = 1;
                break;
            case 'h':
                fprintf(stdout, "Usage: %s [-e] [file...]\n", argv[0]);
                exit(0);
            case '?':
                fprintf(stderr, "Usage: %s [-e] [file...]\n", argv[0]);
                exit(1);
        }
    }

    argc -= optind;
    argv += optind;

    if (argc == 0) {
        do_cat(stdin, escape);
    } else {
        for (int i = 0; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");

            if (!f) {
                perror(argv[i]);
                exit(1);
            }

            do_cat(f, escape);
            fclose(f);
        }
    }

    exit(0);
}

static void do_cat(FILE* f, int escape) {

    int c;

    if (escape) {
        while ((c = fgetc(f)) != EOF) {
            switch (c) {
                case '\t':
                    put_char('\\');
                    put_char('t');
                    break;
                case '\n':
                    put_char('$');
                    put_char('\n');
                    break;
                default:
                    put_char(c);
            }
        }
    } else {
        while ((c = fgetc(f)) != EOF) {
            put_char(c);
        }
    }
}

static void put_char(int c) {
    if (putchar(c) < 0)
        exit(1);
}

