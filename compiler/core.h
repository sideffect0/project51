
char* NLBL[57]={"MOV","MOVC","MOVX","PUSH","POP","XCH","XCHD","ADD","ADDC","SUBB","INC","DEC","MUL","DIV","DA","ANL","ORL","XRL","CPL","CLR","RL","RLC","RR","RRC","SWAP","SETB","ACALL","LCALL","RET","RETI","AJMP","SJMP","LJMP","JMP","JZ","JNZ","JC","JNC","JB","JNB","JBC","CJNE","DJNZ","NOP","A","B","R0","R1","R2","R3","R4","R5","R6","R7","DPTR","DPL","DPH"};

/* variable to store the source filename*/
char* SourceFile;
char* Error="";
/* 'Position' indicates the begining and end of file ( from '{' to '}' )*/
unsigned short int LineCount=1,tmpLC=1;
/*Index of the array for storing fetched chars*/
int ID;
/*'Code' stores the charactors temperorly*/
char ch,Code[25];

bool LabelCheck=true;

/*This structure will be written to the Object file*/
struct objectcode {
       long int LineNo;
       long int Opcode;
       long int Operand1;
       long int Op1IsAdd;
       long int Op1HoldsAdd;
       long int Operand2;
       long int Op2IsAdd;
       long int Op2HoldsAdd;
       long int IVal;
       long int JmpID;
       long int JmpTag;
}ObjectCode;

/* Status structor*/
struct status{
       bool Error;
       bool Opcode;
       bool Operand1;
       bool Operand2;
       bool Operand3;
       bool FetchStarted;
       bool Location;
       bool Value;
       bool StartMem;
       bool EndMem;
       bool StartOrg;
       bool EndOrg;
       bool Label;
       bool Memory;
       bool Data;
       bool JmpStmnt;
       bool Cjne;
       bool Djnz;
       bool Comment;
       bool QMode;
       bool Op2;
}Status; 

// external rom(DPTR)
struct Mem {
       long int Location; 
       long int Value;      
}Mem;

struct label {
       char Name[10];
       unsigned short LineNo;
}Label;


class SIM51 {
      
      public:
            
             void Reset();
             void CheckOpcode();
             void CheckOperand(int);
}S;

/* this function resets the status structor along with the 'Code' and 'ID'*/
void SIM51::Reset() {
     strcpy(Code,"");
     Status.Opcode=false;
     Status.FetchStarted=false;
     Status.Operand1=false;
     Status.Operand2=false;
     ObjectCode.IVal=false;
     ObjectCode.LineNo=0;
     ObjectCode.Opcode=0;
     ObjectCode.Operand1=0;
     ObjectCode.Operand2=0;
     ObjectCode.JmpID=0;
     ObjectCode.JmpTag=false;
     ObjectCode.Op1IsAdd=false;
     ObjectCode.Op2IsAdd=false;
     ObjectCode.Op1HoldsAdd=false;
     ObjectCode.Op2HoldsAdd=false;
     Status.Operand3=false;
     Status.Data=false;
     Status.Memory=false;
     Status.Op2=false;
     ID=0;
}

