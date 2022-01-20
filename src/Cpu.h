#ifndef CPU_H
#define CPU_H
#include <string>
using namespace std;


class Cpu
{
public:
    // Decode instructions into opcodes
    string decode(int instruction);

    // Execute opcodes on instructions


private:
    // Opcode implementations
    int op00E0(int instruction);

};
#endif