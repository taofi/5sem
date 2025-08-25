#include <stdio.h>
#include <unistd.h>  // Для функции sleep и getpid

int main() {
    pid_t pid = getpid();  // Получаем идентификатор процесса

    printf("Идентификатор процесса: %d\n", pid);

    // Длинный цикл с задержкой
    for (int i = 0; i < 10; i++) {
        printf("Цикл %d. PID: %d\n", i + 1, pid);
        sleep(1);  // Задержка в 1 секунду
    }

    printf("Завершение программы\n");
    return 0;
}
