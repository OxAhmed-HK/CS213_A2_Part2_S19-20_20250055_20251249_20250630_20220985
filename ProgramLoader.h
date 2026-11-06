#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <string>
#include <vector>
#include <cstdint>
#include "Memory.h"

class ProgramLoader {
private:
    std::string fileName;
    std::string lastError;

public:
    ProgramLoader();
    
    // Validates that the start address is even and less than 0xFF
    bool validateStartAddress(int startAddress) const;
    
    // Reads, parses, and loads the program into memory starting at startAddress
    bool loadProgram(const std::string& file, int startAddress, Memory& memory);
    
    // Returns the last error message if loading failed
    std::string getLastError() const;
};

#endif // PROGRAM_LOADER_H
