#ifndef __TOKEN_H__
#define __TOKEN_H__

/* R0~R7 A AB C DPTR PC @ # + / . ^ $ , */
enum Token
{
    /* register */
    REG_R0 = 0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7, REG_A, REG_AB, REG_C, REG_DPTR, REG_PC, 
    /* symbol */
    SYMBOL_AT, SYMBOL_HASH, SYMBOL_PLUS, SYMBOL_SLASH, SYMBOL_DOT, SYMBOL_HAT, SYMBOL_DOLLAR, SYMBOL_COMMA, SYMBOL_COLON, SYMBOL_QUOTE, SYMBOL_NUM,
    /* instructions */
    MOV, MOVC, MOVX, PUSH, POP, XCH, XCHD, SWAP, ADD, ADDC, SUBB, INC, DEC, MUL, DIV, DA, CLR, CPL, RL, RLC, RR, RRC, ANL, ORL, XRL,
    AJMP, SJMP, LJMP, JMP, JZ, JNZ, CJNE, DJNZ, ACALL, LCALL, RET, RETI, NOP, SETB, JC, JNC, JB, JNB, JBC, 
    /*identifier*/
    IDENTIFIER, INCLUDE,
    /* directives */
    DB, EQU, DATA, SFR, SBIT, ORG, END
};

#endif // __TOKEN_H__