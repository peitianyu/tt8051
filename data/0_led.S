        ORG     0000H            ;程序执行的起始地址               
        LJMP      Main            ;跳转到main函数
       
        ORG     0100H            ;程序跳转到0100H开始执行
;方法1：       
    Main:                        ;最简单的LED闪烁
        MOV        P1,#0FEH        ;11111110
        LCALL    Delay            ;调用延时函数
 
        MOV        P1,#0FDH        ;11111101
        LCALL    Delay            ;调用延时函数
       
        MOV        P1,#0FBH        ;11111011
        LCALL    Delay            ;调用延时函数
 
        MOV        P1,#0F7H        ;11110111
        LCALL    Delay            ;调用延时函数
       
        MOV        P1,#0EFH        ;11101111
        LCALL    Delay            ;调用延时函数
 
        MOV        P1,#0DFH        ;11011111
        LCALL    Delay            ;调用延时函数
       
        MOV        P1,#0BFH        ;10111111
        LCALL    Delay            ;调用延时函数
 
        MOV        P1,#07FH        ;01111111
        LCALL    Delay            ;调用延时函数
       
        SJMP    Main            ;跳转到main，死循环
 
 
;方法2：        ;通过数组的遍历取值来流水
 
;方法3：        ;通过左移流水灯，RR是右移
 
    Delay:                        ;t=t1*8=1s，修改R5的值即可更改时间(不精准)
        MOV        R5, #08H        ;单周期，8次
    D1:                            ;t1=t2*250=500*250=125000
        MOV        R6, #0FAH       
    D2:                       
        MOV        R7, #0FAH        ;t2=250*2=500
    D3:       
        DJNZ    R7, D3            ;R7=250自减，直到为0跳出循环，双周期
        DJNZ    R6, D2            ;R6=250自减，直到为0跳出循环，双周期
        DJNZ    R5, D1            ;R5=100自减，直到为0跳出循环，双周期
        RET
    LED_table:
        DB 0FEH, 0FDH, 0FBH, 0F7H, 0EFH, 0DFH, 0BFH, 07FH
 
    END    