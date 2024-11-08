//
// Created by evalentin on 10/26/24.
//

#ifndef DES_H
#define DES_H
#include <stdint.h>
uint64_t DESEncrypt(uint64_t plaintext, uint64_t key);
uint64_t DESDecrypt(uint64_t ciphertext, uint64_t key);
uint64_t TextTo64Bit(const char *plaintext);
#endif //DES_H
