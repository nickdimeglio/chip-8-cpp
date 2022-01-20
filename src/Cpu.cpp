#include "Memory.h"
#include "Cpu.h"
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
using namespace std;
using OpcodeFunction = function<int(int, Memory)>;



int execute(int instruction, Memory mem)
{
    OpcodeFunction opcode = decode(instruction);
    return opcode(instruction, mem);
}

OpcodeFunction decode(int instruction)
{
    // For decoding instructions
    int high_nibble = (instruction & 0xF000) >> 12;
    int low_nibble = instruction & 0x000F;

    switch (high_nibble) 
    {
        case 0x0:
            switch (low_nibble)
            {
                case 0x0:
                    return &op00E0;
                case 0xE:
                    return &op00EE;
                default:
                    return &op0NNN;
            }
        case 0x1:
            return &op1NNN;
        case 0x2:
            return &op2NNN;
        case 0x3:
            return &op3XKK;
        case 0x4:
            return &op4XKK;
        case 0x5:
            return &op5XY0;
        case 0x6:
            return &op6XKK;
        case 0x7:
            return &op7XKK;
        case 0x8:
            switch (low_nibble)
            {
                case 0x0:
                    return &op8XY0;
                case 0x1:
                    return &op8XY1;
                case 0x2:
                    return &op8XY2;
                case 0x3:
                    return &op8XY3;
                case 0x4:
                    return &op8XY4;
                case 0x5:
                    return &op8XY5;
                case 0x6:
                    return &op8XY6;
                case 0x7:
                    return &op8XY7;
                case 0xE:
                    return &op8XYE;
            }
        case 0x9:
            return &op9XY0;
        case 0xA:
            return &opANNN;
        case 0xB:
            return &opBNNN;
        case 0xC:
            return &opCXKK;
        case 0xD:
            return &opDXYN;
        case 0xE:
            switch (low_nibble)
            {
                case 0x1:
                    return &opEXA1;
                case 0xE:
                    return &opEX9E;
            }
        case 0xF:
            switch (low_nibble)
            {
                case 0x3:
                    return &opFX33;
                case 0x5:
                    switch ((instruction & 0xF0) >> 4)
                    {
                        case 0x1:
                            return &opFX15;
                        case 0x5:
                            return &opFX55;
                        case 0x6:
                            return &opFX65;
                    }
                case 0x7:
                    return &opFX07;
                case 0x8:
                    return &opFX18;
                case 0x9:
                    return &opFX29;
                case 0xA:
                    return &opFX0A;
                case 0xE:
                    return &opFX1E;
            }
        default:
            return &invalidOpcode;
    }
};


// Chip-8 Instructions
int op00E0(int instruction, Memory mem) { return 0x00E0; } 
int op00EE(int instruction, Memory mem) { return 0x00EE; }
int op0NNN(int instruction, Memory mem) { return 0x0000; }
int op1NNN(int instruction, Memory mem) { return 0x1000; }
int op2NNN(int instruction, Memory mem) { return 0x2000; }
int op3XKK(int instruction, Memory mem) { return 0x3000; }
int op4XKK(int instruction, Memory mem) { return 0x4000; }
int op5XY0(int instruction, Memory mem) { return 0x5000; }
int op6XKK(int instruction, Memory mem) { return 0x6000; }
int op7XKK(int instruction, Memory mem) { return 0x7000; }
int op8XY0(int instruction, Memory mem) { return 0x8000; }
int op8XY1(int instruction, Memory mem) { return 0x8001; }
int op8XY2(int instruction, Memory mem) { return 0x8002; }
int op8XY3(int instruction, Memory mem) { return 0x8003; }
int op8XY4(int instruction, Memory mem) { return 0x8004; }
int op8XY5(int instruction, Memory mem) { return 0x8005; }
int op8XY6(int instruction, Memory mem) { return 0x8006; }
int op8XY7(int instruction, Memory mem) { return 0x8007; }
int op8XYE(int instruction, Memory mem) { return 0x800E; }
int op9XY0(int instruction, Memory mem) { return 0x9000; }
int opANNN(int instruction, Memory mem) { return 0xA000; }
int opBNNN(int instruction, Memory mem) { return 0xB000; }
int opCXKK(int instruction, Memory mem) { return 0xC000; }
int opDXYN(int instruction, Memory mem) { return 0xD001; }
int opEX9E(int instruction, Memory mem) { return 0xE09E; }
int opEXA1(int instruction, Memory mem) { return 0xE0A1; }
int opFX07(int instruction, Memory mem) { return 0xF007; }
int opFX0A(int instruction, Memory mem) { return 0xF00A; }
int opFX15(int instruction, Memory mem) { return 0xF015; }
int opFX18(int instruction, Memory mem) { return 0xF018; }
int opFX1E(int instruction, Memory mem) { return 0xF01E; }
int opFX29(int instruction, Memory mem) { return 0xF029; }
int opFX33(int instruction, Memory mem) { return 0xF033; }
int opFX55(int instruction, Memory mem) { return 0xF055; }
int opFX65(int instruction, Memory mem) { return 0xF065; }
int invalidOpcode(int instruction, Memory mem) { return -1; }