#include "instructions.h"

#include <stdio.h>
#include "libsense/sense.h"

struct Line {
    unsigned int instruction : 5;
    unsigned int reg : 3;
    unsigned int address : 8;
};

struct Line code[256];
short memory[256];

int main() {
    register short aaa;
    register short aab;
    register short aba;
    register short abb;
    register short baa;
    register short bab;
    register short bba;
    register short bbb;

    register unsigned char line = 0;

    while (1) {
        switch (code[line].instruction) {
            case MOVE_: // move>regAddresss [from reg to Addresss]
            case MOVE1: // move<regAddresss [from Addresss to reg]
            case PUT__: // put<<reg________Value... [value into reg]
            case PUT1_: // put<____AddresssValue... [value into Addresss]
        
            case MOVE2: // mov>>regAddresss [from address in (char)reg to Address]
            case MOVE3: // mov<<reg____.reg [from address in (char).reg to reg]
            case SHFTL: // <<___reg.reg-reg [<<, like below]
            case SHFTR: // >>___

            case ADD__: // add__reg.reg-reg [put (.reg + -reg) into reg]
            case SUB__: // sub__
            case MULT_: // mult_
            case DIV__: // div__

            case AND__: // AND__reg.reg-reg [&, like above]
            case OR___: // OR___
            case NAND_: // NAND_
            case NOR__: // NOR__

            case PUSH_: // push_reg________ [push the value in reg onto the stack]
            case POP__: // pop__reg________ [pop the first value off of the stack and put it into reg]
            case _____: //
            case _____: //

            case NIF__: // nIf__regCodeLine [if (!reg) goto CodeLine]
            case GOTO_: // GoTo____CodeLine [goto CodeLine]
            case ____1: //
            case EXIT_: // exit_reg________ [exit with code in reg]
        }
        line++;
    }

    return 0;
}