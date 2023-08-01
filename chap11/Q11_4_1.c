#include <stdio.h>
#include <stdlib.h>

static void tail(FILE* f, int nlines);
static unsigned char* readline(FILE* f);

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }

    tail(stdin, atoi(argv[1]));

    exit(0);
}

#define INC(ptrvar) do { \
    ptrvar++; \
    if (ptrvar >= ringbuf + nlines) \
        ptrvar = ringbuf; \
} while (0)

static void tail(FILE* f, int nlines) {

   unsigned char** ringbuf;
   unsigned char** p;
   unsigned char* line;

   if (nlines == 0)
       return;

    ringbuf = calloc(nlines, sizeof(char*));

    if (ringbuf == NULL)
        exit(1);

    p = ringbuf;

    while(line = readline(f)) {
        if (*p)
            free(*p);
        *p = line;
        INC(p);
    }

    if (*p == NULL)
        p = ringbuf;

    for (int n = nlines; n > 0 && *p; n--) {
        printf("%s", *p);
        free(*p);
        INC(p);
    }

    free(ringbuf);
}

static unsigned char* readline(FILE* f) {

    size_t buflen = BUFSIZ;
    unsigned char* buf, * p;

    buf = p = malloc(sizeof(char) * buflen);

    if (buf == NULL)
        exit(1);

    int c;

    for (;;) {
        c = getc(f);

        if (c == EOF) {
            if (buf == p) {
                free(buf);
                return NULL;
            }
            break;
        }

        *p++ = c;

        if (p >= buf + buflen - 1) {
            buflen *= 2;
            unsigned char* tmp = realloc(buf, buflen);
            if (tmp == NULL) {
                free(buf);
                exit(1);
            }
        }

        if (c == '\n')
            break;
    }

    *p++ = '\0';
    return buf;
}

