#include <iostream>
#include <vector>

using namespace std;

class Chip8_memory 
{
    public:
        // Main memory access
        void mem_write(int address, int value) 
        {
            memory[address] = value;
        }
        int mem_read(int address) 
        {
            return memory[address];
        }
        // Program counter access
        void set_program_counter(int address) 
        {
            program_counter = address;
        }
        int get_program_counter() 
        {
            return program_counter;
        }
        // Address pointer access
        void set_address_pointer(int address) 
        {
            address_pointer = address;
        }
        void set_stack_pointer(int address)
        {
            stack_pointer = address;
        }
    private:
        vector<int> memory = vector<int>(4096);
        int program_counter = 0x200;
        int last_opcode = 0;
        int address_pointer = 0;
        int stack_pointer = 0;

};


int main() 
{
    return 0;
}
