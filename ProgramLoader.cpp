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
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) continue;

        if (line.length() != 4) {
            lastError = "Invalid instruction format (must be exactly 4 hex digits): " + line;
            infile.close();
            return false;
        }

        for (char c : line) {
            if (!isxdigit(static_cast<unsigned char>(c))) {
                lastError = "Invalid hex character in instruction: " + line;
                infile.close();
                return false;
            }
        }

        uint16_t instrValue = static_cast<uint16_t>(std::stoi(line, nullptr, 16));
        instructions.push_back(instrValue);
    }

    infile.close();

    if (startAddress + (static_cast<int>(instructions.size()) * 2) > 0xFF) {
        lastError = "Program size exceeds memory limit!";
        return false;
    }

    try {
        for (uint16_t instr : instructions) {
            uint8_t highByte = (instr >> 8) & 0xFF;
            uint8_t lowByte = instr & 0xFF;
            
            memory.writeByte(currentAddress, highByte);
            memory.writeByte(currentAddress + 1, lowByte);
            currentAddress += 2;
        }
    } catch (const std::exception& e) {
        lastError = std::string("Memory write error: ") + e.what();
        return false;
    }

    return true;
}
