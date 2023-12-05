
P0          SFR        80H
P00         SBIT         P0.0
P01         SBIT         P0.1
P02         SBIT         P0.2
P03         SBIT         P0.3
P04         SBIT         P0.4
P05         SBIT         P0.5
P06         SBIT         P0.6
P07         SBIT         P0.7

SP          SFR        81H
DPL         SFR        82H
DPH         SFR        83H
PCON        SFR        87H
                        
TCON        SFR        88H
TF1         SBIT         TCON.7
TR1         SBIT         TCON.6
TF0         SBIT         TCON.5
TR0         SBIT         TCON.4
IE1         SBIT         TCON.3
IT1         SBIT         TCON.2
IE0         SBIT         TCON.1
IT0         SBIT         TCON.0
                        
TMOD        SFR        89H
TL0         SFR        8AH
TL1         SFR        8BH
TH0         SFR        8CH
TH1         SFR        8DH
AUXR        SFR        8EH
                        
P1          SFR        90H
P10         SBIT         P1.0
P11         SBIT         P1.1
P12         SBIT         P1.2
P13         SBIT         P1.3
P14         SBIT         P1.4
P15         SBIT         P1.5
P16         SBIT         P1.6
P17         SBIT         P1.7

T2          SBIT         P1.0
T2EX        SBIT         P1.1

SCON        SFR        98H
SM0         SBIT         SCON.7
SM1         SBIT         SCON.6
SM2         SBIT         SCON.5
REN         SBIT         SCON.4
TB8         SBIT         SCON.3
RB8         SBIT         SCON.2
TI          SBIT         SCON.1
RI          SBIT         SCON.0
                        
SBUF        SFR        99H
                        
P2          SFR        A0H
P20         SBIT         P2.0
P21         SBIT         P2.1
P22         SBIT         P2.2
P23         SBIT         P2.3
P24         SBIT         P2.4
P25         SBIT         P2.5
P26         SBIT         P2.6
P27         SBIT         P2.7
                        
AUXR1       SFR        A2H
                        
IE          SFR        A8H
EA          SBIT         IE.7
ET2         SBIT         IE.5
ES          SBIT         IE.4
ET1         SBIT         IE.3
EX1         SBIT         IE.2
ET0         SBIT         IE.1
EX0         SBIT         IE.0
                        
SADDR       SFR        A9H
                        
P3          SFR        B0H
P30         SBIT         P3.0
P31         SBIT         P3.1
P32         SBIT         P3.2
P33         SBIT         P3.3
P34         SBIT         P3.4
P35         SBIT         P3.5
P36         SBIT         P3.6
P37         SBIT         P3.7
                        
RD          SBIT         P3.7
WR          SBIT         P3.6
T1          SBIT         P3.5
T0          SBIT         P3.4
INT1        SBIT         P3.3
INT0        SBIT         P3.2
TXD         SBIT         P3.1
RXD         SBIT         P3.0
                        
IPH         SFR        B7H
 
IP          SFR        B8H
PT2         SBIT         IP.5
PS          SBIT         IP.4
PT1         SBIT         IP.3
PX1         SBIT         IP.2
PT0         SBIT         IP.1
PX0         SBIT         IP.0
                        
SADEN       SFR        B9H

XICON       SFR        C0H
PX3         SBIT         XICON.7
EX3         SBIT         XICON.6
IE3         SBIT         XICON.5
IT3         SBIT         XICON.4
PX2         SBIT         XICON.3
EX3         SBIT         XICON.2
IE2         SBIT         XICON.1
IT2         SBIT         XICON.0

T2CON       SFR        C8H
TF2         SBIT         T2CON.7
EXF2        SBIT         T2CON.6
RCLK        SBIT         T2CON.5
TCLK        SBIT         T2CON.4
EXEN2       SBIT         T2CON.3
TR2         SBIT         T2CON.2
C_T2        SBIT         T2CON.1
CP_RL2      SBIT         T2CON.0

T2MOD       SFR        C9H
RCAP2L      SFR        CAH
RCAP2H      SFR        CBH
TL2         SFR        CCH
TH2         SFR        CDH
                        
PSW         SFR        D0H
CY          SBIT         PSW.7
AC          SBIT         PSW.6
F0          SBIT         PSW.5
RS1         SBIT         PSW.4
RS0         SBIT         PSW.3
OV          SBIT         PSW.2
F1          SBIT         PSW.1
P           SBIT         PSW.0

ACC         SFR        E0H

WDT_CONTR   SFR        E1H
ISP_DATA    SFR        E2H
ISP_ADDRH   SFR        E3H
ISP_ADDRL   SFR        E4H
ISP_CMD     SFR        E5H
ISP_TRIG    SFR        E6H
ISP_CONTR   SFR        E7H

P4          SFR        E8H
P40         SBIT         P4.0
P41         SBIT         P4.1
P42         SBIT         P4.2
P43         SBIT         P4.3
P44         SBIT         P4.4
P45         SBIT         P4.5
P46         SBIT         P4.6
P47         SBIT         P4.7

B           SFR        F0H
