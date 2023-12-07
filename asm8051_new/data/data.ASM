; P0          SFR        80H
; P00         SBIT       P0.0
; P01         SBIT       P0^1
; P1          DATA       90H
; P2          SFR        0A0H

; HELLO: 


; mov a, r3
; mov a, 50
; mov a, @r1
; mov a, #50h

; mov r4, A
; mov r4, 50
; mov r4, #00001000b

; mov 30, a
; mov 30, r5
; mov 30, 50h
; mov 30, @r0
; mov 30h, #50h

; mov @r0, A
; mov @r0, 50
; mov @r0, #00001000b

; mov C, P1.0
; mov P1.0, C

; MOV DPTR, #5000h

; movc A, @A+DPTR
; movc A, @A+PC

; movx A, @DPTR
; movx A, @R0
; movx @R0, A
; movx @DPTR, A

; push 30h
; pop 30h

; xch A, r0
; xch a, 30h
; xch a, @r0

; xchd a, @r0

; swap a

; add A, r0
; add a, 30h
; add a, @r0
; add a, #30h

; addc A, r0
; addc a, 30h
; addc a, @r0
; addc a, #30h

; subb A, r0
; subb a, 30h
; subb a, @r0
; subb a, #30h

; inc A
; inc r0
; inc 30h
; inc @r0
; inc DPTR

; dec A
; dec r0
; dec 30h
; dec @r0

; mul ab
; div ab
; da A

; clr A
; clr c
; clr P1.0

; cpl A
; cpl c
; cpl P1.0

; rl A
; rlc A
; rr A
; rrc A

; anl A, r0
; anl a, 30h
; anl a, @r0
; anl a, #30h
; anl 30h, a
; anl 30h, #50h
; anl c, P1.0
; anl c, /P1.0

; orl a, r0
; orl a, 50h
; orl a, @R0
; orl a, #50h
; orl 50h, a
; orl 50h, #30h
; orl c, P1.0
; orl c, /P1.0

; xrl A, r0
; xrl a, 30h
; xrl a, @r0
; xrl a, #30h
; xrl 30h, a
; xrl 30h, #50h

; ajmp 3033h
; sjmp 30h
; ljmp 3003h
; jmp @a+DPTR
; jz 30h
; jnz 30h

; cjne a, 30h, 40h
; cjne a, #30h, 40h
; cjne r0, #30h, 40h
; cjne @r0, #30h, 40h

; djnz r0, 40h
; djnz 30h, 40h

; acall 8033h
; lcall 3003h

; ret 
; reti
; nop

; setb C
; setb P1.0

; jc 40h
; jnc 30h
; jb P1.0, 40h
; jnb P1.0, 30h
; jbc P1.0, 40h

org 00h


END