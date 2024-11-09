//
// Created by evalentin on 10/26/24.
//

#ifndef DES_H
#define DES_H
#include <stdint.h>
#define MAX_PLAINTEXT_SIZE 8
uint64_t DESEncrypt(uint64_t plaintext, uint64_t key);
uint64_t DESDecrypt(uint64_t ciphertext, uint64_t key);
uint64_t TextTo64Bit(const char *plaintext);
char *Bit64ToText(const uint64_t b64, char *text);
#endif //DES_H
