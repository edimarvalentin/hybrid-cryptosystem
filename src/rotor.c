//
// Created by evalentin on 10/26/24.
//

#include <stdio.h>
#include <string.h>

// define rotord
char rotor1[] = "EKMFLGDQVZNTOWYHXUSPAIBRCJ";
char rotor2[] = "AJDKSIRUXBLHWTMCQGZNPYFVOE";
char rotor3[] = "BDFHJLCPRTXVZNYEIWGAKMUSQO";

int rotor_pos[] = {0, 0, 0}; // position initial rotor

char encrypt_char(char c) {
    int index = c - 'A';
    index = (index + rotor_pos[0]) % 26;  // Rotor 1transfo
    c = rotor1[index];

    index = (c - 'A' + rotor_pos[1]) % 26; // Rotor 2 ''
    c = rotor2[index];

    index = (c - 'A' + rotor_pos[2]) % 26; //Rotor 3 ''
    c = rotor3[index];

    // rotate after encrypt
    rotor_pos[0] = (rotor_pos[0] + 1) % 26;
    if (rotor_pos[0] == 0) rotor_pos[1] = (rotor_pos[1] + 1) % 26;
    if (rotor_pos[1] == 0) rotor_pos[2] = (rotor_pos[2] + 1) % 26;

    return c;
}

char decrypt_char(char c) {
    int index;

    for (index = 0; index < 26; index++) {
        if (rotor3[index] == c) {
            c = 'A' + (index - rotor_pos[2] + 26) % 26;
            break;
        }
    }
    for (index = 0; index < 26; index++) {
        if (rotor2[index] == c) {
            c = 'A' + (index - rotor_pos[1] + 26) % 26;
            break;
        }
    }

    for (index = 0; index < 26; index++) {
        if (rotor1[index] == c) {
            c = 'A' + (index - rotor_pos[0] + 26) % 26;
            break;
        }
    }

    // rotate after each char is decrrypt
    rotor_pos[0] = (rotor_pos[0] + 1) % 26;
    if (rotor_pos[0] == 0) rotor_pos[1] = (rotor_pos[1] + 1) % 26;
    if (rotor_pos[1] == 0) rotor_pos[2] = (rotor_pos[2] + 1) % 26;

    return c;
}
void encrypt_message(char *message, char *encrypted_message) {
    for (int i = 0; i < strlen(message); i++) {
        encrypted_message[i] = encrypt_char(message[i]);
    }
    encrypted_message[strlen(message)] = '\0';
}

void decrypt_message(char *encrypted_message, char *decrypted_message) {
    for (int i = 0; i < strlen(encrypted_message); i++) {
        decrypted_message[i] = decrypt_char(encrypted_message[i]);
    }
    decrypted_message[strlen(encrypted_message)] = '\0';
}

int main() {
    char message[100];
    char encrypted_message[100];
    char decrypted_message[100];
// input messagee
    printf("Enter the message that neeeds to be encrypted: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0'; 

    encrypt_message(message, encrypted_message);
    printf("Original: %s\n", message);
    printf("Encrypted: %s\n", encrypted_message);

    decrypt_message(encrypted_message, decrypted_message);
    printf("Decrypted: %s\n", decrypted_message);

    return 0;
}

