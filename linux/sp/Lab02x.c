#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Для usleep и getpid

int main(int argc, char* argv[]) {
    int iterations = 0;

    // Проверка аргументов командной строки
    if (argc >= 2) {
        iterations = atoi(argv[1]);
    }
    else {
        // Получение переменной окружения
        char* envVar = getenv("ITER_NUM");
        if (envVar != NULL) {
            iterations = atoi(envVar);
        }
    }

    // Проверка валидности количества итераций
    if (iterations <= 0) {
        fprintf(stderr, "Error: No valid iteration number provided (either as argument or environment variable).\n");
        return 1; // Возврат кода ошибки
    }

    printf("Number of iterations: %d\n", iterations);

    for (int i = 0; i < iterations; i++) {
        pid_t pid = getpid(); // Получение идентификатора процесса
        printf("Iteration %d, PID: %d\n", i + 1, pid);

        usleep(500000); // Ожидание 500 миллисекунд
    }

    return 0;
}