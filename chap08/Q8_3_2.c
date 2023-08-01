#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

static void do_slice(regex_t* pat, FILE* f);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    int opt;
    int flags = REG_EXTENDED | REG_NEWLINE;

    while ((opt = getopt(argc, argv, "i")) != -1) {
        switch (opt) {
            case 'i':
                flags |= REG_ICASE;
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
        do_slice(&pat, stdin);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");

            if (!f) {
                perror(argv[i]);
                exit(1);
            }

            do_slice(&pat, f);
            fclose(f);
        }
    }

    regfree(&pat);
    exit(0);
}

static void do_slice(regex_t* pat, FILE* f) {
 
    char buf[4096];

    while (fgets(buf, sizeof(buf), f)) {
        regmatch_t matched[1];

        if (regexec(pat, buf, 1, matched, 0) == 0) {
            char* s = buf + matched[0].rm_so;
            regoff_t len = matched[0].rm_eo - matched[0].rm_so;
            fwrite(s, len, 1, stdout);
            fputc('\n', stdout);
        }
    }
}

