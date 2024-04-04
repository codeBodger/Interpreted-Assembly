#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sense.h"

#define SINGLE_OCTAL 0x0007
#define ADDR 0x00ff
#define REG  0x0700
#define REG1 0x0070
#define REG2 0x0007
#define INST 0xf800

#define BAD_REG 1
#define BAD_INST 2
#define STACK_UNDERFLOW 3
#define NO_FILE_PROVIDED 4
#define FILE_NOT_FOUND 5

pi_framebuffer_t *fb;
sense_fb_bitmap_t *bm;

typedef struct StackElement {
    short value;
    struct StackElement* previous;
} StackElement;

void end(short exitcode);

short code[256];
short memory[256];

int main(int argc, char *argv[]) {
    if (argc != 2) return NO_FILE_PROVIDED;
    FILE* source = fopen(argv[1], "rb");
    if (source == NULL) return FILE_NOT_FOUND;
    fread(code, sizeof(short), sizeof(code), source);

	if (fb == NULL && bm == NULL) {
        fb = getFrameBuffer();
        bm = fb->bitmap;
    }

    StackElement* next;
    StackElement* top = malloc(sizeof(StackElement));
        top->previous = NULL;

    register short aaa;
    register short aab;
    register short aba;
    register short abb;
    register short baa;
    register short bab;
    register short bba;
    register short bbb;

    register unsigned char line = 0;
    register short valueInReg;
    register short valueInReg1;
    register short valueInReg2;

    while (1) {
        switch ((code[line] & REG) >> 8) {
            case 0: valueInReg = aaa; break;
            case 1: valueInReg = aab; break;
            case 2: valueInReg = aba; break;
            case 3: valueInReg = abb; break;
            case 4: valueInReg = baa; break;
            case 5: valueInReg = bab; break;
            case 6: valueInReg = bba; break;
            case 7: valueInReg = bbb; break;
            default: end(BAD_REG);
        }
        switch ((code[line] & REG1) >> 4) {
            case 0: valueInReg1 = aaa; break;
            case 1: valueInReg1 = aab; break;
            case 2: valueInReg1 = aba; break;
            case 3: valueInReg1 = abb; break;
            case 4: valueInReg1 = baa; break;
            case 5: valueInReg1 = bab; break;
            case 6: valueInReg1 = bba; break;
            case 7: valueInReg1 = bbb; break;
            default: end(BAD_REG);
        }
        switch (code[line] & REG2) {
            case 0: valueInReg2 = aaa; break;
            case 1: valueInReg2 = aab; break;
            case 2: valueInReg2 = aba; break;
            case 3: valueInReg2 = abb; break;
            case 4: valueInReg2 = baa; break;
            case 5: valueInReg2 = bab; break;
            case 6: valueInReg2 = bba; break;
            case 7: valueInReg2 = bbb; break;
            default: end(BAD_REG);
        }

        switch ((code[line] & INST) >> 11) {
            case MOVE_: // move>regAddresss [from reg to Addresss]
                memory[code[line] & ADDR] = valueInReg;
            break;
            case MOVE1: // move<regAddresss [from Addresss to reg]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = memory[code[line] & ADDR]; break;
                    case 1: aab = memory[code[line] & ADDR]; break;
                    case 2: aba = memory[code[line] & ADDR]; break;
                    case 3: abb = memory[code[line] & ADDR]; break;
                    case 4: baa = memory[code[line] & ADDR]; break;
                    case 5: bab = memory[code[line] & ADDR]; break;
                    case 6: bba = memory[code[line] & ADDR]; break;
                    case 7: bbb = memory[code[line] & ADDR]; break;
                }
            break;
            case PUT__: // put<<reg________Value... [value into reg]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = code[++line]; break;
                    case 1: aab = code[++line]; break;
                    case 2: aba = code[++line]; break;
                    case 3: abb = code[++line]; break;
                    case 4: baa = code[++line]; break;
                    case 5: bab = code[++line]; break;
                    case 6: bba = code[++line]; break;
                    case 7: bbb = code[++line]; break;
                }
            break;
            case PUT1_: // put<____AddresssValue... [value into Addresss]
                memory[code[line] & ADDR] = code[++line];
            break;
        
            case MOVE2: // mov>>regAddresss [from address in (char)reg to Address]
                memory[code[line] & ADDR] = memory[(char)valueInReg];
            break;
            case MOVE3: // mov<<reg____.reg [from address in (char).reg to reg]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg2; break;
                    case 1: aab = valueInReg2; break;
                    case 2: aba = valueInReg2; break;
                    case 3: abb = valueInReg2; break;
                    case 4: baa = valueInReg2; break;
                    case 5: bab = valueInReg2; break;
                    case 6: bba = valueInReg2; break;
                    case 7: bbb = valueInReg2; break;
                }
            break;
            case SHFTL: // <<___reg.reg-reg [<<, like below]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 << valueInReg2; break;
                    case 1: aab = valueInReg1 << valueInReg2; break;
                    case 2: aba = valueInReg1 << valueInReg2; break;
                    case 3: abb = valueInReg1 << valueInReg2; break;
                    case 4: baa = valueInReg1 << valueInReg2; break;
                    case 5: bab = valueInReg1 << valueInReg2; break;
                    case 6: bba = valueInReg1 << valueInReg2; break;
                    case 7: bbb = valueInReg1 << valueInReg2; break;
                }
            break;
            case SHFTR: // >>___
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 >> valueInReg2; break;
                    case 1: aab = valueInReg1 >> valueInReg2; break;
                    case 2: aba = valueInReg1 >> valueInReg2; break;
                    case 3: abb = valueInReg1 >> valueInReg2; break;
                    case 4: baa = valueInReg1 >> valueInReg2; break;
                    case 5: bab = valueInReg1 >> valueInReg2; break;
                    case 6: bba = valueInReg1 >> valueInReg2; break;
                    case 7: bbb = valueInReg1 >> valueInReg2; break;
                }
            break;

            case ADD__: // add__reg.reg-reg [put (.reg + -reg) into reg]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 + valueInReg2; break;
                    case 1: aab = valueInReg1 + valueInReg2; break;
                    case 2: aba = valueInReg1 + valueInReg2; break;
                    case 3: abb = valueInReg1 + valueInReg2; break;
                    case 4: baa = valueInReg1 + valueInReg2; break;
                    case 5: bab = valueInReg1 + valueInReg2; break;
                    case 6: bba = valueInReg1 + valueInReg2; break;
                    case 7: bbb = valueInReg1 + valueInReg2; break;
                }
            break;
            case SUB__: // sub__
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 - valueInReg2; break;
                    case 1: aab = valueInReg1 - valueInReg2; break;
                    case 2: aba = valueInReg1 - valueInReg2; break;
                    case 3: abb = valueInReg1 - valueInReg2; break;
                    case 4: baa = valueInReg1 - valueInReg2; break;
                    case 5: bab = valueInReg1 - valueInReg2; break;
                    case 6: bba = valueInReg1 - valueInReg2; break;
                    case 7: bbb = valueInReg1 - valueInReg2; break;
                }
            break;
            case MULT_: // mult_
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 * valueInReg2; break;
                    case 1: aab = valueInReg1 * valueInReg2; break;
                    case 2: aba = valueInReg1 * valueInReg2; break;
                    case 3: abb = valueInReg1 * valueInReg2; break;
                    case 4: baa = valueInReg1 * valueInReg2; break;
                    case 5: bab = valueInReg1 * valueInReg2; break;
                    case 6: bba = valueInReg1 * valueInReg2; break;
                    case 7: bbb = valueInReg1 * valueInReg2; break;
                }
            break;
            case DIV__: // div__
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 / valueInReg2; break;
                    case 1: aab = valueInReg1 / valueInReg2; break;
                    case 2: aba = valueInReg1 / valueInReg2; break;
                    case 3: abb = valueInReg1 / valueInReg2; break;
                    case 4: baa = valueInReg1 / valueInReg2; break;
                    case 5: bab = valueInReg1 / valueInReg2; break;
                    case 6: bba = valueInReg1 / valueInReg2; break;
                    case 7: bbb = valueInReg1 / valueInReg2; break;
                }
            break;

            case AND__: // AND__reg.reg-reg [&, like above]
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 & valueInReg2; break;
                    case 1: aab = valueInReg1 & valueInReg2; break;
                    case 2: aba = valueInReg1 & valueInReg2; break;
                    case 3: abb = valueInReg1 & valueInReg2; break;
                    case 4: baa = valueInReg1 & valueInReg2; break;
                    case 5: bab = valueInReg1 & valueInReg2; break;
                    case 6: bba = valueInReg1 & valueInReg2; break;
                    case 7: bbb = valueInReg1 & valueInReg2; break;
                }
            break;
            case OR___: // OR___
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = valueInReg1 | valueInReg2; break;
                    case 1: aab = valueInReg1 | valueInReg2; break;
                    case 2: aba = valueInReg1 | valueInReg2; break;
                    case 3: abb = valueInReg1 | valueInReg2; break;
                    case 4: baa = valueInReg1 | valueInReg2; break;
                    case 5: bab = valueInReg1 | valueInReg2; break;
                    case 6: bba = valueInReg1 | valueInReg2; break;
                    case 7: bbb = valueInReg1 | valueInReg2; break;
                }
            break;
            case NAND_: // NAND_
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = ~(valueInReg1 & valueInReg2); break;
                    case 1: aab = ~(valueInReg1 & valueInReg2); break;
                    case 2: aba = ~(valueInReg1 & valueInReg2); break;
                    case 3: abb = ~(valueInReg1 & valueInReg2); break;
                    case 4: baa = ~(valueInReg1 & valueInReg2); break;
                    case 5: bab = ~(valueInReg1 & valueInReg2); break;
                    case 6: bba = ~(valueInReg1 & valueInReg2); break;
                    case 7: bbb = ~(valueInReg1 & valueInReg2); break;
                }
            break;
            case NOR__: // NOR__
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = ~(valueInReg1 | valueInReg2); break;
                    case 1: aab = ~(valueInReg1 | valueInReg2); break;
                    case 2: aba = ~(valueInReg1 | valueInReg2); break;
                    case 3: abb = ~(valueInReg1 | valueInReg2); break;
                    case 4: baa = ~(valueInReg1 | valueInReg2); break;
                    case 5: bab = ~(valueInReg1 | valueInReg2); break;
                    case 6: bba = ~(valueInReg1 | valueInReg2); break;
                    case 7: bbb = ~(valueInReg1 | valueInReg2); break;
                }
            break;

            case DRAW2: // draw_reg.reg_xxx [set the pixel at [.reg][x] to the colour in reg]
                bm->pixel[valueInReg1 & SINGLE_OCTAL][code[line] & ADDR & SINGLE_OCTAL] = valueInReg;
            break;
            case DRAW3: // draw_reg_yyy-reg [set the pixel at [y][-reg] to the colour in reg]
                bm->pixel[(code[line] & ADDR >> 4) & SINGLE_OCTAL][valueInReg2 & SINGLE_OCTAL] = valueInReg;
            break;
            case IF_GO: // if___regCodeLine [if (reg) goto CodeLine]
                if (valueInReg) {
                    line = code[line] & ADDR;
                    continue; //to avoid incrementing
                }
            break;
            // case _____: //

            case PUSH_: // push_reg________ [push the value in reg onto the stack]
                top->value = valueInReg;
                next = malloc(sizeof(StackElement));
                next->previous = top;
                top = next;
            break;
            case POP__: // pop__reg________ [pop the first value off of the stack and put it into reg]
                if (top->previous == NULL) end(STACK_UNDERFLOW);
                next = top;
                top = top->previous;
                switch ((code[line] & REG) >> 8) {
                    case 0: aaa = top->value;
                    case 1: aab = top->value;
                    case 2: aba = top->value;
                    case 3: abb = top->value;
                    case 4: baa = top->value;
                    case 5: bab = top->value;
                    case 6: bba = top->value;
                    case 7: bbb = top->value;
                }
                free(next);
            break;
            case DRAW_: // drawvreg_yyy_xxx [set the pixel at [y][x] to the colour in reg]
                bm->pixel[valueInReg1 & SINGLE_OCTAL][valueInReg2 & SINGLE_OCTAL] = valueInReg;
            break;
            case DRAW1: // drawrreg.reg-reg [set the pixel at [.reg][-reg] to the colour in reg]
                bm->pixel[(code[line] & ADDR >> 4) & SINGLE_OCTAL][code[line] & ADDR & SINGLE_OCTAL] = valueInReg;
            break;

            case NIF__: // nIf__regCodeLine [if (!reg) goto CodeLine]
                if (!valueInReg) {
                    line = code[line] & ADDR;
                    continue;
                }
            break;
            case GOTO_: // GoTo____CodeLine [goto CodeLine]
                line = code[line] & ADDR;
            continue;
            case EXIT1: // exitv___value___ [exit with code value___]
                end(code[line] & ADDR);
            case EXIT_: // exitrreg________ [exit with code in reg]
                end(valueInReg);
            
            default: end(BAD_INST);
        }
        line++;
    }

    end(0);
    return 0;
}

void end(short exitcode) {
    if (fb != NULL) {
        freeFrameBuffer(fb);
    }
    sleep(1);
    fb = NULL; bm = NULL;

    exit(exitcode);
}