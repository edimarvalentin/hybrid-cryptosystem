#include <stdio.h>

int main(const int argc, char *argv[]) {

    if(argc < 3) {
        perror("No plaintext was passed.\n");
        return -1; // bad call
    }

    char *plaintext = argv[1];
    char *key = argv[2];

    printf("size of plaintext: %lu\n", sizeof(plaintext));

    return 0;
}
