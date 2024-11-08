#include <stdio.h>
#include <sys/types.h>

#include "des.h"

int main(const int argc, char *argv[]) {

    if(argc < 0) {
        perror("No plaintext with key was passed.\n");
        return -1; // bad call
    }

    char *plaintext = "NEW YEAR";
    char *key = "NEW YEAR";

    const uint64_t text64b = TextTo64Bit(plaintext);
    printf("Plaintext is: %s\n", plaintext);
    printf("64bit plaintext is:  0x%llx\n", text64b);

    const uint64_t key64b = TextTo64Bit(key);
    printf("Key is: %s\n", key);
    printf("64bit key is: 0x%llx\n", key64b);

    const uint64_t ciphertext = DESEncrypt(text64b, key64b);
    printf("Ciphertext is: 0x%llx\n", ciphertext);

    printf("Getting back the plaintext\n");

    const uint64_t pt = DESDecrypt(ciphertext, key64b);
    printf("The plaintext that came back is: 0x%llx\n", pt);

    return 0;
}
