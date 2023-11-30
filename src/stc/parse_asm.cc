#include "parse_asm.h"

ParseAsm::ParseAsm() : m_curr_addr(0), m_end(false) {}

void ParseAsm::update_vars(const Code& code)
{
    if(code.mnemonic == "ERR") std::cout << "ERROR: illegal command" << std::endl, abort();
    if(m_end) return;

    if(code.mnemonic == "DB"   || code.mnemonic == "db")                asm_data(code);
    else if(code.mnemonic == "SFR"  || code.mnemonic == "sfr")          asm_data(code);
    else if(code.mnemonic == "SBIT" || code.mnemonic == "sbit")         asm_data(code);
    else if(code.mnemonic == "END"  || code.mnemonic == "end")          asm_data(code);
    else                                                                m_asm_codes.push_back(code);
}

std::vector<AsmData> ParseAsm::to_hex()
{
    //update addr
    std::vector<Code> codes;
    for(Code& code : m_asm_codes)
    {
        if(code.mnemonic == "ORG" || code.mnemonic == "org")            org_handle(asm_data(code));
        else if(code.mnemonic == "LABEL" || code.mnemonic == "label")   label_handle(asm_data(code));
        else{
            AsmData data = asm_data(code);
            normal_handle(data);
            codes.push_back(code);
        }
    }

    // update hex
    std::vector<AsmData> hex_datas;
    for(uint i = 0; i < codes.size(); i++)
    {
        std::cout << codes[i].line << ": [ " << codes[i].mnemonic << " ] " << codes[i].operands << std::endl;   
        AsmData data = asm_data(codes[i]);

        if(data.operands.empty()) {
            std::cout << "[ERROR]: " << codes[i].mnemonic << " "; 
            for(auto p : codes[i].operands) std::cout << p << " ";
            std::cout << std::endl;
            abort();
        }

        hex_datas.push_back((AsmData){data.mnemonic, data.operands, (uint16_t)m_asm_datas[i].addr});
    }

    return hex_datas;
}

