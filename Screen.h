#ifndef SCREEN_H
#define SCREEN_H

#include <cstdint>
#include <string>
#include <vector>

using namespace std;

class Screen {
private:
    vector<uint8_t> output;

public:
    void write(uint8_t value);
    void clear();
    const vector<uint8_t>& getOutput() const;
    string getDisplayText() const;
};

#endif