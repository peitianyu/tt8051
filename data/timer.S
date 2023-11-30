#INCLUDE "stc8051.S"

    ORG     0000H
    LJMP    MAIN

    ORG     000BH                   ;(location at 000BH)
    LJMP    T0INT

    ORG     0012H
MAIN:
    MOV     SP,#7FH                 ;initial SP
    MOV     AUXR,#80H               ;timer0 work in 1T mode
    MOV     TMOD,#06H               ;set timer0 as counter mode2 (8-bit auto-reload)
    MOV     A,#FFH
    MOV     TL0,A                   ;fill with 0xff to count one time
    MOV     TH0,A
    SETB    TR0                     ;timer0 start run
    SETB    ET0                     ;enable T0 interrupt
    SETB    EA                      ;open global interrupt switch
    SJMP    0                       ; $功能未实现

T0INT:
    CPL     P0.0
    RETI

    END

