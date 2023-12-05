#!/usr/local/bin/tcc -run
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"

#define uint8_t unsigned char
#define ST_FUNC static

ST_FUNC char toupper(char ch){
    return (ch >= 'a' && ch <= 'z') ? ch-32 : ch;
}

ST_FUNC char* toupper_str(char* str)
{
    char* p = str;
    while(*p != '\0'){
        *p = toupper(*p);
        p++;
    }
    return str;
}

ST_FUNC int to_digit(char* str, int len)
{
    // parse number, three kinds: hex(12H) dec(12) bin(1100B)
    int digit = 0;
    if(str[len-1] == 'H') {
        for(int i=0; i<len-1; ++i) digit = digit*16 + (str[i] >= 'A' ? str[i]-'A'+10 : str[i]-'0'); 
    } else if(str[len-1] == 'B') {
        for(int i=0; i<len-1; ++i) digit = digit*2 + (str[i]-'0'); 
    } else {
        for(int i=0; i<len; ++i) digit = digit*10 + (str[i]-'0'); 
    }

    return digit;
}

/* R0~R7 A AB C DPTR PC @ # + / . ^ $ , */
enum Token
{
    /* register */
    REG_R0 = 0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7, REG_A, REG_AB, REG_C, REG_DPTR, REG_PC, 
    /* symbol */
    SYMBOL_AT, SYMBOL_HASH, SYMBOL_PLUS, SYMBOL_SLASH, SYMBOL_DOT, SYMBOL_HAT, SYMBOL_DOLLAR, SYMBOL_COMMA, SYMBOL_COLON, SYMBOL_NUM,
    /* directives */
    DB, EQU, DATA, SFR, SBIT
};

char* g_src; /*src file string*/

char g_identifier[100]; /*identifier*/
int  g_identifier_len;  /*identifier length*/

uint8_t g_data[4]; /*data*/
int g_data_len; /*data length*/

int g_digit; /*digit*/

uint8_t* g_hex; /*out hex*/
uint8_t g_hex_len;/*out hex length*/

