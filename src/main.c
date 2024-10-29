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
    text64b = InitialPermutation(text64b);
    printf("Permuted Input: 0x%llx\n", text64b);
    text64b = InversePermutation(text64b);
    printf("Inversed:0x%llx\n", text64b);
    return 0;
}
