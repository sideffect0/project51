#include <fstream>
#include <iostream>
#include <conio.h>

#include "sim51c.h"
#include "convert.h" 
#include "core.h"

using namespace std;

void GenLblList(char*);
unsigned int CheckLabel(char*,char*);


/*The main function*/
int main(int argc, char *argv[]) {
    
    /* Check the no. of arguments passed. Exit if no argument.
	   But there is always 1 default (0th) argument
	   and it is the path to the executable itsself */
	   
       if ( argc==1 )
       return 1;

    if ( argc == 3 ) {
       if ( strcmp(argv[1],"-q") == 0)
          Status.QMode=true;
          SourceFile=argv[2];
    }
    else
        SourceFile=argv[1];

    char *tmp;
    tmp=SplitPath(SourceFile,FILE_EXT);
    
    ofstream ObjectStream,MemStream,ErrStream;
    char FTmp0[strlen(SourceFile)],FTmp1[strlen(SourceFile)];
        
    strcpy(FTmp0,"");strcpy(FTmp1,"");
    strcpy(FTmp1,SplitPath(SourceFile,FILE_PATH));

    strcpy(FTmp0,"");
    strcpy(FTmp0,FTmp1);
    strcat(FTmp0,".err");
    ErrStream.open(FTmp0,ios::binary);

    if ( strcmp(tmp,"asm")!=0 ) {
         
         if ( Status.QMode == true ) {          
              ErrStream<<"ERROR: Not a .ASM file.";
              ErrStream.close();
              return 0;
         }else {
               cout<<"ERROR: Not a .ASM file.\nPress any key to continue.....";
               getch();
               return 0;
         }
         
    }
    ifstream SourceStream;
    SourceStream.open(SourceFile);
    if ( !SourceStream ) {
         if ( Status.QMode == true ) {
              ErrStream<<"ERROR: File do not exist.";
              ErrStream.close();
         }else {
               cout<<"ERROR: File do not exist.\nPress any key to continue.....";
               getch();
               return 0;
         }

    }
    
    strcpy(FTmp0,FTmp1);
    strcat(FTmp0,".obj");
    ObjectStream.open(FTmp0,ios::trunc|ios::binary);
    strcpy(FTmp0,"");
    strcpy(FTmp0,FTmp1);
    strcat(FTmp0,".mem");
    MemStream.open(FTmp0,ios::trunc);
    strcpy(FTmp0,"");
    strcpy(FTmp0,FTmp1);
    strcat(FTmp0,".lbl");

    GenLblList(FTmp0);
    
    S.Reset();
    
    Status.Error=false;
    Label.LineNo=0;
    
    bool DivMul=false;
    
    while (SourceStream) {
          SourceStream.get(ch);
          switch (ch) {
                 case '{':
                          if ( (strcmp(Code,"START_MEM")==0||strcmp(Code,"start_mem")==0) && Status.Comment == false ) {
                               Status.StartMem=true;
                               strcpy(Code,"");
                               ID=0;
                          }       
                          else if ( (strcmp(Code,"ORG")==0||strcmp(Code,"org")==0) && Status.Comment == false) {
                               Status.StartOrg=true;
                               Status.StartMem=false;
                               strcpy(Code,"");
                               ID=0;
                          }
                          else if ( strcmp(Code,"")!=0 ) {
                               if ( Status.QMode == true ) {

                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefined symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefined symbol";
                                           return 0;
                                     }
                          }
                          break;
                 case '}':
                          if ( Status.StartMem==true && Status.Comment == false ) {
                               Status.EndMem=true;
                               Status.StartMem=false;
                               S.Reset();
                          }
                          else if ( Status.StartOrg==true && Status.Comment == false ) {
                               Status.EndOrg=true;
                               Status.StartOrg=false;
                               S.Reset();
                          }
                          else if ( Status.QMode == true ) {
                                  ErrStream<<"Error! Line: ";
                                  ErrStream<<LineCount;
                                  ErrStream<<" - Unexpected '}'.";
                                  ErrStream.close();
                                  return 0;
                          }else {
                                cout<<"Error! Line: "<<LineCount<<" - Unexpected '}'.";
                                return 0;
                          }
                          
                          break;
                 case '\n':
                           if( DivMul==true && Status.Comment == false)
                               DivMul=false;
                           
						   if ( Status.FetchStarted==true && Status.Opcode==false && Status.StartOrg == true && Status.Comment == false ) {
                                Status.Error=false;
                                S.CheckOpcode();
                                
                                if ( Status.Error == true )
                                {
                                     if ( Status.QMode == true ) {

                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefined symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefined symbol";
                                           return 0;
                                     }
                                }
                           }
                           else if ( Status.Operand1==false && Status.Opcode == true && Status.Comment == false && Status.JmpStmnt == false ) {
                               
                             S.CheckOperand(1);
                             
                             if ( Status.Error == true )
                                {
                                     if ( Status.QMode == true ) {

                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefined symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefined symbol";
                                           return 0;
                                     }
                                }
                          }
                          
                          else if ( Status.Operand1==true && Status.Operand2 == false && Status.Comment == false && Status.JmpStmnt == false) {
                               
                             S.CheckOperand(2);
                             
                             if ( Status.Error == true )
                                {
                                     if ( Status.QMode == true ) {

                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefined symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefined symbol";
                                           return 0;
                                     }
                                }
                          }
                          else if ( ( Status.Operand1==false || Status.Operand2==false)&& Status.Op2==true && Status.JmpStmnt == false ) {
                             if ( Status.QMode == true ) {

                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefined symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefined symbol";
                                           return 0;
                                     }
                          }
                          
                           else if ( Status.JmpStmnt == true && Status.Comment == false ){
                               int tmp=CheckLabel(Code,FTmp0);
                               
                               
                               if ( tmp != 0 ) {
                                  ObjectCode.JmpTag=true;
                                  ObjectCode.JmpID=tmp;
                                  
                                /*if( Status.Cjne == true ) {  
                                  ObjectCode.LineNo=tmpLC;
                               ObjectStream.write((char*)&ObjectCode,sizeof(ObjectCode));
                               tmpLC++;
                               if ( Status.QMode == false ) {
                               cout<<"\nLine No.             : "<<ObjectCode.LineNo;
                               cout<<"\nOpcode               : "<<ObjectCode.Opcode;
                               cout<<"\nOperand1             : "<<ObjectCode.Operand1;
                               cout<<"\nOperand1 Is Add.     : "<<ObjectCode.Op1IsAdd;
                               cout<<"\nOperand1 Holds Add   : "<<ObjectCode.Op1HoldsAdd;
                               cout<<"\nOperand2             : "<<ObjectCode.Operand2;
                               cout<<"\nOperand2 Is Add.     : "<<ObjectCode.Op2IsAdd;
                               cout<<"\nOperand2 Holds Add   : "<<ObjectCode.Op2HoldsAdd;
                               cout<<"\nImmediate value.     : "<<ObjectCode.IVal;
                               cout<<"\nJmp ID               : "<<ObjectCode.JmpID;
                               cout<<"\nJmpTag.              : "<<ObjectCode.JmpTag;
                               cout<<"\n";
                               }}*/
                               
                               }
                          }
                          else if (Status.EndMem==true && (strcmp(Code,"END_MEM")==0||strcmp(Code,"end_mem")==0) && Status.Comment == false ) {
                               Status.StartMem=false;
                               Status.EndMem=false;
                               MemStream.close();
                          }
                          
                          if( Status.Opcode == true && Status.StartOrg == true && Status.Comment == false  ) {
                               ObjectCode.LineNo=tmpLC;
                               ObjectStream.write((char*)&ObjectCode,sizeof(ObjectCode));
                               tmpLC++;
                               if ( Status.QMode == false ) {
                               cout<<"\nLine No.             : "<<ObjectCode.LineNo;
                               cout<<"\nOpcode               : "<<ObjectCode.Opcode;
                               cout<<"\nOperand1             : "<<ObjectCode.Operand1;
                               cout<<"\nOperand1 Is Add.     : "<<ObjectCode.Op1IsAdd;
                               cout<<"\nOperand1 Holds Add   : "<<ObjectCode.Op1HoldsAdd;
                               cout<<"\nOperand2             : "<<ObjectCode.Operand2;
                               cout<<"\nOperand2 Is Add.     : "<<ObjectCode.Op2IsAdd;
                               cout<<"\nOperand2 Holds Add   : "<<ObjectCode.Op2HoldsAdd;
                               cout<<"\nImmediate value.     : "<<ObjectCode.IVal;
                               cout<<"\nJmp ID               : "<<ObjectCode.JmpID;
                               cout<<"\nJmpTag.              : "<<ObjectCode.JmpTag;
                               cout<<"\n";
                               }
                          }
                          S.Reset();
                          LineCount++;    
                          Status.Comment=false;
                          break;
                 case '\t':
                          break;
                 case ' ':
                          if ( Status.FetchStarted==true && Status.Opcode==false && Status.StartOrg == true && Status.Comment == false ) {
                               
                               Status.Error=false;
                               S.CheckOpcode();
                               if ( ObjectCode.Opcode == DIV || ObjectCode.Opcode == MUL )
                                  DivMul=true;

                               if ( Status.Error == true )
                               {
                                    if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefigned symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefigned symbol";
                                           return 0;
                                     
                               //cout<<"\nOpcode : "<<ObjectCode.Opcode<<"\nOperand1 : "<<ObjectCode.Operand1<<"\nOperand2 : "<<ObjectCode.Operand2<<"\nImmediate value : "<<ObjectCode.IVal<<"\nOperand3 : "<<ObjectCode.Operand3<<"\nOP1 is add. : "<<ObjectCode.Op1IsAdd<<"\nOP2 is add. : "<<ObjectCode.Op2IsAdd<<"\nOP1 holds add. : "<<ObjectCode.Op1HoldsAdd<<"\nOP2 holds add. : "<<ObjectCode.Op2HoldsAdd<<"\n";
                                           //return 0;
                                           }
                               }
                          }
                          break;
                 case ',':

                          if ( Status.FetchStarted==true && Status.Operand1==false && Status.StartOrg == true && Status.Comment == false ) {
                               //cout<<Status.Error;
                               S.CheckOperand(1);
                               if ( Status.Error == true )
                               {
                                    if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefigned symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefigned symbol";
                                           return 0;
                                     }
                               }	
                          }
                          if ( Status.Cjne == true && Status.Operand1 == true && Status.Operand2 == false && Status.Comment == false ) {
                             S.CheckOperand(2);
                             if ( Status.Error == true )
                               {
                                    if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Undefigned symbol.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Undefigned symbol";
                                           return 0;
                                     }
                               }
                          }
                          
                          break;
                 case 'h':
                      
                          if ( Status.StartMem==true && Status.Location==true && Status.EndMem == false && Status.Comment == false ) {

                              int temp=Hex2Dec(Code);

                              if ( temp>=0&&temp<=255){
                                   Mem.Value=temp;
                                   MemStream.write((char*)&Mem,sizeof(Mem));  
                                   S.Reset();
                                   cout<<Mem.Location<<" "<<Mem.Value<<"\n";
                              }else{
                                    if ( Status.QMode == true ) {
                                         ErrStream<<"Error! Line: ";
                                         ErrStream<<LineCount;
                                         ErrStream<<" - Value out of range.";
                                         ErrStream.close();
                                         return 0;
                                    }
                                    else{
                                          cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                          return 0;
                                    }
                              }
                              
                          }
                          else if ( Status.Data==true && strcmp(Code,"") != 0 && Status.Comment == false && Status.Operand2 == false ) {

                             int tmp=Hex2Dec(Code);

                             if ( (( tmp>=0 && tmp <= 255 && ObjectCode.Operand1 != DPTR) || ( tmp>=0 && tmp<= 65535 && ObjectCode.Operand1 == DPTR ))&& Status.Cjne == false ) {
                                  ObjectCode.IVal=true;
                                  ObjectCode.Operand2=tmp;
                                  ObjectCode.LineNo=tmpLC;
                                  
                                  ObjectStream.write((char*)&ObjectCode,sizeof(ObjectCode));
                                  tmpLC++;
                                  if ( Status.QMode == false ){
                                     cout<<"\nLine No.             : "<<ObjectCode.LineNo;
                                     cout<<"\nOpcode               : "<<ObjectCode.Opcode;
                                     cout<<"\nOperand1             : "<<ObjectCode.Operand1;
                                     cout<<"\nOperand1 Is Add.     : "<<ObjectCode.Op1IsAdd;
                                     cout<<"\nOperand1 Holds Add   : "<<ObjectCode.Op1HoldsAdd;
                                     cout<<"\nOperand2             : "<<ObjectCode.Operand2;
                                     cout<<"\nOperand2 Is Add.     : "<<ObjectCode.Op2IsAdd;
                                     cout<<"\nOperand2 Holds Add   : "<<ObjectCode.Op2HoldsAdd;
                                     cout<<"\nImmediate value.     : "<<ObjectCode.IVal;
                                     cout<<"\nJmp ID               : "<<ObjectCode.JmpID;
                                     cout<<"\nJmpTag.              : "<<ObjectCode.JmpTag;
                                     cout<<"\n";
                                     getch();
                                  }
                                  S.Reset();
                             }
                             else if ( tmp>=0 && tmp <= 255 && Status.Cjne == true ) {
                                  ObjectCode.IVal=true;
                                  ObjectCode.Operand2=tmp;
                                  Status.Operand2 = true;
                             }
                             else {
                                   if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Value out of range.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                           return 0;
                                     }
                             }
                             
                          }
                          
                          else if ( Status.Opcode == true && Status.Comment == false && Status.Data == false ) {
                               
                               if ( Status.Operand1 == false && strcmp(Code,"") != 0 ) {
                                    int tmp=Hex2Dec(Code);
                                    if ( tmp >= 32 && tmp <= 127 ) {
                                         ObjectCode.Op1IsAdd=true;
                                         ObjectCode.Operand1=tmp;
                                         Status.Operand1 = true;
                                    }
                                    else {
                                         if ( Status.QMode == true ) {
                                            ErrStream<<"Error! Line: ";
                                            ErrStream<<LineCount;
                                            ErrStream<<" - Value out of range.\n";
                                            ErrStream<<"Must be between ( 20h - 7fh ).";
                                            ErrStream.close();
                                            return 0;
                                         }else{
                                               cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                               return 0;
                                         }
                                    }
                               }
                               else if ( Status.Operand1 == true && Status.Operand2 == false && strcmp(Code,"") != 0 && Status.Comment == false ) {
                                    int tmp=Hex2Dec(Code);
                                   
                                    if ( tmp >= 32 && tmp <= 127 ) {
                                          ObjectCode.Op2IsAdd=true;
                                          ObjectCode.Operand2=tmp;
                                          Status.Operand2 = true;
                                    }
                                    else {
                                         if ( Status.QMode == true ) {
                                              ErrStream<<"Error! Line: ";
                                              ErrStream<<LineCount;
                                              ErrStream<<" - Value out of range.\n";
                                              ErrStream<<"Must be between ( 20h - 7fh ).";
                                              ErrStream.close();
                                              return 0;
                                          }else{
                                                cout<<"Error! Line: "<<LineCount<<" - Value out of range.\nMust be between ( 30h - 7fh ).";
                                                return 0;
                                          }
                                    }
                                    
                                         
                               }
                               
                          }
                          
                              
                          strcpy(Code,"");
                          ID=0;
                          break;
                  case 'H':
                          if ( Status.StartMem==true && Status.Location==true && Status.EndMem == false && Status.Comment == false ) {

                              int temp=Hex2Dec(Code);

                              if ( temp>=0&&temp<=255){
                                   Mem.Value=temp;
                                   MemStream.write((char*)&Mem,sizeof(Mem));  
                                   S.Reset();
                              }else{
                                    if ( Status.QMode == true ) {
                                         ErrStream<<"Error! Line: ";
                                         ErrStream<<LineCount;
                                         ErrStream<<" - Value out of range.";
                                         ErrStream.close();
                                         return 0;
                                    }else{
                                          cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                          return 0;
                                    }
                              }
                          }
                          else if ( Status.Data==true && strcmp(Code,"") != 0 && Status.Comment == false ) {

                             int tmp=Hex2Dec(Code);

                             if ( ( tmp>=0 && tmp <= 255 && ObjectCode.Operand1 != DPTR) || ObjectCode.Operand1 == DPTR ) {
                                  ObjectCode.IVal=true;
                                  ObjectCode.Operand2=tmp;
                                  
                                  ObjectCode.LineNo=tmpLC;
                                  ObjectStream.write((char*)&ObjectCode,sizeof(ObjectCode));
                                  tmpLC++;
                                  if ( Status.QMode == false ){
                                     cout<<"\nLine No.             : "<<ObjectCode.LineNo;
                                     cout<<"\nOpcode               : "<<ObjectCode.Opcode;
                                     cout<<"\nOperand1             : "<<ObjectCode.Operand1;
                                     cout<<"\nOperand1 Is Add.     : "<<ObjectCode.Op1IsAdd;
                                     cout<<"\nOperand1 Holds Add   : "<<ObjectCode.Op1HoldsAdd;
                                     cout<<"\nOperand2             : "<<ObjectCode.Operand2;
                                     cout<<"\nOperand2 Is Add.     : "<<ObjectCode.Op2IsAdd;
                                     cout<<"\nOperand2 Holds Add   : "<<ObjectCode.Op2HoldsAdd;
                                     cout<<"\nImmediate value.     : "<<ObjectCode.IVal;
                                     cout<<"\nJmp ID               : "<<ObjectCode.JmpID;
                                     cout<<"\nJmpTag.              : "<<ObjectCode.JmpTag;
                                     cout<<"\n";
                                     
                                     getch();
                                  }
                                  S.Reset();
                             }
                             else {
                                   if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Value out of range.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                           return 0;
                                     }
                             }
                          }
                          
                          else if ( Status.Opcode == true && Status.Comment == false ) {
                               
                               if ( Status.Operand1 == false && strcmp(Code,"") != 0 ) {
                                    int tmp=Hex2Dec(Code);
                                    if ( tmp >= 32 && tmp <= 127 ) {
                                         ObjectCode.Op1IsAdd=true;
                                         ObjectCode.Operand1=tmp;
                                         Status.Operand1 = true;
                                    }
                                    else {
                                         if ( Status.QMode == true ) {
                                            ErrStream<<"Error! Line: ";
                                            ErrStream<<LineCount;
                                            ErrStream<<" - Value out of range.\n";
                                            ErrStream<<"Must be between ( 20h - 7fh ).";
                                            ErrStream.close();
                                            return 0;
                                         }else{
                                               cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                               return 0;
                                         }
                                    }
                               }
                               else if ( Status.Operand1 == true && Status.Operand2 == false && strcmp(Code,"") != 0 && Status.Comment == false ) {
                                    int tmp=Hex2Dec(Code);
                                     if ( tmp >= 32 && tmp <= 127 && Status.Cjne == false ) {
                                         ObjectCode.Op2IsAdd=true;
                                         ObjectCode.Operand2=tmp;
                                         Status.Operand2 = true;
                                    }
                                    else if ( Status.Cjne == true ) {
                                         ObjectCode.Operand2=tmp;
                                         ObjectCode.IVal=true;
                                         Status.Operand2 = true;
                                    }
                                    else {
                                         if ( Status.QMode == true ) {
                                            ErrStream<<"Error! Line: ";
                                            ErrStream<<LineCount;
                                            ErrStream<<" - Value out of range.\n";
                                            ErrStream<<"Must be between ( 20h - 7fh ).";
                                            ErrStream.close();
                                            return 0;
                                         }else{
                                               cout<<"Error! Line: "<<LineCount<<" - Value out of range.\nMust be between ( 30h - 7fh ).";
                                               return 0;
                                         }
                                    }
                               }
                          }
                          
                              
                          strcpy(Code,"");
                          ID=0;
                          break;
                          
                 case '#':
                          if ( Status.Opcode==true && Status.Operand1 == true && Status.StartOrg == true && Status.Comment == false ) {
                               Status.Data=true;
                          }else if ( Status.Comment == false ) {
                                if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Unexpected '#'.";
                                        ErrStream.close();
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Unexpected '#'.";
                                           return 0;
                                     }
                          }
                          break;
                 case ':':
                          if (Status.StartMem==true && strcmp(Code,"") != 0 && Status.Comment == false ) {
                              int temp=Hex2Dec(Code);
                              if ( temp>=0&&temp<=65535)
                              {
                                 Mem.Location=temp;
                                 Status.Location=true;
                              }
                              else {
                                   if ( Status.QMode == true ) {
                                        ErrStream<<"Error! Line: ";
                                        ErrStream<<LineCount;
                                        ErrStream<<" - Value out of range.";
                                        ErrStream.close();
                                        return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" - Value out of range.";
                                           return 0;
                                     }
                             }
                          }
                          
                          
                          S.Reset();
                          break;
                 case ';':
                          Status.Comment=true;
                          break;
                 default :
                         if ( Status.Comment == false ) {
                            Status.FetchStarted=true;
                            Code[ID]=ch;
                            Code[ID+1]='\0';
                            ID++;
                         }
                          if ( DivMul == true && Status.Comment == false ) {
                             if ( Status.Operand1 == false ){
                                if ( ch=='a'||ch=='A' )
                                    ObjectCode.Operand1=A;
                                else if ( ch=='b'||ch=='B' )
                                    ObjectCode.Operand1=B;
                                else {
                                     if ( Status.QMode == true ) {
                                          ErrStream<<"Error! Line: ";
                                          ErrStream<<LineCount;
                                          ErrStream<<" Invalid register.";
                                          ErrStream.close();
                                          return 0;
                                     }else{
                                           cout<<"Error! Line: "<<LineCount<<" Invalid register.";
                                           return 0;
                                     }
                            
                                    
                                }
                                Status.Operand1=true;
                             }else if ( Status.Operand2 == false ) {
                                   if ( ch=='a'||ch=='A' )
                                      ObjectCode.Operand2=A;
                                    else if ( ch=='b'||ch=='B' )
                                          ObjectCode.Operand2=B;
                                    else {
                                          if ( Status.QMode == true ) {
                                             ErrStream<<"Error! Line: ";
                                             ErrStream<<LineCount;
                                             ErrStream<<" Invalid register.";
                                             ErrStream.close();
                                             return 0;
                                          }else{
                                                cout<<"Error! Line: "<<LineCount<<" Invalid register.";
                                                return 0;
                                          }
                                    }
                                    Status.Operand2=true;
                             }
                             strcpy(Code,"");
                             ID=0;
                          }
                          break;
                          
          }   //switch (ch)
    }     //while (SourceStream) 

   SourceStream.close();
   ObjectStream.close();
   ErrStream.close();
   return EXIT_SUCCESS;
   
}

