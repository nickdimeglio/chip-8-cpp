#include "Memory.h"
#include "Cpu.h"
#include <iostream>
#include <string>
using namespace std;


int Cpu::op00E0(int instruction) { return 0; } 

string Cpu::decode(int instruction)
{
    return string("00E0");
}