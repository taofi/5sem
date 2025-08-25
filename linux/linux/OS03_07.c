#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid = getpid();
    printf("ИД процесса(7): %d\n", pid);

    if (execl("./OS03_05_01", "OS03_05_01", NULL) == -1) {
        perror("system");
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        printf("Parent process (PID: %d), iteration: %d\n", getpid(), i + 1);
        sleep(1);
    }
    return 0;
}
