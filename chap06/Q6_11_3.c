#include <stdio.h>
#include <stdlib.h>

static void do_cat(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s: file name not given\n", argv[0]);
        exit(1);
    }

    int i;
    for (i = 1; i < argc; i++) {
        do_cat(argv[1]);
    }

    exit(0);
}

#define BUFFER_SIZE 2048

static void do_cat(const char *path) {

    unsigned char buf[BUFFER_SIZE];
    FILE *f = fopen(path, "r");

    if (!f)
        die(path);

    for (;;) {
        size_t nr = fread(buf, 1, sizeof buf, f);
        if (ferror(f))
            die(path);
        size_t nw = fwrite(buf, 1, nr, stdout);
        if (nw < nr)
            die(path); 
        if (nr < sizeof buf)
            break; 
    }

    if (fclose(f) != 0)
        die(path);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}