AsmData ParseAsm::asm_data(const Code& code)
{
    if     (code.mnemonic == "MOV"  || code.mnemonic == "mov")          return {"MOV", mov2hex(code), };
    else if(code.mnemonic == "MOVC" || code.mnemonic == "movc")         return {"MOVC", movc2hex(code)};
    else if(code.mnemonic == "MOVX" || code.mnemonic == "movx")         return {"MOVX", movx2hex(code)};
    else if(code.mnemonic == "PUSH" || code.mnemonic == "push")         return {"PUSH", push2hex(code)};
    else if(code.mnemonic == "POP"  || code.mnemonic == "pop")          return {"POP", pop2hex(code)};
    else if(code.mnemonic == "XCH"  || code.mnemonic == "xch")          return {"XCH", xch2hex(code)};
    else if(code.mnemonic == "XCHD" || code.mnemonic == "xchd")         return {"XCHD", xchd2hex(code)};
    else if(code.mnemonic == "SWAP" || code.mnemonic == "swap")         return {"SWAP", swap2hex(code)};
    else if(code.mnemonic == "ADD"  || code.mnemonic == "add")          return {"ADD", add2hex(code)};
    else if(code.mnemonic == "ADDC" || code.mnemonic == "addc")         return {"ADDC", addc2hex(code)};
    else if(code.mnemonic == "SUBB" || code.mnemonic == "subb")         return {"SUBB", subb2hex(code)};
    else if(code.mnemonic == "INC"  || code.mnemonic == "inc")          return {"INC", inc2hex(code)};
    else if(code.mnemonic == "DEC"  || code.mnemonic == "dec")          return {"DEC", dec2hex(code)};
    else if(code.mnemonic == "MUL"  || code.mnemonic == "mul")          return {"MUL", mul2hex(code)};
    else if(code.mnemonic == "DIV"  || code.mnemonic == "div")          return {"DIV", div2hex(code)};
    else if(code.mnemonic == "DA"   || code.mnemonic == "da")           return {"DA", da2hex(code)};
    else if(code.mnemonic == "CLR"  || code.mnemonic == "clr")          return {"CLR", clr2hex(code)};
    else if(code.mnemonic == "CPL"  || code.mnemonic == "cpl")          return {"CPL", cpl2hex(code)};
    else if(code.mnemonic == "RL"   || code.mnemonic == "rl")           return {"RL", rl2hex(code)};
    else if(code.mnemonic == "RLC"  || code.mnemonic == "rlc")          return {"RLC", rlc2hex(code)};
    else if(code.mnemonic == "RR"   || code.mnemonic == "rr")           return {"RR", rr2hex(code)};
    else if(code.mnemonic == "RRC"  || code.mnemonic == "rrc")          return {"RRC", rrc2hex(code)};
    else if(code.mnemonic == "ANL"  || code.mnemonic == "anl")          return {"ANL", anl2hex(code)};
    else if(code.mnemonic == "ORL"  || code.mnemonic == "orl")          return {"ORL", orl2hex(code)};
    else if(code.mnemonic == "XRL"  || code.mnemonic == "xrl")          return {"XRL", xrl2hex(code)};
    else if(code.mnemonic == "AJMP" || code.mnemonic == "ajmp")         return {"AJMP", ajmp2hex(code)};
    else if(code.mnemonic == "SJMP" || code.mnemonic == "sjmp")         return {"SJMP", sjmp2hex(code)};
    else if(code.mnemonic == "LJMP" || code.mnemonic == "ljmp")         return {"LJMP", ljmp2hex(code)};
    else if(code.mnemonic == "JMP"  || code.mnemonic == "jmp")          return {"JMP", jmp2hex(code)};
    else if(code.mnemonic == "JZ"   || code.mnemonic == "jz")           return {"JZ", jz2hex(code)};
    else if(code.mnemonic == "JNZ"  || code.mnemonic == "jnz")          return {"JNZ", jnz2hex(code)};
    else if(code.mnemonic == "CJNE" || code.mnemonic == "cjne")         return {"CJNE", cjne2hex(code)};
    else if(code.mnemonic == "DJNZ" || code.mnemonic == "djnz")         return {"DJNZ", djnz2hex(code)};
    else if(code.mnemonic == "ACALL"|| code.mnemonic == "acall")        return {"ACALL", acall2hex(code)};
    else if(code.mnemonic == "LCALL"|| code.mnemonic == "lcall")        return {"LCALL", lcall2hex(code)};
    else if(code.mnemonic == "RET"  || code.mnemonic == "ret")          return {"RET", ret2hex(code)};
    else if(code.mnemonic == "RETI" || code.mnemonic == "reti")         return {"RETI", reti2hex(code)};
    else if(code.mnemonic == "NOP"  || code.mnemonic == "nop")          return {"NOP", nop2hex(code)};
    else if(code.mnemonic == "SETB" || code.mnemonic == "setb")         return {"SETB", setb2hex(code)};
    else if(code.mnemonic == "JC"   || code.mnemonic == "jc")           return {"JC", jc2hex(code)};
    else if(code.mnemonic == "JNC"  || code.mnemonic == "jnc")          return {"JNC", jnc2hex(code)};
    else if(code.mnemonic == "JB"   || code.mnemonic == "jb")           return {"JB", jb2hex(code)};
    else if(code.mnemonic == "JNB"  || code.mnemonic == "jnb")          return {"JNB", jnb2hex(code)};
    else if(code.mnemonic == "JBC"  || code.mnemonic == "jbc")          return {"JBC", jbc2hex(code)};
    else if(code.mnemonic == "ORG"  || code.mnemonic == "org")          return {"ORG", org2hex(code)};
    else if(code.mnemonic == "LABEL"|| code.mnemonic == "label")        return {"LABEL", label2hex(code)};
    else if(code.mnemonic == "DB"   || code.mnemonic == "db")           return {"DB", db2hex(code)};
    else if(code.mnemonic == "SFR"  || code.mnemonic == "sfr")          return {"SFR", sfr2hex(code)};
    else if(code.mnemonic == "SBIT"  || code.mnemonic == "sbit")        return {"SBIT", sbit2hex(code)};
    else if(code.mnemonic == "END"  || code.mnemonic == "end")          return {"END", end2hex(code)};
    else return {};
}

