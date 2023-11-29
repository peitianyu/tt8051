#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include "stc/parse_split_code.h"
#include <iostream>

// JUST_RUN_TEST(paser_file, test)
TEST(paser_file, test)
{
    LOG_TEST("------------------------------paser_file--------------------------");

    ParseFile parse_file("../data/code_file.S");

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        LOG(code_str);
    }
}

// JUST_RUN_TEST(paser_code, test)
TEST(paser_code, test)
{
    LOG_TEST("------------------------------paser_code--------------------------");

    ParseFile parse_file("../data/code_file.S");
    ParseSplitCode parse_code;

    while(true)
    {
        std::string code_str = parse_file.next_code();
        if(code_str.empty()) break;

        Code code = parse_code.get_code(code_str);

        LOG_TEST(code.addr,": [ ", code.mnemonic, " ] ", code.operands);

        if(code.mnemonic == "ERR") LOG_TEST("WARNING: illegal command");

        if(code.mnemonic == "END") break;
    }
}

