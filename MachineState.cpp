#include "MachineState.h"

MachineState::MachineState()
{
    pc = 0;
    ir = 0;
    halted = false;

    screenText = "";

    for (int i = 0; i < 16; i++)
    {
        registers[i] = 0;
    }

    for (int i = 0; i < 256; i++)
    {
        memory[i] = 0;
    }
}