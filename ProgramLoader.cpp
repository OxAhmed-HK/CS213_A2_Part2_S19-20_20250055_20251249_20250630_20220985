#include "ProgramLoader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

ProgramLoader::ProgramLoader() : fileName(""), lastError("") {}

bool ProgramLoader::validateStartAddress(int startAddress) const {
    if (startAddress < 0 || startAddress >= 0xFF) return false;
    if (startAddress % 2 != 0) return false;
    return true;
}

std::string ProgramLoader::getLastError() const {
    return lastError;
}

bool ProgramLoader::loadProgram(const std::string& file, int startAddress, Memory& memory) {
    fileName = file;
    std::ifstream infile(fileName);
    
    if (!infile.is_open()) {
        lastError = "Cannot open file: " + fileName;
        return false;
    }

    if (!validateStartAddress(startAddress)) {
        lastError = "Invalid start address. Must be even and less than FF.";
        infile.close();
        return false;
    }

    std::string line;
    int currentAddress = startAddress;
    std::vector<uint16_t> instructions;

    while (std::getline(infile, line)) {
        // Strip comments starting with ';'
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        // Trim leading and trailing whitespace
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        // Skip empty lines
        if (line.empty()) continue;

        // Check format (must be 4 hex digits)
        if (line.length() < 4) {
            lastError = "Invalid instruction format (less than 4 hex digits): " + line;
            infile.close();
            return false;
        }

        std::string hexStr = line.substr(0, 4);
        for (char c : hexStr) {
            if (!isxdigit(static_cast<unsigned char>(c))) {
                lastError = "Invalid hex character in instruction: " + hexStr;
                infile.close();
                return false;
            }
        }

        uint16_t instrValue = static_cast<uint16_t>(std::stoi(hexStr, nullptr, 16));
        instructions.push_back(instrValue);
    }

    infile.close();

    // Bound check
    if (startAddress + (static_cast<int>(instructions.size()) * 2) > 0x100) {
        lastError = "Program size exceeds memory limit!";
        return false;
    }

    // Write instructions byte by byte (High byte then Low byte)
    for (uint16_t instr : instructions) {
        uint8_t highByte = (instr >> 8) & 0xFF;
        uint8_t lowByte = instr & 0xFF;
        
        memory.writeByte(currentAddress, highByte);
        memory.writeByte(currentAddress + 1, lowByte);
        currentAddress += 2;
    }

    return true;
}
