#include "Memory.h"
#include <algorithm>
using namespace std;

// Font set
int font_set[80] {
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0 (Starts at address 0)
0x20, 0x60, 0x20, 0x20, 0x70, // 1 (Starts at 5)
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2 (10)
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3 (15)
0x90, 0x90, 0xF0, 0x10, 0x10, // 4 (20)
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5 (25)
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6 (30)
0xF0, 0x10, 0x20, 0x40, 0x40, // 7 (35) 
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8 (40)
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9 (45)
0xF0, 0x90, 0xF0, 0x90, 0x90, // A (50)
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B (55)
0xF0, 0x80, 0x80, 0x80, 0xF0, // C (60) 
0xE0, 0x90, 0x90, 0x90, 0xE0, // D (65)
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E (70)
0xF0, 0x80, 0xF0, 0x80, 0x80, // F (75)
};

// Constructor
Memory::Memory() 
{
    copy(font_set, font_set + 80, memory); 
    program_counter = 0x200;
    delay_timer = -1;
    sound_timer = -1;
}

// Main memory access
int Memory::mem_read(int address) { return memory[address]; }
void Memory::mem_write(int address, int value) { memory[address] = value; }

// Register access
int Memory::reg_read(int address) { return registers[address]; }
void Memory::reg_write(int address, int value) { registers[address] = value; }

// Screen memory access
int Memory::screen_read(int address) { return screen[address]; }
void Memory::screen_write(int address, int value) { screen[address] = value; }

// ROM access 
int Memory::get_program_counter() { return program_counter; }
void Memory::set_program_counter(int address) { program_counter = address; }

// Address pointer access
int Memory::get_address_pointer() { return address_pointer; }
void Memory::set_address_pointer(int address) { address_pointer = address; }

// Stack access
int Memory::get_stack_pointer(int address) { return stack_pointer; }
void Memory::set_stack_pointer(int address) { stack_pointer = address; }

// Timer access
int Memory::get_delay_timer() { return delay_timer; }
void Memory::set_delay_timer(int cycles) { delay_timer = cycles; }
int Memory::get_sound_timer() { return sound_timer; }
void Memory::set_sound_timer(int cycles) { sound_timer = cycles; }
