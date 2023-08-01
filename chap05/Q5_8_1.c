#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE* f);
static void die(const char* s);

int main(int argc, char* argv[]) {

    int i;

    if (argc < 2) {
        do_cat(stdin);
    } else {
        for (i = 1; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                die(argv[i]);
            }
            do_cat(f);
            fclose(f);
        }
    }

    exit(0);
}

static void do_cat(FILE* f) {

    int c;

    while ((c = fgetc(f)) != EOF) {
        if (putchar(c) < 0)
            die("Error occured in putchar(c)).");
    }
}

static void die(const char* s) {
    perror(s);
    exit(1);
}

