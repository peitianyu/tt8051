#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_code.h"
#include "stc/code2hex.h"
#include <iostream>

JUST_RUN_TEST(paser, test)
TEST(paser, test)
{
    LOG_TEST("------------------------------paser--------------------------");

    ParseFile parse_file("../data/add_test.S");
    ParseCode parse_code;
    Code2Hex code_hex;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        std::cout << code.addr << ": [ " << code.mnemonic << " ] " << code.operands << std::endl;

        std::vector<OperandEnum> operand_list = code_hex.parse_operands(code.operands);
        for(OperandEnum op: operand_list) std::cout << op << " ";
        std::cout << std::endl;
    }
}