ST_FUNC int next_token()
{
    int token = -1;
    while (token = toupper(*g_src++))
    {
        if(token == ';') while(*g_src != '\n') ++g_src;
        else if(token == '@') return SYMBOL_AT;
        else if(token == '#') return SYMBOL_HASH;
        else if(token == '+') return SYMBOL_PLUS;
        else if(token == '/') return SYMBOL_SLASH;
        else if(token == '.') return SYMBOL_DOT;
        else if(token == '^') return SYMBOL_HAT;
        else if(token == '$') return SYMBOL_DOLLAR;
        else if(token == ',') return SYMBOL_COMMA;
        else if(token == ':') return SYMBOL_COLON;
        else if((token >= 'A' && token <= 'Z') || (token == '_')) {
            // identifier
            char* start = g_src-1;
            while((token = toupper(*g_src++)) && ((token >= 'A' && token <= 'Z') || (token >= '0' && token <= '9') || (token == '_')));
             g_identifier_len = g_src - start - 1;
            char* identifier = (char*)malloc( g_identifier_len+1);
            memset(identifier, 0,  g_identifier_len+1);
            memcpy(identifier, start,  g_identifier_len);
            identifier = toupper_str(identifier);

            if(strcmp((identifier), "R0") == 0) return REG_R0;
            else if(strcmp(identifier, "R1") == 0) return REG_R1;
            else if(strcmp(identifier, "R2") == 0) return REG_R2;
            else if(strcmp(identifier, "R3") == 0) return REG_R3;
            else if(strcmp(identifier, "R4") == 0) return REG_R4;
            else if(strcmp(identifier, "R5") == 0) return REG_R5;
            else if(strcmp(identifier, "R6") == 0) return REG_R6;
            else if(strcmp(identifier, "R7") == 0) return REG_R7;
            else if(strcmp(identifier, "A") == 0) return REG_A;
            else if(strcmp(identifier, "AB") == 0) return REG_AB;
            else if(strcmp(identifier, "C") == 0) return REG_C;
            else if(strcmp(identifier, "DPTR") == 0) return REG_DPTR;
            else if(strcmp(identifier, "PC") == 0) return REG_PC;
            else if(strcmp(identifier, "MOV") == 0) mov2hex();
            else if(strcmp(identifier, "MOVC") == 0) movc2hex();
            else if(strcmp(identifier, "MOVX") == 0) movx2hex();
            else if(strcmp(identifier, "PUSH") == 0) push2hex();
            else if(strcmp(identifier, "POP") == 0) pop2hex();
            else if(strcmp(identifier, "XCH") == 0) xch2hex();
            else if(strcmp(identifier, "XCHD") == 0) xchd2hex();
            else if(strcmp(identifier, "SWAP") == 0) swap2hex();
            else if(strcmp(identifier, "ADD") == 0) add2hex();
            else if(strcmp(identifier, "ADDC") == 0) addc2hex();
            else if(strcmp(identifier, "SUBB") == 0) subb2hex();
            else if(strcmp(identifier, "INC") == 0) inc2hex();
            else if(strcmp(identifier, "DEC") == 0) dec2hex();
            else if(strcmp(identifier, "MUL") == 0) mul2hex();
            else if(strcmp(identifier, "DIV") == 0) div2hex();
            else if(strcmp(identifier, "DA") == 0) da2hex();
            else if(strcmp(identifier, "CLR") == 0) clr2hex();
            else if(strcmp(identifier, "CPL") == 0) cpl2hex();
            else if(strcmp(identifier, "RL") == 0) rl2hex();
            else if(strcmp(identifier, "RLC") == 0) rlc2hex();
            else if(strcmp(identifier, "RR") == 0) rr2hex();
            else if(strcmp(identifier, "RRC") == 0) rrc2hex();
            else if(strcmp(identifier, "ANL") == 0) anl2hex();
            else if(strcmp(identifier, "ORL") == 0) orl2hex();
            else if(strcmp(identifier, "XRL") == 0) xrl2hex();
            else if(strcmp(identifier, "AJMP") == 0) ajmp2hex();
            else if(strcmp(identifier, "SJMP") == 0) sjmp2hex();
            else if(strcmp(identifier, "LJMP") == 0) ljmp2hex();
            else if(strcmp(identifier, "JMP") == 0) jmp2hex();
            else if(strcmp(identifier, "JZ") == 0) jz2hex();
            else if(strcmp(identifier, "JNZ") == 0) jnz2hex();
            else if(strcmp(identifier, "CJNE") == 0) cjne2hex();
            else if(strcmp(identifier, "DJNZ") == 0) djnz2hex();
            else if(strcmp(identifier, "ACALL") == 0) acall2hex();
            else if(strcmp(identifier, "LCALL") == 0) lcall2hex();
            else if(strcmp(identifier, "RET") == 0) ret2hex();
            else if(strcmp(identifier, "RETI") == 0) reti2hex();
            else if(strcmp(identifier, "NOP") == 0) nop2hex();
            else if(strcmp(identifier, "SETB") == 0) setb2hex();
            else if(strcmp(identifier, "JC") == 0) jc2hex();
            else if(strcmp(identifier, "JNC") == 0) jnc2hex();
            else if(strcmp(identifier, "JB") == 0) jb2hex();
            else if(strcmp(identifier, "JNB") == 0) jnb2hex();
            else if(strcmp(identifier, "JBC") == 0) jbc2hex();
            else if(strcmp(identifier, "ORG") == 0) org2hex();
            else if(strcmp(identifier, "DB") == 0) return DB;
            else if(strcmp(identifier, "EQU") == 0) return EQU;
            else if(strcmp(identifier, "DATA") == 0) return DATA;
            else if(strcmp(identifier, "SFR") == 0) return SFR;
            else if(strcmp(identifier, "SBIT") == 0) return SBIT;
            else if(strcmp(identifier, "END") == 0) end2hex();
            else {
                // identifier
                for(int i=0; i< g_identifier_len; ++i) g_identifier[i] = identifier[i]; 
                g_identifier[ g_identifier_len] = '\0';
    
                // printf("identifier = %s, len = %d\n", g_identifier,  g_identifier_len);
                identifier2hex();
            }
        }
        else if (token >= '0' && token <= '9') {
            // number
            char* start = g_src-1;
            while((token = toupper(*g_src++)) && ((token >= 'A' && token <= 'Z') || (token >= '0' && token <= '9')));
            g_identifier_len = g_src - start - 1;
            char* identifier = (char*)malloc( g_identifier_len+1);
            memset(identifier, 0,  g_identifier_len+1);
            memcpy(identifier, start,  g_identifier_len);            
            g_digit = to_digit(toupper_str(identifier), g_identifier_len);
            return SYMBOL_NUM;
        }
    }
    
    return token;
}

int main(int argc, char *argv[])
{
    char* src_file = "../data/token.S";

    if(argc > 1) src_file = argv[1];
    
    FILE *fp = fopen(src_file, "r");
    int src_size = 10*1024;
    g_src = (char*)malloc(src_size);
    memset(g_src, 0, src_size);

    g_hex = (uint8_t*)malloc(src_size);
    memset(g_hex, 0, src_size);

    if(!fread(g_src, 1, src_size, fp))  printf("read src file fail!\n");
    else                                fclose(fp);
    
    while(*g_src != '\0') {
        int token = next_token();
        printf("token = %d\n", token);
    }
    return 0;
}


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

INSTRUCTIONS void end2hex() {}

