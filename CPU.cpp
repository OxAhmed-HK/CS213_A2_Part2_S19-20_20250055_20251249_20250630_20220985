#include "CPU.h"
#include <iomanip>
#include <sstream>

using namespace std;

CPU::CPU(Memory& memoryRef, Screen& screenRef)
    : memory(memoryRef),
      screen(screenRef),
      programCounter(0),
      instructionRegister(0),
      halted(false),
      lastError("") {
    reset(0);
}

void CPU::reset(int startAddress) {
    for (int i = 0; i < 16; ++i) {
        registers[i] = 0;
    }
    programCounter = startAddress & 0xFF;
    instructionRegister = 0;
    halted = false;
    lastError.clear();
}

bool CPU::fetch() {
    if (programCounter < 0 || programCounter >= 0xFF) {
        ostringstream oss;
        oss << "Fetch error: Program Counter (0x"
            << uppercase << hex << setw(2) << setfill('0')
            << programCounter << ") cannot fetch a 2-byte instruction.";
        lastError = oss.str();
        return false;
    }

    instructionRegister = static_cast<int>(memory.readWord(programCounter));
    programCounter += 2;
    return true;
}

Instruction CPU::decode() const {
    return Instruction(static_cast<uint16_t>(instructionRegister & 0xFFFF));
}


ExecResult CPU::execute(Instruction instr) {
    if (!Instruction::isValidWord(instr.getWord())) {
        ostringstream oss;
        oss << "Invalid instruction 0x"
            << uppercase << hex << setw(4) << setfill('0')
            << instr.getWord() << " at address 0x"
            << setw(2) << (programCounter - 2) << ".";
        lastError = oss.str();
        return ExecResult::ERROR;
    }

    int opcode = instr.getOpcode();
    int r      = instr.getR();
    int s      = instr.getS();
    int t      = instr.getT();
    int xy     = instr.getXY();

    switch (opcode) {
        case 0x1:
            registers[r] = memory.readByte(xy);
            return ExecResult::CONTINUE;

        case 0x2:
            registers[r] = xy;
            return ExecResult::CONTINUE;

        case 0x3:
            if (xy == 0xFF) {
                screen.write(static_cast<uint8_t>(registers[r] & 0xFF));
            } else {
                memory.writeByte(xy, static_cast<uint8_t>(registers[r] & 0xFF));
            }
            return ExecResult::CONTINUE;

        case 0x4:
            registers[t] = registers[s];
            return ExecResult::CONTINUE;

        case 0x5:
            registers[r] = alu.addTwosComplement(registers[s], registers[t]);
            return ExecResult::CONTINUE;

        case 0x6:
            registers[r] = alu.addFloatingPoint(registers[s], registers[t]);
            return ExecResult::CONTINUE;

        case 0x7: // 7RST: Bitwise OR registers S and T, store in R
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
            if ((registers[r] & 0xFF) == (registers[0] & 0xFF)) {
                programCounter = xy;
            }
            return ExecResult::CONTINUE;

        case 0xC:
            halted = true;
            return ExecResult::HALTED;

        default:
            lastError = "Unsupported opcode encountered.";
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

    Instruction instr = decode();
    ExecResult result = execute(instr);

    switch (result) {
        case ExecResult::CONTINUE:
            return StepResult::Running;
        case ExecResult::HALTED:
            return StepResult::Halted;
        case ExecResult::ERROR:
        default:
            return StepResult::Error;
    }
}

int CPU::getRegister(int idx) const {
    if (idx < 0 || idx > 15) {
        return 0;
    }
    return registers[idx] & 0xFF;
}

void CPU::setRegister(int idx, int val) {
    if (idx >= 0 && idx <= 15) {
        registers[idx] = val & 0xFF;
    }
}

int CPU::getPC() const {
    return programCounter;
}

void CPU::setPC(int addr) {
    programCounter = addr & 0xFF;
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