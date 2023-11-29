    P4      SFR    E8H

    abcd    DB     1, 1, 22H

    ORG     0800H
    LJMP    MAIN

    ORG     0100H
MAIN:
    CLR     P4.0
    ACALL    DELAY
    SETB    P4.0
    CLR     P4.1
    ACALL    DELAY
    SETB    P4.1
    CLR     P4.2
    ACALL    DELAY
    SETB    P4.2
    CLR     P4.3
    ACALL    DELAY
    SETB    P4.3

LOOP:
    MOV     P4,#00H
    ACALL    DELAY
    MOV     P4,#FFH
    
    SJMP    LOOP

DELAY:
    MOV     R0,#20
    MOV     R1,#0
    MOV     R2,#0
    
;-----------------------------------------

    END

