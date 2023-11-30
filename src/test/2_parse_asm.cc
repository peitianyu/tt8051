#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_split_code.h"
#include "stc/parse_asm.h"
#include <iostream>
#include <iomanip>

// JUST_RUN_TEST(asm, test)
TEST(asm, test)
{
    LOG_TEST("------------------------------asm--------------------------");

    ParseFile parse_file("../data/code_file.S");
    ParseSplitCode parse_code;
    ParseAsm parse_asm;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        LOG_TEST(code.line,": [ ", code.mnemonic, " ] ", code.operands);

        if(code.mnemonic == "ERR") continue;
        if(code.mnemonic == "END") break;

        OperandData operand_data = parse_asm.parse_operands(code.operands);
        std::vector<OperandEnum> operand_list = operand_data.operands;
        std::vector<uint8_t> datas = operand_data.datas;

        for(OperandEnum op: operand_list) std::cout << op << " ";
        std::cout << std::endl;
        for(uint8_t data: datas) std::cout << std::hex << (int)data << " ";
        std::cout << std::endl;
    }
}

// JUST_RUN_TEST(parse_asm, test0)
TEST(parse_asm, test0)
{
    LOG_TEST("------------------------------parse_asm--------------------------");

    ParseFile parse_file("../data/asm.S");
    ParseSplitCode parse_code;
    ParseAsm parse_asm;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        LOG_TEST(code.line,": [ ", code.mnemonic, " ] ", code.operands);

        if(code.mnemonic == "ERR") continue;
        if(code.mnemonic == "END") break;

        OperandData operand_data = parse_asm.parse_operands(code.operands);
        std::vector<OperandEnum> operand_list = operand_data.operands;
        std::vector<uint8_t> datas = operand_data.datas;

        for(auto& data : parse_asm.asm_data(code).operands) {
            std::cout << std::setfill('0') << std::setw(2) << std::hex   << (int)data << " ";
        }
        std::cout << std::endl; 
    }
}


// JUST_RUN_TEST(parse_asm, test1)
TEST(parse_asm, test1)
{
    LOG_TEST("------------------------------parse_asm--------------------------");

    ParseFile parse_file("../data/demo.S");
    ParseSplitCode parse_code;
    ParseAsm parse_asm;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        parse_asm.update_vars(code);
    }

    std::vector<AsmData> hex_datas = parse_asm.to_hex();
    for(auto hex_data: hex_datas)
    {
        std::cout << hex_data.mnemonic << " addr: " << std::hex << (int)hex_data.addr << " data: ";
        for(auto p: hex_data.operands) std::cout << std::hex << (int)p << " ";
        std::cout  << std::endl;
    }
}
