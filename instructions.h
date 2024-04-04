#define MOVE_ 0b00000 // move>regAddresss [from reg to Addresss]
#define MOVE1 0b00001 // move<regAddresss [from Addresss to reg]
#define PUT__ 0b00010 // put<<reg________Value... [value into reg]
#define PUT1_ 0b00011 // put<____AddresssValue... [value into Addresss]
    
#define MOVE2 0b00100 // mov>>regAddresss [from address in (char)reg to Address]
#define MOVE3 0b00101 // mov<<reg____.reg [from address in (char).reg to reg]
#define SHFTL 0b00110 // <<___reg.reg-reg [<<, like below]
#define SHFTR 0b00111 // >>___

#define ADD__ 0b01000 // add__reg.reg-reg [put (.reg + -reg) into reg]
#define SUB__ 0b01001 // sub__
#define MULT_ 0b01010 // mult_
#define DIV__ 0b01011 // div__

#define AND__ 0b01100 // AND__reg.reg-reg [&, like above]
#define OR___ 0b01101 // OR___
#define NAND_ 0b01110 // NAND_
#define NOR__ 0b01111 // NOR__

#define PUSH_ 0b11000 // push_reg________ [push the value in reg onto the stack]
#define POP__ 0b11001 // pop__reg________ [pop the first value off of the stack and put it into reg]
#define DRAW_ 0b11010 // draw_reg_yyy_xxx [set the pixel at [y][x] to the colour in reg]
#define _____ 0b11011 //

#define NIF__ 0b11100 // nIf__regCodeLine [if (!reg) goto CodeLine]
#define GOTO_ 0b11101 // GoTo____CodeLine [goto CodeLine]
#define EXIT1 0b11110 // exitv___value___ [exit with code value___]
#define EXIT_ 0b11111 // exitrreg________ [exit with code in reg]
