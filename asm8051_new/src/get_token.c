#include "token.h"
#include "get_token.h"
#include "utils.h"

static char* g_src; /*src file string*/

static char* g_src_path; /*src file path*/

void set_token_file(char* src_file, int src_size) {
    FILE *fp = fopen(src_file, "r");
    g_src = (char*)malloc(src_size);
    memset(g_src, 0, src_size);

    if(!fread(g_src, 1, src_size, fp))  printf("read src file fail!\n");
    else                                fclose(fp);

    // printf("%s\n", g_src);

    int pos = 0;
    for(int i=0; i<strlen(src_file); ++i)  { if(src_file[i] == '/') pos = i; }
    g_src_path = (char*)malloc(pos+1);
    memset(g_src_path, 0, pos+2);
    memcpy(g_src_path, src_file, pos+1);
}

void input_include_file(char* include_file)
{
    printf("include file: %s\n", include_file);
    
    char* src_file = (char*)malloc(strlen(g_src_path)+strlen(include_file)+1);
    memset(src_file, 0, strlen(g_src_path)+strlen(include_file)+1);
    memcpy(src_file, g_src_path, strlen(g_src_path));
    memcpy(src_file+strlen(g_src_path), include_file, strlen(include_file));

    int src_size = 10*1024;
    FILE *fp = fopen(src_file, "r");
    char* include_src = (char*)malloc(src_size);
    memset(include_src, 0, src_size);

    if(!fread(include_src, 1, src_size, fp))    printf("read include file fail!\n");
    else                                        fclose(fp);

    char* start_src = g_src;
    int start_len = strlen(g_src);
    int include_len = strlen(include_src);
    g_src = (char*)malloc(start_len+include_len+1);
    memset(g_src, 0, start_len+include_len+1);
    memcpy(g_src, include_src, include_len);
    memcpy(g_src+include_len, start_src, start_len);
}

static char g_identifier[100]; /*identifier*/
static int  g_identifier_len;  /*identifier length*/

static int g_digit; /*digit*/

void get_identifier(char* identifier, int* identifier_len) {
    *identifier_len = g_identifier_len;
    memcpy(identifier, g_identifier, g_identifier_len);
}

int get_digit() {
    return g_digit;
}

