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
    std::cout << "7. Exit / Load Another Program\n";
    std::cout << "Enter your choice (1-7): ";
}

int main() {
    Simulator sim;
    int choice = 0;

    do {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                std::string filename;
                int startAddress;
                std::cout << "Enter file path/name: ";
                std::cin >> filename;
                std::cout << "Enter even start address in hex (e.g. 00): ";
                std::cin >> std::hex >> startAddress >> std::dec;

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
                std::cout << "Executing step by step. Press [Enter] after each step.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                while (sim.getStatus() == READY || sim.getStatus() == RUNNING) {
                    sim.step();
                    sim.displayMachineState();
                    if (sim.getStatus() == HALTED || sim.getStatus() == ERROR) {
                        std::cout << "Execution stopped.\n";
                        break;
                    }
                    std::cout << "Press [Enter] for next step...";
                    std::cin.get();
                }
                break;
            }
            case 4: {
                int editType;
                std::cout << "Edit options:\n1. Register\n2. Memory Cell\nChoice: ";
                std::cin >> editType;
                if (editType == 1) {
                    int reg, val;
                    std::cout << "Enter Register Index (0-F in hex): ";
                    std::cin >> std::hex >> reg >> std::dec;
                    std::cout << "Enter new Value in hex (00-FF): ";
                    std::cin >> std::hex >> val >> std::dec;
                    sim.editRegister(reg, static_cast<uint8_t>(val));
                    std::cout << "Register updated.\n";
                } else if (editType == 2) {
                    int addr, val;
                    std::cout << "Enter Memory Address in hex (00-FF): ";
                    std::cin >> std::hex >> addr >> std::dec;
                    std::cout << "Enter new Value in hex (00-FF): ";
                    std::cin >> std::hex >> val >> std::dec;
                    sim.editMemoryCell(addr, static_cast<uint8_t>(val));
                    std::cout << "Memory cell updated.\n";
                }
                sim.displayMachineState();
                break;
            }
            case 5:
                sim.displayMachineState();
                break;
            case 6:
                sim.reset();
                std::cout << "Simulator reset.\n";
                break;
            case 7:
                std::cout << "Exiting simulator. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice! Please select between 1 and 7.\n";
        }
    } while (choice != 7);

    return 0;
}
