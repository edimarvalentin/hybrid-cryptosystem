//
// Created by evalentin on 10/26/24.
//

#ifndef DES_H
#define DES_H
#include <stdint.h>
uint64_t TextTo64Bit(const char *plaintext, int length);
uint64_t InitialPermutation(uint64_t plaintext);
#endif //DES_H
