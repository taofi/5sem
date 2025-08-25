#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int fd = -1;
char* buffer = NULL;
off_t bufferSize;
int rowNum = 0;

void OpenFileMy(const char* filePath) {
    fd = open(filePath, O_RDWR);
    if (fd == -1) {
        printf("Cannot open file");
        printf("\nEnter, to continue...\n");
        getchar();
        return;
    }

    struct stat st;
    fstat(fd, &st);
    bufferSize = st.st_size;

    buffer = (char*)malloc(bufferSize + 1);
    if (read(fd, buffer, bufferSize) != bufferSize) {
        printf("Read file error");
        printf("\nEnter, to continue...\n");
        getchar();

        close(fd);
        fd = -1;
        free(buffer);
        buffer = NULL;
        return;
    }
    buffer[bufferSize] = '\0';

    for (int i = 0; i < bufferSize; i++) {
        if (buffer[i] == '\n') {
            rowNum++;
        }
    }
    if (buffer[bufferSize - 1] != '\n') {
        rowNum++;
    }
    printf("SUCCSESS open");
    printf("\nEnter, to continue...\n");
    getchar();

}

void AddRow(const char* row, int pos) {
    if (fd == -1) {
        printf("File is not open");
        printf("\nEnter, to continue...\n");
        getchar();

        return;
    }
    if ((pos > rowNum + 1) || (pos < -1)) {
        printf("Invalid index");
        printf("\nEnter, to continue...\n");
        getchar();

        return;
    }

    if (pos > 0) {
        pos--;
    }
    if (pos == -1) {
        pos = rowNum - 1;
    }

    int rows = 0;
    char* newBuffer = (char*)malloc(bufferSize + strlen(row) + 2 + (pos == -1 ? 1 : 0));
    int bi = 0;
    int nbi = 0;
    for (int i = 0; i < bufferSize; i++) {
        if (rows == pos) {
            break;
        }
        newBuffer[nbi++] = buffer[bi++];
        if (buffer[bi - 1] == '\n') {
            rows++;
        }
    }

    if (pos == -1) {
        newBuffer[nbi++] = '\n';
    }
    for (int i = 0; i < strlen(row); i++) {
        newBuffer[nbi++] = row[i];
    }

    if (pos != rowNum && pos != -1) {
        newBuffer[nbi++] = '\n';
    }

    for (int i = bi; i < bufferSize; i++) {
        newBuffer[nbi++] = buffer[i];
    }

    newBuffer[nbi] = '\0';

    buffer = newBuffer;
    bufferSize = strlen(buffer);

    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, bufferSize);
    write(fd, buffer, bufferSize);
    rowNum++;
    printf("SUCCSESS add row");
    printf("\nEnter, to continue...\n");
    getchar();
}

void RemRow(int pos) {
    if (fd == -1 || buffer == NULL) {
        printf("File is not open");
        printf("\nEnter, to continue...\n");
        getchar();

        return;
    }
    if ((pos > rowNum) || (pos < -1)) {
        printf("Invalid index");
        printf("\nEnter, to continue...\n");

        getchar();

        return;
    }
    if (pos != -1 && pos != 0) {
        pos--;
    }
    if (pos == -1) {
        pos = rowNum - 1;
    }
    int rows = 0;
    int startPos = 0;
    for (int i = 0; i < bufferSize; i++) {
        if (rows == pos) {
            break;
        }
        if (buffer[i] == '\n') {
            rows++;
        }
        startPos++;
    }
    
    int endPos = startPos;
    while (buffer[endPos] != '\n' && endPos < bufferSize) {
        endPos++;
    }

    if (pos == rowNum - 1) {
        startPos--;
    }
    int deleteSize = endPos - startPos + 1;
    char* newBuffer = (char*)malloc(bufferSize - deleteSize + 1);

    int nbi = 0;
    for (int i = 0; i < bufferSize; i++) {
        if (i >= startPos && i <= endPos) {
            continue;
        }
        newBuffer[nbi++] = buffer[i];
    }
    newBuffer[nbi] = '\0';
    buffer = newBuffer;
    bufferSize = strlen(buffer);

    lseek(fd, 0, SEEK_SET);
    ftruncate(fd, bufferSize);
    write(fd, buffer, bufferSize);
    rowNum--;
    printf("SUCCSESS rem row");
    getchar();
}

void PrintRow(int pos) {
    printf("\n\n");

    if (fd == -1) {
        printf("File is not open");
        printf("\nEnter, to continue...\n");
        getchar();

        return;
    }

    if ((pos > rowNum) || (pos < -1)) {
        printf("Invalid index");
        printf("\nEnter, to continue...\n");
        getchar();
        return;
    }

    if (pos > 0) {
        pos--;
    }
    if (pos == -1) {
        pos = rowNum - 1;
    }
    int rows = 0;
    for (int i = 0; i < bufferSize; i++) {
        if (rows == pos) {
            if (buffer[i] != '\n')
                printf("%c", buffer[i]);
        }
        if (buffer[i] == '\n') {
            rows++;
        }
    }
    printf("\nEnter, to continue...\n");
    getchar();

}

void PrintRows() {
    printf("\n\n");

    if (fd == -1) {
        printf("File is not open");
        printf("\nEnter, to continue...\n");
        getchar();
        return;
    }

    printf("%s\n", buffer);
    printf("\nEnter, to continue...\n");
    getchar();

}

void CloseFile() {
    if (fd != -1) {
        close(fd);
        fd = -1;

        free(buffer);
        buffer = NULL;
    }
    else {
        printf("File is not open");
        printf("\nEnter, to continue...\n");
        getchar();

    }
}

int main() {
    int choice;
    char filePath[256];
    char row[256];
    int pos;

    while (1) {
        system("clear");
        printf("Choose operation:\n");
        printf("1. Open file\n");
        printf("2. Add row\n");
        printf("3. Remove row\n");
        printf("4. Print row\n");
        printf("5. Print all rows\n");
        printf("6. Close file\n");
        printf("0. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter file path: ");
            scanf(" %[^\n]s\n", filePath);
            OpenFileMy(filePath);
            break;
        case 2:
            printf("Enter row: ");
            scanf("%s", row);
            while (getchar() != '\n') {}
            printf("Enter position: ");
            scanf("%5d", &pos);
            AddRow(row, pos);
            break;
        case 3:
            printf("Enter position: ");
            scanf("%5d", &pos);
            RemRow(pos);
            break;
        case 4:
            printf("Enter position: ");
            scanf("%5d", &pos);
            PrintRow(pos);
            break;
        case 5:
            PrintRows();
            break;
        case 6:
            CloseFile();
            break;
        case 0:
            CloseFile();
            return 0;
        default:
            printf("Invalid choice\n");
        }
        while (getchar() != '\n') {}
    }

    return 0;
}
