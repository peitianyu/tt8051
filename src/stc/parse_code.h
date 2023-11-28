#ifndef __PARSE_CODE_H__
#define __PARSE_CODE_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include "code.h"

enum OpCode
{
    // 数据传送类指令
    OP_MOV,
    OP_MOVC,
    OP_MOVX,
    OP_PUSH,
    OP_POP,
    OP_XCH,
    OP_XCHD,
    OP_SWAP,

    // 算数操作类指令
    OP_ADD,
    OP_ADDC,
    OP_SUBB,
    OP_INC,
    OP_DEC,
    OP_MUL,
    OP_DIV,
    OP_DA,

    // 逻辑运算指令
    OP_CLR,
    OP_CPL,
    OP_RL,
    OP_RLC,
    OP_RR,
    OP_RRC,
    OP_ANL,
    OP_ORL,
    OP_XRL,

    // 控制转移类指令
    OP_AJMP,
    OP_SJMP,
    OP_LJMP,
    OP_JMP,
    OP_JZ,
    OP_JNZ,
    OP_CJNE,
    OP_DJNZ,
    OP_ACALL,
    OP_LCALL,
    OP_RET,
    OP_RETI,
    OP_NOP,

    // 位操作指令
    OP_SETB,
    OP_JC,
    OPJNC,
    OP_JB,
    OP_JNB,
    OP_JBC,

    // 伪指令
    ORG,
    LABEL,
    EQU,
    DB,
    END
};





class ParseCode
{
public:
    ParseCode() = default;

    Code get_code(const std::string& str)
    {
        static std::size_t addr = 0;
        return splite_code(addr++, str);
    }

private:
    Code splite_code(const std::size_t addr, const std::string& str)
    {
        //  TODO: 调整顺序, 让查找速度更快
        static std::vector<std::string> instructions = {
            "MOV", "mov", "MOVC", "movc", "MOVX", "movx", "PUSH", "push", "POP", "pop", "XCH", "xch", "XCHD", "xchd", "SWAP", "swap",
            "ADD", "add", "ADDC", "addc", "SUBB", "subb", "INC", "inc", "DEC", "dec", "MUL", "mul", "DIV", "div", "DA", "da",
            "CLR", "clr", "CPL", "cpl", "RL", "rl", "RLC", "rlc", "RR", "rr", "RRC", "rrc", "ANL", "anl", "ORL", "orl", "XRL", "xrl",
            "AJMP", "ajmp", "SJMP", "sjmp", "LJMP", "ljmp", "JMP", "jmp", "JZ", "jz", "JNZ", "jnz", "CJNE", "cjne", "DJNZ", "djnz", 
            "ACALL", "acall", "LCALL", "lcall", "RET", "ret", "RETI", "reti", "NOP", "nop",
            "SETB", "setb", "JC", "jc", "JNC", "jnc", "JB", "jb", "JNB", "jnb", "JBC", "jbc",
            "ORG", "org", "LABEL", "label", "DB", "db", "EQU", "equ", "END", "end"
        };

        // 1. 判断LABEL
        if(is_label(str)) return {addr, "LABEL", get_label(str)};
        
        // 2. 根据空格分割成前后两个部分(前部分为指令, 后部分为数据)
        std::string front_str, back_str; 
        splite_front_back(str, front_str, back_str);
        for(auto& ins: instructions) 
        {
            if(front_str == ins) return {addr, front_str, remove_spaces(back_str)};
        }

        // 3. 若还没有, 则判断是否是"EQU"或者"DATA"等放在中间的特殊指令
        static std::vector<std::string> s_special_instructions = {"EQU", "equ"};
        for(auto& ins: s_special_instructions)
        {
            if(ins == back_str.substr(0, ins.size())) 
            {
                std::string special_ins, special_data;
                splite_front_back(back_str, special_ins, special_data);
                
                return {addr, ins, splice_front_back(front_str, remove_spaces(special_data))};
            }
        } 

        return {addr, "ERR", "illegal command"}; 
    }
private:
    void splite_front_back(const std::string& str, std::string& front_str, std::string& back_str)
    {
        std::size_t space_pos = str.find_first_of(' ');
        if (space_pos == std::string::npos) front_str = str;
        else                                
        {
            front_str = str.substr(0, space_pos); 
            back_str = str.substr(space_pos); 
            std::size_t back_space_pos = back_str.find_first_not_of(' ');
            if (back_space_pos != std::string::npos) back_str = back_str.substr(back_space_pos);
        }
    }

    std::string remove_spaces(const std::string& input) 
    {
        std::string output;
        for (char ch : input) if (ch != ' ') { output += ch;} 
        return output;
    }

    std::string splice_front_back(const std::string& front_str, const std::string& back_str) { return front_str + ',' + back_str; }

    bool is_label(const std::string& str) { return (str.find(':') != std::string::npos); }

    std::string get_label(const std::string& str) { return str.substr(0, str.find(':')); }
};


#endif // __PARSE_CODE_H__ 