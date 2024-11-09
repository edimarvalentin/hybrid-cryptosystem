#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "des.h"
#include "rotor.h"

int main(const int argc, char *argv[]) {
    char *plaintext = NULL;
    char *ciphertext = NULL;
    int rotor_conf[] = {0, 0, 0}; // Assuming an array for rotor configuration
    const char *key = NULL;
    int option;

    // Parse command-line arguments
    while ((option = getopt(argc, argv, "p:c:r:k:")) != -1) {
        switch (option) {
            case 'p':
                plaintext = optarg;
            break;
            case 'c':
                ciphertext = optarg;
            break;
            case 'r': {
                // Parse rotor configuration as a comma-separated list (e.g., "1,2,3")
                char *token = strtok(optarg, ",");
                for (int i = 0; token != NULL && i < 3; i++) {
                    rotor_conf[i] = atoi(token);
                    token = strtok(NULL, ",");
                }
                break;
            }
            case 'k':
                key = optarg;
            break;
            default:
                fprintf(stderr, "Usage: %s [-p plaintext | -c ciphertext] -r rotor -k key\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    // Check if key and rotor configuration are provided
    if (key == NULL || rotor_conf[0] == 0) {
        fprintf(stderr, "Error: Missing key and rotor configuration\n");
        exit(EXIT_FAILURE);
    }

    // Check if either plaintext or ciphertext is provided (not both)
    if ((plaintext == NULL && ciphertext == NULL) || (plaintext != NULL && ciphertext != NULL)) {
        fprintf(stderr, "Error: Provide either plaintext for encryption or ciphertext for decryption, not both.\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt if plaintext is provided, decrypt if ciphertext is provided
    if (plaintext != NULL) {
        printf("Encrypting plaintext (M): %s\n", plaintext);
        char *E1 = RotorEncrypt(plaintext, rotor_conf);
        printf("E1: %s\n", E1);
        E1 = RotorDecrypt(E1, rotor_conf);
        printf("E1 Decrypted: %s\n", E1);

        // DES works with blocks of bits
        const uint64_t key_64b = TextTo64Bit(key);

        // Calculate the number of 64 bits in E1
        const int blocks = (strlen(E1) + MAX_PLAINTEXT_SIZE) / MAX_PLAINTEXT_SIZE;
        uint64_t ciphers[blocks];

        // Encrypt
        for (int i = 0; i < blocks; i++) {

            //get 8 chars, and add space for padding
            char block[MAX_PLAINTEXT_SIZE + 1] = {0};
            strncpy(block, E1 + i * MAX_PLAINTEXT_SIZE, MAX_PLAINTEXT_SIZE);

            printf("Block: %s\n", block);

            // Convert the block to 64-bit integer
            uint64_t block_64b = TextTo64Bit(block);
            printf("Block_64b: %llx\n", block_64b);

            // Encrypt the block with DES
            ciphers[i] = DESEncrypt(block_64b, key_64b);

            char E2[] = "ABCDEFGH\0";

            Bit64ToText(ciphers[i], E2);

            const uint64_t plaintext_block = DESDecrypt(ciphers[i], key_64b);

            char M[] = "ABCDEFGH\0";

            Bit64ToText(plaintext_block, M);

            printf("Cipher block %d (hex): %llx | %s\n", i, ciphers[i], E2);
            printf("Plaintext block %d (hex): %llx| %s\n",i, plaintext_block, M);


        }

    } else if (ciphertext != NULL) {
        printf("Decrypting ciphertext (D2): %s\n", ciphertext);
        char* D2 = ciphertext;
        const int key_64b = TextTo64Bit(key);

        // Calculate the number of 64 bits in D2
        const int blocks = (strlen(D2) + MAX_PLAINTEXT_SIZE - 1) / MAX_PLAINTEXT_SIZE;
        uint64_t ciphers[blocks];

        // Decrypt
        for (int i = 0; i < blocks; i++) {

            //get 8 chars, and add 0s for padding
            char block[MAX_PLAINTEXT_SIZE + 1] = {0};

            strncpy(block, D2 + i * MAX_PLAINTEXT_SIZE, MAX_PLAINTEXT_SIZE);

            // Convert the block to 64-bit integer
            uint64_t block_64b = TextTo64Bit(block);

            // Encrypt the block with DES
            ciphers[i] = DESDecrypt(block_64b, key_64b);

            char E2[] = "ABCDEFGH";

            Bit64ToText(ciphers[i], E2);

            printf("Cipher block %d (hex): %llx | %s\n", i, ciphers[i], E2);
        }


    }

    return 0;
}