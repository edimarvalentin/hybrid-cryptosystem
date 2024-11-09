//
// Created by evalentin on 10/26/24.
//

#ifndef DES_H
#define DES_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PLAINTEXT_SIZE 8
uint64_t DESEncrypt(uint64_t plaintext, uint64_t key);
uint64_t DESDecrypt(uint64_t ciphertext, uint64_t key);
uint64_t TextTo64Bit(const char *plaintext);
char* Bit64ToText(const uint64_t b64);
size_t HexStringTo64BitBlocks(const char *hex_string, uint64_t **array);
#endif //DES_H