OperandData ParseAsm::parse_operands(const std::string& operands_str)
{
    if(operands_str.empty()) return OperandData();

    std::vector<std::string> split_operand_str = split_operands(operands_str);
    OperandData operand_data;   

    if(split_operand_str.size() == 0) return OperandData();
    else if(split_operand_str.size() == 1) return split_operand(split_operand_str[0]);

    OperandData sd = split_operand(split_operand_str[0]);
    operand_data.operands.insert(operand_data.operands.end(), sd.operands.begin(), sd.operands.end());
    operand_data.datas.insert(operand_data.datas.end(), sd.datas.begin(), sd.datas.end());
    for(uint i = 1; i < split_operand_str.size(); i++){
        operand_data.operands.push_back(SYMBOL_COMMA);
        sd = split_operand(split_operand_str[i]);
        operand_data.operands.insert(operand_data.operands.end(), sd.operands.begin(), sd.operands.end());
        operand_data.datas.insert(operand_data.datas.end(), sd.datas.begin(), sd.datas.end());
    }

    return operand_data;
}

void ParseAsm::org_handle(const AsmData& data) { m_curr_addr = data.operands[0] * 256 + data.operands[1];}

void ParseAsm::label_handle(const AsmData& data) 
{
    std::string str;
    for(uint i = 2; i < data.operands.size(); i++) str.push_back(data.operands[i]);
    m_labels[str] = m_curr_addr;
}

void ParseAsm::normal_handle(const AsmData& data)
{
    AsmData ad = data;
    ad.addr = m_curr_addr;
    m_asm_datas.push_back(ad);
    m_curr_addr += data.operands.size();
}

std::vector<std::string> ParseAsm::split_operands(const std::string& operands_str, char ch)
{
    std::stringstream ss(operands_str);
    std::vector<std::string> segments;
    std::string segment;

    while (std::getline(ss, segment, ch)) {
        segments.push_back(segment);
    }

    return segments;
}

