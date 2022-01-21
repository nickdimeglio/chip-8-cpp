
# CHIP-8 Programming Language Interpreter/Emulator

![Pong Demo Gif](https://media.giphy.com/media/WrRiawIuXeF4uVYxUR/giphy.gif)

CHIP-8 is an interpreted programming language developed by Joseph Weisbecker in the late 1970s. Weisbecker designed CHIP-8 to allow video games to be programmed once and then run on any computer. As such, the CHIP-8 interpreter takes the form of a console emulator, even though an actual CHIP-8 console never existed. 
 
Programs written for the CHIP-8 console were typically run on 8-bit computers (like the COSMAC VIP) using a virtual console. This virtual console consists of 4K main memory, 16 registers, a stack, two timers, a font set, and input/output with support for graphics and sound. Programs are written for CHIP-8 using hexadecimal opcodes. For a detailed language specification, check out [CowGod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#3.0). 

Here's a code snippet from CHIP-8.com:
```
                    0x00E0             // Clear the Screen.
                    0x6A00             // Set the X co-ordinate to 00H.
                    0x6B00             // Set the Y co-ordinate to 00H.
                    0x2400             // Draw SPRITE.
                    0x2500             // Keep on the Screen for a moment.
                    0x2400             // Erase the Sprite. (XOR with the previously drawn Sprite).
```  



**Important:** If you would like to run my emulator on your own machine, please run the ROM 'test_opcode.ch8' first:

![Test ROM Gif](https://media.giphy.com/media/KqSmW2BimasSZ8wxt8/giphy.gif)

If all opcodes in the test suite return 'OK', you are good to go. I have included PONG and Kaleidoscope (by Weisbecker) in the repo, but there is a world of CHIP-8 ROMs out there ([this repository](https://github.com/kripod/chip8-roms) has quite a few). 
