#include "stc8051.S"

org 0000h
start:
  mov P0, #0
  mov P1, #0xf0

repeat:
  setb P0.0
  clr P0.1