OperandData ParseAsm::split_operand(const std::string& str)
{
    OperandData operand_data;    
    std::string operand_str = str;
    
    // 1. 包含'+', 只有两种情况: @A+DPTR与@A+PC
    std::size_t plus_pos = operand_str.find('+');
    if(plus_pos != std::string::npos){
        operand_data.operands.push_back(SYMBOL_AT);
        operand_data.operands.push_back(REG_A);
        operand_data.operands.push_back(SYMBOL_PLUS);
        std::string plus_str = operand_str.substr(plus_pos+1);
        if(plus_str == "PC" || operand_str == "pc")     operand_data.operands.push_back(REG_PC);
        else if(plus_str == "DPTR" || operand_str == "dptr") operand_data.operands.push_back(REG_DPTR);

        return operand_data;
    }

    // 2. 预处理一下, 将@ # / $, 提取出来
    if(operand_str.front() == '@'){
        operand_str.erase(operand_str.begin());
        operand_data.operands.push_back(SYMBOL_AT);
    }else if(operand_str.front() == '#'){
        operand_str.erase(operand_str.begin());
        operand_data.operands.push_back(SYMBOL_HASH);
    }else if(operand_str.front() == '/'){
        operand_str.erase(operand_str.begin());
        operand_data.operands.push_back(SYMBOL_SLASH);
    }else if(operand_str.front() == '$'){
        operand_str.erase(operand_str.begin());
        operand_data.operands.push_back(SYMBOL_DOLLAR);
    }

    // 3. 如果有'.', 一般针对bit数据, 如P0.1这种
    std::size_t bit_pos = operand_str.find('.');
    if(bit_pos != std::string::npos) 
    {
        std::string sfr_sr = operand_str.substr(0, bit_pos);
        auto iter = m_sfrs.find(sfr_sr);
        if(iter != m_sfrs.end()){
            operand_data.operands.push_back(DATA_U8); 
            operand_data.datas.push_back(m_sfrs[sfr_sr]^(operand_str.back()-'0'));
        } 
        else operand_data.operands.push_back(DATA_VAR);
        return operand_data;
    }
    
    // 4. 若剩下是寄存器, 则直接返回operand_list
    if(operand_str == "A" || operand_str == "a")            operand_data.operands.push_back(REG_A);
    else if(operand_str == "AB" || operand_str == "ab")     operand_data.operands.push_back(REG_AB);
    else if(operand_str == "C" || operand_str == "c")       operand_data.operands.push_back(REG_C);
    else if(operand_str == "PC" || operand_str == "pc")     operand_data.operands.push_back(REG_PC);
    else if(operand_str == "DPTR" || operand_str == "dptr") operand_data.operands.push_back(REG_DPTR);
    else if(operand_str == "R0" || operand_str == "r0")     operand_data.operands.push_back(REG_R0);
    else if(operand_str == "R1" || operand_str == "r1")     operand_data.operands.push_back(REG_R1);
    else if(operand_str == "R2" || operand_str == "r2")     operand_data.operands.push_back(REG_R2);
    else if(operand_str == "R3" || operand_str == "r3")     operand_data.operands.push_back(REG_R3);
    else if(operand_str == "R4" || operand_str == "r4")     operand_data.operands.push_back(REG_R4);
    else if(operand_str == "R5" || operand_str == "r5")     operand_data.operands.push_back(REG_R5);
    else if(operand_str == "R6" || operand_str == "r6")     operand_data.operands.push_back(REG_R6);
    else if(operand_str == "R7" || operand_str == "r7")     operand_data.operands.push_back(REG_R7);
    else{
        // 5. 若是数据, 则判断是u8, 还是u16
        uint16_t val = 0;
        if((operand_str.back() == 'B' || operand_str.back() == 'b') && is_binary(operand_str)){
            val = std::stoi(operand_str, nullptr, 2);
            if(operand_str.size()>9)        operand_data.operands.push_back(DATA_U16), 
                                            operand_data.datas.push_back(val >> 8), operand_data.datas.push_back(val & 0xFF); 
            else                            operand_data.operands.push_back(DATA_U8), operand_data.datas.push_back(val);
        }else if((operand_str.back() == 'H' || operand_str.back() == 'h') && is_hex(operand_str)) {
            val = std::stoi(operand_str, nullptr, 16);
            if(operand_str.size()>3)        operand_data.operands.push_back(DATA_U16), 
                                            operand_data.datas.push_back(val >> 8), operand_data.datas.push_back(val & 0xFF); 
            else                            operand_data.operands.push_back(DATA_U8), operand_data.datas.push_back(val);
        }else if((operand_str[0] == '0' && (operand_str[1] == 'x' || operand_str[1] == 'X')) && operand_str.size() > 2){
            val = std::stoi(operand_str.substr(2), nullptr, 16);
            if(operand_str.size()>4)        operand_data.operands.push_back(DATA_U16), 
                                            operand_data.datas.push_back(val >> 8), operand_data.datas.push_back(val & 0xFF); 
            else                            operand_data.operands.push_back(DATA_U8), operand_data.datas.push_back(val);
        }else{
            // 6. 可能为变量, 也可能为十进制数
            if(!is_number(operand_str)){
                auto sfr_iter = m_sfrs.find(operand_str);
                auto label_iter = m_labels.find(operand_str);
                if(sfr_iter != m_sfrs.end()) operand_data.operands.push_back(DATA_U8), operand_data.datas.push_back(m_sfrs[operand_str]);
                else if(label_iter != m_labels.end()) 
                {
                    operand_data.operands.push_back(DATA_U16), 
                    operand_data.datas.push_back(m_labels[operand_str] >> 8), 
                    operand_data.datas.push_back(m_labels[operand_str] & 0xFF);  

                    // std::cout << operand_str << " : " << std::hex <<  (int)(m_labels[operand_str] >> 8) 
                    //             << " " << (int)(m_labels[operand_str] & 0xFF) <<std::endl;
                }
                else {
                    operand_data.operands.push_back(DATA_VAR);
                    operand_data.datas.push_back(operand_str.size());
                    operand_data.datas.insert(operand_data.datas.end(), operand_str.begin(), operand_str.end());
                }
            }     
            else{
                val = std::stoi(operand_str, nullptr, 10);
                if(val > 255)               operand_data.operands.push_back(DATA_U16), 
                                            operand_data.datas.push_back(val >> 8), operand_data.datas.push_back(val & 0xFF);                     
                else                        operand_data.operands.push_back(DATA_U8), operand_data.datas.push_back(val);
            }
        }
    }

    return operand_data;
}

