#ifndef __ASM2HEX_H__
#define __ASM2HEX_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include "code.h"
#include <iomanip>

void write_hex_file(const std::string& hex_file_path, const std::vector<AsmData>& datas);


#endif // __ASM2HEX_H__