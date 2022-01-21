#ifndef MEMORY_H
#define MEMORY_H

class Memory
{
public:
    // Constructor
    Memory();

    // Main memory access
    int mem_size = 4096;
    int mem_read(int address);
    void mem_write(int address, int value);

    // Register access
    int reg_read(int address);
    void reg_write(int address, int value); 

    // Screen memory access
    int screen_size = 2048;
    int screen_read(int address);
    void screen_write(int address, int value);

    // ROM access 
    int get_program_counter();
    void set_program_counter(int address);

    // Address pointer access
    int get_address_pointer();
    void set_address_pointer(int address);

    // Stack access
    int get_stack_pointer(int address);
    void set_stack_pointer(int address);

    // Timer access
    int get_delay_timer();
    void set_delay_timer(int cycles);
    int get_sound_timer();
    void set_sound_timer(int cycles);

private:
    int memory[4096] {0};
    int registers[16] {0};
    int screen[2048] {0};
    int program_counter; 
    int address_pointer; 
    int stack_pointer; 
    int delay_timer; 
    int sound_timer; 
};

#endif