#include "utils.h"
int next_token()
{
    int token = -1;
    while (token = toupper(*g_src++))
    {
        if(token == ';') while(*g_src != '\n') ++g_src;
        else if(token == '@') return SYMBOL_AT;
        else if(token == '#') return SYMBOL_HASH;
        else if(token == '+') return SYMBOL_PLUS;
        else if(token == '/') return SYMBOL_SLASH;
        else if(token == '.') return SYMBOL_DOT;
        else if(token == '^') return SYMBOL_HAT;
        else if(token == '$') return SYMBOL_DOLLAR;
        else if(token == ',') return SYMBOL_COMMA;
        else if(token == ':') return SYMBOL_COLON;
        else if(token == '"') return SYMBOL_QUOTE;
        else if((token >= 'A' && token <= 'Z') || (token == '_')) {
            // identifier
            char* start = g_src-1;
            while((token = toupper(*g_src++)) && ((token >= 'A' && token <= 'Z') || (token >= '0' && token <= '9') || (token == '_')));
            g_identifier_len = g_src - start - 1;
            char* identifier = (char*)malloc( g_identifier_len+1);
            memset(identifier, 0,  g_identifier_len+1);
            memcpy(identifier, start,  g_identifier_len);
            identifier = toupper_str(identifier);
            
            // printf("'%s' : ", identifier);
            *g_src--;

            if(strcmp((identifier), "R0") == 0) return REG_R0;
            else if(strcmp(identifier, "R1") == 0) return REG_R1;
            else if(strcmp(identifier, "R2") == 0) return REG_R2;
            else if(strcmp(identifier, "R3") == 0) return REG_R3;
            else if(strcmp(identifier, "R4") == 0) return REG_R4;
            else if(strcmp(identifier, "R5") == 0) return REG_R5;
            else if(strcmp(identifier, "R6") == 0) return REG_R6;
            else if(strcmp(identifier, "R7") == 0) return REG_R7;
            else if(strcmp(identifier, "A") == 0) return REG_A;
            else if(strcmp(identifier, "AB") == 0) return REG_AB;
            else if(strcmp(identifier, "C") == 0) return REG_C;
            else if(strcmp(identifier, "DPTR") == 0) return REG_DPTR;
            else if(strcmp(identifier, "PC") == 0) return REG_PC;
            else if(strcmp(identifier, "MOV") == 0) return MOV;
            else if(strcmp(identifier, "MOVC") == 0) return MOVC;
            else if(strcmp(identifier, "MOVX") == 0) return MOVX;
            else if(strcmp(identifier, "PUSH") == 0) return PUSH;
            else if(strcmp(identifier, "POP") == 0) return POP;
            else if(strcmp(identifier, "XCH") == 0) return XCH;
            else if(strcmp(identifier, "XCHD") == 0) return XCHD;
            else if(strcmp(identifier, "SWAP") == 0) return SWAP;
            else if(strcmp(identifier, "ADD") == 0) return ADD;
            else if(strcmp(identifier, "ADDC") == 0) return ADDC;
            else if(strcmp(identifier, "SUBB") == 0) return SUBB;
            else if(strcmp(identifier, "INC") == 0) return INC;
            else if(strcmp(identifier, "DEC") == 0) return DEC;
            else if(strcmp(identifier, "MUL") == 0) return MUL;
            else if(strcmp(identifier, "DIV") == 0) return DIV;
            else if(strcmp(identifier, "DA") == 0) return DA;
            else if(strcmp(identifier, "CLR") == 0) return CLR;
            else if(strcmp(identifier, "CPL") == 0) return CPL;
            else if(strcmp(identifier, "RL") == 0) return RL;
            else if(strcmp(identifier, "RLC") == 0) return RLC;
            else if(strcmp(identifier, "RR") == 0) return RR;
            else if(strcmp(identifier, "RRC") == 0) return RRC;
            else if(strcmp(identifier, "ANL") == 0) return ANL;
            else if(strcmp(identifier, "ORL") == 0) return ORL;
            else if(strcmp(identifier, "XRL") == 0) return XRL;
            else if(strcmp(identifier, "AJMP") == 0) return AJMP;
            else if(strcmp(identifier, "SJMP") == 0) return SJMP;
            else if(strcmp(identifier, "LJMP") == 0) return LJMP;
            else if(strcmp(identifier, "JMP") == 0) return JMP;
            else if(strcmp(identifier, "JZ") == 0) return JZ;
            else if(strcmp(identifier, "JNZ") == 0) return JNZ;
            else if(strcmp(identifier, "CJNE") == 0) return CJNE;
            else if(strcmp(identifier, "DJNZ") == 0) return DJNZ;
            else if(strcmp(identifier, "ACALL") == 0) return ACALL;
            else if(strcmp(identifier, "LCALL") == 0) return LCALL;
            else if(strcmp(identifier, "RET") == 0) return RET;
            else if(strcmp(identifier, "RETI") == 0) return RETI;
            else if(strcmp(identifier, "NOP") == 0) return NOP;
            else if(strcmp(identifier, "SETB") == 0) return SETB;
            else if(strcmp(identifier, "JC") == 0) return JC;
            else if(strcmp(identifier, "JNC") == 0) return JNC;
            else if(strcmp(identifier, "JB") == 0) return JB;
            else if(strcmp(identifier, "JNB") == 0) return JNB;
            else if(strcmp(identifier, "JBC") == 0) return JBC;
            else if(strcmp(identifier, "ORG") == 0) return ORG;
            else if(strcmp(identifier, "DB") == 0) return DB;
            else if(strcmp(identifier, "EQU") == 0) return EQU;
            else if(strcmp(identifier, "DATA") == 0) return DATA;
            else if(strcmp(identifier, "SFR") == 0) return SFR;
            else if(strcmp(identifier, "SBIT") == 0) return SBIT;
            else if(strcmp(identifier, "END") == 0) return END;
            else if(strcmp(identifier, "INCLUDE") == 0) return INCLUDE;
            else {
                // identifier
                memset(g_identifier, 0,  g_identifier_len+1);
                memcpy(g_identifier, start,  g_identifier_len);

                return IDENTIFIER;
            }
        }
        else if (token >= '0' && token <= '9') {
            // number
            char* start = g_src-1;
            while((token = toupper(*g_src++)) && ((token >= 'A' && token <= 'Z') || (token >= '0' && token <= '9')));
            g_identifier_len = g_src - start - 1;
            char* identifier = (char*)malloc( g_identifier_len+1);
            memset(identifier, 0,  g_identifier_len+1);
            memcpy(identifier, start,  g_identifier_len);            
            g_digit = to_digit(toupper_str(identifier), g_identifier_len);
            *g_src--;
            return SYMBOL_NUM;
        }
    }
    
    return -1;
}

int token_empty()
{
    return *g_src == '\0';
}

int try_next_token()
{
    char* start = g_src;
    int token = next_token();
    g_src = start;
    return token;
}


