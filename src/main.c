#include <stdio.h>
#include "des.h"

int main(const int argc, char *argv[]) {

    if(argc < 2) {
        perror("No plaintext was passed.\n");
        return -1; // bad call
    }

    char *plaintext = argv[1];
    //char *key = argv[2];

    uint64_t text64b = TextTo64Bit(plaintext, 8);
    printf("Plaintext is: %s\n", plaintext);
    printf("64bit plaintext is:  0x%llx\n", text64b);
   // text64b = InitialPermutation(text64b);
   // printf("Permuted Input: 0x%llx\n", text64b);

    uint64_t r32 = text64b & 0x00000000ffffffff;
    uint64_t l32 = text64b & 0xffffffff00000000;

    printf("Right half: 0x%llx\n", r32);
    printf("Left half: 0x%llx\n", l32);

    uint64_t r48 = Expansion(r32);

    printf("Expansion: 0x%llx\n", r48);

    text64b = InversePermutation(text64b);
    printf("Inversed:0x%llx\n", text64b);
    return 0;
}
