#include <iostream>
#include <string>
#include <bitset>
#include "SDES.h"

void printUsage() {
    std::cout << "SDES File Encryption/Decryption Tool" << std::endl;
    std::cout << "Usage: " << std::endl;
    std::cout << "  1. Encrypt a file" << std::endl;
    std::cout << "  2. Decrypt a file" << std::endl;
    std::cout << "  3. Test SDES with sample data" << std::endl;
    std::cout << "  4. Exit" << std::endl;
    std::cout << std::endl;
}

void testSDES() {
    std::cout << "\n=== SDES Test ===" << std::endl;
    
    // Test with known values
    std::bitset<10> key("1010000010");
    std::bitset<8> plaintext("10101010");
    
    std::cout << "Key: " << key << std::endl;
    std::cout << "Plaintext: " << plaintext << std::endl;
    
    SDES sdes(key);
    std::bitset<8> ciphertext = sdes.encrypt(plaintext);
    std::bitset<8> decrypted = sdes.decrypt(ciphertext);
    
    std::cout << "Ciphertext: " << ciphertext << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;
    
    if (plaintext == decrypted) {
        std::cout << "✓ Test PASSED - Decryption matches original plaintext!" << std::endl;
    } else {
        std::cout << "✗ Test FAILED - Decryption does not match original!" << std::endl;
    }
    
    // Test with ASCII character
    std::cout << "\n=== ASCII Character Test ===" << std::endl;
    char testChar = 'A';
    std::bitset<8> charBits = sdes.charToBinary(testChar);
    std::bitset<8> encryptedChar = sdes.encrypt(charBits);
    std::bitset<8> decryptedChar = sdes.decrypt(encryptedChar);
    
    std::cout << "Original character: '" << testChar << "' (ASCII " << (int)testChar << ")" << std::endl;
    std::cout << "Binary: " << charBits << std::endl;
    std::cout << "Encrypted: " << encryptedChar << std::endl;
    std::cout << "Decrypted: " << decryptedChar << std::endl;
    std::cout << "Decrypted character: '" << (char)decryptedChar.to_ulong() << "'" << std::endl;
}

std::bitset<10> getKeyFromUser() {
    std::string keyInput;
    std::bitset<10> key;
    
    while (true) {
        std::cout << "Enter 10-bit key (e.g., 1010000010): ";
        std::cin >> keyInput;
        
        if (keyInput.length() != 10) {
            std::cout << "Error: Key must be exactly 10 bits long!" << std::endl;
            continue;
        }
        
        bool validKey = true;
        for (char c : keyInput) {
            if (c != '0' && c != '1') {
                validKey = false;
                break;
            }
        }
        
        if (!validKey) {
            std::cout << "Error: Key must contain only 0s and 1s!" << std::endl;
            continue;
        }
        
        key = std::bitset<10>(keyInput);
        break;
    }
    
    return key;
}

void encryptFile() {
    std::string inputFile, outputFile;
    
    std::cout << "\n=== File Encryption ===" << std::endl;
    std::cout << "Enter input file path: ";
    std::cin >> inputFile;
    std::cout << "Enter output file path: ";
    std::cin >> outputFile;
    
    std::bitset<10> key = getKeyFromUser();
    
    SDES sdes(key);
    
    std::cout << "Encrypting file..." << std::endl;
    if (sdes.encryptFile(inputFile, outputFile)) {
        std::cout << "✓ File encrypted successfully!" << std::endl;
        std::cout << "Encrypted file saved as: " << outputFile << std::endl;
    } else {
        std::cout << "✗ Encryption failed! Please check file paths." << std::endl;
    }
}

void decryptFile() {
    std::string inputFile, outputFile;
    
    std::cout << "\n=== File Decryption ===" << std::endl;
    std::cout << "Enter encrypted file path: ";
    std::cin >> inputFile;
    std::cout << "Enter output file path: ";
    std::cin >> outputFile;
    
    std::bitset<10> key = getKeyFromUser();
    
    SDES sdes(key);
    
    std::cout << "Decrypting file..." << std::endl;
    if (sdes.decryptFile(inputFile, outputFile)) {
        std::cout << "✓ File decrypted successfully!" << std::endl;
        std::cout << "Decrypted file saved as: " << outputFile << std::endl;
    } else {
        std::cout << "✗ Decryption failed! Please check file paths." << std::endl;
    }
}

int main() {
    int choice;
    
    std::cout << "=== SDES File Encryption/Decryption Tool ===" << std::endl;
    std::cout << "Simplified Data Encryption Standard Implementation" << std::endl;
    std::cout << "Author: Network Security Project" << std::endl;
    std::cout << std::endl;
    
    while (true) {
        printUsage();
        std::cout << "Enter your choice (1-4): ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                encryptFile();
                break;
            case 2:
                decryptFile();
                break;
            case 3:
                testSDES();
                break;
            case 4:
                std::cout << "Goodbye!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice! Please enter 1-4." << std::endl;
                break;
        }
        
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        std::cout << std::endl;
    }
    
    return 0;
}