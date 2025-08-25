#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <errno.h>

int IsTextFile(int fd) {
    char buffer[512];
    ssize_t bytesRead;
    lseek(fd, 0, SEEK_SET);
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buffer[i] < 32 && buffer[i] != 9 && buffer[i] != 10 && buffer[i] != 13) {
                return 0;
            }
        }
    }
    lseek(fd, 0, SEEK_SET);
    return 1;
}

void PrintText(const char* fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    if (!IsTextFile(fd)) {
        close(fd);
        return;
    }

    char buffer[512];
    ssize_t bytesRead;
    lseek(fd, 0, SEEK_SET);
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, bytesRead, stdout);
    }
    close(fd);
}

void PrintInfo(const char* fileName) {
    struct stat fileInfo;
    if (stat(fileName, &fileInfo) == -1) {
        perror("Error retrieving file info");
        return;
    }

    printf("Size: %lld bytes (%.2f KB, %.2f MB)\n",
        (long long)fileInfo.st_size,
        (double)fileInfo.st_size / 1024,
        (double)fileInfo.st_size / (1024 * 1024));

    printf("Name: %s\n", fileName);

    // Определение типа файла
    if (S_ISREG(fileInfo.st_mode)) {
        printf("File type: simple file\n");
    }
    else if (S_ISDIR(fileInfo.st_mode)) {
        printf("File type: directory\n");
    }
    else if (S_ISCHR(fileInfo.st_mode)) {
        printf("File type: char device\n");
    }
    else if (S_ISBLK(fileInfo.st_mode)) {
        printf("File type: block device\n");
    }
    else if (S_ISFIFO(fileInfo.st_mode)) {
        printf("File type: FIFO\n");
    }
    else if (S_ISLNK(fileInfo.st_mode)) {
        printf("File type: symbolic link\n");
    }
    else if (S_ISSOCK(fileInfo.st_mode)) {
        printf("File type: socket\n");
    }
    else {
        printf("File type: undefined\n");
    }

    char timeBuf[64];
    struct tm* tm;

    tm = localtime(&fileInfo.st_ctime);
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", tm);
    printf("Creation time: %s\n", timeBuf);

    tm = localtime(&fileInfo.st_atime);
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", tm);
    printf("Access time: %s\n", timeBuf);

    tm = localtime(&fileInfo.st_mtime);
    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", tm);
    printf("Modification time: %s\n", timeBuf);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Invalid arguments\n");
        return 1;
    }

    const char* filename = argv[1];
    PrintInfo(filename);
    printf("\n");
    PrintText(filename);
    printf("\n");

    return 0;
}
