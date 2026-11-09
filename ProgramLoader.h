#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <string>
#include <vector>
#include <cstdint>
#include "Memory.h"
#include "Instruction.h" // أضفنا ترويسة Instruction للتأكد من توفر isValidWord

class ProgramLoader {
private:
    std::string fileName;
    std::string lastError;

public:
    ProgramLoader();
    
    bool validateStartAddress(int startAddress) const;
    bool loadProgram(const std::string& file, int startAddress, Memory& memory);
    std::string getLastError() const;
};

#endif
