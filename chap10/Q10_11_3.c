#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static void make_path(const char* path);
static void die(const char* s);
static void die2(const char* s);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        make_path(argv[i]); 
    }

    exit(0);
}

static void make_path(const char* path) {

    if (mkdir(path, 0777) == 0) {
        // OK
        return;
    }

    if (errno == EEXIST) {
        struct stat st;

        if (stat(path, &st) < 0)
            die("stat");

        if (!S_ISDIR(st.st_mode)) {
            char buf[1024];
            sprintf(buf, "file exists but is not a directory: %s\n", path);
            die2(buf);
        }

        // directory already exists
        return; 
    } else if(errno == ENOENT) {
        char* parent_path = strdup(path);

        if (!parent_path)
            die("strdup");

        char* last = parent_path + strlen(parent_path) - 1;

        while (*last == '/' && last != parent_path) {
            *last-- = '\0';
        }

        if (strcmp(parent_path, "/") == 0) {
            die2("error: root directory is not a directory???\n");
        }

        char* sep = strrchr(parent_path, '/');

        if (!sep) {
            // no slash found
            die2("error: current directory is not a directory???\n");
        } else if (sep == parent_path) {
            // e.g. "/something"
            die2("error: root directory is not a directory???\n");
        }

        // e.g. "/a/b/c" -> "/a/b\0c"
        *sep = '\0';

        make_path (parent_path);

        if (mkdir(path, 0777) < 0)
            die(path);

        return;
    } else {
        die(path);
    }
}

static void die(const char* s) {
    perror(s);
    exit(1);
}

static void die2(const char* s) {
    fprintf(stderr, "%s", s);
    exit(1);
}

