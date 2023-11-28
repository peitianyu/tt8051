#ifndef __CODE2HEX_H__
#define __CODE2HEX_H__

#include "code.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
/*
R0~R7 A AB C DPTR PC 
@ # + / . $ , bit, u8, u16 
*/
enum OperandEnum
{
    REG_R0 = 0, REG_R1, REG_R2, REG_R3, REG_R4, REG_R5, REG_R6, REG_R7,
    REG_A, REG_AB, REG_C, REG_DPTR, REG_PC, SYMBOL_AT, SYMBOL_HASH, SYMBOL_PLUS, 
    SYMBOL_SLASH, SYMBOL_DOT, SYMBOL_DOLLAR, SYMBOL_COMMA, DATA_BIT, DATA_U8, DATA_U16
};

class Code2Hex
{
public:
    Code2Hex() = default;

    std::string hex(const Code& code)
    {
        if     (code.mnemonic == "MOV"  || code.mnemonic == "mov")          return mov2hex(code);
        // else if(code.mnemonic == "MOVC" || code.mnemonic == "movc")         return movc2hex(code);
        // else if(code.mnemonic == "MOVX" || code.mnemonic == "movx")         return movx2hex(code);
        // else if(code.mnemonic == "PUSH" || code.mnemonic == "push")         return push2hex(code);
        // else if(code.mnemonic == "POP"  || code.mnemonic == "pop")          return pop2hex(code);
        // else if(code.mnemonic == "XCH"  || code.mnemonic == "xch")          return xch2hex(code);
        // else if(code.mnemonic == "XCHD" || code.mnemonic == "xchd")         return xchd2hex(code);
        // else if(code.mnemonic == "SWAP" || code.mnemonic == "swap")         return swap2hex(code);
        // else if(code.mnemonic == "ADD"  || code.mnemonic == "add")          return add2hex(code);
        // else if(code.mnemonic == "ADDC" || code.mnemonic == "addc")         return addc2hex(code);
        // else if(code.mnemonic == "SUBB" || code.mnemonic == "subb")         return subb2hex(code);
        // else if(code.mnemonic == "INC"  || code.mnemonic == "inc")          return inc2hex(code);
        // else if(code.mnemonic == "DEC"  || code.mnemonic == "dec")          return dec2hex(code);
        // else if(code.mnemonic == "MUL"  || code.mnemonic == "mul")          return mul2hex(code);
        // else if(code.mnemonic == "DIV"  || code.mnemonic == "div")          return div2hex(code);
        // else if(code.mnemonic == "DA"   || code.mnemonic == "da")           return da2hex(code);
        // else if(code.mnemonic == "CLR"  || code.mnemonic == "clr")          return clr2hex(code);
        // else if(code.mnemonic == "CPL"  || code.mnemonic == "cpl")          return cpl2hex(code);
        // else if(code.mnemonic == "RL"   || code.mnemonic == "rl")           return rl2hex(code);
        // else if(code.mnemonic == "RLC"  || code.mnemonic == "rlc")          return rlc2hex(code);
        // else if(code.mnemonic == "RR"   || code.mnemonic == "rr")           return rr2hex(code);
        // else if(code.mnemonic == "RRC"  || code.mnemonic == "rrc")          return rrc2hex(code);
        // else if(code.mnemonic == "ANL"  || code.mnemonic == "anl")          return anl2hex(code);
        // else if(code.mnemonic == "ORL"  || code.mnemonic == "orl")          return orl2hex(code);
        // else if(code.mnemonic == "XRL"  || code.mnemonic == "xrl")          return xrl2hex(code);
        // else if(code.mnemonic == "AJMP" || code.mnemonic == "ajmp")         return ajmp2hex(code);
        // else if(code.mnemonic == "SJMP" || code.mnemonic == "sjmp")         return sjmp2hex(code);
        // else if(code.mnemonic == "LJMP" || code.mnemonic == "ljmp")         return ljmp2hex(code);
        // else if(code.mnemonic == "JMP"  || code.mnemonic == "jmp")          return jmp2hex(code);
        // else if(code.mnemonic == "JZ"   || code.mnemonic == "jz")           return jz2hex(code);
        // else if(code.mnemonic == "JNZ"  || code.mnemonic == "jnz")          return jnz2hex(code);
        // else if(code.mnemonic == "CJNE" || code.mnemonic == "cjne")         return cjne2hex(code);
        // else if(code.mnemonic == "DJNZ" || code.mnemonic == "djnz")         return djnz2hex(code);
        // else if(code.mnemonic == "ACALL"|| code.mnemonic == "acall")        return acall2hex(code);
        // else if(code.mnemonic == "LCALL"|| code.mnemonic == "lcall")        return lcall2hex(code);
        // else if(code.mnemonic == "RET"  || code.mnemonic == "ret")          return ret2hex(code);
        // else if(code.mnemonic == "RETI" || code.mnemonic == "reti")         return reti2hex(code);
        // else if(code.mnemonic == "NOP"  || code.mnemonic == "nop")          return nop2hex(code);
        // else if(code.mnemonic == "SETB" || code.mnemonic == "setb")         return setb2hex(code);
        // else if(code.mnemonic == "JC"   || code.mnemonic == "jc")           return jc2hex(code);
        // else if(code.mnemonic == "JNC"  || code.mnemonic == "jnc")          return jnc2hex(code);
        // else if(code.mnemonic == "JB"   || code.mnemonic == "jb")           return jb2hex(code);
        // else if(code.mnemonic == "JNB"  || code.mnemonic == "jnb")          return jnb2hex(code);
        // else if(code.mnemonic == "JBC"  || code.mnemonic == "jbc")          return jbc2hex(code);
        // else if(code.mnemonic == "ORG"  || code.mnemonic == "org")          return org2hex(code);
        // else if(code.mnemonic == "LABEL"|| code.mnemonic == "label")        return label2hex(code);
        // else if(code.mnemonic == "DB"   || code.mnemonic == "db")           return db2hex(code);
        // else if(code.mnemonic == "EQU"  || code.mnemonic == "equ")          return equ2hex(code);
        // else if(code.mnemonic == "END"  || code.mnemonic == "end")          return end2hex(code);
        else return "";
    }

