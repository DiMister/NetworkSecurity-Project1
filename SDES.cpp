#include "SDES.h"
#include <iostream>
#include <fstream>

// Initialize static permutation tables
const int SDES::P10[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int SDES::P8[8] = {6, 3, 7, 4, 8, 5, 10, 9};
const int SDES::IP[8] = {2, 6, 3, 1, 4, 8, 5, 7};
const int SDES::IP_INV[8] = {4, 1, 3, 5, 7, 2, 8, 6};
const int SDES::EP[8] = {4, 1, 2, 3, 2, 3, 4, 1};
const int SDES::P4[4] = {2, 4, 3, 1};

// S-Box 0
const int SDES::S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

// S-Box 1
const int SDES::S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

SDES::SDES(const std::bitset<10>& key) : masterKey(key) {
    generateSubkeys();
}

std::bitset<10> SDES::permute10(const std::bitset<10>& input, const int* table) {
    std::bitset<10> output;
    for (int i = 0; i < 10; i++) {
        output[9-i] = input[10-table[i]];
    }
    return output;
}

std::bitset<8> SDES::permute8(const std::bitset<8>& input, const int* table) {
    std::bitset<8> output;
    for (int i = 0; i < 8; i++) {
        if (table == P8) {
            // For P8, we're working with a 10-bit input stored in 8-bit
            // We need to handle this differently
            output[7-i] = input[10-table[i]];
        } else {
            output[7-i] = input[8-table[i]];
        }
    }
    return output;
}

std::bitset<8> SDES::permute8(const std::bitset<10>& input, const int* table) {
    std::bitset<8> output;
    for (int i = 0; i < 8; i++) {
        output[7-i] = input[10-table[i]];
    }
    return output;
}

std::bitset<4> SDES::permute4(const std::bitset<4>& input, const int* table) {
    std::bitset<4> output;
    for (int i = 0; i < 4; i++) {
        output[3-i] = input[4-table[i]];
    }
    return output;
}

std::bitset<8> SDES::expandPermute(const std::bitset<4>& input) {
    std::bitset<8> output;
    for (int i = 0; i < 8; i++) {
        output[7-i] = input[4-EP[i]];
    }
    return output;
}

std::bitset<10> SDES::leftShift(const std::bitset<10>& input, int positions) {
    std::bitset<10> output = input;
    
    // Split into two 5-bit halves
    std::bitset<5> left, right;
    
    // Extract left half (bits 9-5)
    for (int i = 0; i < 5; i++) {
        left[i] = input[i + 5];
    }
    
    // Extract right half (bits 4-0)
    for (int i = 0; i < 5; i++) {
        right[i] = input[i];
    }
    
    // Perform left shift on each half
    for (int shift = 0; shift < positions; shift++) {
        bool leftMSB = left[4];
        bool rightMSB = right[4];
        
        left <<= 1;
        left[0] = leftMSB;
        
        right <<= 1;
        right[0] = rightMSB;
    }
    
    // Recombine halves
    for (int i = 0; i < 5; i++) {
        output[i + 5] = left[i];
        output[i] = right[i];
    }
    
    return output;
}

std::bitset<2> SDES::sboxLookup(const std::bitset<4>& input, const int sbox[4][4]) {
    int row = (input[3] << 1) | input[0];  // First and last bits
    int col = (input[2] << 1) | input[1];  // Middle two bits
    
    int value = sbox[row][col];
    return std::bitset<2>(value);
}

std::bitset<4> SDES::fFunction(const std::bitset<4>& right, const std::bitset<8>& subkey) {
    // Expansion/Permutation
    std::bitset<8> expanded = expandPermute(right);
    
    // XOR with subkey
    expanded ^= subkey;
    
    // Split into two 4-bit halves for S-boxes
    std::bitset<4> left4, right4;
    for (int i = 0; i < 4; i++) {
        left4[i] = expanded[i + 4];
        right4[i] = expanded[i];
    }
    
    // S-box substitution
    std::bitset<2> s0_output = sboxLookup(left4, S0);
    std::bitset<2> s1_output = sboxLookup(right4, S1);
    
    // Combine S-box outputs
    std::bitset<4> combined;
    combined[3] = s0_output[1];
    combined[2] = s0_output[0];
    combined[1] = s1_output[1];
    combined[0] = s1_output[0];
    
    // P4 permutation
    return permute4(combined, P4);
}

void SDES::generateSubkeys() {
    // Apply P10 permutation
    std::bitset<10> permuted = permute10(masterKey, P10);
    
    // Left shift by 1 for K1
    std::bitset<10> shifted1 = leftShift(permuted, 1);
    k1 = permute8(shifted1, P8);
    
    // Left shift by 2 more (total 3) for K2
    std::bitset<10> shifted3 = leftShift(shifted1, 2);
    k2 = permute8(shifted3, P8);
}

std::bitset<8> SDES::encrypt(const std::bitset<8>& plaintext) {
    // Initial permutation
    std::bitset<8> permuted = permute8(plaintext, IP);
    
    // Split into left and right halves
    std::bitset<4> left, right;
    for (int i = 0; i < 4; i++) {
        left[i] = permuted[i + 4];
        right[i] = permuted[i];
    }
    
    // Round 1: fK1
    std::bitset<4> f_output = fFunction(right, k1);
    std::bitset<4> new_right = left ^ f_output;
    std::bitset<4> new_left = right;
    
    // Round 2: fK2
    f_output = fFunction(new_right, k2);
    std::bitset<4> final_left = new_left ^ f_output;
    std::bitset<4> final_right = new_right;
    
    // Combine halves
    std::bitset<8> combined;
    for (int i = 0; i < 4; i++) {
        combined[i + 4] = final_left[i];
        combined[i] = final_right[i];
    }
    
    // Final permutation (IP^-1)
    return permute8(combined, IP_INV);
}

std::bitset<8> SDES::decrypt(const std::bitset<8>& ciphertext) {
    // Initial permutation
    std::bitset<8> permuted = permute8(ciphertext, IP);
    
    // Split into left and right halves
    std::bitset<4> left, right;
    for (int i = 0; i < 4; i++) {
        left[i] = permuted[i + 4];
        right[i] = permuted[i];
    }
    
    // Round 1: fK2 (reversed order)
    std::bitset<4> f_output = fFunction(right, k2);
    std::bitset<4> new_right = left ^ f_output;
    std::bitset<4> new_left = right;
    
    // Round 2: fK1
    f_output = fFunction(new_right, k1);
    std::bitset<4> final_left = new_left ^ f_output;
    std::bitset<4> final_right = new_right;
    
    // Combine halves
    std::bitset<8> combined;
    for (int i = 0; i < 4; i++) {
        combined[i + 4] = final_left[i];
        combined[i] = final_right[i];
    }
    
    // Final permutation (IP^-1)
    return permute8(combined, IP_INV);
}

bool SDES::encryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return false;
    }
    
    char ch;
    while (inFile.get(ch)) {
        std::bitset<8> plaintext = charToBinary(ch);
        std::bitset<8> ciphertext = encrypt(plaintext);
        
        // Convert bitset back to char and write
        char encrypted_char = static_cast<char>(ciphertext.to_ulong());
        outFile.put(encrypted_char);
    }
    
    inFile.close();
    outFile.close();
    return true;
}

bool SDES::decryptFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return false;
    }
    
    char ch;
    while (inFile.get(ch)) {
        std::bitset<8> ciphertext = charToBinary(ch);
        std::bitset<8> plaintext = decrypt(ciphertext);
        
        // Convert bitset back to char and write
        char decrypted_char = static_cast<char>(plaintext.to_ulong());
        outFile.put(decrypted_char);
    }
    
    inFile.close();
    outFile.close();
    return true;
}

std::string SDES::binaryToString(const std::bitset<8>& bits) {
    return bits.to_string();
}

std::bitset<8> SDES::charToBinary(char c) {
    return std::bitset<8>(static_cast<unsigned char>(c));
}