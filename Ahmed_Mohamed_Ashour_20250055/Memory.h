#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

class Memory
{
private:
    uint8_t cells[256];

public:
    Memory();

    uint8_t readByte(int address) const;
    void writeByte(int address, uint8_t value);

    uint16_t readWord(int address) const;
    void writeWord(int address, uint16_t word);

    void clear();
};

#endif