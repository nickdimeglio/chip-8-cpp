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

        REQUIRE( memo.get_key(0xC) == 0 );
        memo.flip_key(0xC);
        REQUIRE( memo.get_key(0xC) == 1 );
        memo.set_key(0xC, 0);
        REQUIRE( memo.get_key(0xC) == 0);
    }
}


TEST_CASE( "Chip-8 CPU" )
{
    Memory mem = Memory();
    
    SECTION( "Execute 00E0" ) 
    {
        // 00E0 Clears the Screen
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
        REQUIRE( mem.get_program_counter() == 0x200 );
        REQUIRE( execute(0x1750, mem) == 0x1000 );
        REQUIRE( mem.get_program_counter() == 0x750 );
    }

    SECTION( "Execute 2NNN" )
    {
        // 2nnn calls the subroutine at NNN
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
        // Should not skip if VX = KK
        int pc = mem.get_program_counter();
        mem.reg_write(0x8, 0xBB);
        // pc should not have skipped
        REQUIRE( execute(0x48BB, mem) == 0x4000 );
        REQUIRE( mem.get_program_counter() == pc );

        // Should skip otherwise
        REQUIRE( execute(0x40BB, mem) == 0x4000 );
        REQUIRE( mem.get_program_counter() == (pc + 2) );
    }
    SECTION( "Execute 5XY0" )
    {
        // 5XY0 skips the next instruction if VX = VY
        mem.reg_write(0xB, 0xA);
        mem.reg_write(0xC, 0xA);
        mem.reg_write(0xD, 0x1);

        // Should skip if VX = VY
        REQUIRE( mem.get_program_counter() == 0x200);
        REQUIRE( execute(0x5BC0, mem) == 0x5000 );
        REQUIRE( mem.get_program_counter() == 0x202);

        // Should not skip if VX != VY
        REQUIRE( execute(0x5BD0, mem) == 0x5000 );
        REQUIRE( mem.get_program_counter() == 0x202);
    }
    SECTION( "Execute 6XKK" )
    {
        // 6XKK puts the value KK into register VX
        REQUIRE( mem.reg_read(0x7) == 0 );
        REQUIRE( execute(0x67BC, mem) == 0x6000 );
        REQUIRE( mem.reg_read(0x7) == 0xBC );
    }
    SECTION( "Execute 7XKK" )
    {
        // 7XKK puts the value VX + KK in register VX
        mem.reg_write(0xC, 0xA);
        REQUIRE( execute(0x7C06, mem) == 0x7000 );
        REQUIRE( mem.reg_read(0xC) == 0x10 );
    }
    SECTION( "Execute 8XY0" )
    {
        // 8XY0 Stores the value VY in VX
        mem.reg_write(0xA, 0x2);
        mem.reg_write(0xB, 0xFF);
        REQUIRE( execute(0x8AB0, mem) == 0x8000 );
        REQUIRE( mem.reg_read(0xB) == 0x2 );
    }
    SECTION( "Execute 8XY1" )
    {
        // 8XY1 sets VX = VX or VY
        mem.reg_write(0xA, 0xAF);
        mem.reg_write(0xB, 0xFA);
        REQUIRE( execute(0x8AB1, mem) == 0x8001 );
        REQUIRE( mem.reg_read(0xA) == 0xFF );
    }
    SECTION( "Execute 8XY2" )
    {
        // 8XY2 sets VX = VX and VY
        mem.reg_write(0xA, 0xAF);
        mem.reg_write(0xB, 0xFA);
        REQUIRE( execute(0x8AB2, mem) == 0x8002 );
        REQUIRE( mem.reg_read(0xA) == 0xAA );
    }
    SECTION( "Execute 8XY3" )
    {
        // 8XY3 sets VX = VX xor VY
        mem.reg_write(0xA, 0xAF);
        mem.reg_write(0xB, 0xFA);
        REQUIRE( execute(0x8AB3, mem) == 0x8003 );
        REQUIRE( mem.reg_read(0xA) == 0x55 );
    }
    SECTION( "Execute 8XY4" )
    {
        // 8XY4 sets VX = VX + VY,  VF = carry
        mem.reg_write(0xA, 0xFF);
        mem.reg_write(0xB, 0x01);

        // 0xFF + 0x1 == 0x0, carry = 0x1
        REQUIRE( execute(0x8AB4, mem) == 0x8004 );
        REQUIRE( mem.reg_read(0xA) == 0x0 );
        REQUIRE( mem.reg_read(0xF) == 0x1 );

        // 0x0 + 0x1 = 0x1, carry = 0x0
        REQUIRE( execute(0x8AB4, mem) == 0x8004 );
        REQUIRE( mem.reg_read(0xA) == 0x1 );
        REQUIRE( mem.reg_read(0xF) == 0x0 );
    }
    SECTION( "Execute 8XY5" )
    {
        // 8XY5 sets VX = VX - VY,  VF = NOT borrow
        mem.reg_write(0xA, 0xF0);
        mem.reg_write(0xB, 0xF2);
        mem.reg_write(0xC, 0x07);

        // 0xF0 - 0xF2 = 0xFE, not borrow = 0
        REQUIRE( execute(0x8AB5, mem) == 0x8005 );
        REQUIRE( mem.reg_read(0xA) == 0XFE );
        REQUIRE( mem.reg_read(0xF) == 0 );

        // 0xF2 - 0x7 = 0xEB not borrow = 1
        REQUIRE( execute(0x8BC5, mem) == 0x8005 );
        REQUIRE( mem.reg_read(0xB) == 0xEB );
        REQUIRE( mem.reg_read(0xF) == 0x1 );
    }
    SECTION( "Execute 8XY6" )
    {
        // 8XY6 sets VX = VX >> 1. VF = lost bit
        mem.reg_write(0xA, 0xFF);
        mem.reg_write(0xB, 0xFE);

        // 0xFF >> 1 = 0x7F, VF = 1
        REQUIRE( execute(0x8A06, mem) == 0x8006 );
        REQUIRE( mem.reg_read(0xA) == 0x7F );
        REQUIRE( mem.reg_read(0xF) == 0x1 );

        // 0xFE >> 1 = 0x7F, VF = 0
        REQUIRE( execute(0x8B06, mem) == 0x8006 );
        REQUIRE( mem.reg_read(0xB) == 0x7F );
        REQUIRE( mem.reg_read(0xF) == 0x0 );
    }
    SECTION( "Execute 8XY7" )
    {
        // 8XY5 sets VX = VY - VX,  VF = NOT borrow
        mem.reg_write(0xA, 0xF2);
        mem.reg_write(0xB, 0xF0);
        mem.reg_write(0xC, 0x07);
        mem.reg_write(0xD, 0xF2);

        // 0xF0 - 0xF2 = 0xFE, not borrow = 0
        REQUIRE( execute(0x8AB7, mem) == 0x8007 );
        REQUIRE( mem.reg_read(0xA) == 0xFE );
        REQUIRE( mem.reg_read(0xF) == 0x0 );

        // 0xF2 - 0x7 = 0xEB not borrow = 1
        REQUIRE( execute(0x8CD7, mem) == 0x8007 );
        REQUIRE( mem.reg_read(0xC) == 0xEB );
        REQUIRE( mem.reg_read(0xF) == 0x1 );
    }
    SECTION( "Execute 8XYE" )
    {
        // 8XYE set VX = VX << 1. VF = lost bit
        // 0xAB << 1 = 0x56, VF = 1
        mem.reg_write(0xA, 0xAB);
        REQUIRE( execute(0x8A0E, mem) == 0x800E );
        REQUIRE( mem.reg_read(0xA) == 0x56 );
        REQUIRE( mem.reg_read(0xF) == 0x1 );

        // 0x6F << 1 = 0xDE, VF = 0
        mem.reg_write(0xB, 0x6F);
        REQUIRE( execute(0x8B0E, mem) == 0x800E );
        REQUIRE( mem.reg_read(0xB) == 0xDE );
        REQUIRE( mem.reg_read(0xF) == 0x0 );
    }
    SECTION( "Execute 9XY0" )
    {
        // 9XY0 skips the next instruction if VX != VY
        mem.reg_write(0xA, 0x1);
        mem.reg_write(0xB, 0x1);

        // VA == VB
        REQUIRE( mem.get_program_counter() == 0x200 );
        REQUIRE( execute(0x9AB0, mem) == 0x9000 );
        REQUIRE( mem.get_program_counter() == 0x200 );

        // VB != VC
        mem.reg_write(0xC, 0x2);
        REQUIRE( execute(0x9BC0, mem) == 0x9000 );
        REQUIRE( mem.get_program_counter() == 0x202 );
    }
    SECTION( "Execute ANNN" )
    {
        // ANNN sets address pointer = NNN
        REQUIRE( mem.get_address_pointer() == 0);
        REQUIRE( execute(0xA678, mem) == 0xA000 );
        REQUIRE( mem.get_address_pointer() == 0x678 );
    }
    SECTION( "Execute BNNN" )
    {
        // BNNN sets program counter = NNN + V0
        REQUIRE( mem.get_program_counter() == 0x200 );
        mem.reg_write(0x0, 0x50);
        REQUIRE( execute(0xB400, mem) == 0xB000 );
        REQUIRE( mem.get_program_counter() == 0x450 );
    }
    SECTION( "Execute CXKK" )
    {
        // CXKK sets VX = random byte and KK 
        mem.reg_write(0xB, 0xED);
        REQUIRE( execute(0xCB11, mem) == 0xC000 );
        REQUIRE( mem.reg_read(0xB) != 0xED );
        REQUIRE( mem.reg_read(0xB) <= 0x11 );
    }
    SECTION( "Execute DXYN" )
    {
        // Draw a sprite by XOring n-bytes from address_pointer
        // with the Nx8 pixel grid starting at coordinate (VX, VY)
        // set VF = collision
        int i = 0x500;
        for (int i = 0x500; i < 0x50B; i++)
            mem.mem_write(i, 0xAA);
        mem.set_address_pointer(0x500);
        mem.reg_write(0xA, 24);
        mem.reg_write(0xB, 12);
        cout << "\n\n\n";
        draw_screen(mem);
        REQUIRE( execute(0xDABC, mem) == 0xD000 );
        REQUIRE( mem.reg_read(0xF) == 1 );

        mem.reg_write(0xA, 33); 
        REQUIRE( execute(0xDABC, mem) == 0xD000 );

        // Set a byte to all zeroes to ensure collision flag is unset
        for (int i = 0x500; i < 0x507; i++)
            mem.mem_write(i, 0x00);
        for (int i = 0; i < 8; i++)
            mem.screen_write(i, 0);
        mem.reg_write(0xA, 0);
        mem.reg_write(0xA, 0);
        REQUIRE( execute(0xDAB1, mem) == 0xD000 );
        REQUIRE( mem.reg_read(0xF) == 0 );

        cout << "\n\n\n";
        draw_screen(mem);
    }
    SECTION( "Execute EX9E" )
    {
        // EX9E skips the next instruction if key VX is pressed
        // 0xB not pressed
        REQUIRE( mem.get_program_counter() == 0x200 );
        REQUIRE( mem.reg_read(0xA) == 0 );
        REQUIRE( execute(0xEA9E, mem) == 0xE09E );
        REQUIRE( mem.get_program_counter() == 0x200 );
        // 0xB pressed
        mem.flip_key(0xB);
        mem.reg_write(0xA, 0xB);
        REQUIRE( mem.reg_read(0xA) == 0xB );
        REQUIRE( mem.get_key(0xB) == 1 );
        REQUIRE( execute(0xEA9E, mem) == 0xE09E );
        REQUIRE( mem.get_program_counter() == 0x202 );
    }
    SECTION( "Execute EXA1" )
    {
        // EXA1 skips the next instruction if key VX is not pressed
        // 0xB not pressed
        REQUIRE( mem.get_program_counter() == 0x200 );
        REQUIRE( mem.get_key(0xB) == 0 );
        mem.reg_write(0xA, 0xB);
        REQUIRE( mem.reg_read(0xA) == 0xB );
        REQUIRE( execute(0xEAA1, mem) == 0xE0A1 );
        REQUIRE( mem.get_program_counter() == 0x202 );
        // 0xB pressed
        mem.flip_key(0xB);
        REQUIRE( mem.reg_read(0xA) == 0xB );
        REQUIRE( mem.get_key(0xB) == 1 );
        REQUIRE( execute(0xEAA1, mem) == 0xE0A1 );
        REQUIRE( mem.get_program_counter() == 0x202 );
    }
}
