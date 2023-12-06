#ifndef __GET_TOKEN_H__
#define __GET_TOKEN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint8_t unsigned char

void set_token_file(char* src_file, int src_size);

int next_token();

int token_empty();

#endif // __GET_TOKEN_H__