bool ParseAsm::is_binary(const std::string& str)
{
    if(str.size()<2) return false;
    for(uint i = 0; i < str.size()-1; i++) if(str[i] < '0' || str[i] > '1') return false;
    return true;
}
bool ParseAsm::is_number(const std::string& str)
{
    for(auto c:str) if(c < '0' || c > '9') return false;
    return true;
}

bool ParseAsm::is_hex(const std::string& str)
{
    if(str.size()<2) return false;
    for(uint i = 0; i < str.size()-1; i++) if(!std::isxdigit(str[i])) return false;
    return true;
}

std::vector<uint8_t> ParseAsm::mov2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0xE8 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xE5), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0xE6 + parse_operand_list[3]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x74), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] >= 0 && parse_operand_list[0] < 8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == REG_A) operand_list.push_back(0xF8 + parse_operand_list[0]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xA8 + parse_operand_list[0]), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x78 + parse_operand_list[0]), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == REG_A) operand_list.push_back(0xF5), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x88 + parse_operand_list[2]), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x85), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x86 + parse_operand_list[3]), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x75), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
        else if(parse_operand_list[2] == REG_C) operand_list.push_back(0x92), operand_list.push_back(datas[0]);
    }
    else if (parse_operand_list[0] == SYMBOL_AT && (parse_operand_list[1] == 0 || parse_operand_list[1] == 1))
    {
        if(parse_operand_list[2] != SYMBOL_COMMA) return {};

        if(parse_operand_list[3] == REG_A) operand_list.push_back(0xF6 + parse_operand_list[1]);
        else if(parse_operand_list[3] == DATA_U8) operand_list.push_back(0xA6 + parse_operand_list[1]), operand_list.push_back(datas[0]);
        else if(parse_operand_list[3] == SYMBOL_HASH && parse_operand_list[4] == DATA_U8) operand_list.push_back(0x76 + parse_operand_list[1]), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == REG_C)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xA2), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == REG_DPTR)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U16) operand_list.push_back(0x90), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::movc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};
        if(parse_operand_list[2] != SYMBOL_AT) return {};
        if(parse_operand_list[3] != REG_A) return {};
        if(parse_operand_list[4] != SYMBOL_PLUS) return {};

        if(parse_operand_list[5] == REG_DPTR)       operand_list.push_back(0x93);
        else if(parse_operand_list[5] == REG_PC)    operand_list.push_back(0x83);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::movx2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};
        if(parse_operand_list[2] != SYMBOL_AT) return {};

        if(parse_operand_list[3] == 0 || parse_operand_list[3] == 1)        operand_list.push_back(0xE2+parse_operand_list[3]);
        else if(parse_operand_list[3] == REG_DPTR)                          operand_list.push_back(0xE0);
    }
    else if(parse_operand_list[0] == SYMBOL_AT)
    {
        if((parse_operand_list[1] == 0 || parse_operand_list[1] == 1)&&parse_operand_list[2] == SYMBOL_COMMA&&parse_operand_list[3] == REG_A)
            operand_list.push_back(0xF2+parse_operand_list[1]);
        else if(parse_operand_list[1] == REG_DPTR&&parse_operand_list[2] == SYMBOL_COMMA&&parse_operand_list[3] == REG_A)
            operand_list.push_back(0xF0);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::push2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0xC0), operand_list.push_back(datas[0]);       

    return operand_list;
}

