#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#ifndef INSTRUCTIONS
#   define INSTRUCTIONS
#endif

INSTRUCTIONS void identifier2hex();

INSTRUCTIONS void mov2hex();

INSTRUCTIONS void movc2hex();

INSTRUCTIONS void movx2hex();

INSTRUCTIONS void push2hex();

INSTRUCTIONS void pop2hex();

INSTRUCTIONS void xch2hex();

INSTRUCTIONS void xchd2hex();

INSTRUCTIONS void swap2hex();

INSTRUCTIONS void add2hex();

INSTRUCTIONS void addc2hex();

INSTRUCTIONS void subb2hex();

INSTRUCTIONS void inc2hex();

INSTRUCTIONS void dec2hex();

INSTRUCTIONS void mul2hex();

INSTRUCTIONS void div2hex();

INSTRUCTIONS void da2hex();

INSTRUCTIONS void clr2hex();

INSTRUCTIONS void cpl2hex();

INSTRUCTIONS void rl2hex();

INSTRUCTIONS void rlc2hex();

INSTRUCTIONS void rr2hex();

INSTRUCTIONS void rrc2hex();

INSTRUCTIONS void anl2hex();

INSTRUCTIONS void orl2hex();

INSTRUCTIONS void xrl2hex();

INSTRUCTIONS void ajmp2hex();

INSTRUCTIONS void sjmp2hex();

INSTRUCTIONS void ljmp2hex();

INSTRUCTIONS void jmp2hex();

INSTRUCTIONS void jz2hex();

INSTRUCTIONS void jnz2hex();

INSTRUCTIONS void cjne2hex();

INSTRUCTIONS void djnz2hex();

INSTRUCTIONS void acall2hex();

INSTRUCTIONS void lcall2hex();

INSTRUCTIONS void ret2hex();

INSTRUCTIONS void reti2hex();

INSTRUCTIONS void nop2hex();

INSTRUCTIONS void setb2hex();

INSTRUCTIONS void jc2hex();

INSTRUCTIONS void jnc2hex();

INSTRUCTIONS void jb2hex();

INSTRUCTIONS void jnb2hex();

INSTRUCTIONS void jbc2hex();

INSTRUCTIONS void org2hex();

INSTRUCTIONS void end2hex();




#endif // __INSTRUCTION_H__