    std::vector<OperandEnum> parse_operands(const std::string& operands_str)
    {
        if(operands_str.empty()) return std::vector<OperandEnum>();

        std::vector<std::string> split_operand_str = split_operands(operands_str);
        std::vector<OperandEnum> operand_list;

        if(split_operand_str.size() == 0) return std::vector<OperandEnum>();
        else if(split_operand_str.size() == 1) return split_operand(split_operand_str[0]);

        std::vector<OperandEnum> so_list = split_operand(split_operand_str[0]);
        operand_list.insert(operand_list.end(), so_list.begin(), so_list.end());
        for(uint i = 1; i < split_operand_str.size(); i++){
            operand_list.push_back(SYMBOL_COMMA);
            so_list = split_operand(split_operand_str[i]);
            operand_list.insert(operand_list.end(), so_list.begin(), so_list.end());
        }

        return operand_list;
    }

    std::vector<std::string> split_operands(const std::string& operands_str, char ch = ',')
    {
        std::stringstream ss(operands_str);
        std::vector<std::string> segments;
        std::string segment;

        while (std::getline(ss, segment, ch)) {
            segments.push_back(segment);
        }

        return segments;
    }

    std::vector<OperandEnum> split_operand(const std::string& str)
    {
        // @ # + / . $
        std::vector<OperandEnum> operand_list;
        std::string operand_str = str;
        
        // 1. 包含'+', 只有两种情况: @A+DPTR与@A+PC
        std::size_t plus_pos = operand_str.find('+');
        if(plus_pos != std::string::npos){
            operand_list.push_back(SYMBOL_AT);
            operand_list.push_back(REG_A);
            operand_list.push_back(SYMBOL_PLUS);
            std::string plus_str = operand_str.substr(plus_pos);
            if(plus_str == "PC" || operand_str == "pc")     operand_list.push_back(REG_PC);
            else if(plus_str == "DPTR" || operand_str == "dptr") operand_list.push_back(REG_DPTR);

            return operand_list;
        }

        // 2. 预处理一下, 将@ # / $, 提取出来
        if(operand_str.front() == '@'){
            operand_str.erase(operand_str.begin());
            operand_list.push_back(SYMBOL_AT);
        }else if(operand_str.front() == '#'){
            operand_str.erase(operand_str.begin());
            operand_list.push_back(SYMBOL_HASH);
        }else if(operand_str.front() == '/'){
            operand_str.erase(operand_str.begin());
            operand_list.push_back(SYMBOL_SLASH);
        }else if(operand_str.front() == '$'){
            operand_str.erase(operand_str.begin());
            operand_list.push_back(SYMBOL_DOLLAR);
        }

        // 3. 如果有'.', 一般针对bit数据, 如P0.1这种
        // FIXME: 注意这里数据/地址的处理
        if(operand_str.find('.') != std::string::npos){
            operand_list.push_back(DATA_BIT);
        }

        // 4. 若剩下是寄存器, 则直接返回operand_list
        if(operand_str == "A" || operand_str == "a")            operand_list.push_back(REG_A);
        else if(operand_str == "AB" || operand_str == "ab")     operand_list.push_back(REG_AB);
        else if(operand_str == "C" || operand_str == "c")       operand_list.push_back(REG_C);
        else if(operand_str == "PC" || operand_str == "pc")     operand_list.push_back(REG_PC);
        else if(operand_str == "DPTR" || operand_str == "dptr") operand_list.push_back(REG_DPTR);
        else if(operand_str == "R0" || operand_str == "r0")     operand_list.push_back(REG_R0);
        else if(operand_str == "R1" || operand_str == "r1")     operand_list.push_back(REG_R1);
        else if(operand_str == "R2" || operand_str == "r2")     operand_list.push_back(REG_R2);
        else if(operand_str == "R3" || operand_str == "r3")     operand_list.push_back(REG_R3);
        else if(operand_str == "R4" || operand_str == "r4")     operand_list.push_back(REG_R4);
        else if(operand_str == "R5" || operand_str == "r5")     operand_list.push_back(REG_R5);
        else if(operand_str == "R6" || operand_str == "r6")     operand_list.push_back(REG_R6);
        else if(operand_str == "R7" || operand_str == "r7")     operand_list.push_back(REG_R7);
        else{
            // 5. 若是数据, 则判断是u8, 还是u16
            uint16_t val = 0;
            if(operand_str.back() == 'B' || operand_str.back() == 'b'){
                val = std::stoi(operand_str, nullptr, 2);

                if(operand_str.size()>9)   operand_list.push_back(DATA_U16);
                else                       operand_list.push_back(DATA_U8);

            }else if(operand_str.back() == 'H' || operand_str.back() == 'h'){
                val = std::stoi(operand_str, nullptr, 16);
                if(operand_str.size()>3)   operand_list.push_back(DATA_U16);
                else                       operand_list.push_back(DATA_U8);
            }else{
                val = std::stoi(operand_str, nullptr, 10);
                if(val > 255)              operand_list.push_back(DATA_U16);
                else                       operand_list.push_back(DATA_U8);
            }
        }

        return operand_list;
    }

