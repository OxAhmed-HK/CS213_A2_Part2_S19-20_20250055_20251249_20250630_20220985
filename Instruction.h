#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <cstdint>
#include <string>

using namespace std;

class Instruction {
private:
    uint16_t rawWord;

public:
    explicit Instruction(uint16_t word = 0);

    static uint16_t parseHex(const string& line);

    static bool isValidWord(uint16_t word);

    uint16_t getWord() const;
    int getOpcode() const;
    int getR() const;
    int getS() const;
    int getT() const;
    int getXY() const;
};

#endif