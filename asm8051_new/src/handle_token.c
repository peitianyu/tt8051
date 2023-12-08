#include "handle_token.h"
#include "get_token.h"
#include "token.h"
#include "map.h"
#include "utils.h"

static int g_is_end;
static int g_curr_addr;

static int* g_hex; /*out hex*/
static int g_hex_len;/*out hex length*/

enum DATA_TYPE {
    DATA_TYPE_IMMEDIATE_NUM = 0, /* immediate data : #0x12 */
    DATA_TYPE_REGISTER, /* register : R0~R7 */
    DATA_TYPE_DIRECT_ADDRESS, /*direct address : 0x12 */
    DATA_TYPE_INDIRECT_ADDRESS, /* indirect address : @R0||@R1 */
    DATA_TYPE_BIT_ADDRESS,   /* bit address : 0x12^0 || P0^1 */
    DATA_TYPE_SLASH, /* / */
    DATA_TYPE_A, /* A */
    DATA_TYPE_AB, /* AB */
    DATA_TYPE_C, /* C */
    DATA_TYPE_DPTR, /* DPTR */
    DATA_TYPE_PC, /* PC */
    DATA_TYPE_AT_A, /* @A */
    DATA_TYPE_AT_DPTR, /* @DPTR */
    DATA_TYPE_NONE /* none */
};

static int get_data_type(int *val)
{
    int token = next_token();
    int data_type = DATA_TYPE_NONE;
    switch(token)
    {
        case REG_R0: 
        case REG_R1:
        case REG_R2:
        case REG_R3:
        case REG_R4:
        case REG_R5:
        case REG_R6:
        case REG_R7:    *val = token; data_type = DATA_TYPE_REGISTER; break;
        case SYMBOL_AT: 
        {
            token = next_token(); 
            if(token == REG_R0 || token == REG_R1) { *val = token; data_type = DATA_TYPE_INDIRECT_ADDRESS;} 
            else if(token == REG_DPTR) data_type = DATA_TYPE_AT_DPTR;
            else if(token == REG_A) data_type = DATA_TYPE_AT_A;
            break;
        }
        case SYMBOL_SLASH: data_type = DATA_TYPE_SLASH; break;
        case REG_A:     data_type = DATA_TYPE_A; break;
        case REG_AB:    data_type = DATA_TYPE_AB; break;
        case REG_C:     data_type = DATA_TYPE_C; break;
        case REG_DPTR:  data_type = DATA_TYPE_DPTR; break;
        case REG_PC:    data_type = DATA_TYPE_PC; break;
        case SYMBOL_HASH: {if(next_token() == SYMBOL_NUM) { *val = get_digit(); data_type = DATA_TYPE_IMMEDIATE_NUM; }  break;}
        case SYMBOL_NUM:
        case IDENTIFIER:
            {
                if(token == SYMBOL_NUM) *val = get_digit();
                else {
                    char identifier[16];
                    memset(identifier, 0, 16);
                    int identifier_len;
                    get_identifier(identifier, &identifier_len);

                    if(is_in_map(SFR_MAP_TYPE, identifier))
                        *val = map_get_value(SFR_MAP_TYPE, identifier);
                    else if(is_in_map(SBIT_MAP_TYEP, identifier)){
                        *val = map_get_value(SBIT_MAP_TYEP, identifier);
                        data_type = DATA_TYPE_BIT_ADDRESS;
                    }
                    
                    // printf("%s: %d\n", identifier, *val);
                }

                if(try_next_token() == SYMBOL_HAT || try_next_token() == SYMBOL_DOT) {
                    next_token();
                    if(next_token() == SYMBOL_NUM) {
                        *val = *val^get_digit();
                        data_type = DATA_TYPE_BIT_ADDRESS;
                    }
                }
                break;
            }
    }

    return data_type;
}

void handle_token_init()
{
    g_is_end = 0;
    g_curr_addr = 0;

    g_hex = (int*)malloc(HEX_MAX_SIZE);
    memset(g_hex, 0, HEX_MAX_SIZE);
    g_hex_len = 0;

    init_map();
}

#include <stdarg.h>
static void set_hex(int hex_len, ...)
{
    va_list ap;
    va_start(ap, hex_len);
    for(int i = 0; i < hex_len; ++i) {
        g_hex[g_hex_len++] = va_arg(ap, int);
    }
    va_end(ap);

    g_curr_addr += hex_len;
    // printf("0x%04X: ", g_curr_addr);
}