void GenLblList(char* lss) {
     ifstream ls;ls.open(SourceFile);
     ofstream lo;lo.open(lss);
     char lc;
     short int lcnt=1,cnt=0;
     short int instcnt=0;
     char lbl[25];
     bool lblstatus=false;
     strcpy(lbl,"");
     while ( ls ) {
           ls.get(lc);
           switch ( lc ) {
                  case '\n':
                           /* if ( Status.StartOrg==true && strcmp(lbl,"")!=0 && Status.Comment == false ){                     
                               cout<<lbl<<"\n\n";
                               for ( int i=0;i<57;i++ ) {
                                    if ( strcmpi(lbl,NLBL[i])==0 ){
                                         instcnt++;
                                         if ( lblstatus==true){
                                            Label.LineNo=instcnt;
                                            //cout<<instcnt<<"\n";
                                            lo.write((char*)&Label,sizeof(Label));
                                            lblstatus=false;
                                         }
                                       break;
                                    }
                               }
                            }*/
                            lcnt++;
                            cnt=0;
                            strcpy(lbl,"");
                            Status.Comment =false;
                            break;
                  case ':':
                           if ( strcmp(lbl,"")!=0 && Status.StartOrg == true  && Status.Comment == false ) {

                                bool tag=false;
                                for ( int i=0;i<57;i++ ) {
                                    if ( strcmpi(lbl,NLBL[i])==0 ){
                                       tag=true;
                                       break;
                                    }
                                }
                                if ( tag==false ) {
                                     strcpy(Label.Name,lbl);
                                     cout<<lbl<<"\n\n";
                                     lblstatus=true;
                                }else {
                                      cout<<"Error! Line: "<<lcnt<<" Invalid label name.";
                                      exit(0);
                                }
                           }
                           lc=0;
                           cnt=0;
                           strcpy(lbl,"");
                           break;
                  case ' ':
                       if ( Status.StartOrg==true && strcmp(lbl,"")!=0 && Status.Comment == false ){
                            cout<<lbl<<"\n\n";
                            for ( int i=0;i<57;i++ ) {
                                    if ( strcmpi(lbl,NLBL[i])==0 ){
                                         instcnt++;
                                         if ( lblstatus==true){
                                            Label.LineNo=instcnt;
                                            cout<<instcnt<<"\n";
                                            lo.write((char*)&Label,sizeof(Label));
                                            lblstatus=false;
                                         }
                                       break;
                                    }
                                }cnt=0;
                       strcpy(lbl,"");
                       }
                       break;
                  case '{':
                           if ( strcmp(lbl,"ORG")==0||strcmp(lbl,"org")==0 && Status.Comment == false ) {
                               Status.StartOrg=true;
                               strcpy(Code,"");
                               ID=0;
                          }
                           break;
                  case '}':break;
                  case '\t':break;
                  case ';':Status.Comment=true;
                           break;
                  default:
                          if ( Status.Comment == false ){
                             lbl[cnt]=lc;
                             lbl[cnt+1]='\0';
                             cnt++;
                          }
                          break;
           }
     }
     Status.StartOrg = false;
     Status.Comment = false;
}

unsigned int CheckLabel(char* __Label, char* __FN ) {
    ifstream fin;
    fin.open(__FN,ios::out);
    while ( fin ) {
          fin.read((char*)&Label,sizeof(Label));
          if ( Label.LineNo != 0 && strcmp( __Label,Label.Name ) == 0 ) {
             return Label.LineNo;
          }
          Label.LineNo=0;
    }
    return 0;
}

