#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // Ошибка при создании дочернего процесса
        perror("fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Дочерний процесс OS03_05_1
        for (int i = 0; i < 50; i++) {
            printf("Child process (PID: %d), iteration: %d\n", getpid(), i + 1);
            sleep(1);  // Задержка в 1 секунду
        }
    }
    else {
        // Родительский процесс OS03_05
        for (int i = 0; i < 100; i++) {
            printf("Parent process (PID: %d), iteration: %d\n", getpid(), i + 1);
            sleep(1);  // Задержка в 1 секунду
        }
        // Ожидание завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}