/*db is not supported yet*/
static void identifier_handle() {
    char identifier[16];
    memset(identifier, 0, 16);
    int identifier_len;
    get_identifier(identifier, &identifier_len);

    /*equ sfr data sbit db label*/
    int token = next_token();
    if(token == DATA || token == SFR || token == EQU) 
    {
        if(next_token() == SYMBOL_NUM)   {
            map_add_pair(SFR_MAP_TYPE, identifier, get_digit());
            return;
        }
    }
    else if(token == SBIT) /* identifier sbit , (80H^1/P0^1) */
    {
        token = next_token();
        if(token == SYMBOL_NUM || token == IDENTIFIER) {
            int value = -1;
            if(token == SYMBOL_NUM) value = get_digit();
            else {
                char data_identifier[16];
                memset(data_identifier, 0, 16);
                get_identifier(data_identifier, &identifier_len);
                if(is_in_map(SFR_MAP_TYPE, data_identifier))
                    value = map_get_value(SFR_MAP_TYPE, data_identifier);
            }

            token = next_token();
            if((token == SYMBOL_HAT || token == SYMBOL_DOT) && next_token() == SYMBOL_NUM) {
                map_add_pair(SBIT_MAP_TYEP, identifier, value^get_digit());
                return ;
            }
        }
    }
    else if(token == SYMBOL_COLON) {
        map_add_pair(LABEL_MAP_TYPE, identifier, g_curr_addr);

        // TODO: 通过这里更新label的地址
        return;
    } 

    printf("[ERROR]: identifier: %s (%s): %d\n", identifier, __FUNCTION__, __LINE__);
}

static void mov2hex() 
{
    int val = -1;
    int data_type = get_data_type(&val);
    
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0xE8+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xE5, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0xE6+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x74, val);
    }
    else if (data_type == DATA_TYPE_REGISTER)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int reg_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(1, 0xF8+reg_addr);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xA8+reg_addr, val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x78+reg_addr, val);
    }
    else if(data_type == DATA_TYPE_DIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int direct_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(2, 0xF5, direct_addr);
        else if(data_type == DATA_TYPE_REGISTER) set_hex(2, 0x88+val, direct_addr);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x85, direct_addr, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(2, 0x86+val, direct_addr);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(3, 0x75, direct_addr, val);
    }
    else if(data_type == DATA_TYPE_INDIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int indirect_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(2, 0xF6+indirect_addr);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xA6+indirect_addr, val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x76+indirect_addr, val);
    }
    else if(data_type == DATA_TYPE_C)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xA2, val);
    }
    else if(data_type == DATA_TYPE_BIT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int bit_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_C) set_hex(2, 0x92, bit_addr);
    }
    else if(data_type == DATA_TYPE_DPTR)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(3, 0x90, (val>>8)&0xFF, (val)&0xFF);
    }
}

static void movc2hex()
{
    int val = -1;
    int data_type = get_data_type(&val);
    
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        if(get_data_type(&val) != DATA_TYPE_AT_A) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        if(next_token() != SYMBOL_PLUS) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int token = next_token();
        if(token == REG_DPTR) set_hex(1, 0x93);
        else if(token == REG_PC) set_hex(1, 0x83);
    }
}

static void movx2hex()
{
    int val = -1;
    int data_type = get_data_type(&val);
    
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_AT_DPTR) set_hex(1, 0xE0);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0xE2+val);
    }
    else if(data_type == DATA_TYPE_INDIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int reg_addr = val;
        if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0xF2+reg_addr);
    }
    else if(data_type == DATA_TYPE_AT_DPTR)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0xF0);
    }
}

static void push2hex()
{
    int val = -1;
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xC0, val);
}

static void pop2hex()
{
    int val = -1;
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xD0, val);
}

static void xch2hex()
{
    int val = -1;
    int data_type = get_data_type(&val);
    
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0xC8+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xC5, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0xC6+val);
    }
}

static void xchd2hex()
{
    int val = -1;    
    if (get_data_type(&val) == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        if(get_data_type(&val) == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0xD6+val);
    }
}

static void swap2hex()
{
    int val = -1;    
    if (get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0xC4);
}

static void add2hex()
{
    int val = -1;    
    if (get_data_type(&val) == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x28+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x25, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x26+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x24, val);
    }
}

static void addc2hex()
{
    int val = -1;    
    if (get_data_type(&val) == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x38+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x35, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x36+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x34, val);
    }
}

static void subb2hex()
{
    int val = -1;    
    if (get_data_type(&val) == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x98+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x95, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x96+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x94, val);
    }
}

static void inc2hex()
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_A) set_hex(1, 0x04);
    else if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x08+val);
    else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x05, val);
    else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x06+val);
    else if(data_type == DATA_TYPE_DPTR) set_hex(1, 0xA3);
}

static void dec2hex()
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_A) set_hex(1, 0x14);
    else if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x18+val);
    else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x15, val);
    else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x16+val);
}

