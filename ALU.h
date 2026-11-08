#ifndef ALU_H
#define ALU_H

using namespace std;

class ALU {
public:
    // Member 2 Tasks: Opcodes 5 & 6
    int addTwosComplement(int a, int b);
    int addFloatingPoint(int a, int b);
    float decodeFloat(int bits);
    int encodeFloat(float value);

    // Member 3 (Marwan) Tasks: Opcodes 7, 8, 9, A
    int bitwiseOr(int a, int b);
    int bitwiseAnd(int a, int b);
    int bitwiseXor(int a, int b);
    int rotateRight(int value, int count);
};

#endif // ALU_H