mov a, r3
mov a, 50
mov a, @r1
mov a, #50h

mov r4, A
mov r4, 50
mov r4, #00001000b

mov 30, a
mov 30, r5
mov 30, 50h
mov 30, @r0
mov 30h, #50h

mov @r0, A
mov @r0, 50
mov @r0, #00001000b

mov C, P1.0
mov P1.0, C

MOV DPTR, #5000h

movc A, @A+DPTR
movc A, @A+PC

movx A, @DPTR
movx A, @R0
movx @R0, A
movx @DPTR, A

mul ab

nop

orl a, r0
orl a, 50h
orl a, @R0
orl a, #50h
orl 50h, a
orl 50h, #30h
orl c, P0.1
orl c, /P0.1

pop 30h
push 30h

ret 
reti

rl A
rlc A
rr A
rrc A

setb C
setb P1.0

sjmp 30h

subb A, r0
subb a, 30h
subb a, @r0
subb a, #30h

swap a

xch A, r0
xch a, 30h
xch a, @r0

xchd a, @r0

xrl A, r0
xrl a, 30h
xrl a, @r0
xrl a, #30h
xrl 30h, a
xrl 30h, #50h

add A, r0
add a, 30h
add a, @r0
add a, #30h

addc A, r0
addc a, 30h
addc a, @r0
addc a, #30h

ajmp 3033h

anl A, r0
anl a, 30h
anl a, @r0
anl a, #30h
anl 30h, a
anl 30h, #50h
anl c, P0.1
anl c, /P0.1

cjne a, 30h, 40h
cjne a, #30h, 40h
cjne r0, #30h, 40h
cjne @r0, #30h, 40h

clr A
clr c
clr P1.0

cpl A
cpl c
cpl P1.0

da A

dec A
dec r1
dec 30h
dec @r0

div ab

djnz r1, 40h
djnz 30h, 40h

inc A
inc r1
inc 30h
inc @r0
inc DPTR

jb P3.0, 40h
jbc P3.0, 40h
jc 40h

jmp @a+DPTR

jnb P0.3, 30h
jnc 30h
jnz 30h
jz 30h

lcall 3003h
ljmp 3003h

acall 8033h




