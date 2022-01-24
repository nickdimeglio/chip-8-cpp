#include "Memory.h"
#include <algorithm>
#include <unordered_map>
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
}

// Main memory access
int Memory::mem_read(int address) { return memory[address]; }
void Memory::mem_write(int address, int value) { memory[address] = value; }

// Register access
int Memory::reg_read(int address) { return registers[address]; }
void Memory::reg_write(int address, int value) { registers[address] = value; }

// Stack access
int Memory::stack_pop()
{
    stack_pointer--;
    if (0 <= stack_pointer && stack_pointer <= 15) 
        return stack[stack_pointer];
    else
        return -1;
}
int Memory::stack_peek()
{
    if (1 <= stack_pointer && stack_pointer <= 16)
        return stack[stack_pointer - 1];
    else
        return -1;
}
void Memory::stack_push(int address) 
{
    if (0 <= stack_pointer && stack_pointer <= 15)
    {
        stack[stack_pointer] = address;
        stack_pointer++;
    }
}

// Screen memory access
int Memory::screen_read(int address) { return screen[address]; }
void Memory::screen_write(int address, int value) { screen[address] = value; }

// Pointer access
int Memory::get_address_pointer() { return address_pointer; }
void Memory::set_address_pointer(int address) { address_pointer = address; }
int Memory::get_program_counter() { return program_counter; }
void Memory::inc_program_counter() { program_counter += 2; }
void Memory::set_program_counter(int address) { program_counter = address; }

// Timer access
int Memory::get_delay_timer() { return delay_timer; }
void Memory::set_delay_timer(int cycles) { delay_timer = cycles; }
int Memory::get_sound_timer() { return sound_timer; }
void Memory::set_sound_timer(int cycles) { sound_timer = cycles; }

// Keyboard access
bool Memory::get_key(int key) { return keys[key]; }
void Memory::set_key(int key, bool state) { keys[key] = state; }
void Memory::flip_key(int key) { keys[key] = !keys[key]; }