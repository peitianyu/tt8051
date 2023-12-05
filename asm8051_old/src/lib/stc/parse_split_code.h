#ifndef __PARSE_CODE_H__
#define __PARSE_CODE_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <cctype>
#include "code.h"

class ParseSplitCode
{
public:
    ParseSplitCode();

    Code get_code(const std::string& str);

private:
    Code splite_code(const std::size_t line, const std::string& str);
private:
    void splite_front_back(const std::string& str, std::string& front_str, std::string& back_str);

    std::string remove_spaces(const std::string& input);

    std::string splice_front_back(const std::string& front_str, const std::string& back_str);

    bool is_label(const std::string& str);

    std::string get_label(const std::string& str);
};


#endif // __PARSE_CODE_H__ 