#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static void test_rename(const char* filename);
static void test_unlink(const char* filename);
static void test_rename_other(const char* name, const char* other_name);
static void die(const char* s);

int main(int argc, char* argv[]) {

    if (argc < 3) {
        fprintf(stderr, "wrong argument\n");
        exit(1);
    }

    int num = atoi(argv[1]);

    switch (num) {
        case 1:
            test_rename(argv[2]);
            break;
        case 2:
            test_unlink(argv[2]);
            break;
        case 3:
            test_rename_other(argv[2], argv[3]);
            break;
        default:
            fprintf(stderr, "wrong argument\n");
            exit(1);
    }

    exit(0);
}

static void test_rename(const char* path) {

    int fd;
    char new_name[40];

    printf("before file(%s) open\n", path);

    fd = open(path, O_RDWR);
    if (fd < 0)
        die(path);

    printf("after file(%s:%d) open\n", path, fd);
    
    const char* msg = "Hello!\n";

    int n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    sprintf(new_name,"%s_%s", path, "renamed");
    printf("before rename(%s->%s)\n", path, new_name);

    if (rename(path, new_name) < 0) {
        perror(path);
        exit(1);
    }

    printf("after rename\n");

    n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    printf("before close\n");

    if (close(fd) < 0)
        die(path);

    printf("after close\n");
}

static void test_unlink(const char* path) {

    int fd;

    printf("before file(%s) open\n", path);

    fd = open(path, O_RDWR);
    if (fd < 0)
        die(path);

    printf("after file(%s:%d) open\n", path, fd);
    
    const char* msg = "Hello!\n";

    int n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    printf("before unlink\n");

    if (unlink(path) < 0) {
        perror(path);
        exit(1);
    }

    printf("after unlink\n");

    n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    printf("before close\n");

    if (close(fd) < 0)
        die(path);

    printf("after close\n");
}

static void test_rename_other(const char* path, const char* other_name) {

    int fd;

    printf("before file(%s) open\n", path);

    fd = open(path, O_RDWR);
    if (fd < 0)
        die(path);

    printf("after file(%s:%d) open\n", path, fd);
    
    const char* msg = "Hello!\n";

    int n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    printf("before rename(%s->%s)\n", other_name, path);

    if (rename(other_name, path) < 0) {
        perror(other_name);
        exit(1);
    }

    printf("after rename\n");

    n = write(fd, msg, sizeof msg);
    if (n < 0)
        die(path);

    printf("before close\n");

    if (close(fd) < 0)
        die(path);

    printf("after close\n");
}

static void die(const char* s) {
    perror(s);
    exit(1);
}

