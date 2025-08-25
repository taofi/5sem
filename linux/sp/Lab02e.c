#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
    pid_t pid1, pid2;
    char* iter_num_env;

    // ��������, ��� ������� �������� ��������� ������
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number_of_iterations>\n", argv[0]);
        return 1;
    }

    if (setenv("ITER_NUM", "50", 1) != 0) {
        perror("������ ��� ��������� ���������� ���������");
        return 1;
    }
    // �������� �������� ITER_NUM �� ���������

    iter_num_env = getenv("ITER_NUM");

    // �������� ������� ��������� ��������
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid1 == 0) {
        // �������� ������� 1: ���������� �������� ��������� ������
        execl("./Lab02x", "Lab02x", argv[1], (char*)NULL);
        perror("execl failed");
        return 1;
    }

    // �������� ������� ��������� ��������
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        return 1;
    }

    if (pid2 == 0) {
        // �������� ������� 2: ���������� ITER_NUM �� ���������
        if (iter_num_env != NULL) {
            execl("./Lab02x", "Lab02x", (char*)NULL);
        }
        else {
            fprintf(stderr, "ITER_NUM environment variable is not set\n");
            return 1;
        }
        perror("execl failed");
        return 1;
    }

    // ������������ �������: ���� ���������� �������� ���������
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Both child processes have finished.\n");
    return 0;
}