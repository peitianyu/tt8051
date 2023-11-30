#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
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

