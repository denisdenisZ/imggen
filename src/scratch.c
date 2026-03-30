
#include <stdint.h>
#include <stdio.h>

int main(void) {
    #define BYTE_MASK 0xFF

    const uint16_t max = 0xFFAA;
    uint8_t arr[2] = {0};

    arr[0] = max & BYTE_MASK;
    arr[1] = (max >> 8) & BYTE_MASK;

    printf("%X\n", arr[0]);
    printf("%X\n", arr[1]);
}
