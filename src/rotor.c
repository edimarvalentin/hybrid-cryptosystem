#include <stdio.h>
#include <string.h>

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26

char rotors[ROTOR_COUNT][ALPHABET_SIZE] = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // Rotor 1
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // Rotor 2
    "BDFHJLCPRTXVZNYEIWGAKMUSQO"  // Rotor 3
};

// Function to rotate a character
char rotate_char(char ch, int shift) {
    if (ch >= 'A' && ch <= 'Z') {
        return 'A' + (ch - 'A' + shift) % ALPHABET_SIZE;
    }
    return ch;
}

// Function to find the substitution character based on rotor
char substitute_char(char ch, int rotor_index, int rotor_position, int forward) {
    if (forward) {
        // Adjust for the rotor position
        int adjusted_index = (ch - 'A' + rotor_position) % ALPHABET_SIZE;
        return rotors[rotor_index][adjusted_index];
    } else {
        // Find the character in the rotor and adjust for the rotor position
        char substituted_char = ch;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (rotors[rotor_index][(i + rotor_position) % ALPHABET_SIZE] == substituted_char) {
                return 'A' + i;
            }
        }
    }
    return ch;
}

// Function to rotate the rotors
void rotate_rotors(int *rotor_positions) {
    rotor_positions[0]++;
    for (int i = 0; i < ROTOR_COUNT - 1; i++) {
        if (rotor_positions[i] >= ALPHABET_SIZE) {
            rotor_positions[i] = 0;
            rotor_positions[i + 1]++;
        }
    }
}

// Function to encrypt a message using the rotor machine
void encrypt(char *message, char *encrypted, int *rotor_positions) {
    strcpy(encrypted, message);
    for (int i = 0; i < strlen(message); i++) {
        if (encrypted[i] >= 'A' && encrypted[i] <= 'Z') {
            for (int j = 0; j < ROTOR_COUNT; j++) {
                encrypted[i] = substitute_char(encrypted[i], j, rotor_positions[j], 1);
            }
            rotate_rotors(rotor_positions);
        }
    }
}

// Function to decrypt a message using the rotor machine
void decrypt(char *encrypted, char *decrypted, int *rotor_positions) {
    strcpy(decrypted, encrypted);
    for (int i = 0; i < strlen(encrypted); i++) {
        if (decrypted[i] >= 'A' && decrypted[i] <= 'Z') {
            for (int j = ROTOR_COUNT - 1; j >= 0; j--) {
                decrypted[i] = substitute_char(decrypted[i], j, rotor_positions[j], 0);
            }
            rotate_rotors(rotor_positions);
        }
    }
}

int main() {
    char message[100], encrypted[100], decrypted[100];
    int rotor_positions[ROTOR_COUNT] = {0, 0, 0};
    int choice;

    printf("Enter the text to be encrypted or decrypted (uppercase letters only): ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    printf("Choose an option:\n1. Encrypt\n2. Decrypt\n");
    scanf("%d", &choice);
    getchar(); // Consume newline character

    if (choice == 1) {
        encrypt(message, encrypted, rotor_positions);
        printf("Encrypted Text: %s\n", encrypted);
    } else if (choice == 2) {
        decrypt(message, decrypted, rotor_positions);
        printf("Decrypted Text: %s\n", decrypted);
    } else {
        printf("Invalid choice. Please enter 1 for encryption or 2 for decryption.\n");
    }

    return 0;
}
