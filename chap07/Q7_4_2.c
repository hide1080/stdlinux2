#include <stdio.h>
#include <stdlib.h>

#define LINES 10
#define MAX_LINE_LENGTH 1024

static void tail(FILE* f);

int main(int argc, char* argv[]) {

    if (argc != 1) {
        fprintf(stderr, "Usage: %s < infile\n", argv[0]);
        exit(1);
    }

    tail(stdin);
    exit(0);
}

static char ring_buffer[LINES][MAX_LINE_LENGTH];

#define RING_BUFFER_INC(idx) do { \
    idx++; \
    if (idx >= LINES) { \
        idx = 0; \
    } \
} while (0)

static void tail(FILE* f) {

    long cur = 0;
    long read_lines = 0;

    while (fgets(ring_buffer[cur], MAX_LINE_LENGTH, f)) {
        RING_BUFFER_INC(cur);
        read_lines++;
    }

    if (read_lines < LINES) {
        cur = 0;
    }

    for (int n = LINES; n > 0 && read_lines; n--, read_lines--) {
        printf("%s", ring_buffer[cur]);
        RING_BUFFER_INC(cur);
    }
}
