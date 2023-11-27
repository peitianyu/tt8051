#ifndef __PARSE_CODE_H__
#define __PARSE_CODE_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>

enum Mnemonic
{
    Identifier,
    OP_ADD,
    OP_ADDC,
    OP_INC,
    OP_SUBB,
    OP_DEC,
    OP_MUL,
    OP_DIV,
    OP_DA,
    OP_ANL,
    OP_ORL,
    OP_XRL,
    OP_CLR,
    OP_CPL,
    OP_RL,
    OP_RLC,
    OP_RR,
    OP_RRC,
    OP_SWAP,
    OP_MOV,
    OP_MOVC,
    OP_MOVX,
    OP_PUSH,
    OP_POP,
    OP_XCH,
    OP_XCHD,
    OP_SETB,
    OP_ACALL,
    OP_LCALL,
    OP_RET,
    OP_RETI,
    OP_AJMP,
    OP_LJMP,
    OP_SJMP,
    OP_JZ,
    OP_JNZ,
    OP_JC,
    OP_JNC,
    OP_JB,
    OP_JNB,
    OP_JBC,
    OP_CJNE,
    OP_DJNZ,
    OP_NOP,
    REG_A,
    REG_R0,
    REG_R1,
    REG_R2,
    REG_R3,
    REG_R4,
    REG_R5,
    REG_R6,
    REG_R7,    
    REG_DPTR,
    REG_AB,
    REG_PC,
    REG_C,
    TOKEN_SEP_COMMA,
    TOKEN_OP_LEN,
    TOKEN_SEP_DOT,
    TOKEN_SEP_PLUS,
    Number,
    TOKEN_SEP_COLON,
    Directive_ORG,
    Directive_END,
    Directive_DB,
    Directive_EQU,
    TOKEN_SEP_ARE,
    TOKEN_SEP_SLASH,
    OP_JMP,
    TOKEN_SEP_DOLLAR,
    TOKEN_Symbol = Identifier,
};
struct Code
{
    std::size_t addr;
    Mnemonic mnemonic;
    std::string operands;
};

class ParseCode
{
public:
    ParseCode(const std::string& code)
    {
        static std::size_t addr = 0;
        if(is_table(code)) m_codes.push_back({addr, TOKEN_Symbol, get_colon(code)});

        addr++;
    }
private:
    bool is_table(const std::string& code)
    {
        std::size_t pos = code.find_first_not_of(':');
        return (pos != std::string::npos);
    }

    std::string get_colon(const std::string& code) 
    {
        std::size_t pos = code.find(':');
        if (pos != std::string::npos) return code.substr(0, pos); 
           
        return code;
    }
private:
    std::vector<Code> m_codes;
};


#endif // __PARSE_CODE_H__ 