std::vector<uint8_t> ParseAsm::pop2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0xD0), operand_list.push_back(datas[0]);

    return operand_list;
}

std::vector<uint8_t> ParseAsm::xch2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0xC8 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xC5), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0xC6 + parse_operand_list[3]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::xchd2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0xD6 + parse_operand_list[3]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::swap2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0xC4);

    return operand_list;
}

std::vector<uint8_t> ParseAsm::add2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x28 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x25), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x26 + parse_operand_list[3]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x24), operand_list.push_back(datas[0]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::addc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x38 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x35), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x36 + parse_operand_list[3]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x34), operand_list.push_back(datas[0]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::subb2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x98 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x95), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x96 + parse_operand_list[3]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x94), operand_list.push_back(datas[0]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::inc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)  operand_list.push_back(0x04);
    else if(parse_operand_list[0] >= 0 && parse_operand_list[0] < 8) operand_list.push_back(0x08 + parse_operand_list[0]);
    else if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x05), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == SYMBOL_AT && (parse_operand_list[1] == 0 || parse_operand_list[1] == 1)) 
        operand_list.push_back(0x06 + parse_operand_list[1]);
    else if(parse_operand_list[0] == REG_DPTR) operand_list.push_back(0xA3);

    return operand_list;
}

std::vector<uint8_t> ParseAsm::dec2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)  operand_list.push_back(0x14);
    else if(parse_operand_list[0] >= 0 && parse_operand_list[0] < 8) operand_list.push_back(0x18 + parse_operand_list[0]);
    else if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x15), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == SYMBOL_AT && (parse_operand_list[1] == 0 || parse_operand_list[1] == 1)) operand_list.push_back(0x16 + parse_operand_list[1]);        

    return operand_list;
}

std::vector<uint8_t> ParseAsm::mul2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_AB) operand_list.push_back(0xA4);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::div2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_AB)  operand_list.push_back(0x84);

    return operand_list;
}

std::vector<uint8_t> ParseAsm::da2hex(const Code& code)
{   
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0xD4);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::clr2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0xE4);
    else if(parse_operand_list[0] == REG_C) operand_list.push_back(0xC3);
    else if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0xC2), operand_list.push_back(datas[0]);
                                                
    return operand_list;
}

std::vector<uint8_t> ParseAsm::cpl2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0xF4);
    else if(parse_operand_list[0] == REG_C) operand_list.push_back(0xB3);
    else if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0xB2), operand_list.push_back(datas[0]);
                                                
    return operand_list;
}

