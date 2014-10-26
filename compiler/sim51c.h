/*General Tags Declarations*/

#define NONE 0
#define BEGIN 1
#define END 2

/*Numeric code for instructions*/

#define MOV    101
#define MOVC   102
#define MOVX   103
#define PUSH   104//1
#define POP    105//1
#define XCH    106
#define XCHD   107

/* DATA TRANSFER */

#define ADD     108
#define ADDC    109
#define SUBB    110
#define INC     111//1
#define DEC     112//1
#define MUL     113
#define DIV     114
#define DA      115//0

/* ARITHMETIC */

#define ANL  116
#define ORL  117
#define XRL  118
#define CPL  119//1
#define CLR  120//1

/* LOGICAL-BYTE LEVEL */

#define RL      121//1
#define RLC     122//1
#define RR      123//1
#define RRC     124//1
#define SWAP    125//1

/* ROTATE */


/* BIT LEVEL */

#define SETB    126//1

// below instructions have label operand
/* BRANCHING - PROGRAM CONTROL */
#define ACALL  127//1
#define LCALL  128//1
#define RET    129//0
//#define RETI   130//0
//#define AJMP   131//1
#define SJMP   132//1
#define LJMP   133//1
#define JMP    134// spcial case
/* UNCONDITIONAL-JUMP */

#define JZ     135//1
#define JNZ    136//1
#define JC     137//1
#define JNC    138//1
#define JB     139//1
#define JNB    140//1
#define JBC    141//1
#define CJNE   142//3
#define DJNZ   143//2
#define NOP    144//0
/* CONDITIONAL- JUMP */

/* Registers */

#define A    0
#define B    1
#define DPTR 2
#define R1   3
#define R2   4
#define R3   5
#define R4   6
#define R5   7
#define R6   8
#define R7   9
#define R0   10

#define CY   500
#define AC   501
#define FO   502
#define RS0  503
#define RS1  504
#define OV   505
#define USR  506
#define P    507

#define P0_0 600
#define P0_1 601
#define P0_2 602
#define P0_3 603
#define P0_4 604
#define P0_5 605
#define P0_6 606
#define P0_7 607

#define P1_0 700
#define P1_1 701
#define P1_2 702
#define P1_3 703
#define P1_4 704
#define P1_5 705
#define P1_6 706
#define P1_7 707

#define P2_0 800
#define P2_1 801
#define P2_2 802
#define P2_3 803
#define P2_4 804
#define P2_5 805
#define P2_6 806
#define P2_7 807

#define P3_0 900
#define P3_1 901
#define P3_2 902
#define P3_3 903
#define P3_4 904
#define P3_5 905
#define P3_6 906
#define P3_7 907
