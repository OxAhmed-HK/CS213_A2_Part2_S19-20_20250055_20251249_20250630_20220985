#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <cstdint>
#include <string>

struct MachineState
{
    int registers[16];
    int pc;
    int ir;
    bool halted;

    uint8_t memory[256];

    std::string screenText;

    MachineState();
};

#endif
