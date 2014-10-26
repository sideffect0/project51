/*General Tags Declarations*/

#define NONE 0
#define BEGIN 1
#define END 2


#define BANK0 0
#define BANK1 1
#define BANK2 2
#define BANK3 3

#define PORT0  0
#define PORT1  1
#define PORT2  2
#define PORT3  3

#define CY    0
#define AC    1
#define FO    2
#define RS0   3
#define RS1   4
#define OV    5
#define USR   6
#define P     7


/*Numeric code for instructions*/

#define MOV    101
//#define MOVC   102
#define MOVX   103
//#define PUSH   104//1
//#define POP    105//1
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
//#define ACALL  127//1
//#define LCALL  128//1
//#define RET    129//0
//#define RETI   130//0
//#define AJMP   131//1
//#define SJMP   132//1
//#define LJMP   133//1
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
#define R0   10
#define R1   3
#define R2   4
#define R3   5
#define R4   6
#define R5   7
#define R6   8
#define R7   9

#define DPTR 2
#define DPL  11
#define DPH  12

#define _A    513
#define _B    514
#define _R0   515
#define _R1   516
#define _R2   517
#define _R3   518
#define _R4   519
#define _R5   520
#define _R6   521
#define _R7   522

#define _DPTR 523
#define _DPL  524
#define _DPH  525