std::vector<uint8_t> ParseAsm::rl2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0x23);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::rlc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0x33);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::rr2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0x03);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::rrc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A) operand_list.push_back(0x13);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::anl2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x58 + parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x55), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x56 + parse_operand_list[3]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x54), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == REG_A) operand_list.push_back(0x52), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x53), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }
    else if(parse_operand_list[0] == REG_C)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x82), operand_list.push_back(datas[0]);
        if(parse_operand_list[2] == SYMBOL_SLASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0xB0), operand_list.push_back(datas[0]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::orl2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};
        
        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x48+parse_operand_list[2]);
        else if(parse_operand_list[2] == DATA_U8)                   operand_list.push_back(0x45), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == REG_R0 || parse_operand_list[3] == REG_R1)) operand_list.push_back(0x46+parse_operand_list[3]);                                                          
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x44), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == REG_A)  operand_list.push_back(0x42), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x43), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }
    else if(parse_operand_list[0] == REG_C)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8)  operand_list.push_back(0x72), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_SLASH && parse_operand_list[3] == DATA_U8)  operand_list.push_back(0xA0), operand_list.push_back(datas[0]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::xrl2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] >= 0 && parse_operand_list[2] < 8) operand_list.push_back(0x68 + parse_operand_list[2]);
        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x65), operand_list.push_back(datas[0]);
        if(parse_operand_list[2] == SYMBOL_AT && (parse_operand_list[3] == 0 || parse_operand_list[3] == 1)) operand_list.push_back(0x66 + parse_operand_list[3]);
        if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x64), operand_list.push_back(datas[0]);
    }
    else if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == REG_A)  operand_list.push_back(0x62), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8) operand_list.push_back(0x63), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::ajmp2hex(const Code& code)
{   
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(1), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(datas[0]+1), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR) operand_list.push_back(0), operand_list.push_back(0);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::sjmp2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x80), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x80), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::ljmp2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x02), operand_list.push_back(0), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x02), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::jmp2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> operand_list;

    if(parse_operand_list[0] != SYMBOL_AT) return {};
    if(parse_operand_list[1] != REG_A) return {};
    if(parse_operand_list[2] != SYMBOL_PLUS) return {};
    if(parse_operand_list[3] == REG_DPTR)       operand_list.push_back(0x73);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::jz2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x60), operand_list.push_back(datas[0]); 
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x60), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::jnz2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x70), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x70), operand_list.push_back(datas[1]); 
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::cjne2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_A)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8 && parse_operand_list[3] == SYMBOL_COMMA && parse_operand_list[4] == DATA_U8) 
            operand_list.push_back(0xB5), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
        else if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8 && parse_operand_list[4] == SYMBOL_COMMA && parse_operand_list[5] == DATA_U8) 
            operand_list.push_back(0xB4), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }
    else if(parse_operand_list[0] >= 0 && parse_operand_list[0] < 8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == SYMBOL_HASH && parse_operand_list[3] == DATA_U8 && parse_operand_list[4] == SYMBOL_COMMA) 
        {
            if(parse_operand_list[5] == DATA_U8) operand_list.push_back(0xB8+parse_operand_list[0]), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
            else if(parse_operand_list[5] == DATA_U16) operand_list.push_back(0xB8+parse_operand_list[0]), operand_list.push_back(datas[0]), operand_list.push_back(datas[2]);
        }
            
    }
    else if(parse_operand_list[0] == SYMBOL_AT && (parse_operand_list[1] == 0 || parse_operand_list[1] == 1))
    {
        if(parse_operand_list[2] != SYMBOL_COMMA) return {};

        if(parse_operand_list[3] == SYMBOL_HASH && parse_operand_list[4] == DATA_U8 && parse_operand_list[5] == SYMBOL_COMMA && parse_operand_list[6] == DATA_U8) 
            operand_list.push_back(0xB6+parse_operand_list[1]), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::djnz2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] >= 0 && parse_operand_list[0] < 8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xD8+parse_operand_list[0]), operand_list.push_back(datas[0]);
        else if(parse_operand_list[2] == DATA_U16) operand_list.push_back(0xD8+parse_operand_list[0]), operand_list.push_back(datas[1]);
    }
    else if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0xD5), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]); 
        else if(parse_operand_list[2] == DATA_U16) operand_list.push_back(0xD5+parse_operand_list[0]), operand_list.push_back(datas[2]);
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::acall2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;
    std::vector<uint8_t> operand_list;

    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x11), operand_list.push_back(datas[0]);
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(datas[0]+0x11), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::lcall2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x12), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::ret2hex(const Code& code)
{
    return {0x22};
}

std::vector<uint8_t> ParseAsm::reti2hex(const Code& code)
{
    return {0x32};
}

std::vector<uint8_t> ParseAsm::nop2hex(const Code& code)
{
    return {0x00};
}

