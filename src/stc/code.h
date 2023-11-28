#ifndef __CODE_H__
#define __CODE_H__
#include <iostream>

struct Code
{
    std::size_t addr;
    std::string mnemonic;
    std::string operands;
};

#endif // !__CODE_H__
