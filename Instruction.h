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

    static bool isValidHexFormat(const string& line);
    static int parseHex(const string& line);
    static bool parseHex(const string& line, uint16_t& outWord);
    static bool isValidWord(int word);
    static bool isValidInstructionString(const string& line);

    uint16_t getWord() const;
    int getOpcode() const;
    int getR() const;
    int getS() const;
    int getT() const;
    int getXY() const;
};

#endif