/* Function used to check the Opcode*/
void SIM51::CheckOpcode() {
     
     if (strcmp(Code,"MOV")==0||strcmp(Code,"mov")==0) {
        ObjectCode.Opcode=MOV;
        Status.FetchStarted=false;
        Status.Opcode=true;
        Status.Op2=true;
        Status.JmpStmnt=false;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"MOVC")==0||strcmp(Code,"movc")==0) {
        ObjectCode.Opcode=MOVC;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.Op2=true;
        Status.JmpStmnt=false;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"MOVX")==0||strcmp(Code,"movx")==0) {
        ObjectCode.Opcode=MOVX;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }/*
     else if (strcmp(Code,"PUSH")==0||strcmp(Code,"push")==0) {
        ObjectCode.Opcode=PUSH;
        Status.FetchStarted=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
     else if (strcmp(Code,"POP")==0||strcmp(Code,"pop")==0) {
        ObjectCode.Opcode=POP;
        Status.FetchStarted=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
     */else if (strcmp(Code,"XCG")==0||strcmp(Code,"xcg")==0) {
        ObjectCode.Opcode=XCH;
        Status.FetchStarted=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"XCGD")==0||strcmp(Code,"xcgd")==0) {
        ObjectCode.Opcode=XCHD;
        Status.FetchStarted=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
// data transfer instructions
        else if (strcmp(Code,"ADD")==0||strcmp(Code,"add")==0) {
        ObjectCode.Opcode=ADD;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"ADDC")==0||strcmp(Code,"addc")==0) {
        ObjectCode.Opcode=ADDC;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"SUBB")==0||strcmp(Code,"subb")==0) {
        ObjectCode.Opcode=SUBB;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"INC")==0||strcmp(Code,"inc")==0) {
        ObjectCode.Opcode=INC;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"DEC")==0||strcmp(Code,"dec")==0) {
        ObjectCode.Opcode=DEC;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"MUL")==0||strcmp(Code,"mul")==0) {
        ObjectCode.Opcode=MUL;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"DIV")==0||strcmp(Code,"div")==0) {
        ObjectCode.Opcode=DIV;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"DA")==0||strcmp(Code,"da")==0) {
        ObjectCode.Opcode=DA;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
// arithmetic instructions
     else if (strcmp(Code,"ANL")==0||strcmp(Code,"anl")==0) {
        ObjectCode.Opcode=ANL;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"ORL")==0||strcmp(Code,"orl")==0) {
        ObjectCode.Opcode=ORL;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"XRL")==0||strcmp(Code,"xrl")==0) {
        ObjectCode.Opcode=XRL;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        Status.Op2=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"CPL")==0||strcmp(Code,"cpl")==0) {
        ObjectCode.Opcode=CPL;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"CLR")==0||strcmp(Code,"clr")==0) {
        ObjectCode.Opcode=CLR;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
// logical instructions
     else if (strcmp(Code,"RL")==0||strcmp(Code,"rl")==0) {
        ObjectCode.Opcode=RL;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"RLC")==0||strcmp(Code,"rlc")==0) {
        ObjectCode.Opcode=RLC;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"RR")==0||strcmp(Code,"rr")==0) {
        ObjectCode.Opcode=RR;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"RRC")==0||strcmp(Code,"rrc")==0) {
        ObjectCode.Opcode=RRC;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"SWAP")==0||strcmp(Code,"swap")==0) {
        ObjectCode.Opcode=SWAP;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
// rotate intructions (byte/bit level)
   else if (strcmp(Code,"SETB")==0||strcmp(Code,"setb")==0) {
        ObjectCode.Opcode=SETB;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=false;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }
     
     else if (strcmp(Code,"ACALL")==0||strcmp(Code,"acall")==0) {
        ObjectCode.Opcode=ACALL;
        Status.FetchStarted=false;  
        Status.JmpStmnt=true;                          
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"LCALL")==0||strcmp(Code,"lcall")==0) {
        ObjectCode.Opcode=LCALL;
        Status.FetchStarted=false;                            
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"RET")==0||strcmp(Code,"ret")==0) {
        ObjectCode.Opcode=RET;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"SJMP")==0||strcmp(Code,"sjmp")==0) {
        ObjectCode.Opcode=SJMP;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"LJMP")==0||strcmp(Code,"ljmp")==0) {
        ObjectCode.Opcode=LJMP;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JMP")==0||strcmp(Code,"jmp")==0) {
        ObjectCode.Opcode=JMP;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JZ")==0||strcmp(Code,"jz")==0) {
        ObjectCode.Opcode=JZ;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JC")==0||strcmp(Code,"jc")==0) {
        ObjectCode.Opcode=JC;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JNZ")==0||strcmp(Code,"jnz")==0) {
        ObjectCode.Opcode=JNZ;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JNC")==0||strcmp(Code,"jnc")==0) {
        ObjectCode.Opcode=JNC;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JB")==0||strcmp(Code,"jb")==0) {
        ObjectCode.Opcode=JB;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JNB")==0||strcmp(Code,"jnb")==0) {
        ObjectCode.Opcode=JNB;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"JBC")==0||strcmp(Code,"jbc")==0) {
        ObjectCode.Opcode=JBC;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"CJNE")==0||strcmp(Code,"cjne")==0) {
        ObjectCode.Opcode=CJNE;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Cjne=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"DJNZ")==0||strcmp(Code,"djnz")==0) {
        ObjectCode.Opcode=DJNZ;
        Status.FetchStarted=false;
        Status.JmpStmnt=true;
        Status.Djnz=true;
        Status.Opcode=true;
        strcpy(Code,"");
        ID=0;
     }else if (strcmp(Code,"NOP")==0||strcmp(Code,"nop")==0) {
        ObjectCode.Opcode=NOP;
        Status.FetchStarted=false;                            
        Status.Opcode=true;
        Status.JmpStmnt=false;
        strcpy(Code,"");
        ID=0;
     }
     else if (strcmp(Code,"")!=0 && Status.JmpStmnt == false ){
         Status.Error=true;
         Status.JmpStmnt=false;
     }
// bit level intructions

                         

}/* end of Function used to check the Opcode*/