    bool is_num(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
private:
    std::string mov2hex(const Code& code)
    {
        return "";
    }

    // std::string movc2hex(const Code& code)
    // {

    // }

    // std::string movx2hex(const Code& code)
    // {

    // }

    // std::string push2hex(const Code& code)
    // {

    // }

    // std::string pop2hex(const Code& code)
    // {

    // }

    // std::string xch2hex(const Code& code)
    // {

    // }

    // std::string xchd2hex(const Code& code)
    // {

    // }

    // std::string swap2hex(const Code& code)
    // {

    // }

    // std::string add2hex(const Code& code)
    // {

    // }

    // std::string addc2hex(const Code& code)
    // {

    // }

    // std::string subb2hex(const Code& code)
    // {

    // }

    // std::string inc2hex(const Code& code)
    // {

    // }

    // std::string dec2hex(const Code& code)
    // {

    // }

    // std::string mul2hex(const Code& code)
    // {

    // }

    // std::string div2hex(const Code& code)
    // {

    // }

    // std::string da2hex(const Code& code)
    // {

    // }

    // std::string clr2hex(const Code& code)
    // {

    // }

    // std::string cpl2hex(const Code& code)
    // {

    // }

    // std::string rl2hex(const Code& code)
    // {

    // }

    // std::string rlc2hex(const Code& code)
    // {

    // }

    // std::string rr2hex(const Code& code)
    // {

    // }

    // std::string rrc2hex(const Code& code)
    // {

    // }

    // std::string anl2hex(const Code& code)
    // {

    // }

    // std::string orl2hex(const Code& code)
    // {

    // }

    // std::string xrl2hex(const Code& code)
    // {

    // }

    // std::string ajmp2hex(const Code& code)
    // {

    // }

    // std::string sjmp2hex(const Code& code)
    // {

    // }

    // std::string ljmp2hex(const Code& code)
    // {

    // }

    // std::string jmp2hex(const Code& code)
    // {

    // }

    // std::string jz2hex(const Code& code)
    // {

    // }

    // std::string jnz2hex(const Code& code)
    // {

    // }

    // std::string cjne2hex(const Code& code)
    // {

    // }

    // std::string djnz2hex(const Code& code)
    // {

    // }

    // std::string acall2hex(const Code& code)
    // {

    // }

    // std::string lcall2hex(const Code& code)
    // {

    // }

    // std::string ret2hex(const Code& code)
    // {

    // }

    // std::string reti2hex(const Code& code)
    // {

    // }

    // std::string nop2hex(const Code& code)
    // {

    // }

    // std::string setb2hex(const Code& code)
    // {

    // }

    // std::string jc2hex(const Code& code)
    // {

    // }

    // std::string jnc2hex(const Code& code)
    // {

    // }

    // std::string jb2hex(const Code& code)
    // {

    // }

    // std::string jnb2hex(const Code& code)
    // {

    // }

    // std::string jbc2hex(const Code& code)
    // {

    // }

    // std::string org2hex(const Code& code)
    // {

    // }

    // std::string label2hex(const Code& code)
    // {

    // }

    // std::string db2hex(const Code& code)
    // {

    // }

    // std::string equ2hex(const Code& code)
    // {

    // }

    // std::string end2hex(const Code& code)
    // {

    // }
};

#endif // !__CODE2HEX_H__
