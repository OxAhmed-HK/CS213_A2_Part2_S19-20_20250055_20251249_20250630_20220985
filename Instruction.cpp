#include "Instruction.h"
#include <cctype>

using namespace std;

Instruction::Instruction(uint16_t word) : rawWord(word) {}

uint16_t Instruction::parseHex(const string& line) {
    size_t start = 0;
    while (start < line.size() && isspace(static_cast<unsigned char>(line[start]))) {
        ++start;
    }

    if (start + 4 > line.size()) {
        return 0x0000;
    }

    uint16_t result = 0;
    for (size_t i = 0; i < 4; ++i) {
        char c = line[start + i];
        if (!isxdigit(static_cast<unsigned char>(c))) {
            return 0x0000;
        }

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

    return result;
}

bool Instruction::isValidWord(uint16_t word) {
    int opcode = (word >> 12) & 0x0F;
    int r      = (word >> 8)  & 0x0F;
    int s      = (word >> 4)  & 0x0F;
    int t      = word & 0x0F;

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
            return (r == 0);

        case 0xA:
            return (s == 0);

        case 0xC:
            return (r == 0 && s == 0 && t == 0);

        default:
            return false;
    }
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