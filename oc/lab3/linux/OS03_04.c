#include <stdio.h>
#include <unistd.h>  

int main() {
    pid_t pid = getpid(); 

    printf("Идентификатор процесса: %d\n", pid);

    for (int i = 0; i < 10; i++) {
        printf("Цикл %d. PID: %d\n", i + 1, pid);
        sleep(1);  
    }

    printf("Завершение программы\n");
    return 0;
}
