#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = getpid();
    printf("ИД процесса(6): %d\n", pid);

    if (system("./OS03_05_01 &") == -1) {
        perror("system");
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        printf("РП - %d\n", i);
        sleep(1);
    }

    return 0;
}
