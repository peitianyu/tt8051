#INCLUDE "stc8051.S"
P4      SFR    E8H

    ORG     0000H
    LJMP    MAIN

MAIN:
    CLR     P1.0
    SETB    P1.0

    CLR     P3.0
    SETB    P3.0

    CLR     P4.0
    SETB    P4.0

LOOP:
    MOV     P2,#00H
    MOV     P2,#FFH
    
    LJMP    LOOP

    END