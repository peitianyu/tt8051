#!/usr/local/bin/tcc -run handle_token.c get_token.c utils.c map.c

#include "handle_token.h"
#include "get_token.h"
#include "token.h"

static char* g_token_str[] = {
    "REG_R0", "REG_R1", "REG_R2", "REG_R3", "REG_R4", "REG_R5", "REG_R6", "REG_R7", "REG_A", "REG_AB", "REG_C", "REG_DPTR", "REG_PC", 
    "SYMBOL_AT", "SYMBOL_HASH", "SYMBOL_PLUS", "SYMBOL_SLASH", "SYMBOL_DOT", "SYMBOL_HAT", "SYMBOL_DOLLAR", "SYMBOL_COMMA", "SYMBOL_COLON", "SYMBOL_QUOTE", "SYMBOL_NUM",
    "MOV", "MOVC", "MOVX", "PUSH", "POP", "XCH", "XCHD", "SWAP", "ADD", "ADDC", "SUBB", "INC", "DEC", "MUL", "DIV", "DA", "CLR", "CPL", "RL", "RLC", "RR", "RRC", "ANL", "ORL", "XRL",
    "AJMP", "SJMP", "LJMP", "JMP", "JZ", "JNZ", "CJNE", "DJNZ", "ACALL", "LCALL", "RET", "RETI", "NOP", "SETB", "JC", "JNC", "JB", "JNB", "JBC", 
    "IDENTIFIER", "INCLUDE",
    "DB", "EQU", "DATA", "SFR", "SBIT", "ORG", "END"
};

int main(int argc, char *argv[])
{
    char* src_file = (argc > 1) ? argv[1] : "../data/io.ASM";
    int src_size = 10*1024;
    set_token_file(src_file, src_size);

    handle_token_init();

    while(!token_empty()) {
        int token = next_token();
        // printf("%s\n", g_token_str[token]);
        handle_token(token);
    }
    return 0;
}

