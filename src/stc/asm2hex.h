#ifndef __ASM2HEX_H__
#define __ASM2HEX_H__

#include "code.h"

class Asm2Hex
{
public:
    Asm2Hex() = default;

    void read_code(const AsmData& data)
    {
        
    }

    void write_hex(const AsmData& data);
private:
    std::vector<Code> m_codes;
};


#endif // __ASM2HEX_H__