std::vector<uint8_t> ParseAsm::setb2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == REG_C)  operand_list.push_back(0xD3);
    else if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0xD2), operand_list.push_back(datas[0]);

    return operand_list;
}

std::vector<uint8_t> ParseAsm::jc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x40), operand_list.push_back(datas[0]); 
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x40), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR)  operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::jnc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8) operand_list.push_back(0x50), operand_list.push_back(datas[0]); 
    else if(parse_operand_list[0] == DATA_U16) operand_list.push_back(0x50), operand_list.push_back(datas[1]);
    else if(parse_operand_list[0] == DATA_VAR) operand_list.push_back(0), operand_list.push_back(0);
    
    return operand_list;
}

std::vector<uint8_t> ParseAsm::jb2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x20), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]); 
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::jnb2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x30), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]); 
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::jbc2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8)
    {
        if(parse_operand_list[1] != SYMBOL_COMMA) return {};

        if(parse_operand_list[2] == DATA_U8) operand_list.push_back(0x10), operand_list.push_back(datas[0]), operand_list.push_back(datas[1]); 
    }

    return operand_list;
}

std::vector<uint8_t> ParseAsm::org2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    if(parse_operand_list[0] == DATA_U8)        operand_list.push_back(0), operand_list.push_back(datas[0]); 
    else if(parse_operand_list[0] == DATA_U16)  operand_list.push_back(datas[0]), operand_list.push_back(datas[1]); 
    return operand_list;
}

std::vector<uint8_t> ParseAsm::label2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    operand_list.push_back(datas[0]);                       // string长度
    operand_list.push_back(0);                              // data长度
    if(parse_operand_list[0] == DATA_VAR) for(uint8_t i = 1; i < datas.size(); i++) operand_list.push_back(datas[i]);
    return operand_list;
}

std::vector<uint8_t> ParseAsm::db2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    operand_list.push_back(datas[0]);                       // string长度
    operand_list.push_back(datas.size()-datas[0]-1);        // data长度
    if(parse_operand_list[0] == DATA_VAR) for(uint8_t i = 1; i < datas.size(); i++) operand_list.push_back(datas[i]);

    std::string str;
    std::size_t str_pos = operand_list[0]+2;
    for(uint i = 2; i < str_pos; i++) str.push_back(operand_list[i]);

    std::vector<uint8_t> vals;
    for(uint i = str_pos; i < operand_list.size(); i++) vals.push_back(operand_list[i]);

    m_dbs[str] = vals;

    return operand_list;
}

std::vector<uint8_t> ParseAsm::sfr2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    operand_list.push_back(datas[0]);                       // string长度
    operand_list.push_back(1);                              // data长度
    if(parse_operand_list[0] == DATA_VAR) for(uint8_t i = 1; i < datas.size(); i++) operand_list.push_back(datas[i]);

    std::string str;
    for(uint i = 2; i < operand_list.size()-1; i++) str.push_back(operand_list[i]);
    m_sfrs[str] = operand_list.back();

    return operand_list;
}

std::vector<uint8_t> ParseAsm::sbit2hex(const Code& code)
{
    OperandData operand_data = parse_operands(code.operands);
    std::vector<OperandEnum> parse_operand_list = operand_data.operands;
    std::vector<uint8_t> datas = operand_data.datas;

    std::vector<uint8_t> operand_list;
    operand_list.push_back(datas[0]);                       // string长度
    operand_list.push_back(1);                              // data长度
    if(parse_operand_list[0] == DATA_VAR) for(uint8_t i = 1; i < datas.size(); i++) operand_list.push_back(datas[i]);

    std::string str;
    for(uint i = 2; i < operand_list.size()-1; i++) str.push_back(operand_list[i]);
    m_sbits[str] = operand_list.back();

    return operand_list;
}

std::vector<uint8_t> ParseAsm::end2hex(const Code& code) { m_end = true; return {}; }
