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
char *RotorEncrypt(char *message, int *rotor_positions) {
    for (int i = 0; i < strlen(message); i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            for (int j = 0; j < ROTOR_COUNT; j++) {
                message[i] = substitute_char(message[i], j, rotor_positions[j], 1);
            }
            rotate_rotors(rotor_positions);
        }
    }
    return message;
}

// Function to decrypt a message using the rotor machine
char *RotorDecrypt(char *encrypted, int *rotor_positions) {
    for (int i = 0; i < strlen(encrypted); i++) {
        if (encrypted[i] >= 'A' && encrypted[i] <= 'Z') {
            for (int j = ROTOR_COUNT - 1; j >= 0; j--) {
                encrypted[i] = substitute_char(encrypted[i], j, rotor_positions[j], 0);
            }
            rotate_rotors(rotor_positions);
        }
    }
    return encrypted;
}

