#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Process ID: %d\n", getpid());

    while (true) {
        if (i % 1000000000 == 0) {
            printf("Iteration: %lld\n", i);
            sleep(1); 
        }
    }

    return 0;
}
