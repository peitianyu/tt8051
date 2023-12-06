#include "handle_token.h"

INSTRUCTIONS void identifier2hex() {}

INSTRUCTIONS void mov2hex() {}

INSTRUCTIONS void movc2hex() {}

INSTRUCTIONS void movx2hex() {}

INSTRUCTIONS void push2hex() {}

INSTRUCTIONS void pop2hex() {}

INSTRUCTIONS void xch2hex() {}

INSTRUCTIONS void xchd2hex() {}

INSTRUCTIONS void swap2hex() {}

INSTRUCTIONS void add2hex() {}

INSTRUCTIONS void addc2hex() {}

INSTRUCTIONS void subb2hex() {}

INSTRUCTIONS void inc2hex() {}

INSTRUCTIONS void dec2hex() {}

INSTRUCTIONS void mul2hex() {}

INSTRUCTIONS void div2hex() {}

INSTRUCTIONS void da2hex() {}

INSTRUCTIONS void clr2hex() {}

INSTRUCTIONS void cpl2hex() {}

INSTRUCTIONS void rl2hex() {}

INSTRUCTIONS void rlc2hex() {}

INSTRUCTIONS void rr2hex() {}

INSTRUCTIONS void rrc2hex() {}

INSTRUCTIONS void anl2hex() {}

INSTRUCTIONS void orl2hex() {}

INSTRUCTIONS void xrl2hex() {}

INSTRUCTIONS void ajmp2hex() {}

INSTRUCTIONS void sjmp2hex() {}

INSTRUCTIONS void ljmp2hex() {}

INSTRUCTIONS void jmp2hex() {}

INSTRUCTIONS void jz2hex() {}

INSTRUCTIONS void jnz2hex() {}

INSTRUCTIONS void cjne2hex() {}

INSTRUCTIONS void djnz2hex() {}

INSTRUCTIONS void acall2hex() {}

INSTRUCTIONS void lcall2hex() {}

INSTRUCTIONS void ret2hex() {}

INSTRUCTIONS void reti2hex() {}

INSTRUCTIONS void nop2hex() {}

INSTRUCTIONS void setb2hex() {}

INSTRUCTIONS void jc2hex() {}

INSTRUCTIONS void jnc2hex() {}

INSTRUCTIONS void jb2hex() {}

INSTRUCTIONS void jnb2hex() {}

INSTRUCTIONS void jbc2hex() {}

INSTRUCTIONS void org2hex() {}

INSTRUCTIONS void db2hex() {}

INSTRUCTIONS void data2hex() {}

INSTRUCTIONS void equ2hex() {}

INSTRUCTIONS void sfr2hex() {}

INSTRUCTIONS void sbit2hex() {}

INSTRUCTIONS void end2hex() {}

INSTRUCTIONS void include2hex() {}

#include "token.h"
void handle_token(int token)
{
    switch(token)
    {
        case MOV: mov2hex(); break;
        case MOVC: movc2hex(); break;
        case MOVX: movx2hex(); break;
        case PUSH: push2hex(); break;
        case POP: pop2hex(); break;
        case XCH: xch2hex(); break;
        case XCHD: xchd2hex(); break;
        case SWAP: swap2hex(); break;
        case ADD: add2hex(); break;
        case ADDC: addc2hex(); break;
        case SUBB: subb2hex(); break;
        case INC: inc2hex(); break;
        case DEC: dec2hex(); break;
        case MUL: mul2hex(); break;
        case DIV: div2hex(); break;
        case DA: da2hex(); break;
        case CLR: clr2hex(); break;
        case CPL: cpl2hex(); break;
        case RL: rl2hex(); break;
        case RLC: rlc2hex(); break;
        case RR: rr2hex(); break;
        case RRC: rrc2hex(); break;
        case ANL: anl2hex(); break;
        case ORL: orl2hex(); break;
        case XRL: xrl2hex(); break;
        case AJMP: ajmp2hex(); break;
        case SJMP: sjmp2hex(); break;
        case LJMP: ljmp2hex(); break;
        case JMP: jmp2hex(); break;
        case JZ: jz2hex(); break;
        case JNZ: jnz2hex(); break;
        case CJNE: cjne2hex(); break;
        case DJNZ: djnz2hex(); break;
        case ACALL: acall2hex(); break;
        case LCALL: lcall2hex(); break;
        case RET: ret2hex(); break;
        case RETI: reti2hex(); break;
        case NOP: nop2hex(); break;
        case SETB: setb2hex(); break;
        case JC: jc2hex(); break;
        case JNC: jnc2hex(); break;
        case JB: jb2hex(); break;
        case JNB: jnb2hex(); break;
        case JBC: jbc2hex(); break;
        case ORG: org2hex(); break;
        case DB: db2hex(); break;
        case EQU: equ2hex(); break;
        case DATA: data2hex(); break;
        case SFR: sfr2hex(); break;
        case SBIT: sbit2hex(); break;
        case END: end2hex(); break;
        case INCLUDE: include2hex(); break;
        case IDENTIFIER: identifier2hex(); break;
    }
}