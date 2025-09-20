#include <iostream>
#include <string>
#include <bitset>
#include "SDES.h"
using namespace std;

void encryptFile(SDES sdes, string inputFile, string outputFile) {
    cout << "Encrypting file: " << inputFile << endl;
    if (sdes.encryptFile(inputFile, outputFile)) {
        cout << "File encrypted successfully!" << endl;
        cout << "Encrypted file saved as: " << outputFile << endl;
    } else {
        cout << "Encryption failed! Please check file paths." << endl;
    }
}

void decryptFile(SDES sdes, string inputFile, string outputFile) {
    cout << "Decrypting file: " << inputFile << endl;
    if (sdes.decryptFile(inputFile, outputFile)) {
        cout << "File decrypted successfully!" << endl;
        cout << "Decrypted file saved as: " << outputFile << endl;
    } else {
        cout << "Decryption failed! Please check file paths." << endl;
    }
}

int main() {    
    cout << "=== SDES File Encryption/Decryption ===" << endl;

    bitset<10> key = 0b1011111111;
    cout << "Using key: " << key << endl;
    SDES sdes(key);

    encryptFile(sdes, "input.txt", "encrypted.txt");

    decryptFile(sdes, "encrypted.txt", "decrypted.txt");
      
    return 0;
}