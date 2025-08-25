#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define INITIAL_HEAP_SIZE (2 * 1024 * 1024)
#define MAX_HEAP_SIZE (8 * 1024 * 1024)
#define BLOCK_SIZE (512 * 1024) // 1024 * 1024
#define NUM_BLOCKS 10 //5

void HeapInfo(HANDLE heap) {

    PROCESS_HEAP_ENTRY entry;
    entry.lpData = NULL;

    while (HeapWalk(heap, &entry)) {
        if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) {
            printf("Address: %p, Size: %lu, Type: Busy\n", entry.lpData, (unsigned long)entry.cbData);
        }
        else {
            printf("Address: %p, Size: %lu, Type: Free\n", entry.lpData, (unsigned long)entry.cbData);
        }
    }
    printf("\n");
}

int main() {

    HANDLE heap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, INITIAL_HEAP_SIZE, MAX_HEAP_SIZE);
    if (heap == NULL) {
        printf("error: %lu\n", GetLastError());
        return 1;
    }
    printf("heap was created\n");
    HeapInfo(heap);
    system("pause & cls");

    void* blocks[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        printf("Allocating block %d of size %zu\n", i, BLOCK_SIZE);
        blocks[i] = HeapAlloc(heap, HEAP_GENERATE_EXCEPTIONS, BLOCK_SIZE);
        if (blocks[i] == NULL) {
            DWORD lastError = GetLastError();
            printf("HeapAlloc failed for block %d: error %lu\n", i, lastError);
            HeapDestroy(heap);
            return 1;
        }


        printf("Filling block %d\n", i);
        int* intArray = (int*)blocks[i];
        size_t numIntegers = BLOCK_SIZE / sizeof(int);
        for (size_t j = 0; j < numIntegers; j++) {
            intArray[j] = (int)(j + i * numIntegers);
        }

        printf("Block %d created and filled.\n", i);
        HeapInfo(heap);

        int result = system("pause & cls");
        if (result != 0) {
            printf("System command failed with code %d\n", result);
            return 1;
        }
    }


    for (int i = 0; i < NUM_BLOCKS; i++) {
        HeapFree(heap, 0, blocks[i]);
        printf("block %d was freed.\n", i);
        HeapInfo(heap);
        system("pause & cls");
    }

    if (HeapDestroy(heap) == 0) {
        printf("error: %lu\n", GetLastError());
        return 1;
    }
    printf("heap was destroyed.\n");

    return 0;
}