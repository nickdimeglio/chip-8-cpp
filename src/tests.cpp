#define CATCH_CONFIG_MAIN
#include "Memory.h"
#include <iostream>
#include <catch2/catch.hpp>


TEST_CASE( "CHIP-8 Memory" )
{
    Memory memo = Memory();

    REQUIRE( memo.mem_read(0) == 0xF0 );
    REQUIRE( memo.mem_read(79) == 0x80 );
    REQUIRE( memo.mem_read(80) == 0 );
    REQUIRE( memo.mem_read(4095) == 0 );
    REQUIRE( memo.get_program_counter() == 0x200 );
    REQUIRE( memo.get_delay_timer() == -1 );

    SECTION( "setting memory members" )
    {
        memo.mem_write(0x600, 99);
        memo.set_address_pointer(0x400);
        memo.set_sound_timer(60);

        REQUIRE( memo.mem_read(0x600) == 99 );
        REQUIRE( memo.get_address_pointer() == 0x400 );
        REQUIRE( memo.get_sound_timer() == 60 );
    }
}
