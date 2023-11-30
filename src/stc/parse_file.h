#ifndef __PARSE_FILE_H__
#define __PARSE_FILE_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cctype>

class ParseFile
{
public:
    ParseFile(const std::string& code_file);

    std::string next_code();
private:
    void read_file(const std::string& code_file);
    
    std::string find_include(const std::string& code_file, const std::string& line, const std::string& key_code);

    bool is_whitespace(const std::string& line) ;

    std::string filter_semicolon(const std::string& str) ;

    std::string filter_space(const std::string& str);
private:
    std::vector<std::string> m_codes;
};





#endif // __PARSE_FILE_H__