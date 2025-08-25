#include <stdio.h>
#include <time.h>
#include <unistd.h>

int main()
{
    struct timespec start_real, end_real, current_real;
    clock_t start_cpu, end_cpu;
    long iteration_count = 0;
    
    clock_gettime(CLOCK_REALTIME, &start_real); // �������� �����
    start_cpu = clock();                       // ������������ �����

    printf("Loop started\n");

    while (1)
    {
        iteration_count++;
        clock_gettime(CLOCK_REALTIME, &current_real); // ��������� �������� ��������� �������
        double elapsed_cpu = (double)(clock() - start_cpu) / CLOCKS_PER_SEC; // ������������ ����� � ��������

        if (elapsed_cpu >= 2.0) // ��������� ���� ����� 2 ������ ������������� �������
        {
            clock_gettime(CLOCK_REALTIME, &end_real); // �������� �������� �����
            end_cpu = clock();
            break;
        }
    }

    double elapsed_real = (end_real.tv_sec - start_real.tv_sec) +
        (end_real.tv_nsec - start_real.tv_nsec) / 1e9;
    double elapsed_cpu = (double)(end_cpu - start_cpu) / CLOCKS_PER_SEC;
    
    printf("Loop completed\n");
    printf("Iterations: %ld\n", iteration_count);
    printf("Elapsed real time: %.3f seconds\n", elapsed_real);
    printf("Elapsed CPU time: %.3f seconds\n", elapsed_cpu);

    return 0;
}
