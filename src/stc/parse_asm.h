#ifndef __PARSE_ASM_H__
#define __PARSE_ASM_H__

#include "code.h"
#include <string>
#include <sstream>
#include <iostream>
#include <map>
/*
特殊寄存器, 常用符号, 数据类型
R0~R7 A AB C DPTR PC @ # + / . $ , bit, u8, u16, var
*/
enum OperandEnum
{
    REG_R0 = 0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
    REG_A, REG_AB, REG_C, REG_DPTR, REG_PC, SYMBOL_AT, SYMBOL_HASH, SYMBOL_PLUS, 
    SYMBOL_SLASH, SYMBOL_DOT, SYMBOL_DOLLAR, SYMBOL_COMMA, DATA_BIT, DATA_U8, DATA_U16, DATA_VAR
};

struct OperandData
{
    std::vector<OperandEnum> operands;
    std::vector<uint8_t> datas;
};

class ParseAsm
{
public:
    ParseAsm();

    void update_vars(const Code& code);

    std::vector<AsmData> to_hex();

    AsmData asm_data(const Code& code);

    OperandData parse_operands(const std::string& operands_str);
private:
    void org_handle(const AsmData& data);

    void label_handle(const AsmData& data);

    void normal_handle(const AsmData& data);

    std::vector<std::string> split_operands(const std::string& operands_str, char ch = ',');

    OperandData split_operand(const std::string& str);
private:
    bool is_binary(const std::string& str);
    bool is_number(const std::string& str);
    bool is_hex(const std::string& str);
private:
    std::vector<uint8_t> mov2hex(const Code& code);
    std::vector<uint8_t> movc2hex(const Code& code);
    std::vector<uint8_t> movx2hex(const Code& code);
    std::vector<uint8_t> push2hex(const Code& code);
    std::vector<uint8_t> pop2hex(const Code& code);
    std::vector<uint8_t> xch2hex(const Code& code);
    std::vector<uint8_t> xchd2hex(const Code& code);
    std::vector<uint8_t> swap2hex(const Code& code);
    std::vector<uint8_t> add2hex(const Code& code);
    std::vector<uint8_t> addc2hex(const Code& code);
    std::vector<uint8_t> subb2hex(const Code& code);
    std::vector<uint8_t> inc2hex(const Code& code);
    std::vector<uint8_t> dec2hex(const Code& code);
    std::vector<uint8_t> mul2hex(const Code& code);
    std::vector<uint8_t> div2hex(const Code& code);
    std::vector<uint8_t> da2hex(const Code& code);
    std::vector<uint8_t> clr2hex(const Code& code);
    std::vector<uint8_t> cpl2hex(const Code& code);
    std::vector<uint8_t> rl2hex(const Code& code);
    std::vector<uint8_t> rlc2hex(const Code& code);
    std::vector<uint8_t> rr2hex(const Code& code);
    std::vector<uint8_t> rrc2hex(const Code& code);
    std::vector<uint8_t> anl2hex(const Code& code);
    std::vector<uint8_t> orl2hex(const Code& code);
    std::vector<uint8_t> xrl2hex(const Code& code);
    std::vector<uint8_t> ajmp2hex(const Code& code);
    std::vector<uint8_t> sjmp2hex(const Code& code);
    std::vector<uint8_t> ljmp2hex(const Code& code);
    std::vector<uint8_t> jmp2hex(const Code& code);
    std::vector<uint8_t> jz2hex(const Code& code);
    std::vector<uint8_t> jnz2hex(const Code& code);
    std::vector<uint8_t> cjne2hex(const Code& code);
    std::vector<uint8_t> djnz2hex(const Code& code);
    std::vector<uint8_t> acall2hex(const Code& code);
    std::vector<uint8_t> lcall2hex(const Code& code);
    std::vector<uint8_t> ret2hex(const Code& code);
    std::vector<uint8_t> reti2hex(const Code& code);
    std::vector<uint8_t> nop2hex(const Code& code);
    std::vector<uint8_t> setb2hex(const Code& code);
    std::vector<uint8_t> jc2hex(const Code& code);
    std::vector<uint8_t> jnc2hex(const Code& code);
    std::vector<uint8_t> jb2hex(const Code& code);
    std::vector<uint8_t> jnb2hex(const Code& code);
    std::vector<uint8_t> jbc2hex(const Code& code);
    std::vector<uint8_t> org2hex(const Code& code);
    std::vector<uint8_t> label2hex(const Code& code);
    std::vector<uint8_t> db2hex(const Code& code);
    std::vector<uint8_t> sfr2hex(const Code& code);
    std::vector<uint8_t> sbit2hex(const Code& code);
    std::vector<uint8_t> end2hex(const Code& code);
private:
    uint16_t m_curr_addr;
    bool m_end;
    std::vector<Code> m_asm_codes;
    std::vector<AsmData> m_asm_datas;
    std::map<std::string, uint16_t> m_labels;
    std::map<std::string, std::vector<uint8_t>> m_dbs;
    std::map<std::string, uint8_t> m_sfrs;
    std::map<std::string, uint8_t> m_sbits;
};

#endif // !__PARSE_ASM_H__
