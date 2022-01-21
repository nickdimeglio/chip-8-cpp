#ifndef CPU_H
#define CPU_H
#include "Memory.h"
#include <string>
#include <functional>
using namespace std;
using OpcodeFunction = function<int(int, Memory&)>;


// Execute opcodes on instructions
int execute(int instruction, Memory &mem);

// Decode instructions into opcodes
OpcodeFunction decode(int instruction);

// Opcode implementations
int op00E0(int instruction, Memory &mem);
int op00EE(int instruction, Memory &mem);
int op1NNN(int instruction, Memory &mem);
int op2NNN(int instruction, Memory &mem);
int op3XKK(int instruction, Memory &mem);
int op4XKK(int instruction, Memory &mem);
int op5XY0(int instruction, Memory &mem);
int op6XKK(int instruction, Memory &mem);
int op7XKK(int instruction, Memory &mem);
int op8XY0(int instruction, Memory &mem);
int op8XY1(int instruction, Memory &mem);
int op8XY2(int instruction, Memory &mem);
int op8XY3(int instruction, Memory &mem);
int op8XY4(int instruction, Memory &mem);
int op8XY5(int instruction, Memory &mem);
int op8XY6(int instruction, Memory &mem);
int op8XY7(int instruction, Memory &mem);
int op8XY8(int instruction, Memory &mem);
int op8XY9(int instruction, Memory &mem);
int op8XYE(int instruction, Memory &mem);
int op9XY0(int instruction, Memory &mem);
int opANNN(int instruction, Memory &mem);
int opBNNN(int instruction, Memory &mem);
int opCXKK(int instruction, Memory &mem);
int opDXYN(int instruction, Memory &mem);
int opEX9E(int instruction, Memory &mem);
int opEXA1(int instruction, Memory &mem);
int opFX07(int instruction, Memory &mem);
int opFX0A(int instruction, Memory &mem);
int opFX15(int instruction, Memory &mem);
int opFX18(int instruction, Memory &mem);
int opFX1E(int instruction, Memory &mem);
int opFX29(int instruction, Memory &mem);
int opFX33(int instruction, Memory &mem);
int opFX55(int instruction, Memory &mem);
int opFX65(int instruction, Memory &mem);
int invalidOpcode(int instruction, Memory &mem);

#endif