#include "core/tt_test.h"
#include "core/log.h"
#include "stc/parse_file.h"
#include <iostream>

JUST_RUN_TEST(paser, test)
TEST(paser, test)
{
    LOG_TEST("------------------------------paser--------------------------");

    ParseFile parse_file("/root/work_space/stc/tt_stc/data/0_led.S");

    while(true)
    {
        std::string code = parse_file.next_code();
        if(code.empty()) break;

        LOG(code);
    }
}