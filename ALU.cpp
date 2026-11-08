#include "ALU.h"
#include <cmath>
#include <cstdint>

using namespace std;

int ALU::addTwosComplement(int a, int b) {
    return (a + b) & 0xFF;
}

float ALU::decodeFloat(int bits) {
    uint8_t byteVal = static_cast<uint8_t>(bits & 0xFF);
    if (byteVal == 0) {
        return 0.0f;
    }
    int sign = (byteVal >> 7) & 0x01;
    int exp  = ((byteVal >> 4) & 0x07) - 4;
    int mant = byteVal & 0x0F;

    float fraction = static_cast<float>(mant) / 16.0f;
    float value = ldexp(fraction, exp);
    return sign ? -value : value;
}


int ALU::encodeFloat(float value) {
    if (value == 0.0f) {
        return 0;
    }
    int sign = (value < 0.0f) ? 1 : 0;
    float absVal = fabs(value);

    int exp = 0;
    float fraction = frexp(absVal, &exp);

    int biasedExp = exp + 4;
    if (biasedExp < 0) {
        return 0;
    }
    if (biasedExp > 7) {
        biasedExp = 7;
    }

    int mant = static_cast<int>(round(fraction * 16.0f));
    if (mant >= 16) {
        mant = 15;
    }

    return ((sign << 7) | ((biasedExp & 0x07) << 4) | (mant & 0x0F)) & 0xFF;
}

int ALU::addFloatingPoint(int a, int b) {
    float sum = decodeFloat(a) + decodeFloat(b);
    return encodeFloat(sum);
}

int ALU::bitwiseOr(int a, int b) {
    return (a | b) & 0xFF;
}

int ALU::bitwiseAnd(int a, int b) {
    return (a & b) & 0xFF;
}

int ALU::bitwiseXor(int a, int b) {
    return (a ^ b) & 0xFF;
}

int ALU::rotateRight(int value, int count) {
    uint8_t byteVal = static_cast<uint8_t>(value & 0xFF);
    int steps = (count & 0x0F) % 8;

    for (int i = 0; i < steps; ++i) {
        uint8_t lowBit = byteVal & 0x01;
        byteVal = static_cast<uint8_t>((byteVal >> 1) | (lowBit << 7));
    }

    return static_cast<int>(byteVal);
}