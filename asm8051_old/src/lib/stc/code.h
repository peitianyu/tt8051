#ifndef __CODE_H__
#define __CODE_H__
#include <iostream>
#include <vector>

struct Code
{
    std::size_t line;
    std::string mnemonic;
    std::string operands;
};

struct AsmData
{
    std::string mnemonic;
    std::vector<uint8_t> operands;
    std::size_t addr = 0;
};

#endif // !__CODE_H__
