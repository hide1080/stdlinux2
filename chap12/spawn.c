#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <command> <arg>\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        fprintf(stderr, "fork(2) failed\n");
        exit(1);
    }
    if (pid == 0) {
        // child 
        execl(argv[1], argv[1], argv[2], NULL);
        // if execl() is returned, failure
        perror(argv[1]);
        exit(99);
    } else {
        // parent
        printf("child (PID=%d) finished; ", pid);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
            printf("exit, status=%d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("signal, sig=%d\n", WTERMSIG(status));
        else
            printf("abnormal exit\n");

        exit(0);
    }
}

