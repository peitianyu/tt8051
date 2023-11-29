#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_split_code.h"
#include "stc/parse_asm.h"
#include "stc/asm2hex.h"
#include <iostream>
#include <iomanip>

JUST_RUN_TEST(asm2hex, test)
TEST(asm2hex, test)
{
    LOG_TEST("------------------------------asm2hex--------------------------");

    ParseFile parse_file("../data/demo.S");
    ParseSplitCode parse_code;
    ParseAsm parse_asm;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        LOG_TEST(code.addr,": [ ", code.mnemonic, " ] ", code.operands);

        if(code.mnemonic == "ERR") continue;
        if(code.mnemonic == "END") break;

        OperandData operand_data = parse_asm.parse_operands(code.operands);
        std::vector<OperandEnum> operand_list = operand_data.operands;
        std::vector<uint8_t> datas = operand_data.datas;

        for(auto& data : parse_asm.asm_data(code).operands) 
            std::cout << std::setfill('0') << std::setw(2) << std::hex   << (int)data << " ";
        std::cout << std::endl; 
    }
}
