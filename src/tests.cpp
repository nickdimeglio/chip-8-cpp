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
        memo.set_address_pointer(0x400);
        memo.set_sound_timer(60);

        REQUIRE( memo.mem_read(0x600) == 99 );
        REQUIRE( memo.get_address_pointer() == 0x400 );
        REQUIRE( memo.get_sound_timer() == 60 );
    }
}

TEST_CASE( "CHIP-8 CPU" )
{
    Cpu cpu = Cpu();

    REQUIRE( cpu.decode(0x00E0) == string("00E0") );
}
