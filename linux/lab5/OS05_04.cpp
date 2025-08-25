#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/resource.h>  // Добавлен заголовок для getpriority и PRIO_PROCESS

int main() {
    // Получение и вывод идентификатора текущего процесса
    pid_t process_id = getpid();
    std::cout << "Process ID: " << process_id << std::endl;

    // Получение и вывод идентификатора текущего (main) потока
    pthread_t thread_id = pthread_self();
    std::cout << "Thread ID: " << thread_id << std::endl;

    // Получение и вывод приоритета (nice) текущего потока
    int nice_value = getpriority(PRIO_PROCESS, 0); // 0 означает текущий процесс
    if (nice_value == -1 && errno != 0) {
        perror("getpriority failed");
    }
    else {
        std::cout << "Thread nice value: " << nice_value << std::endl;
    }

    // Получение и вывод номеров доступных процессоров
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    sched_getaffinity(0, sizeof(cpu_set), &cpu_set);
    std::cout << "Available CPUs: ";
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpu_set)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