static void mul2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_AB) set_hex(1, 0xA4);
}

static void div2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_AB) set_hex(1, 0x84);
}

static void da2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0xD4);
}

static void clr2hex()
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_A) set_hex(1, 0xE4);
    else if(data_type == DATA_TYPE_C) set_hex(1, 0xC3);
    else if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xC2, val);
}

static void cpl2hex() 
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_A) set_hex(1, 0xF4);
    else if(data_type == DATA_TYPE_C) set_hex(1, 0xB3);
    else if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xB2, val);
}

static void rl2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0x23);
}

static void rlc2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0x33);
}

static void rr2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0x03);
}

static void rrc2hex() 
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_A) set_hex(1, 0x13);
}

static void anl2hex()
{
    int val = -1; 
    int data_type = get_data_type(&val);   
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x58+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x55, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x56+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x54, val);
    }
    else if (data_type == DATA_TYPE_DIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int direct_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(2, 0x52, direct_addr);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(3, 0x53, direct_addr, val);
    }
    else if (data_type == DATA_TYPE_C)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0x82, val);
        else if(data_type == DATA_TYPE_SLASH && get_data_type(&val) == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xB0, val);
    }
}

static void orl2hex()
{
    int val = -1; 
    int data_type = get_data_type(&val);   
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x48+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x45, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x46+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x44, val);
    }
    else if (data_type == DATA_TYPE_DIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int direct_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(2, 0x42, direct_addr);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(3, 0x43, direct_addr, val);
    }
    else if (data_type == DATA_TYPE_C)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0x72, val);
        else if(data_type == DATA_TYPE_SLASH && get_data_type(&val) == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xA0, val);
    }
}

static void xrl2hex()
{
    int val = -1; 
    int data_type = get_data_type(&val);   
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_REGISTER) set_hex(1, 0x68+val);
        else if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x65, val);
        else if(data_type == DATA_TYPE_INDIRECT_ADDRESS) set_hex(1, 0x66+val);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(2, 0x64, val);
    }
    else if (data_type == DATA_TYPE_DIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
        int direct_addr = val;
        data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_A) set_hex(2, 0x62, direct_addr);
        else if(data_type == DATA_TYPE_IMMEDIATE_NUM) set_hex(3, 0x63, direct_addr, val);
    }
}

static void ajmp2hex() 
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, (val<<5+1)&0xff, val&0xff);
}

static void sjmp2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x80, val);
}

static void ljmp2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x02, (val>>8)&0xFF, (val)&0xFF);
}

static void jmp2hex()
{
    int val = -1; 
    if(get_data_type(&val) != DATA_TYPE_AT_A) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    if(next_token() != SYMBOL_PLUS) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    if(next_token() == REG_DPTR) set_hex(1, 0x73);
}

static void jz2hex() 
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x60, val);
}

static void jnz2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x70, val);
}

static void cjne2hex()
{
    int val = -1; 
    int data_type = get_data_type(&val);   
    if (data_type == DATA_TYPE_A)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int val1 = -1;
        int data_type = get_data_type(&val1);
        if(data_type == DATA_TYPE_DIRECT_ADDRESS){
            if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
            if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0xB5, val1, val);
        }else if(data_type == DATA_TYPE_IMMEDIATE_NUM){
            if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
            if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0xB4, val1, val);
        }
    }
    else if (data_type == DATA_TYPE_REGISTER)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int reg_addr = val;
        int val1 = -1;
        int data_type = get_data_type(&val1);
        if(data_type == DATA_TYPE_IMMEDIATE_NUM){
            if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
            if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0xB8+reg_addr, val1, val);
        }
    }
    else if (data_type == DATA_TYPE_INDIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int reg_addr = val;
        int val1 = -1;
        int data_type = get_data_type(&val1);
        if(data_type == DATA_TYPE_IMMEDIATE_NUM){
            if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
            if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0xB6+reg_addr, val1, val);
        }
    }
}

static void djnz2hex() 
{
    int val = -1; 
    int data_type = get_data_type(&val);   
    if (data_type == DATA_TYPE_REGISTER)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int reg_addr = val;
        int data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0xD8+reg_addr, val);
    }
    else if (data_type == DATA_TYPE_DIRECT_ADDRESS)
    {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int direct_addr = val;
        int data_type = get_data_type(&val);
        if(data_type == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0xD5, direct_addr, val);
    }
}

static void acall2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, ((val<<5)+0x11)&0xff, val&0xff);
}

static void lcall2hex()
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x12, (val>>8)&0xFF, (val)&0xFF);
}

static void ret2hex() {set_hex(1, 0x22);}

static void reti2hex() {set_hex(1, 0x32);}

static void nop2hex() {set_hex(1, 0x00);}

