#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#define DEFAULT_FIFO_NAME "/tmp/myFifo"
#define DEFAULT_SEM_NAME "/MySemafor"
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string> [sem_name]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* message = argv[1];
    const char* fifo_name = DEFAULT_FIFO_NAME;
    const char* sem_name = (argc > 2) ? argv[2] : DEFAULT_SEM_NAME;

    char buffer[BUFFER_SIZE];

    sem_t* sem = sem_open(sem_name, O_CREAT, 0644, 1);
    sleep(10);


    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    printf("wait\n");

    sem_wait(sem);
    sleep(5);
    printf("send\n");

    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open");
        sem_post(sem);
        exit(EXIT_FAILURE);
    }
    write(fd, message, strlen(message) * 1 + 1);
    close(fd);

    sleep(5);

    fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open");
        sem_post(sem);
        exit(EXIT_FAILURE);
    }

    read(fd, buffer, BUFFER_SIZE);
    close(fd);

    printf("Response: %s\n", buffer);
    sleep(10);
    sem_post(sem);

    sem_close(sem);

    return 0;
}
