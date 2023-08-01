#include <dirent.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

static void dl_ls(char* path);
static void die(const char* s);
static char* allocate_path_buffer(size_t required_len);

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "%s: no arguments\n", argv[0]);
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        dl_ls(argv[i]);
    }

    exit(0);
}

static void dl_ls(char* path) {

    DIR* d = opendir(path);

    if (!d) {
        die(path);
    }

    struct dirent* ent;

    while ((ent = readdir(d)) != NULL) {
        char* buf = allocate_path_buffer(strlen(path)
            + 1 + strlen(ent->d_name));
        // sprintf does not cause buffer overflow here, because
        // we calculate required buffer length beforehand.
        sprintf(buf, "%s/%s", path, ent->d_name);

        struct stat st;

        if (lstat(buf, &st)) {
            die(buf);
        }

        char* mtime = ctime(&st.st_mtime);
        // ctime returns a string terminated by '\n', so remove it.
        mtime[strlen(mtime) - 1] = '\0';

        struct passwd* pw = getpwuid(st.st_uid);

        if (pw) {
            printf("%s owner=%s mtime=%s\n", ent->d_name, pw->pw_name, mtime);
        } else {
            printf("%s owner=%d mtime=%s\n", ent->d_name, st.st_uid, mtime);
        }
    }

    closedir(d);
}

static char* file_path = NULL;
size_t path_len = 0;

static char* allocate_path_buffer(size_t required_len) {

    size_t len = path_len;

    while (len < required_len) {
        len += 1024;
    }

    if (len > path_len) {
        if (!file_path) {
            file_path = malloc(len);
            if (!file_path) {
                die("malloc");
            }
        } else {
            char* tmp = realloc(file_path, len);
            if (!tmp) {
                die("realloc");
            }
            file_path = tmp;
        }
    }

    return file_path;
}

static void die(const char* s) {
    perror(s);
    exit(1);
}

