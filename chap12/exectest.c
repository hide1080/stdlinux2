#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

static void die(char* msg) {
    printf("%s error\n", msg);
    exit(1);
}

int main(int argc, char* argv[]) {

    puts("[execl]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        execl("./args", "PROGNAME", "called by execl", NULL);
        die("execl");
    }


    puts("[execlp]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        execlp("./args", "PROGNAME", "called by execlp", NULL);
        die("execlp");
    }

    puts("[execle]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        execle("./args", "PROGNAME", "called by execle", NULL, environ);
        die("execle");
    }

    puts("[execv]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        char* args[4] = { "PROGNAME", "called by execv", NULL };
        execv("./args", args);
        die("execv");
    }

    puts("[execvp]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        char* args[4] = { "PROGNAME", "called by execvp", NULL };
        execvp("./args", args);
        die("execvp");
    }

    puts("[execve]");
    if (fork() != 0) {
        wait(NULL);
    } else {
        char* args[4] = { "PROGNAME", "called by execve", NULL };
        execve("./args", args, environ);
        die("execve");
    }
}

