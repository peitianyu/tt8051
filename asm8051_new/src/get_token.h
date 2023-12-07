#ifndef __GET_TOKEN_H__
#define __GET_TOKEN_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint8_t unsigned char

void set_token_file(char* src_file, int src_size);

void input_include_file(char* include_file);

int next_token();

int try_next_token();

int token_empty();

void get_identifier(char* identifier, int* identifier_len);

int get_digit();

#endif // __GET_TOKEN_H__