/*function used to check the First operand*/
void SIM51::CheckOperand(int Type) {
       
      if (strcmp(Code,"A")==0||strcmp(Code,"a")==0) {
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=A;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=A;
         }
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }   
      else if (strcmp(Code,"B")==0||strcmp(Code,"b")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=B;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=B;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R0")==0||strcmp(Code,"r0")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R0;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R0;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R1")==0||strcmp(Code,"r1")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R1;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R1;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R2")==0||strcmp(Code,"r2")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R2;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R2;
         }         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R3")==0||strcmp(Code,"r3")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R3;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R3;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R4")==0||strcmp(Code,"r4")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R4;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R4;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R5")==0||strcmp(Code,"r5")==0) {

         Status.FetchStarted=false;
        if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R5;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R5;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R6")==0||strcmp(Code,"r6")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R6;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R6;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"R7")==0||strcmp(Code,"r7")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R7;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R7;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"DPTR")==0||strcmp(Code,"dptr")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=DPTR;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=DPTR;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@A")==0||strcmp(Code,"@a")==0) {
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=A;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=A;
              ObjectCode.Op2HoldsAdd=true;
         }
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }   
      else if (strcmp(Code,"@B")==0||strcmp(Code,"@b")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=B;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=B;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R0")==0||strcmp(Code,"@r0")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R0;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R0;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R1")==0||strcmp(Code,"@r1")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R1;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R1;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R2")==0||strcmp(Code,"@r2")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R2;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R2;
              ObjectCode.Op2HoldsAdd=true;
         }         
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R3")==0||strcmp(Code,"@r3")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R3;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R3;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R4")==0||strcmp(Code,"@r4")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R4;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R4;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R5")==0||strcmp(Code,"@r5")==0) {

         Status.FetchStarted=false;
        if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R5;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R5;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R6")==0||strcmp(Code,"@r6")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R6;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R6;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@R7")==0||strcmp(Code,"@r7")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=R7;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=R7;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      else if (strcmp(Code,"@DPTR")==0||strcmp(Code,"@dptr")==0) {
         Status.FetchStarted=false;
         if ( Type == 1 ) {
             Status.Operand1=true;
             ObjectCode.Operand1=DPTR;
             ObjectCode.Op1HoldsAdd=true;
         }
         else {
              Status.Operand2=true;
              ObjectCode.Operand2=DPTR;
              ObjectCode.Op2HoldsAdd=true;
         }
         strcpy(Code,"");
         ID=0;
      }
      
      /*                                  */
      else if (strcmp(Code,"C")==0||strcmp(Code,"c")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=CY;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"AC")==0||strcmp(Code,"ac")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=AC;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"FO")==0||strcmp(Code,"fo")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=FO;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"RS0")==0||strcmp(Code,"rs0")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=RS0;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"RS1")==0||strcmp(Code,"rs1")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=RS1;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"OV")==0||strcmp(Code,"ov")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=OV;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P")==0||strcmp(Code,"p")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }
       
       /*    PORT-0    */
       else if (strcmp(Code,"P0.0")==0||strcmp(Code,"p0.0")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_0;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.1")==0||strcmp(Code,"p0.1")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_1;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.2")==0||strcmp(Code,"p0.2")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_2;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.3")==0||strcmp(Code,"p0.3")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_3;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.4")==0||strcmp(Code,"p0.4")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_4;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.5")==0||strcmp(Code,"p0.5")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_5;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.6")==0||strcmp(Code,"p0.6")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_6;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P0.7")==0||strcmp(Code,"p0.7")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P0_7;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }
       
       /*    PORT-1    */
       else if (strcmp(Code,"P1.0")==0||strcmp(Code,"p1.0")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_0;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.1")==0||strcmp(Code,"p1.1")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_1;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.2")==0||strcmp(Code,"p1.2")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_2;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.3")==0||strcmp(Code,"p1.3")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_3;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.4")==0||strcmp(Code,"p1.4")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_4;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.5")==0||strcmp(Code,"p1.5")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_5;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.6")==0||strcmp(Code,"p1.6")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_6;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P1.7")==0||strcmp(Code,"p1.7")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P1_7;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }
       
       
       else if (strcmp(Code,"P2.0")==0||strcmp(Code,"p2.0")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_0;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.1")==0||strcmp(Code,"p2.1")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_1;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.2")==0||strcmp(Code,"p2.2")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_2;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.3")==0||strcmp(Code,"p2.3")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_3;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.4")==0||strcmp(Code,"p2.4")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_4;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.5")==0||strcmp(Code,"p2.5")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_5;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.6")==0||strcmp(Code,"p2.6")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_6;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P2.7")==0||strcmp(Code,"p2.7")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P2_7;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }
       
       else if (strcmp(Code,"P3.0")==0||strcmp(Code,"p3.0")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_0;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.1")==0||strcmp(Code,"p3.1")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_1;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.2")==0||strcmp(Code,"p3.2")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_2;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.3")==0||strcmp(Code,"p3.3")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_3;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.4")==0||strcmp(Code,"p3.4")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_4;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.5")==0||strcmp(Code,"p3.5")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_5;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.6")==0||strcmp(Code,"p3.6")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_6;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }else if (strcmp(Code,"P3.7")==0||strcmp(Code,"p3.7")==0) {
         Status.Operand1=true;
         ObjectCode.Operand1=P3_7;
         Status.FetchStarted=false;
         strcpy(Code,"");
         ID=0;
       }
       
       
      else if (strcmp(Code,"")!=0 && Status.JmpStmnt == false ){
         Status.Error=true;
     }
      
}/*end of function used to check the First operand*/
