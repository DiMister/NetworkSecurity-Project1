# SDES File Encryption/Decryption Tool

## Overview
This project implements the Simplified Data Encryption Standard (SDES) algorithm in C++ for encrypting and decrypting files containing character data. SDES is a simplified version of the Data Encryption Standard (DES) designed for educational purposes to demonstrate the principles of symmetric-key cryptography.

## Features
- Complete SDES implementation with proper key scheduling
- File encryption and decryption capabilities
- Interactive command-line interface
- Built-in testing functionality
- Support for any text files or binary files
- Cross-platform compatibility

## Algorithm Details
SDES operates on:
- **Key size**: 10 bits
- **Block size**: 8 bits
- **Rounds**: 2
- **Subkeys**: K1 and K2 (8 bits each)

### Key Components:
1. **Permutation Tables**: P10, P8, P4, IP, IP⁻¹, E/P
2. **S-Boxes**: S0 and S1 (4×4 substitution tables)
3. **Key Scheduling**: Generates two 8-bit subkeys from 10-bit master key
4. **Feistel Structure**: Two rounds with function f

## Compilation

### Using Makefile (Recommended)
```bash
# Build the program
make

# Create sample test files
make samples

# Clean build artifacts
make clean

# Run basic test
make test
```

### Manual Compilation
```bash
# Using g++
g++ -std=c++11 -Wall -Wextra -O2 -o sdes_crypto main.cpp SDES.cpp

# Using Microsoft Visual C++
cl /EHsc main.cpp SDES.cpp /Fe:sdes_crypto.exe
```

## Usage

### Running the Program
```bash
./sdes_crypto          # On Linux/Mac
sdes_crypto.exe        # On Windows
```

### Interactive Menu
The program provides an interactive menu with the following options:
1. **Encrypt a file** - Encrypt any file using SDES
2. **Decrypt a file** - Decrypt a previously encrypted file
3. **Test SDES** - Run built-in tests to verify implementation
4. **Exit** - Quit the program

### Example Workflow
1. Choose option 1 (Encrypt a file)
2. Enter input file path: `sample_message.txt`
3. Enter output file path: `encrypted_message.bin`
4. Enter 10-bit key: `1010000010`
5. File is encrypted and saved

To decrypt:
1. Choose option 2 (Decrypt a file)
2. Enter encrypted file path: `encrypted_message.bin`
3. Enter output file path: `decrypted_message.txt`
4. Enter the same 10-bit key: `1010000010`
5. File is decrypted and saved

## Test Files Included
- `sample_message.txt` - Sample text with various characters
- `test_short.txt` - Short test file for quick testing

## Key Format
Keys must be exactly 10 bits long and contain only 0s and 1s.

### Example Valid Keys:
- `1010000010`
- `0000000000`
- `1111111111`
- `1001010010`

## File Formats
- **Input**: Any text or binary file
- **Output**: Binary file containing encrypted data
- **Decrypted**: Original file format restored

## Implementation Notes

### Security Warning
⚠️ **SDES is designed for educational purposes only and should NOT be used for actual security applications.** It has known vulnerabilities and a small key space (only 2¹⁰ = 1024 possible keys).

### Technical Details
- Each character is encrypted independently as an 8-bit block
- Binary files are supported but may contain non-printable characters after encryption
- The implementation follows the standard SDES specification exactly
- All permutation tables and S-boxes match the standard values

## File Structure
```
NetworkSecurity-Project1/
├── SDES.h              # SDES class header file
├── SDES.cpp            # SDES implementation
├── main.cpp            # Main program with user interface
├── Makefile            # Build configuration
├── README.md           # This documentation
├── sample_message.txt  # Sample test file
└── test_short.txt      # Short test file
```

## Testing

### Built-in Tests
The program includes built-in tests (option 3) that verify:
- Basic encryption/decryption cycle
- ASCII character handling
- Known test vectors

### Manual Testing
1. Create a test file with known content
2. Encrypt it with a known key
3. Decrypt the result with the same key
4. Compare the decrypted file with the original

### Example Test Sequence
```bash
# Create test file
echo "Hello World" > test.txt

# Run program and encrypt test.txt with key 1010000010
# Run program and decrypt the result
# Verify the decrypted content matches original
```

## Troubleshooting

### Common Issues
1. **"Error opening files!"**
   - Check that file paths are correct
   - Ensure you have read/write permissions
   - Use full paths if relative paths don't work

2. **"Key must be exactly 10 bits long!"**
   - Ensure key contains exactly 10 characters
   - Use only 0s and 1s

3. **Compilation errors**
   - Ensure you have a C++11 compatible compiler
   - Check that all source files are in the same directory

### Windows-Specific Notes
- Use backslashes or forward slashes in file paths
- You may need to add `.exe` extension when running
- Use PowerShell or Command Prompt to run the program

## Educational Value
This implementation demonstrates:
- Symmetric-key cryptography principles
- Block cipher design
- Feistel network structure
- Key scheduling algorithms
- Substitution and permutation operations
- File I/O in cryptographic applications

## Author
Network Security Project 1
Created for educational purposes to demonstrate SDES implementation.

## License
This project is created for educational use. Feel free to modify and extend for learning purposes.