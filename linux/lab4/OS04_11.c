#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h> 

void* OS04_11_T1(void* arg) {
    for (int i = 0; i < 75; ++i) {
        pid_t spid = syscall(SYS_gettid);
        printf("Thread SPID: %d\n", spid); 
        sleep(1); 
    }
    pthread_exit(NULL); 
}

int main() {
    pthread_t tid;

    if (pthread_create(&tid, NULL, OS04_11_T1, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    for (int i = 0; i < 100; ++i) {
        printf("Main Process ID: %d\n", getpid()); 
        sleep(1); 
    }

    pthread_join(tid, NULL);
    return 0;
}
