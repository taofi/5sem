#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 0;
    while (true)
    {
        printf("Iteration: %d\n", i++);
        sleep(1);
    }  
    return 0;
}
