ORG 0000h ; 注释
people  EQU 05h ; 测试变量
; 注释
nihao: mov b, 45
mov h, 45

hello:
    db 1, 1,    1
    mov @a, #0fh
    mov e, /0b
    mov e, $f
    mov e,P1.0
    mov @a,DPTR
    mov c,@a+DPTR
    adbc
END

asddfghj
END
