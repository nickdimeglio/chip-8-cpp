#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Memory.h"
#include "Cpu.h"
#include <iostream>
#include <string>
using namespace std;


TEST_CASE( "CHIP-8 Memory" )
{
    Memory memo = Memory();

    REQUIRE( memo.mem_read(0) == 0xF0 );
    REQUIRE( memo.mem_read(79) == 0x80 );
    REQUIRE( memo.mem_read(80) == 0 );
    REQUIRE( memo.mem_read(4095) == 0 );
    REQUIRE( memo.get_program_counter() == 0x200 );
    REQUIRE( memo.get_delay_timer() == -1 );

    SECTION( "setting memory state" )
    {
        memo.mem_write(0x600, 99);
        REQUIRE( memo.mem_read(0x600) == 99 );

        memo.set_address_pointer(0x400);
        REQUIRE( memo.get_address_pointer() == 0x400 );

        memo.set_sound_timer(60);
        REQUIRE( memo.get_sound_timer() == 60 );

        memo.stack_push(0x300);
        REQUIRE( memo.stack_pop() == 0x300);
    }
}

TEST_CASE( "CHIP-8 CPU" )
{
    Memory mem = Memory();
    SECTION( "decoding chip-8 instructions" ) 
    {
        REQUIRE( execute(0x00E0, mem) == 0x00E0 );    // 00E0
        REQUIRE( execute(0x00EE, mem) == 0x00EE );    // 00EE 
        REQUIRE( execute(0x199B, mem) == 0x1000 );    // 1nnn
        REQUIRE( execute(0x2761, mem) == 0x2000 );    // 2nnn
        REQUIRE( execute(0x37FD, mem) == 0x3000 );    // 3xkk
        REQUIRE( execute(0x4DF7, mem) == 0x4000 );    // 4xkk
        REQUIRE( execute(0x5340, mem) == 0x5000 );    // 5xy0
        REQUIRE( execute(0x69AA, mem) == 0x6000 );    // 6xkk
        REQUIRE( execute(0x7213, mem) == 0x7000 );    // 7xkk
        REQUIRE( execute(0x8890, mem) == 0x8000 );    // 8xy0
        REQUIRE( execute(0x8BB1, mem) == 0x8001 );    // 8xy1
        REQUIRE( execute(0x8422, mem) == 0x8002 );    // 8xy2
        REQUIRE( execute(0x8983, mem) == 0x8003 );    // 8xy3
        REQUIRE( execute(0x8164, mem) == 0x8004 );    // 8xy4
        REQUIRE( execute(0x8775, mem) == 0x8005 );    // 8xy5
        REQUIRE( execute(0x8CD6, mem) == 0x8006 );    // 8xy6
        REQUIRE( execute(0x8E17, mem) == 0x8007 );    // 8xy7
        REQUIRE( execute(0x837E, mem) == 0x800E );    // 8xyE
        REQUIRE( execute(0x9880, mem) == 0x9000 );    // 9xy0
        REQUIRE( execute(0xABCD, mem) == 0xA000 );    // Annn
        REQUIRE( execute(0xB321, mem) == 0xB000 );    // Bnnn
        REQUIRE( execute(0xC43C, mem) == 0xC000 );    // Cxkk
        REQUIRE( execute(0xD982, mem) == 0xD001 );    // Dxyn
        REQUIRE( execute(0xE99E, mem) == 0xE09E );    // Ex9E
        REQUIRE( execute(0xEBA1, mem) == 0xE0A1 );    // ExA1
        REQUIRE( execute(0xF007, mem) == 0xF007 );    // Fx07
        REQUIRE( execute(0xF10A, mem) == 0xF00A );    // Fx0A
        REQUIRE( execute(0xF215, mem) == 0xF015 );    // Fx15
        REQUIRE( execute(0xFF18, mem) == 0xF018 );    // Fx18
        REQUIRE( execute(0xF11E, mem) == 0xF01E );    // Fx1E
        REQUIRE( execute(0xF829, mem) == 0xF029 );    // Fx29
        REQUIRE( execute(0xF533, mem) == 0xF033 );    // Fx33
        REQUIRE( execute(0xFE55, mem) == 0xF055 );    // Fx55
        REQUIRE( execute(0xF965, mem) == 0xF065 );    // Fx65
        REQUIRE( execute(0xF331, mem) == -1 );        // Invalid
    }
    SECTION( "Execute 00E0" ) 
    {
        // 00E0 Clears the Screen
        Memory mem = Memory();
        REQUIRE( mem.screen_read(0x600) == 0 );
        mem.screen_write(0x600, 1);
        // Two pixels set to one
        REQUIRE( mem.screen_read(0x600) == 1 );
        // 00E0 executes without error 
        REQUIRE( execute(0x00E0, mem) == 0x00E0 );
        // Two pixels reset to zero
        REQUIRE( mem.screen_read(0x600) == 0 );
    }

    SECTION( "Execute 00EE" )
    {
        // 00E0 Returns from a Subroutine
        Memory mem = Memory();
        // Jump from 0x200 to 0x800, storing 0x200 on the stack
        REQUIRE( mem.get_program_counter() == 0x200 );
        mem.stack_push(mem.get_program_counter());
        mem.set_program_counter(0x800);
        // Return to 0x200
        REQUIRE( mem.get_program_counter() == 0x800 );
        REQUIRE( execute(0x00EE, mem) == 0x00EE );
        REQUIRE( mem.get_program_counter() == 0x200 );
    }
    SECTION( "Execute 1NNN" ) 
    {
        // 1nnn sets the program counter to NNN
        Memory mem = Memory();
        REQUIRE( mem.get_program_counter() == 0x200 );
        REQUIRE( execute(0x1750, mem) == 0x1000 );
        REQUIRE( mem.get_program_counter() == 0x750 );
    }

    SECTION( "Execute 2NNN" )
    {
        // 2nnn calls the subroutine at NNN
        Memory mem = Memory();
        int pc = mem.get_program_counter();
        // Executing 2NNN should return 0x2000
        REQUIRE( execute(0x2600, mem) == 0x2000 );
        // Last pc should be on top of stack
        REQUIRE( mem.stack_peek() == pc );
        // New pc should be set
        REQUIRE( mem.get_program_counter() == 0x600 );
    }
    SECTION( "Execute 3XKK" )
    {
        // 3XKK skips the next instruction if VX != KK
        Memory mem = Memory();
        
        // Should skip if VX = KK
        int pc = mem.get_program_counter();
        mem.reg_write(0x8, 0xBB);
        // 3XKK should return 0x3000
        REQUIRE( execute(0x38BB, mem) == 0x3000 );
        // pc should skip an instruction (2 bytes)
        REQUIRE( mem.get_program_counter() == (pc + 2) );
        
        // Should not skip otherwise
        pc = mem.get_program_counter();
        // 3XKK should return 0x3000
        REQUIRE( execute(0x30BB, mem) == 0x3000 );
        REQUIRE( mem.get_program_counter() == pc );
    }
    SECTION( "Execute 4XKK" )
    {
        // 4XKK skips the next instruction if VX != KK
        Memory mem = Memory();

        // Should not skip if VX = KK
        int pc = mem.get_program_counter();
        mem.reg_write(0x8, 0xBB);
        // 4XKK should return 0x4000
        REQUIRE( execute(0x48BB, mem) == 0x4000 );
        // pc should not have skipped
        REQUIRE( mem.get_program_counter() == pc );

        // Should skip otherwise
        // 4XKK should return 0x4000
        REQUIRE( execute(0x40BB, mem) == 0x4000 );
        REQUIRE( mem.get_program_counter() == (pc + 2) );
    }
}
