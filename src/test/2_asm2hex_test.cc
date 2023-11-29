#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_split_code.h"
#include "stc/parse_asm.h"
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

        parse_asm.read_code(code);
    }

    parse_asm.write_hex();
}
