#include "utils.h"

char toupper(char ch){
    return (ch >= 'a' && ch <= 'z') ? ch-32 : ch;
}

char* toupper_str(char* str)
{
    char* p = str;
    while(*p != '\0'){
        *p = toupper(*p);
        p++;
    }
    return str;
}

char tolower(char ch){
    return (ch >= 'A' && ch <= 'Z') ? ch+32 : ch;
}

char* tolower_str(char* str)
{
    char* p = str;
    while(*p != '\0'){
        *p = tolower(*p);
        p++;
    }
    return str;
}

int to_digit(char* str, int len)
{
    // parse number, three kinds: hex(12H) dec(12) bin(1100B)
    int digit = 0;
    if(str[len-1] == 'H') {
        for(int i=0; i<len-1; ++i) digit = digit*16 + (str[i] >= 'A' ? str[i]-'A'+10 : str[i]-'0'); 
    } else if(str[len-1] == 'B') {
        for(int i=0; i<len-1; ++i) digit = digit*2 + (str[i]-'0'); 
    } else {
        for(int i=0; i<len; ++i) digit = digit*10 + (str[i]-'0'); 
    }

    return digit;
}

