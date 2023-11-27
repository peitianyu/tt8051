#ifndef __PARSE_CODE_H__
#define __PARSE_CODE_H__

#include <vector>
#include <iostream>
#include <fstream>

struct Code
{
    std::size_t addr;
    std::string mnemonic;
    std::string operands;
};

enum Token
{
    
};

class ParseCode
{
public:
    ParseCode(const std::string& code)
    {

    }
private:
    stc::vector<Code> m_codes;
};


#endif // __PARSE_CODE_H__ 