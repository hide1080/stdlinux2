#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

static void do_grep(regex_t* pat, FILE* f, int invert);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    int opt;
    int flags = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    int invert = 0;

    while ((opt = getopt(argc, argv, "iv")) != -1) {
        switch (opt) {
            case 'i':
                flags |= REG_ICASE;
                break;
            case 'v':
                invert = REG_NOMATCH;
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-i -v]\n", argv[0]);
                exit(1);
        }
    }

    argc -= optind;
    argv += optind;

    regex_t pat;

    int err = regcomp(&pat, argv[0], flags);

    if (err != 0) {
        char buf[1024];

        regerror(err, &pat, buf, sizeof(buf));
        puts(buf);
        exit(1);
    }

    if (argc == 1) {
        do_grep(&pat, stdin, invert);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");

            if (!f) {
                perror(argv[i]);
                exit(1);
            }

            do_grep(&pat, f, invert);
            fclose(f);
        }
    }

    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t* pat, FILE* f, int invert) {
 
    char buf[4096];

    while (fgets(buf, sizeof(buf), f)) {
        if (regexec(pat, buf, 0, NULL, 0) == invert) {
            fputs(buf, stdout);
        }
    }
}

