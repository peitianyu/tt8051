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
    ParseFile(const std::string& code_file)
    {
        read_file(code_file);
    }

    std::string next_code() 
    {
        if(m_codes.empty()) return "";

        std::string code = m_codes.front();
        m_codes.erase(m_codes.begin());

        return code;
    }
private:
    void read_file(const std::string& code_file)
    {
        std::ifstream ifs(code_file);

        assert(ifs.is_open() && "Failed to open the file.");

        std::string line;
        while (std::getline(ifs, line)) 
        { 
            std::string include_path = find_include(code_file, line, "#INCLUDE");
            if(!include_path.empty()){
                read_file(include_path);
                continue;
            }
            include_path = find_include(code_file, line, "#include");
            if(!include_path.empty()){
                read_file(include_path);
                continue;
            }

            std::string str = filter_semicolon(line);
            std::size_t pos = str.find(':');
            if (pos != std::string::npos) 
            {
                m_codes.push_back(str.substr(0, pos+1));
                str = str.substr(pos+1);
            }
            str = filter_space(str);

            if(is_whitespace(str)) continue;

            m_codes.push_back(str);
        }
        ifs.close(); 
    }

    std::string find_include(const std::string& code_file, const std::string& line, const std::string& key_code)
    {
        std::string str = line;
        std::size_t pos = str.find(key_code);
        if (pos == std::string::npos) return "";
        str = str.substr(pos+8);
        size_t start = str.find("\"") + 1; // 寻找双引号的起始位置
        size_t end = str.find("\"", start); // 寻找双引号的结束位置
        
        if (start != std::string::npos && end != std::string::npos) {
            str = str.substr(start, end - start);

            std::string file_path = code_file;
            size_t pos = file_path.find_last_of('/');
            file_path = file_path.substr(0, pos) + "/" + str;

            return file_path;
        }
        return "";
    }

    bool is_whitespace(const std::string& line) 
    {
        for (char c : line) 
            if (!std::isspace(static_cast<unsigned char>(c)))  return false;
        return true;
    }

    std::string filter_semicolon(const std::string& str) 
    {
        std::size_t pos = str.find(';');
        if (pos != std::string::npos) return str.substr(0, pos);
            
        return str;
    }

    std::string filter_space(const std::string& str) 
    {
        static std::vector<char> s_space_char = {'\t','\n','\v','\f','\r',' '};
        std::size_t space_pos = 0;
        for(char ch : s_space_char){
            std::size_t pos = str.find_first_not_of(ch);
            if (pos != std::string::npos && pos > space_pos) space_pos = pos;
        }

        return str.substr(space_pos);
    }
private:
    std::vector<std::string> m_codes;
};





#endif // __PARSE_FILE_H__