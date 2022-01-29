#include "Memory.h"
#include "Cpu.h"
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <sstream>
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

    mem.reg_write(0xF, vx >= vy);
    vx += 0x100 * (vx < vy); // For wrap-around
    vx -= vy;
    mem.reg_write(x, vx);
    return 0x8005; 
}

/* VF = least significant bit of VX,  VX >>= 1 */
int op8XY6(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xFFF) >> 8;
    int vx = mem.reg_read(x);     
    mem.reg_write(0xF, vx & 0x1);
    mem.reg_write(x, vx >> 1);
    return 0x8006; 
}

/* Put (VY - VX) in VX, set VF to not borrow */
int op8XY7(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int y = (instruction & 0xF0) >> 4;
    int vx = mem.reg_read(x);
    int vy = mem.reg_read(y);

    mem.reg_write(0xF, vy >= vx);
    vy += 0x100 * (vy < vx); // For wrap-around
    vy -= vx;
    mem.reg_write(x, vy);

    return 0x8007; 
}

/* VF = most significant bit of VX,  VX <<= 1 */
int op8XYE(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xFFF) >> 8;
    int vx = mem.reg_read(x);
    mem.reg_write(0xF, vx >> 7);
    mem.reg_write(x, vx << 1 & 0xFF);
    return 0x800E; 
}

/* Skip next instruction if VX != VY */
int op9XY0(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    int vy = mem.reg_read((instruction & 0xF0) >> 4);
    if (vx != vy)
        mem.inc_program_counter();
    return 0x9000; 
}

/* Set address pointer to NNN */
int opANNN(int instruction, Memory &mem) 
{ 
    mem.set_address_pointer(instruction & 0xFFF);
    return 0xA000; 
}

/* Set program counter to NNN + V0 */
int opBNNN(int instruction, Memory &mem) 
{ 
    int nnn = instruction & 0xFFF;
    int v0 = mem.reg_read(0x0);
    mem.set_program_counter(nnn + v0);
    return 0xB000; 
}

/* Set VX = random byte and KK */
int opCXKK(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int vx = mem.reg_read(x);
    int kk = instruction & 0xFF;
      
    // Gen random int [0, 255]
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    int rand_byte = generator() % 256;

    mem.reg_write(x, rand_byte & kk);
    return 0xC000; 
}

// Draw a sprite by XOring the n-bytes beginning at address_pointer
// with the Nx8 pixel grid starting at coordinate (VX, VY)
// set VF = collision
int opDXYN(int instruction, Memory &mem) 
{ 
    // Start sprite drawing at coordinate (VX, VY)
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    int vy = mem.reg_read((instruction & 0xF0) >> 4);
    int draw_start = vx + (vy * 64);

    // Initialize VF to no collision
    mem.reg_write(0xF, 0);

    // XOR byte-by-byte
    int num_bytes = instruction & 0xF;
    for (int i = 0; i < num_bytes; i++)
    {
        // XOR bit-by-bit
        int sprite_byte = mem.mem_read(mem.get_address_pointer() + i);
        for (int j = 0; j < 8; j++ )
        {
            // XOR next bit
            int sprite_bit = (sprite_byte & (int) pow(2, 7 - j)) >> (7 - j);
            int screen_bit = mem.screen_read(draw_start + (64 * i + j));
            int xored_bit = sprite_bit ^ screen_bit;

            // Set VF on collision 
            if (xored_bit != screen_bit)
                mem.reg_write(0xF, 1);

            // Update Screen
            mem.screen_write(draw_start + (64 * i + j), xored_bit);
        }
    }
    return 0xD000;
}

/* Skip next instruction if key with the value of VX is pressed */ 
int opEX9E(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read(((instruction & 0xF00) >> 8));
    if (mem.get_key(vx))
        mem.inc_program_counter();
    return 0xE09E; 
}

/* Skip next instruction if key with the value of VX is not pressed */
int opEXA1(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    if (!mem.get_key(vx))
        mem.inc_program_counter();
    return 0xE0A1; 
}

/* Set register VX equal to the delay timer value */
int opFX07(int instruction, Memory &mem) 
{ 
    int dt = mem.get_delay_timer();
    int x = (instruction & 0xF00) >> 8;
    mem.reg_write(x, dt);
    return 0xF007; 
}

/* Wait for a key press, store value of key in VX */
int opFX0A(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    for (;;)
        for (int i = 0; i < 16; i ++)
            if (mem.get_key(i))
            {
                mem.reg_write(x, i);
                return 0xF00A;
            } 
}

/* Set delay timer = VX */
int opFX15(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    mem.set_delay_timer(vx);
    return 0xF015; 
}

/* Set sound timer = VX */
int opFX18(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    mem.set_sound_timer(vx);
    return 0xF018; 
}

/* Set address pointer = address pointer + VX */
int opFX1E(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    int ap = mem.get_address_pointer();
    mem.set_address_pointer(ap + vx);
    return 0xF01E; 
}

/* Set address pointer = location of sprite for digit VX */
int opFX29(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    mem.set_address_pointer(5 * vx);
    return 0xF029; 
}

/* Store BCD representation of VX in memory location
   address_pointer .. address_pointer + 2
*/
int opFX33(int instruction, Memory &mem) 
{ 
    int vx = mem.reg_read((instruction & 0xF00) >> 8);
    int ap = mem.get_address_pointer();

    // ones place
    mem.mem_write(ap + 2, vx % 10);
    // tens place
    vx = floor(vx / 10);
    mem.mem_write(ap + 1, vx % 10);
    // hundreds place
    vx = floor(vx / 10);
    mem.mem_write(ap, vx);

    return 0xF033; 
}

/* Store registers V0 through VX in memory starting at address_pointer */
int opFX55(int instruction, Memory &mem) 
{ 
    int x = (instruction & 0xF00) >> 8;
    int a = mem.get_address_pointer();
    for (int i = 0; i <= x; i++)
    {
        int val = mem.reg_read(i);
        mem.mem_write(a + i, val);
    }
    return 0xF055; 
}


int opFX65(int instruction, Memory &mem) { return 0xF065; }
int invalidOpcode(int instruction, Memory &mem) { return -1; }


/* Testing utilities */
void draw_screen(Memory mem) 
{
    cout << "\n\n";
    for (auto i = 0; i < 2048; i++)
    {
        if (i % 64 == 0)
            cout << "\n";
        if (mem.screen_read(i))
            cout << " ";
        else
            cout << "X";
    }
    cout << "\n\n";
}
