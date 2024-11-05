//
// Created by evalentin on 10/26/24.
//

#ifndef DES_H
#define DES_H
#include <stdint.h>
uint64_t TextTo64Bit(const char *plaintext);
uint64_t DESEncrypt(uint64_t plaintext, uint64_t key);
#endif //DES_H
