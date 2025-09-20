# SDES File Encryption/Decryption Makefile
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Target executable
TARGET = sdes_crypto

# Source files
SOURCES = main.cpp SDES.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = SDES.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	del /f $(OBJECTS) $(TARGET).exe 2>nul || true

# Clean and rebuild
rebuild: clean all

# Test the program with sample data
test: $(TARGET)
	@echo Testing SDES implementation...
	echo Hello, World! This is a test file for SDES encryption. > test_input.txt
	./$(TARGET)

# Create sample test files
samples:
	@echo Creating sample test files...
	echo Hello, World! This is a test message for SDES encryption. > sample_message.txt
	echo The quick brown fox jumps over the lazy dog. 1234567890 > sample_text.txt
	echo ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz > sample_alphabet.txt

# Install (copy to system directory - optional)
install: $(TARGET)
	copy $(TARGET).exe C:\Windows\System32\ 2>nul || echo "Install requires administrator privileges"

# Help
help:
	@echo Available targets:
	@echo   all      - Build the SDES encryption program
	@echo   clean    - Remove build artifacts
	@echo   rebuild  - Clean and rebuild
	@echo   test     - Build and run basic test
	@echo   samples  - Create sample test files
	@echo   install  - Install to system directory
	@echo   help     - Show this help message

.PHONY: all clean rebuild test samples install help