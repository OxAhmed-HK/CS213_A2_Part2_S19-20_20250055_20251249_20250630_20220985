#ifndef CPU_H
#define CPU_H

#include <string>
#include "ALU.h"
#include "Instruction.h"
#include "Memory.h"
#include "Screen.h"

using namespace std;

enum class StepResult {
    Running,
    Halted,
    Error
};

enum class ExecResult {
    CONTINUE,
    HALTED,
    ERROR
};

class CPU {
private:
    Memory& memory;
    Screen& screen;
    int registers[16];
    int programCounter;
    int instructionRegister;
    bool halted;
    ALU alu;
    string lastError;

    bool fetch();
    Instruction decode() const;
    ExecResult execute(Instruction instr);

public:
    CPU(Memory& memory, Screen& screen);

    StepResult stepCycle();
    void reset(int startAddress = 0);

    int getRegister(int idx) const;
    void setRegister(int idx, int val);
    int getPC() const;
    void setPC(int addr);
    int getIR() const;
    bool isHalted() const;
    string getLastError() const;
};

#endif