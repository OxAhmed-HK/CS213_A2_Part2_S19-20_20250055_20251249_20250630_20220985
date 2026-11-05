#include "Memory.h"
#include <stdexcept>

Memory::Memory()
{
    clear();
}

void Memory::clear()
{
    for (int i = 0; i < 256; i++)
    {
        cells[i] = 0;
    }
}

uint8_t Memory::readByte(int address) const
{
    if (address < 0 || address > 255)
        throw std::out_of_range("Invalid memory address");

    return cells[address];
}

void Memory::writeByte(int address, uint8_t value)
{
    if (address < 0 || address > 255)
        throw std::out_of_range("Invalid memory address");

    cells[address] = value;
}

uint16_t Memory::readWord(int address) const
{
    if (address < 0 || address >= 255)
        throw std::out_of_range("Invalid word address");

    return (static_cast<uint16_t>(cells[address]) << 8) | cells[address + 1];
}

void Memory::writeWord(int address, uint16_t word)
{
    if (address < 0 || address >= 255)
        throw std::out_of_range("Invalid word address");

    cells[address] = static_cast<uint8_t>(word >> 8);
    cells[address + 1] = static_cast<uint8_t>(word & 0xFF);
}