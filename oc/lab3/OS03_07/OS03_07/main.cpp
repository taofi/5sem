#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0) {
        execl("./OS03_05_1", "OS03_05_1", NULL); 
        perror("execl failed");
        return 1;
    }
    else {
        for (int i = 0; i < 100; i++) {
            printf("Parent process (PID: %d), iteration: %d\n", getpid(), i + 1);
            sleep(1); 
        }

        wait(NULL);
    }

    return 0;
}
