#include "Screen.h"
#include <iomanip>
#include <sstream>

using namespace std;

void Screen::write(uint8_t value) {
    output.push_back(value);
}

void Screen::clear() {
    output.clear();
}

const vector<uint8_t>& Screen::getOutput() const {
    return output;
}

string Screen::getDisplayText() const {
    ostringstream oss;
    for (size_t i = 0; i < output.size(); ++i) {
        if (i > 0) {
            oss << ' ';
        }
        oss << uppercase << hex << setw(2) << setfill('0')
            << static_cast<int>(output[i]);
    }
    return oss.str();
}