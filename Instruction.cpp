#include "Instruction.h"
#include <cctype>

using namespace std;

Instruction::Instruction(uint16_t word) : rawWord(word) {}

bool Instruction::isValidHexFormat(const string& line) {
    if (line.size() != 4) {
        return false;
    }

    for (size_t i = 0; i < 4; ++i) {
        if (!isxdigit(static_cast<unsigned char>(line[i]))) {
            return false;
        }
    }

    return true;
}

int Instruction::parseHex(const string& line) {
    if (!isValidHexFormat(line)) {
        return -1;
    }

    uint16_t result = 0;
    for (size_t i = 0; i < 4; ++i) {
        char c = line[i];
        uint16_t digit = 0;

        if (c >= '0' && c <= '9') {
            digit = static_cast<uint16_t>(c - '0');
        } else if (c >= 'a' && c <= 'f') {
            digit = static_cast<uint16_t>(10 + (c - 'a'));
        } else if (c >= 'A' && c <= 'F') {
            digit = static_cast<uint16_t>(10 + (c - 'A'));
        }

        result = static_cast<uint16_t>((result << 4) | digit);
    }

    return static_cast<int>(result);
}

bool Instruction::parseHex(const string& line, uint16_t& outWord) {
    int parsed = parseHex(line);
    if (parsed < 0) {
        return false;
    }
    outWord = static_cast<uint16_t>(parsed);
    return true;
}

bool Instruction::isValidWord(int word) {
    if (word < 0 || word > 0xFFFF) {
        return false;
    }

    int opcode = (word >> 12) & 0x0F;
    int d1     = (word >> 8)  & 0x0F;
    int d2     = (word >> 4)  & 0x0F;
    int d3     = word & 0x0F;

    switch (opcode) {
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xB:
            return true;

        case 0x4:
            return (d1 == 0);

        case 0xA:
            return (d2 == 0);

        case 0xC:
            return (d1 == 0 && d2 == 0 && d3 == 0);

        default:
            return false;
    }
}

bool Instruction::isValidInstructionString(const string& line) {
    int word = parseHex(line);
    return isValidWord(word);
}

uint16_t Instruction::getWord() const {
    return rawWord;
}

int Instruction::getOpcode() const {
    return (rawWord >> 12) & 0x0F;
}

int Instruction::getR() const {
    return (rawWord >> 8) & 0x0F;
}

int Instruction::getS() const {
    return (rawWord >> 4) & 0x0F;
}

int Instruction::getT() const {
    return rawWord & 0x0F;
}

int Instruction::getXY() const {
    return rawWord & 0xFF;
}