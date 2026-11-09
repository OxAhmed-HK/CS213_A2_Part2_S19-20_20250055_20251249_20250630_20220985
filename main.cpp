#include <iostream>
#include <string>
#include <limits>
#include "Simulator.h"

void displayMenu() {
    std::cout << "\n========== VOLE MACHINE SIMULATOR ==========\n";
    std::cout << "1. Load Program from File\n";
    std::cout << "2. Run Program (Run All)\n";
    std::cout << "3. Execute Step by Step\n";
    std::cout << "4. Edit Register or Memory Cell\n";
    std::cout << "5. Display Machine State\n";
    std::cout << "6. Reset Simulator\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice (1-7): ";
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Simulator sim;
    int choice = 0;

    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a valid number.\n";
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: {
                std::string filename;
                int startAddress;
                std::cout << "Enter file path/name: ";
                std::cin >> filename;
                std::cout << "Enter even start address in hex (e.g. 00): ";
                if (!(std::cin >> std::hex >> startAddress >> std::dec)) {
                    std::cout << "Invalid address format.\n";
                    clearInputBuffer();
                    break;
                }

                if (sim.loadProgramFile(filename, startAddress)) {
                    std::cout << "Program loaded successfully!\n";
                    sim.displayMachineState();
                } else {
                    std::cout << "Error loading program: " << sim.getErrorMessage() << "\n";
                }
                break;
            }
            case 2:
                std::cout << "Running program continuously...\n";
                sim.runAll();
                sim.displayMachineState();
                break;
            case 3: {
                std::cout << "Executing step by step. Press [Enter] after each step (Type 'q' then Enter to quit step mode).\n";
                clearInputBuffer();
                while (sim.getStatus() == READY || sim.getStatus() == RUNNING) {
                    sim.step();
                    sim.displayMachineState();
                    if (sim.getStatus() == HALTED || sim.getStatus() == ERROR) {
                        std::cout << "Execution stopped.\n";
                        break;
                    }
                    std::cout << "Press [Enter] for next step (or 'q' to exit): ";
                    char input = std::cin.get();
                    if (input == 'q' || input == 'Q') {
                        break;
                    }
                }
                break;
            }
            case 4: {
                int editType;
                std::cout << "Edit options:\n1. Register\n2. Memory Cell\nChoice (1-2): ";
                if (!(std::cin >> editType)) {
                    std::cout << "Invalid choice.\n";
                    clearInputBuffer();
                    break;
                }

                if (editType == 1) {
                    int reg, val;
                    std::cout << "Enter Register Index (0-F in hex): ";
                    if (!(std::cin >> std::hex >> reg >> std::dec) || reg < 0 || reg > 15) {
                        std::cout << "Error: Register index must be between 0 and 15 (0-F).\n";
                        clearInputBuffer();
                        break;
                    }
                    std::cout << "Enter new Value in hex (00-FF): ";
                    if (!(std::cin >> std::hex >> val >> std::dec) || val < 0 || val > 255) {
                        std::cout << "Error: Value must be between 00 and FF.\n";
                        clearInputBuffer();
                        break;
                    }
                    sim.editRegister(reg, static_cast<uint8_t>(val));
                    std::cout << "Register updated successfully.\n";
                } 
                else if (editType == 2) {
                    int addr, val;
                    std::cout << "Enter Memory Address in hex (00-FF): ";
                    if (!(std::cin >> std::hex >> addr >> std::dec) || addr < 0 || addr > 255) {
                        std::cout << "Error: Memory address must be between 00 and FF.\n";
                        clearInputBuffer();
                        break;
                    }
                    if (addr == 0xFF) {
                        std::cout << "Error: Cannot edit address FF directly (Reserved for Screen).\n";
                        break;
                    }
                    std::cout << "Enter new Value in hex (00-FF): ";
                    if (!(std::cin >> std::hex >> val >> std::dec) || val < 0 || val > 255) {
                        std::cout << "Error: Value must be between 00 and FF.\n";
                        break;
                    }
                    sim.editMemoryCell(addr, static_cast<uint8_t>(val));
                    std::cout << "Memory cell updated successfully.\n";
                } else {
                    std::cout << "Invalid edit option.\n";
                }
                sim.displayMachineState();
                break;
            }
            case 5:
                sim.displayMachineState();
                break;
            case 6:
                sim.reset();
                std::cout << "Simulator reset successfully.\n";
                break;
            case 7:
                std::cout << "Exiting simulator. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice! Please select a number between 1 and 7.\n";
        }
    } while (choice != 7);

    return 0;
}
