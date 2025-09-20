#ifndef SDES_H
#define SDES_H

#include <bitset>
#include <vector>
#include <string>

class SDES {
private:
    // P10 permutation table for 10-bit key
    static const int P10[10];
    
    // P8 permutation table for 8-bit subkey generation
    static const int P8[8];
    
    // Initial Permutation (IP) table
    static const int IP[8];
    
    // Final Permutation (IP-1) table  
    static const int IP_INV[8];
    
    // Expansion/Permutation (EP) table
    static const int EP[8];
    
    // P4 permutation table
    static const int P4[4];
    
    // S-Box 0
    static const int S0[4][4];
    
    // S-Box 1
    static const int S1[4][4];
    
    std::bitset<10> masterKey;
    std::bitset<8> k1, k2;
    
    // Helper functions
    std::bitset<10> permute10(const std::bitset<10>& input, const int* table);
    std::bitset<8> permute8(const std::bitset<8>& input, const int* table);
    std::bitset<8> permute8(const std::bitset<10>& input, const int* table);
    std::bitset<4> permute4(const std::bitset<4>& input, const int* table);
    std::bitset<8> expandPermute(const std::bitset<4>& input);
    std::bitset<10> leftShift(const std::bitset<10>& input, int positions);
    std::bitset<2> sboxLookup(const std::bitset<4>& input, const int sbox[4][4]);
    std::bitset<4> fFunction(const std::bitset<4>& right, const std::bitset<8>& subkey);
    
    void generateSubkeys();
    
public:
    // Constructor
    SDES(const std::bitset<10>& key);
    
    // Main encryption/decryption functions
    std::bitset<8> encrypt(const std::bitset<8>& plaintext);
    std::bitset<8> decrypt(const std::bitset<8>& ciphertext);
    
    // File operations
    bool encryptFile(const std::string& inputFile, const std::string& outputFile);
    bool decryptFile(const std::string& inputFile, const std::string& outputFile);
    
    // Utility functions
    std::string binaryToString(const std::bitset<8>& bits);
    std::bitset<8> charToBinary(char c);
};

#endif // SDES_H