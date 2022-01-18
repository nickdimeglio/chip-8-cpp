#include "Memory.h"
#include <vector>

using namespace std;


// Constructor
Memory::Memory() 
{
    memory = vector<int>(4096);
    registers = vector<int>(16);
    screen = vector<int>(2048);
    program_counter = 0x200;
    address_pointer = 0;
    stack_pointer = 0;
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

