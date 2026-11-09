#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "MachineState.h"
#include "Screen.h"
#include "Memory.h"
#include "Screen.h"
#include "CPU.h"
#include "ProgramLoader.h"
#include <string>

enum MachineStatus
{
    READY,
    RUNNING,
    HALTED,
    ERROR
};

class Simulator
{
private:
    Memory memory;
    Screen screen;
    CPU cpu;
    ProgramLoader loader;
    MachineStatus status;
    std::string errorMessage;

public:
    Simulator();

    bool loadProgramFile(const std::string &filename, int startAddress);
    void runAll();
    bool step();
    void reset();

    void editMemoryCell(int address, uint8_t value);
    void editRegister(int regIndex, uint8_t value);

    void displayMachineState() const;
    MachineStatus getStatus() const;
    std::string getErrorMessage() const;
    MachineState getMachineState() const;
};

#endif 
