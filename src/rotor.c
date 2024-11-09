#include "rotor.h"
#include <string.h>

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26

char rotors[ROTOR_COUNT][ALPHABET_SIZE] = {
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", // Rotor 1
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", // Rotor 2
    "BDFHJLCPRTXVZNYEIWGAKMUSQO"  // Rotor 3
};

// Function to rotate a character
char rotate_char(const char ch, const int shift) {
    if (ch >= 'A' && ch <= 'Z') {
        return 'A' + (ch - 'A' + shift) % ALPHABET_SIZE;
    }
    return ch;
}

// Function to find the substitution character based on rotor
char substitute_char(const char ch, const int rotor_index, const int forward) {
    if (forward) {
        return rotors[rotor_index][ch - 'A'];
    } else {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (rotors[rotor_index][i] == ch) {
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
char *RotorEncrypt(char *plaintext, int *rotor_positions) {
    for (int i = 0; i < strlen(plaintext); i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            for (int j = 0; j < ROTOR_COUNT; j++) {
                plaintext[i] = substitute_char(plaintext[i], j, 1);
            }
            rotate_rotors(rotor_positions);
        }
    }
    return plaintext;
}

// Function to decrypt a message using the rotor machine
char *RotorDecrypt(char *ciphertext, int *rotor_positions) {
    for (int i = 0; i < strlen(ciphertext); i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            for (int j = ROTOR_COUNT - 1; j >= 0; j--) {
                ciphertext[i] = substitute_char(ciphertext[i], j, 0);
            }
            rotate_rotors(rotor_positions);
        }
    }
    return ciphertext;
}

