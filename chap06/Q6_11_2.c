#include <stdio.h>
#include <stdlib.h>

static void do_count(FILE *f);
static void die(const char *s);

int main(int argc, char *argv[]) {

    if (argc == 1) {
       do_count(stdin);
    } else {
        int i;
        for (i = 1; i < argc; i++) {
            FILE *f = fopen(argv[i], "r");
            if (!f) {
                die(argv[i]);
            }
            do_count(f);
            fclose(f);
        }
    }

    exit(0);
}

static void do_count(FILE *f) {

    int c;
    int prev = '\n';
    unsigned long cnt = 0;

    while ((c = fgetc(f)) != EOF) {
        if (c == '\n')
            cnt++;
        prev = c;
    }

    if (prev != '\n')
        cnt++;

    printf("%lu\n", cnt);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
