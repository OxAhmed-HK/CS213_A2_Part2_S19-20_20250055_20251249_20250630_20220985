#include "Memory.h"

using namespace std;

Memory::Memory() {
    clear();
}

uint8_t Memory::readByte(int address) const {
    if (address < 0 || address > 255) {
        return 0;
    }
    return cells[address];
}

void Memory::writeByte(int address, uint8_t value) {
    if (address >= 0 && address <= 255) {
        cells[address] = value;
    }
}

uint16_t Memory::readWord(int address) const {
    uint16_t high = readByte(address);
    uint16_t low  = readByte(address + 1);
    return static_cast<uint16_t>((high << 8) | low);
}

void Memory::writeWord(int address, uint16_t word) {
    uint8_t high = static_cast<uint8_t>((word >> 8) & 0xFF);
    uint8_t low  = static_cast<uint8_t>(word & 0xFF);
    writeByte(address, high);
    writeByte(address + 1, low);
}

void Memory::clear() {
    for (int i = 0; i < 256; ++i) {
        cells[i] = 0;
    }
}