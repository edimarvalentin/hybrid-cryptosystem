#include <stdio.h>
#include <string.h>

#include "des.h"

int main(const int argc, char *argv[]) {

    if(argc < 2) {
        perror("No plaintext was passed.\n");
        return -1; // bad call
    }

    char *plaintext = argv[1];
    //char *key = argv[2];

    printf("Plaintext is: %s\n", plaintext);
    printf("Encrypted plaintext is: %lu\n", TextTo64Bit(plaintext, 4));
    return 0;
}
