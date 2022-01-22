#include "Memory.h"
#include "Cpu.h"
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
using namespace std;
using OpcodeFunction = function<int(int, Memory&)>;



int execute(int instruction, Memory &mem)
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

/* Clear the screen */
int op00E0(int instruction, Memory &mem) 
{ 
    for (auto i = 0; i != mem.screen_size; ++i)
        mem.screen_write(i, 0);
    return 0x00E0; 
}

/* Return from a subroutine */
int op00EE(int instruction, Memory &mem) 
{ 
    int last_pc = mem.stack_pop();
    mem.set_program_counter(last_pc);
    return 0x00EE; 
}

/* Set program counter to NNN */
int op1NNN(int instruction, Memory &mem) 
{ 
    int new_address = instruction & 0xFFF;
    mem.set_program_counter(new_address);
    return 0x1000; 
}

/* Call subroutine at NNN */
int op2NNN(int instruction, Memory &mem) 
{ 
    // Store current program counter
    int pc = mem.get_program_counter();
    mem.stack_push(pc);
    // Jump to NNN
    int new_pc = instruction & 0xF00;
    mem.set_program_counter(new_pc);
    return 0x2000; 
}

/* Skip next instruction if VX = KK */
int op3XKK(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int kk = (instruction & 0xFF);
    if (mem.reg_read(x) == kk)
        mem.inc_program_counter();
    return 0x3000; 
}

/* Skip next instruction of VX != KK */
int op4XKK(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int kk = (instruction & 0xFF);
    if (mem.reg_read(x) != kk)
        mem.inc_program_counter();
    return 0x4000; 
}

/* Skip the next instruction if VX = VY */
int op5XY0(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);
    if (vx == vy)
        mem.inc_program_counter();
    return 0x5000; 
}

/* Put the value KK in VX */
int op6XKK(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int kk = (instruction & 0xFF);
    mem.reg_write(x, kk);
    return 0x6000; 
}

/* Put VX + KK in VX */
int op7XKK(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int kk = (instruction & 0xFF);
    int vx = mem.reg_read(x);
    mem.reg_write(x, vx + kk);
    return 0x7000; 
}

/* Put VX in VY */
int op8XY0(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    mem.reg_write(y, vx);
    return 0x8000; 
}

/* Put (VX or VY) in VX */
int op8XY1(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);
    mem.reg_write(x, vx | vy);
    return 0x8001; 
}

/* Put (VX and VY) in VX */
int op8XY2(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);
    mem.reg_write(x, vx & vy);
    return 0x8002; 
}

/* Put (VX xor VY) in VX */
int op8XY3(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);
    mem.reg_write(x, vx ^ vy);
    return 0x8003; 
}

/* Put (VX + VY % 0x100) in VX, set VF to carry */
int op8XY4(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);

    int sum = vx + vy;
    mem.reg_write(0xF, sum > 0xFF);     // Check for carry
    mem.reg_write(x, sum % 0x100);      // Wraparound if needed
    return 0x8004; 
}

/* Put (VX - VY) in VX, set VF to not borrow */
int op8XY5(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);

    // check for borrow
    if (vx > vy)
        mem.reg_write(0xF, 1);
    else
    {
        mem.reg_write(0xF, 0);
        vx += 0x100;
    }
    mem.reg_write(x, vx - vy);
    return 0x8005; 
}
int op8XY6(int instruction, Memory &mem) { return 0x8006; }
int op8XY7(int instruction, Memory &mem) { return 0x8007; }
int op8XYE(int instruction, Memory &mem) { return 0x800E; }
int op9XY0(int instruction, Memory &mem) { return 0x9000; }
int opANNN(int instruction, Memory &mem) { return 0xA000; }
int opBNNN(int instruction, Memory &mem) { return 0xB000; }
int opCXKK(int instruction, Memory &mem) { return 0xC000; }
int opDXYN(int instruction, Memory &mem) { return 0xD001; }
int opEX9E(int instruction, Memory &mem) { return 0xE09E; }
int opEXA1(int instruction, Memory &mem) { return 0xE0A1; }
int opFX07(int instruction, Memory &mem) { return 0xF007; }
int opFX0A(int instruction, Memory &mem) { return 0xF00A; }
int opFX15(int instruction, Memory &mem) { return 0xF015; }
int opFX18(int instruction, Memory &mem) { return 0xF018; }
int opFX1E(int instruction, Memory &mem) { return 0xF01E; }
int opFX29(int instruction, Memory &mem) { return 0xF029; }
int opFX33(int instruction, Memory &mem) { return 0xF033; }
int opFX55(int instruction, Memory &mem) { return 0xF055; }
int opFX65(int instruction, Memory &mem) { return 0xF065; }
int invalidOpcode(int instruction, Memory &mem) { return -1; }
