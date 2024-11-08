//
// Created by evalentin on 10/26/24.
//

#include "des.h"

#define DES_KEY_SIZE            64
#define DES_BLOCK_SIZE          64
#define DES_ROUNDS              16
#define DES_PC1_SIZE            56
#define DES_PC2_SIZE            48
#define PERMUTATION_SIZE        32
#define EXPANSION_SIZE          48
#define S_BOXES                 8
#define UINT64_CHUNK_SIZE       16
#define L64_MASK                0xffffffff00000000
#define R64_MASK                0x00000000ffffffff
#define L56_MASK                0x00fffffff0000000
#define R56_MASK                0x000000000fffffff
#define R28_SHIFT_LEFT1_MASK    0x000000001ffffffe
#define R28_SHIFT_LEFT2_MASK    0x000000003ffffffc
#define L28_LSB1_MASK           0x0000000010000000
#define R28_LSB1_MASK           0x0000000000000001
#define L28_LSB2_MASK           0x0000000030000000
#define R28_LSB2_MASK           0x0000000000000003

/* Initial Permutation Table */
static int IP_TABLE[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

/* Inverse Permutation Table */
static int INVERSE_IP_TABLE[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};

/* Expansion Permutation Table */
static int E[] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

/* Permutation Function */
static int P[] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

/* The S-Box tables */
static uint64_t S[8][64] = {{
    /* S1 */
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
},{
    /* S2 */
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
},{
    /* S3 */
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
},{
    /* S4 */
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
},{
    /* S5 */
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
},{
    /* S6 */
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
},{
    /* S7 */
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
},{
    /* S8 */
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
}};

/* Permuted Choice 1 Table */
static int PC1[] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,

    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

/* Permuted Choice 2 Table */
static int PC2[] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

/* Iteration Shift Array */
static int shift_round[] = {
    //Round number
    /* 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    1,  1,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2,  2,  1 // number of shifts
};


/**
 * @brief First step of the DES is to permute the input plaintext.
 * @param plaintext 64-bit plaintext
 * @return 64-bit permuted input
 */
uint64_t InitialPermutation(const uint64_t plaintext) {
    uint64_t permuted_input = 0x0000000000000000;
    for (int i = 0; i < DES_BLOCK_SIZE; i++) {
        const int bit_pos = IP_TABLE[i];
        const uint64_t bit = (plaintext >> (bit_pos - 1)) & 1; // shift to the bit and get it
        permuted_input |= bit << i; // place in the permuted input
    }
    return permuted_input;
}


/**
 * @brief Final step of a DES round
 * @param pre_output 64-bit input after DES rounds
 * @return 64-bit ciphertext
 */
uint64_t InversePermutation(const uint64_t pre_output) {
    uint64_t ciphertext = 0x0000000000000000;
    for(int i = 0; i < DES_BLOCK_SIZE; i++) {
        const int bit_pos = INVERSE_IP_TABLE[i];
        const uint64_t bit = (pre_output >> (bit_pos - 1)) & 1; // shift to the bit and get it
        ciphertext |= bit << i; // place in the permuted input
    }
    return ciphertext;
}

/**
 * @brief Takes a char array of MAX LENGTH 8 and packs it inside
 * a 64-bit integer.
 * @param text char pointer to the first letter in the array
 * @return 64-bit unsigned integer
 */
uint64_t TextTo64Bit(const char *text) {
    const int length = strlen(text);
    if (length > MAX_PLAINTEXT_SIZE) {
        perror("Can't fit text into 64 bits");
        return -1;
    }
    uint64_t b64 = 0x0000000000000000;

    for (int i = 0; i < length; i++) {
        b64 |= (uint64_t)(text[i] & 0xFF) << ((7 - i) * 8);  // Shift each byte by multiples of 8
    }
    return b64;
}

/**
 * @brief Converts a 64 bit block into a char array
 * @param b64 unsigned 64 bit integer
 * @return a char array of 8 characters maximum
 */
char* Bit64ToText(const uint64_t b64) {
    // Allocate memory for 8 characters + null terminator
    char *text = malloc(9 * sizeof(char));

    if (text == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Convert each byte to a character and store in reverse order
    for (int i = 0; i < 8; i++) {
        text[7 - i] = (char)((b64 >> (i * 8)) & 0xFF);  // Extract each byte
    }
    text[8] = '\0';  // Null-terminate the string

    return text;  // Return the pointer to the caller
}

/**
 * @brief Expands the input of F to 48-bits
 * @param R32 32 bit
 * @return a 48-bit expansion permutation
 */
uint64_t Expansion(const uint64_t R32) {
    uint64_t R48 = 0;
    for (int i = 0; i < EXPANSION_SIZE; i++) {
        const int bit_pos = E[i];  // Get the bit position from the expansion table
        const uint64_t bit = (R32 >> (32 - bit_pos)) & 1;  // Extract the bit from R
        R48 |= bit << (EXPANSION_SIZE - 1 - i);  // Place the bit in the correct position in b48
    }
    return R48;
}

/**
 * @brief Brings down the key to 58-bits
 * @param K64 The full 64-bit
 * @return 58-bit permuted key
 */
uint64_t PermutedChoice1(const uint64_t K64) {
    uint64_t K56 = 0x0000000000000000;
    for(int i = 0; i < DES_PC1_SIZE; i++) {
        const int bit_pos = PC1[i];
        const uint64_t bit = (K64 >> (DES_KEY_SIZE - bit_pos)) & 1;
        K56 |= bit << (DES_PC1_SIZE - i - 1);
    }
    return K56;
}

/**
 * Brings down
 * @param K56 The 56-bit key
 * @return 48-bit permuted key
 */
uint64_t PermutedChoice2(const uint64_t K56) {
    uint64_t K48 = 0x0000000000000000;
    for(int i = 0; i < DES_PC2_SIZE; i++) {
        const int bit_pos = PC2[i];
        const uint64_t bit = (K56 >> (DES_PC1_SIZE - bit_pos)) & 1;
        K48 |= bit << (DES_PC2_SIZE - i - 1);
    }
    return K48;
}

/**
 * @brief Shifts circularly the upper and lower 28-bits of the
 * key
 * @param K56 The 56-bit key
 * @param round the number of current round
 * @return a circular left shifted key
 */
uint64_t LeftCircularShift(const uint64_t K56, const int round) {
    // split the 56 bits
    uint64_t L28 = K56 & L56_MASK;
    uint64_t R28 = K56 & R56_MASK;

    // How many times do we shift in this round?
    const int shifts = shift_round[round];

    // shift left by shifts
    L28 <<= shifts;
    R28 <<= shifts;

    if(shifts == 1) {
        // the LSB behavior on a left shift is undefined
        // mask it out
        R28 &= R28_SHIFT_LEFT1_MASK;

        // put the overflowing bit in the LSB position
        L28 |= (L28 >> 28) & L28_LSB1_MASK;
        R28 |= (R28 >> 28) & R28_LSB1_MASK;

    }else {
        // mask it out
        R28 &= R28_SHIFT_LEFT2_MASK;

        // put the overflowing bit duo last
        L28 |= (L28 >> 28) & L28_LSB2_MASK;
        R28 |= (R28 >> 28) & R28_LSB2_MASK;

    }

    // Clear out overflowing bits
    L28 &= L56_MASK;
    R28 &= R56_MASK;

    // Merge the halves
    return L28 | R28;

}

/**
 * @brief Circularly shift the upper and lower 28-bits of the key
 * @param K56 The 56-bit key
 * @param round the number of the current round
 * @return a circular right shifted key
 */
uint64_t RightCircularShift(const uint64_t K56, const int round) {
    // Split the 56-bit key into two 28-bit halves
    uint64_t L28 = K56 & L56_MASK;
    uint64_t R28 = K56 & R56_MASK;

    // how many shifts?
    const int shifts = shift_round[round];

    // Right circular shift
    L28 = (L28 >> shifts) | ((L28 << (28 - shifts)) & L56_MASK);
    R28 = (R28 >> shifts) | ((R28 << (28 - shifts)) & R56_MASK);

    // Mask to ensure we only have 28 bits in each half
    L28 &= L56_MASK;
    R28 &= R56_MASK;

    // combine the halves
    return L28 | R28;
}

/**
 * @brief S-Box substituted the 48-bits
 * @param R48 The lower 48-bits
 * @return S-box substituted 32-bits
 */
uint64_t SBoxSubstitution(const uint64_t R48) {
    uint64_t b32 = 0x00000000;

    for (int i = 0; i < S_BOXES; ++i) {
        // extract 6 bits for the current block
        const uint8_t six_bits = (R48 >> (42 - 6 * i)) & 0x3F; // Mask to get 6 bits

        // calculate the row: first and last bits of the 6-bit block
        const uint8_t row = ((six_bits & 0x20) >> 4) | (six_bits & 0x01);

        // calculate the column: middle 4 bits of the 6-bit block
        const uint8_t col = (six_bits >> 1) & 0x0F;

        // compute index into the S-box array
        const uint8_t index = (row << 4) | col; // index between 0 and 63

        // get the S-box value for the current block
        const uint8_t value = S[i][index];

        // append the 4-bit S-box value to the output
        b32 = (b32 << 4) | (value & 0x0F);
    }

    return b32;
}

/**
 * Takes in out 32-bit sbox substition and permutes it
 * to increase diffusion.
 * @param R32 The lower 32-bits
 * @return 32-bit permutation
 */
uint64_t Permutation(const uint64_t R32) {
    uint64_t b32 = 0x0000000000000000;
    for (int i = 0; i < PERMUTATION_SIZE; i++) {
        const int bit_pos = P[i];
        const uint64_t bit = (R32 >> (PERMUTATION_SIZE - bit_pos)) & 1; //shift to the bit and get it
        b32 |= bit << (PERMUTATION_SIZE - i - 1); // place in permuted output
    }
    return b32;
}

/**
 * @brief The F function
 * @param R32 The lower 32-bits
 * @param K The 48-bit key
 * @return 32-bits
 */
uint64_t F(uint64_t R32, const uint64_t K){
    // Step 1: Expansion permutation from 32 bits to 48 bits
    uint64_t R48 = Expansion(R32);

    // Step 2: bitwise XOR with permuted and shifted Key
    R48 = R48 ^ K;

    // Step 3: Substitution box
    R32 = SBoxSubstitution(R48);

    // Step 4: Permutation
    R32 = Permutation(R32);

    return R32;
}

/**
 * @brief Data Encryption Standard - Encryption
 * @param plaintext the 64-bit plaintext
 * @param key the 64-bit key
 * @return 64-bit ciphertext
 */
uint64_t DESEncrypt(const uint64_t plaintext, const uint64_t key) {

    // Initial Permutation
    const uint64_t permuted_input = InitialPermutation(plaintext);

    //Split in half
    uint64_t L32 = permuted_input & L64_MASK;
    uint64_t R32 = permuted_input & R64_MASK;

    //Permuted choice 1
    uint64_t K56 = PermutedChoice1(key); // This key will be doing circular shifts

    for(int R = 0; R < DES_ROUNDS; R++) {

        //left circular shift
        K56 = LeftCircularShift(K56, R);

        //Permute and reduce key to 48 bits
        const uint64_t K48 = PermutedChoice2(K56);

        // Save it for now
        const uint64_t temp = R32;

        // Function F and bitwise XOR the output with the left
        R32 = F(R32, K48) ^ (L32 >> 32);

        L32 = (temp << 32) & L64_MASK; // Get the original R32
    }

    // Swap the sides
    const uint64_t temp = L32; // save it for now
    L32 = (R32 << 32) & L64_MASK;
    R32 = (temp >> 32) & R64_MASK;

    // the ciphertext
    return InversePermutation(L32 | R32);

}

/**
 * @brief Data Encryption Standard - Decryption
 * @param ciphertext 64-bit ciphertext
 * @param key 64-bit key
 * @return 64-bit plaintext
 */
uint64_t DESDecrypt(const uint64_t ciphertext, const uint64_t key) {
    // Initial Permutation
    const uint64_t permuted_input = InitialPermutation(ciphertext);

    // Split in half
    uint64_t L32 = permuted_input & L64_MASK;
    uint64_t R32 = permuted_input & R64_MASK;

    // Permute 64-bit key
    uint64_t K56 = PermutedChoice1(key);

    for (int r = 0; r < DES_ROUNDS; r++) {
        K56 = LeftCircularShift(K56, r);
    }


    for (int R = DES_ROUNDS - 1; R >= 0; R--) {  // go in reverse

        //permute and reduce key to 48 bits
        const uint64_t K48 = PermutedChoice2(K56);

        // Save it for now
        const uint64_t temp = R32;

        // Function F and bitwise XOR the output with the left
        R32 = F(R32, K48) ^ (L32 >> 32);

        L32 = (temp << 32) & L64_MASK; // Get the original R32

        K56 = RightCircularShift(K56, R);

    }

    // Swap the sides
    const uint64_t temp = L32;
    L32 = (R32 << 32) & L64_MASK;
    R32 = (temp >> 32) & R64_MASK;

    // The plaintext
    return InversePermutation(L32 | R32);
}

/**
 * @brief Convert a hexadecimal string into an array of uint64_t values
 * @param hex_string The input hexadecimal string
 * @param array The output array of uint64_t values
 * @return The number of uint64_t values filled in the array
 */
size_t HexStringTo64BitBlocks(const char *hex_string, uint64_t **array) {
    size_t length = strlen(hex_string);
    size_t num_chunks = (length + UINT64_CHUNK_SIZE - 1) / UINT64_CHUNK_SIZE;  // Calculate the number of 64-bit chunks
    *array = malloc(num_chunks * sizeof(uint64_t));  // Allocate memory for the array of uint64_t

    if (!*array) {
        perror("Memory allocation failed");
        return 0;
    }

    for (size_t i = 0; i < num_chunks; i++) {
        // Prepare a temporary buffer for each 16-character chunk
        char chunk[UINT64_CHUNK_SIZE + 1] = {0};  // Initialize with zeroes
        size_t chunk_length = UINT64_CHUNK_SIZE;

        // Adjust chunk length if it's the last chunk and not a full 16 characters
        if (i * UINT64_CHUNK_SIZE + chunk_length > length) {
            chunk_length = length - i * UINT64_CHUNK_SIZE;
        }

        // Copy the chunk into the buffer
        strncpy(chunk, hex_string + i * UINT64_CHUNK_SIZE, chunk_length);

        // Convert the chunk to uint64_t and store in the array
        (*array)[i] = strtoull(chunk, NULL, 16);
    }

    return num_chunks;  // Return the number of uint64_t elements in the array
}



