#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int status = system("./OS03_05_01");

    if (status == -1) {
        perror("system call failed");
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        printf("Parent process (PID: %d), iteration: %d\n", getpid(), i + 1);
        sleep(1); 
    }

    return 0;
}
