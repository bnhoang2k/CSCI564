#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    // Cache Parameters
    // 16 Bytes per block
    // 512 rows
    // 2-Way Set Associative
    // 16 KB then 32 KB, need 4096 and 8192 ints

    int array_size = (16 * 512 * 2) * 2;
    int arr[array_size];
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

}