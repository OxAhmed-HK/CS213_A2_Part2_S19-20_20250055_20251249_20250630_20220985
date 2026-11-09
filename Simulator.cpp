#include "Simulator.h"
#include <iostream>
#include <iomanip>

Simulator::Simulator()
    : cpu(memory, screen), status(READY), errorMessage("")
{
}

bool Simulator::loadProgramFile(const std::string &filename, int startAddress)
{
    memory.clear();
    screen.clear();

    bool success = loader.loadProgram(filename, startAddress, memory);

    if (success)
    {
        cpu.reset(startAddress);
        status = READY;
        errorMessage.clear();
    }
    else
    {
        status = ERROR;
        errorMessage = loader.getLastError();
    }

    return success;
}

void Simulator::runAll()
{
    status = RUNNING;
    int cycleCount = 0;
    const int maxCycles = 1000000; // Safety cap against infinite loops

    while (status == RUNNING && cycleCount < maxCycles)
    {
        if (!step())
        {
            break;
        }
        cycleCount++;
    }

    if (cycleCount >= maxCycles)
    {
        status = ERROR;
        errorMessage = "Execution stopped: Max cycle limit reached (Infinite loop detected).";
    }
}

bool Simulator::step()
{
    if (status == HALTED || status == ERROR)
        return false;

    status = RUNNING;

    StepResult result = cpu.stepCycle();

    if (result == StepResult::Running)
        return true;

    if (result == StepResult::Halted)
    {
        status = HALTED;
        return false;
    }

    status = ERROR;
    errorMessage = cpu.getLastError();

    return false;
}

void Simulator::reset()
{
    memory.clear();
    screen.clear();

    cpu.reset(0);

    status = READY;
    errorMessage.clear();
}

void Simulator::editMemoryCell(int address, uint8_t value)
{
    if (address >= 0 && address < 255)
    {
        memory.writeByte(address, value);
    }
}

void Simulator::editRegister(int regIndex, uint8_t value)
{
    cpu.setRegister(regIndex, value);
}

void Simulator::displayMachineState() const
{
    std::cout << "\n=================== MACHINE STATE ===================\n";
    std::cout << "Status: ";
    switch (status)
    {
    case READY:
        std::cout << "READY\n";
        break;
    case RUNNING:
        std::cout << "RUNNING\n";
        break;
    case HALTED:
        std::cout << "HALTED\n";
        break;
    case ERROR:
        std::cout << "ERROR: " << errorMessage << "\n";
        break;
    }

    std::cout << "PC: 0x" << std::hex << std::setw(2) << std::setfill('0') << (int)cpu.getPC() << std::dec << "\n";
    std::cout << "IR: 0x" << std::hex << std::setw(4) << std::setfill('0') << (int)cpu.getIR() << std::dec << "\n";

    std::cout << "Registers:\n";
    for (int i = 0; i < 16; ++i)
    {
        std::cout << "R" << std::uppercase << std::hex << i << std::dec << ": 0x"
                  << std::hex << std::setw(2) << std::setfill('0') << (int)cpu.getRegister(i) << std::dec << "\t";
        if ((i + 1) % 4 == 0)
            std::cout << "\n";
    }

    std::cout << "\nMemory Contents Matrix (00 - FF):\n";
    std::cout << "     ";
    for (int c = 0; c < 16; ++c)
        std::cout << std::hex << c << "  ";
    std::cout << std::dec << "\n----------------------------------------------------\n";

    for (int r = 0; r < 256; r += 16)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << r << "| ";
        for (int c = 0; c < 16; ++c)
        {
            std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)memory.readByte(r + c) << std::dec << " ";
        }
        std::cout << "\n";
    }
    std::cout << "----------------------------------------------------\n";

    std::cout << "Screen Output:\n";
    std::cout << screen.getDisplayText() << "\n";

    std::cout << "=====================================================\n";
}

MachineState Simulator::getMachineState() const
{
    MachineState state;

    // Registers
    for (int i = 0; i < 16; i++)
    {
        state.registers[i] = cpu.getRegister(i);
    }

    // CPU state
    state.pc = cpu.getPC();
    state.ir = cpu.getIR();
    state.halted = cpu.isHalted();

    // Memory
    for (int i = 0; i < 256; i++)
    {
        state.memory[i] = memory.readByte(i);
    }

    // Screen
    state.screenText = screen.getDisplayText();

    return state;
}

MachineStatus Simulator::getStatus() const
{
    return status;
}

std::string Simulator::getErrorMessage() const
{
    return errorMessage;
}
