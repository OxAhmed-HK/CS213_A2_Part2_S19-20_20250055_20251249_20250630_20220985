#include "CPU.h"

using namespace std;

CPU::CPU(Memory& memoryRef, Screen& screenRef)
        : memory(memoryRef), screen(screenRef), programCounter(0), instructionRegister(0),
        halted(false)
{
    reset(0);
}

void CPU::reset(int startAddress) {

    for (int i = 0; i < 16; i++) {
        registers[i] = 0;
    }

    programCounter = startAddress & 0xFF;
    instructionRegister = 0;
    halted = false;
    lastError.clear();
}

bool CPU::fetch() {

    if (programCounter > 0xFD || programCounter % 2 != 0) {
        lastError = "Fetch error, invalid programCounter";
        return false;
    }

    instructionRegister = memory.readWord(programCounter);
    programCounter += 2;
    return true;
}

Instruction CPU::decode() const {
    return Instruction(static_cast<uint16_t>(instructionRegister & 0xFFFF));
}

ExecResult CPU::execute(Instruction instr) {
    if (!Instruction::isValidWord(instr.getWord())) {
        lastError = "Invalid Instruction";
        return ExecResult::ERROR;
    }

    int opcode = instr.getOpcode();
    int r = instr.getR();
    int s = instr.getS();
    int t = instr.getT();
    int xy = instr.getXY();

    switch (opcode) {
        case 0x1:
            registers[r] = memory.readByte(xy);
            return ExecResult::CONTINUE;

        case 0x2:
            registers[r] = xy;
            return ExecResult::CONTINUE;

        case 0x3:
            if (xy == 0xFF) {
                screen.write(static_cast<uint8_t>(registers[r]));
            }
            else {
                memory.writeByte(xy, static_cast<uint8_t>(registers[r]));
            }
            return ExecResult::CONTINUE;

        case 0x4:
            registers[t] = registers[s];
            return ExecResult::CONTINUE;


        case 0x5:
            registers[r] = alu.addTwosComplement(registers[s],registers[t]);
            return ExecResult::CONTINUE;

        case 0x6:
            registers[r] = alu.addFloatingPoint(registers[s],registers[t]);
            return ExecResult::CONTINUE;

        case 0x7:
            registers[r] = alu.bitwiseOr(registers[s], registers[t]);
            return ExecResult::CONTINUE;

        case 0x8:
            registers[r] = alu.bitwiseAnd(registers[s], registers[t]);
            return ExecResult::CONTINUE;

        case 0x9:
            registers[r] = alu.bitwiseXor(registers[s], registers[t]);
            return ExecResult::CONTINUE;

        case 0xA:
            registers[r] = alu.rotateRight(registers[r], t);
            return ExecResult::CONTINUE;

        case 0xB:
            if (registers[r] == registers[0]) {
                if (xy > 0xFD || xy % 2 != 0) {
                    lastError = "Invalid Address";
                    return ExecResult::ERROR;
                }
                programCounter = xy;
            }
            return ExecResult::CONTINUE;

        case 0xC:
            halted = true;
            return ExecResult::HALTED;

        default:
            lastError = "Invalid opcode";
            return ExecResult::ERROR;

    }
}

StepResult CPU::stepCycle() {
    if (halted) {
        return StepResult::Halted;
    }
    if (!fetch()) {
        return StepResult::Error;
    }

    Instruction instruction = decode();

    ExecResult execResult = execute(instruction);

    switch (execResult) {
        case ExecResult::CONTINUE:
            return StepResult::Running;

        case ExecResult::HALTED:
            return StepResult::Halted;

        case ExecResult::ERROR:
            return StepResult::Error;

        default:
            return StepResult::Error;
    }
}

int CPU::getRegister(int reg) const {
    if (reg < 0 || reg > 15) {
        return 0;
    }
    return registers[reg];
}

void CPU::setRegister(int reg, int value) {
    if (reg < 0 || reg > 15) {
        return;
    }
    registers[reg] = value & 0xFF;
}

int CPU::getPC() const {
    return programCounter;
}

void CPU::setPC(int value) {
    programCounter = value & 0xFF;
}

int CPU::getIR() const {
    return instructionRegister;
}

bool CPU::isHalted() const {
    return halted;
}

string CPU::getLastError() const {
    return lastError;
}