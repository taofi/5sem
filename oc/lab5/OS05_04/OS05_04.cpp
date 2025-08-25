#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>

int main() {
    // Идентификатор текущего процесса
    pid_t process_id = getpid();

    // Идентификатор текущего (main) потока
    pid_t thread_id = syscall(SYS_gettid);

    // Приоритет (nice) текущего потока
    int nice_value = getpriority(PRIO_PROCESS, 0);

    // Номера доступных процессоров
    cpu_set_t cpu_set;
    CPU_ZERO(&cpu_set);
    sched_getaffinity(0, sizeof(cpu_set), &cpu_set);

    std::cout << "Process ID: " << process_id << std::endl;
    std::cout << "Main Thread ID: " << thread_id << std::endl;

    if (nice_value == -1 && errno != 0) {
        std::cerr << "Error getting nice value" << std::endl;
    }
    else {
        std::cout << "Nice value: " << nice_value << std::endl;
    }

    std::cout << "Available CPUs: ";
    for (int i = 0; i < CPU_SETSIZE; i++) {
        if (CPU_ISSET(i, &cpu_set)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

    return 0;
}