static void setb2hex() 
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_C) set_hex(1, 0xD3);
    else if(data_type == DATA_TYPE_BIT_ADDRESS) set_hex(2, 0xD2, val);
}

static void jc2hex() 
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x40, val);
}

static void jnc2hex() 
{
    int val = -1;    
    if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(2, 0x50, val);
}

static void jb2hex() 
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_BIT_ADDRESS) {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int bit_addr = val;
        if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x20, bit_addr, val);
    }
}

static void jnb2hex() 
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_BIT_ADDRESS) {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int bit_addr = val;
        if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x30, bit_addr, val);
    }
}

static void jbc2hex()
{
    int val = -1;    
    int data_type = get_data_type(&val);
    if(data_type == DATA_TYPE_BIT_ADDRESS) {
        if(next_token() != SYMBOL_COMMA) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

        int bit_addr = val;
        if(get_data_type(&val) == DATA_TYPE_DIRECT_ADDRESS) set_hex(3, 0x10, bit_addr, val);
    }
}

static void org_handle() {
    if(next_token() != SYMBOL_NUM) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);

    g_curr_addr = get_digit();
}

static void end_handle() { 
    g_is_end = 1; 

    printf("hex: ");
    for(int i = 0; i < g_hex_len; ++i) {
        printf("%02X ", g_hex[i]);
    }
    printf("\n");
}

static void include_handle() {
    if(next_token() != INCLUDE) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    if(next_token() != SYMBOL_QUOTE) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    if(next_token() != IDENTIFIER) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    char file_name[100];
    int file_name_len;
    get_identifier(file_name, &file_name_len);
    // printf("file_name: %s\n", file_name);
    if(next_token() != SYMBOL_DOT) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    if(next_token() != IDENTIFIER) printf("ERROR: %s (%s): %d\n", ERROR_TOKEN, __FUNCTION__, __LINE__);
    char suffix[100]; 
    int suffix_len;
    get_identifier(suffix, &suffix_len);
    if(next_token() != SYMBOL_QUOTE) printf("ERROR: %s (%s): %d", ERROR_TOKEN, __FUNCTION__, __LINE__);

    /*file_name + '.' + suffix*/
    char* include_file = (char*)malloc(file_name_len+suffix_len+2);
    memset(include_file, 0, file_name_len+suffix_len+2);
    memcpy(include_file, file_name, file_name_len);
    memcpy(include_file+file_name_len, ".", 1);
    memcpy(include_file+file_name_len+1, suffix, suffix_len);

    input_include_file(include_file);
}

#include "token.h"
void handle_token(int token)
{
    if(g_is_end) return;

    switch(token)
    {
        case MOV: mov2hex(); break;
        case MOVC: movc2hex(); break;
        case MOVX: movx2hex(); break;
        case PUSH: push2hex(); break;
        case POP: pop2hex(); break;
        case XCH: xch2hex(); break;
        case XCHD: xchd2hex(); break;
        case SWAP: swap2hex(); break;
        case ADD: add2hex(); break;
        case ADDC: addc2hex(); break;
        case SUBB: subb2hex(); break;
        case INC: inc2hex(); break;
        case DEC: dec2hex(); break;
        case MUL: mul2hex(); break;
        case DIV: div2hex(); break;
        case DA: da2hex(); break;
        case CLR: clr2hex(); break;
        case CPL: cpl2hex(); break;
        case RL: rl2hex(); break;
        case RLC: rlc2hex(); break;
        case RR: rr2hex(); break;
        case RRC: rrc2hex(); break;
        case ANL: anl2hex(); break;
        case ORL: orl2hex(); break;
        case XRL: xrl2hex(); break;
        case AJMP: ajmp2hex(); break;
        case SJMP: sjmp2hex(); break;
        case LJMP: ljmp2hex(); break;
        case JMP: jmp2hex(); break;
        case JZ: jz2hex(); break;
        case JNZ: jnz2hex(); break;
        case CJNE: cjne2hex(); break;
        case DJNZ: djnz2hex(); break;
        case ACALL: acall2hex(); break;
        case LCALL: lcall2hex(); break;
        case RET: ret2hex(); break;
        case RETI: reti2hex(); break;
        case NOP: nop2hex(); break;
        case SETB: setb2hex(); break;
        case JC: jc2hex(); break;
        case JNC: jnc2hex(); break;
        case JB: jb2hex(); break;
        case JNB: jnb2hex(); break;
        case JBC: jbc2hex(); break;
        case ORG: org_handle(); break;
        case END: end_handle(); break;
        case IDENTIFIER: identifier_handle(); break;
        case SYMBOL_HASH: if(try_next_token() == INCLUDE) include_handle(); break;
    }
}
