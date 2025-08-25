#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctype.h>
#include <wchar.h>
#include <locale.h>
#include <wctype.h>

#define DEFAULT_FIFO_NAME "/tmp/myFifo"
#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    const char* fifo_name = DEFAULT_FIFO_NAME;
    
    char buffer[BUFFER_SIZE];

    if (access(fifo_name, F_OK) == -1) {
        if (mkfifo(fifo_name, 0666) == -1) {
            perror("mkfifo");
            return 1;
        }
    }

    while (1) {
        int fd = open(fifo_name, O_RDONLY);
        if (fd == -1) {
            perror("open");
            continue;
        }
        sleep(5);
        read(fd, buffer, BUFFER_SIZE);
        close(fd);

        printf("Received: %s\n", buffer);
        fflush(stdout);

        // ѕреобразование строки в верхний регистр
        wchar_t wbuffer[BUFFER_SIZE];
        mbstowcs(wbuffer, buffer, BUFFER_SIZE);
        for (int i = 0; i < wcslen(wbuffer); i++) {
            wbuffer[i] = towupper(wbuffer[i]);
        }
        wcstombs(buffer, wbuffer, BUFFER_SIZE);

        fd = open(fifo_name, O_WRONLY);
        write(fd, buffer, strlen(buffer) + 1);
        close(fd);
        printf("Sent: %s\n\n", buffer);
        fflush(stdout);
    }

    unlink(fifo_name);

    return 0;
}
