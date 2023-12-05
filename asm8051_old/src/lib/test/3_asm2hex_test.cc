#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_split_code.h"
#include "stc/parse_asm.h"
#include "stc/asm2hex.h"
#include <iostream>
#include <iomanip>

// JUST_RUN_TEST(asm2hex, test)
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

        parse_asm.update_vars(code);
    }

    std::vector<AsmData> hex_datas = parse_asm.to_hex();

    for(auto hex_data: hex_datas)
    {
        std::cout << hex_data.mnemonic << " addr: " << std::hex << (int)hex_data.addr << " len: " << hex_data.operands.size() << " data: ";
        for(auto p: hex_data.operands) std::cout << std::hex << (int)p << " ";
        std::cout  << std::endl;
    }

    write_hex_file("../out/demo.hex", hex_datas);
}
