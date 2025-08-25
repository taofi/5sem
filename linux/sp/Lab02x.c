#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // ��� usleep � getpid

int main(int argc, char* argv[]) {
    int iterations = 0;

    // �������� ���������� ��������� ������
    if (argc >= 2) {
        iterations = atoi(argv[1]);
    }
    else {
        // ��������� ���������� ���������
        char* envVar = getenv("ITER_NUM");
        if (envVar != NULL) {
            iterations = atoi(envVar);
        }
    }

    // �������� ���������� ���������� ��������
    if (iterations <= 0) {
        fprintf(stderr, "Error: No valid iteration number provided (either as argument or environment variable).\n");
        return 1; // ������� ���� ������
    }

    printf("Number of iterations: %d\n", iterations);

    for (int i = 0; i < iterations; i++) {
        pid_t pid = getpid(); // ��������� �������������� ��������
        printf("Iteration %d, PID: %d\n", i + 1, pid);

        usleep(500000); // �������� 500 �����������
    }

    return 0;
}