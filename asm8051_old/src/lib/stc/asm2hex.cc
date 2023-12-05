#include "asm2hex.h"

static uint8_t calc_checksum(const std::string& str)
{
    uint8_t sum = 0;
    for (size_t i = 0; i < str.length(); i += 2)         
        sum += std::stoi(str.substr(i, 2), nullptr, 16);
    return ~sum+1;
}

void write_hex_file(const std::string& hex_file_path, const std::vector<AsmData>& datas)
{
    std::ofstream ofs(hex_file_path);
    bool start_hex = true;
    uint curr_addr = 0;
    std::stringstream ss;
    uint8_t check_sum = 0;
    for (const auto& data : datas)
    {
        if (data.addr != curr_addr) curr_addr = data.addr;
        for (const auto& p : data.operands)
        {
            if (ss.str().size() % 32 == 0 && !start_hex) { 
                std::stringstream ss_tmp;           
                ss_tmp << "10" << std::hex << std::setw(4) << std::setfill('0') << (int)(curr_addr - 0x10) << "00" << ss.str();
                ofs << ":" << ss_tmp.str() << std::hex << std::setw(2) << std::setfill('0') << (int)calc_checksum(ss_tmp.str()) << std::endl;
                ss.str(""); 
            }
    
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(p);
            check_sum += p;
            curr_addr++;
            start_hex = false;
        }
    }

    int len = ss.str().size()/2;
    std::stringstream ss_tmp; 
    ss_tmp << std::hex << std::setw(2) << std::setfill('0') << len;          
    ss_tmp << std::hex << std::setw(4) << std::setfill('0') << (int)(curr_addr - len) << "00" << ss.str();
    ofs << ":" << ss_tmp.str() << std::hex << std::setw(2) << std::setfill('0') << (int)calc_checksum(ss_tmp.str()) << std::endl;
    
    ofs << ":00000001FF" << std::endl;
    
    ofs.close();
}

