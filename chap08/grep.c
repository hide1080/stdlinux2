#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

static void do_grep(regex_t* pat, FILE* f);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fputs("no pattern\n", stderr);
        exit(1);
    }

    regex_t pat;

    int err = regcomp(&pat, argv[1], REG_EXTENDED | REG_NOSUB | REG_NEWLINE);

    if (err != 0) {
        char buf[1024];

        regerror(err, &pat, buf, sizeof(buf));
        puts(buf);
        exit(1);
    }

    if (argc == 2) {
        do_grep(&pat, stdin);
    } else {
        for (int i = 2; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");

            if (!f) {
                perror(argv[i]);
                exit(1);
            }

            do_grep(&pat, f);
            fclose(f);
        }
    }

    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t* pat, FILE* f) {
 
    char buf[4096];

    while (fgets(buf, sizeof(buf), f)) {
        if (regexec(pat, buf, 0, NULL, 0) == 0) {
            fputs(buf, stdout);
        }
    }
}

