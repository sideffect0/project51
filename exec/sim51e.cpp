#include <cstdlib>
#include <iostream>
#include <fstream>
#include <conio.h>

#include "sim51e.h"
#include "convert.h"

void Execute( );
void SaveTmpReg( int , int , int );
void InitReg ();
int CheckBank(bool);
//void CheckAB(bool);

void WriteIMem( int, int );
int  ReadIMem( int );
void WriteXMem( int, int );
int  ReadIMem( int );
void SaveReg();


void _ADD( void );

int _ANL ( int , int );
int _ORL ( int , int );
int _XRL ( int , int );
int _CPL ( int );
int _RL ( int );
int _RLC ( int );
int _RR ( int );
int _RRC ( int );

void _DA( void );
void _SWAP( void );
void _XCHD ( int );


bool CheckParity();

using namespace std;

char* ObjectFile;
char* XMemFile;
char* IMemFile="imem.tmp";
char db[9];
char sb[9];

bool Step;
bool ab;
int res;

int TmpReg[15][4];
int TmpPsw[8];
short int TmpPort[8][4];
//int SpReg[50];

/* Object code structore */
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
}OC;

struct memory {
        long int Location; 
        long int Value;
}IMem,XMem;

/* The main function */

int main(int argc, char *argv[])
{
    //TmpPsw[CY]=0;
    //cout<<Dec2Bin(_RRC(Bin2Dec("11000101")));

//return 0;
     if ( argc==1 )
        return 1;
     
     
     if ( argc == 3 ) {
       if ( strcmp(argv[1],"-s") == 0)
          Step=true;
          ObjectFile=argv[2];
    }
    else
        ObjectFile=argv[1];


    ifstream ObjectStream;
    ObjectStream.open(ObjectFile,ios::binary|ios::ate);
    ObjectStream.seekg(0,ios::beg);
    XMemFile=SplitPath(ObjectFile,FILE_PATH);
    strcat(XMemFile,".mem");

    InitReg();
    
    while ( !ObjectStream.eof() ){
          
          OC.LineNo=-1;
                    
          ObjectStream.read((char*)&OC,sizeof(OC));
          
          if ( OC.LineNo == -1 )
          break;
          
          Execute();    
                           
          if ( OC.JmpTag == true ){

             switch ( OC.Opcode ) {
                    
                    case DJNZ:
                              if ( OC.Operand1 <= DPTR ) {
                                 TmpReg[OC.Operand1][CheckBank(1)]=TmpReg[OC.Operand1][CheckBank(1)]-1;
                                 if ( TmpReg[OC.Operand1][CheckBank(1)]  > 0)
                                    ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                              }
                              else
                              if ( OC.Operand1 > DPTR ) {
                                 TmpReg[OC.Operand1][CheckBank(0)]=TmpReg[OC.Operand1][CheckBank(0)]-1;
                                 if ( TmpReg[OC.Operand1][CheckBank(0)]  > 0)
                                    ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);   
                              }
                              break;
                    case CJNE:
                              if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                                  
                                   if ( OC.Operand1 <= DPTR ) {
                                        if ( TmpReg[OC.Operand1][CheckBank(1)] != OC.Operand2 )
                                           ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( TmpReg[OC.Operand1][CheckBank(1)] < OC.Operand2 )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                            
                                   }
                                   else
                                   if ( OC.Operand1 > DPTR ) {
                                        if ( TmpReg[OC.Operand1][CheckBank(0)] != OC.Operand2 )
                                           ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( TmpReg[OC.Operand1][CheckBank(0)] < OC.Operand2 )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                   
                                   }
                                   
                              }
                              else if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                                   
                                   if ( OC.Operand1 <= DPTR ) {
                                        if ( TmpReg[OC.Operand1][CheckBank(1)] != ReadIMem(OC.Operand2) )
                                             ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( TmpReg[OC.Operand1][CheckBank(1)] < ReadIMem(OC.Operand2) )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                   }
                                   else
                                   if ( OC.Operand1 > DPTR ) {
                                        if ( TmpReg[OC.Operand1][CheckBank(0)] != ReadIMem(OC.Operand2) )
                                             ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( TmpReg[OC.Operand1][CheckBank(0)] < ReadIMem(OC.Operand2) )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                   }
                                   
                              }
                              else if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == true && OC.Op2HoldsAdd == false ) {
                                  
                                   if ( OC.Operand1 <= DPTR ) {
                                        if ( ReadIMem(TmpReg[OC.Operand1][CheckBank(1)]) != OC.Operand2 )
                                           ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( ReadIMem(TmpReg[OC.Operand1][CheckBank(1)]) < OC.Operand2 )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                            
                                   }
                                   else
                                   if ( OC.Operand1 > DPTR ) {
                                        if ( ReadIMem(TmpReg[OC.Operand1][CheckBank(0)]) != OC.Operand2 )
                                           ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                        if ( ReadIMem(TmpReg[OC.Operand1][CheckBank(0)]) < OC.Operand2 )
                                             TmpPsw[CY]=true;
                                        else
                                            TmpPsw[CY]=false;
                                   
                                   }
                              }
                              
                              break;
                              
                              /* TODO (#1#): SWAP
                                             XCGD
                                              DA  */
                              
                              
                    case JMP:
                             ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                             break;
                    case JZ:
                            if ( TmpReg[A][CheckBank(0)] == 0 )
                               ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
                    case JNZ:
                            if ( TmpReg[A][CheckBank(0)] != 0 )
                               ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
                    case JC:
                            if ( TmpPsw[CY] == true )
                               ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
                    case JNC:
                            if ( TmpPsw[CY] == false )
                               ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
                            
                    case JB:
                         
                            if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 && TmpPsw[OC.Operand1-500] == true )
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 && TmpPort[OC.Operand1-600][PORT0] == true )
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 && TmpPort[OC.Operand1-700][PORT1] == true )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 && TmpPort[OC.Operand1-800][PORT2] == true )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 && TmpPort[OC.Operand1-900][PORT3] == true )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
                            
                    case JBC:
                            if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 && TmpPsw[OC.Operand1-500] == true ) {
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                 TmpPsw[OC.Operand1-500] = false;
                            }
                            else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 && TmpPort[OC.Operand1-600][PORT0] == true ) {
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                 TmpPort[OC.Operand1-600][PORT0] = false;
                            }
                            else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 && TmpPort[OC.Operand1-700][PORT1] == true ) {
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                TmpPort[OC.Operand1-700][PORT1] = false;
                            }
                            else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 && TmpPort[OC.Operand1-800][PORT2] == true ) {
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                TmpPort[OC.Operand1-800][PORT2] = false;
                            }
                            else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 && TmpPort[OC.Operand1-900][PORT3] == true ) {
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                                TmpPort[OC.Operand1-900][PORT3] = false;
                            }
                            break;
                            
                    case JNB:
                         
                            if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 && TmpPsw[OC.Operand1-500] == false )
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 && TmpPort[OC.Operand1-600][PORT0] == false )
                                 ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 && TmpPort[OC.Operand1-700][PORT1] == false )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 && TmpPort[OC.Operand1-800][PORT2] == false )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 && TmpPort[OC.Operand1-900][PORT3] == false )
                                ObjectStream.seekg((sizeof(OC)*(OC.JmpID-1)),ios::beg);
                            break;
             }
             
          }
          
    
    
    //clrscr();
    
    //}
    cout<<"A    : "<<TmpReg[A][BANK0]<<"\n";
    cout<<"B    : "<<TmpReg[B][BANK0]<<"\n";
    cout<<"R0   : "<<TmpReg[R0][BANK0]<<"\n";
    cout<<"R1   : "<<TmpReg[R1][BANK0]<<"\n";
    cout<<"R2   : "<<TmpReg[R2][BANK0]<<"\n";
    cout<<"R3   : "<<TmpReg[R3][BANK0]<<"\n";
    cout<<"R4   : "<<TmpReg[R4][BANK0]<<"\n";
    cout<<"R5   : "<<TmpReg[R5][BANK0]<<"\n";
    cout<<"R6   : "<<TmpReg[R6][BANK0]<<"\n";
    cout<<"R7   : "<<TmpReg[R7][BANK0]<<"\n";
    cout<<"DPTR : "<<TmpReg[DPTR][BANK0]<<"\n\n";
//getch();




}


    ObjectStream.close();
    SaveReg();
    return EXIT_SUCCESS;
}
/* end of main function */


void InitReg() {
     
     for ( int i=0;i<15;i++ )
         for ( int j=0;j<4;j++ )
             TmpReg[i][j]=0;

     for ( int i=0;i<8;i++ )
         TmpPsw[i]=0;
     
     for ( int i=0;i<4;i++ )
         for ( int j=0;j<8;j++ )
             TmpPort[j][i]=0;
}

void WriteIMem( int Loc ,int Val) {

     int tag=0,pos,count=0;
     ifstream fin;
     fin.open("imem.tmp",ios::in);
     
     while ( fin ){
           fin.read((char*)&IMem,sizeof(IMem));
           count++;
           if ( 
              IMem.Location==Loc ) {
              tag=1;
              pos=fin.tellg();
              break;
           }
     }

     fin.close();
     ofstream fout0,fout1;
     fout0.open("imem.tmp",ios::ate|ios::in);
     
     IMem.Location=Loc;
     IMem.Value=Val;
     
     if ( tag==1 ){
        fout0.seekp(pos-8);
        fout0.write((char*)&IMem,sizeof(IMem));
     }
     else {
          if ( !fout0 ) {
             fout0.close();
             fout1.open("imem.tmp",ios::out|ios::trunc);
             fout1.write((char*)&IMem,sizeof(IMem));
             fout1.close();
          }
          fout0.write((char*)&IMem,sizeof(IMem));
          fout0.flush();
     }                
     fout0.close();

}

int ReadIMem ( int Loc ) {
    ifstream fin;
    fin.open("imem.tmp",ios::in);
     int Val;
     while ( fin ){
           fin.read((char*)&IMem,sizeof(IMem));
           if ( IMem.Location == Loc )
              return IMem.Value;
     }
     fin.close();
     return 0;
}

void WriteXMem( int Loc ,int Val) {

     int tag=0,pos,count=0;
     ifstream fin;
     fin.open(XMemFile,ios::in);
     
     while ( fin ){
           fin.read((char*)&XMem,sizeof(XMem));
           count++;
           if ( 
              XMem.Location==Loc ) {
              tag=1;
              pos=fin.tellg();
              break;
           }
     }

     fin.close();
     ofstream fout0,fout1;
     fout0.open(XMemFile,ios::ate|ios::in);
     
     XMem.Location=Loc;
     XMem.Value=Val;
     if ( tag==1 ){
        fout0.seekp(pos-8);
        fout0.write((char*)&XMem,sizeof(XMem));
     }
     else {
          if ( !fout0 ) {
             fout0.close();
             fout1.open(XMemFile,ios::out|ios::trunc);
             fout1.write((char*)&XMem,sizeof(XMem));
             fout1.close();
          }
          fout0.write((char*)&XMem,sizeof(XMem));
          fout0.flush();
     }                
     fout0.close();
}

int ReadXMem ( int Loc ) {
    ifstream fin;
    fin.open(XMemFile,ios::in);
     int Val;
     while ( fin ){
           fin.read((char*)&XMem,sizeof(XMem));
           if ( XMem.Location == Loc )
              return XMem.Value;
     }
     fin.close();
     return 0;
}


void SaveTmpReg( int REG , int BANK , int VAL ) {
     TmpReg[REG][BANK]=VAL;
}

void SaveReg() {

     ofstream fout;
     fout.open("reg.tmp",ios::out);

     for ( int i=0;i<4;i++ ){
         fout<<"BANK"<<i<<"\n";
         for ( int j=0;j<15;j++ )
             fout<<Dec2Hex(TmpReg[j][i])<<"\n";

     }

     fout<<"PSW\n";
     for ( int i=0;i<8;i++ )
             fout<<Dec2Hex(TmpPsw[i])<<"\n";

     fout<<"END";
     fout.close();
     ofstream fo;
     fo.open("mem.tmp");
     ifstream fin;
     
     fin.open(XMemFile,ios::in);

     fo<<"XMEM\n";
     
     while ( fin ){
           XMem.Location=-1;
           fin.read((char*)&XMem,sizeof(XMem));
           if ( XMem.Location != -1 ){
              fo<<Dec2Hex(XMem.Location)<<"  =  ";
              Dec2Hex(0);
              fo<<Dec2Hex(XMem.Value)<<"\n";
           }
     }
     fin.close();
     fo<<"IMEM\n";
     ifstream fi;
     fi.open("imem.tmp",ios::in);

     while ( fi ){
           IMem.Location=-1;
           fi.read((char*)&IMem,sizeof(IMem));
           if ( IMem.Location != -1 ){
              fo<<Dec2Hex(IMem.Location)<<"  =  ";
              Dec2Hex(0);
              fo<<Dec2Hex(IMem.Value)<<"\n";
           }
     }
     fin.close();
     fo<<"END";
     fo.close();
     
     ofstream ofs;
     ofs.open("port.tmp",ios::out);
     
     for ( int i=0;i<4;i++ ){
         ofs<<"PORT"<<i<<"\n";
         for ( int j=0;j<8;j++ ){
  
            
            ofs<<TmpPort[j][i]<<"\n";
         }

     }
     ofs<<"END_PORT_BITS\n";
     
     char tmp[9];
     for ( int i=0;i<4;i++ ){
         for ( int j=0;j<8;j++ ) {           
             if ( TmpPort[j][i] == 0 )
                  tmp[j]='0';
             else           
                  tmp[j]='1';
         }
         tmp[8]='\0';
         ofs<<"P"<<i<<" = "<<Dec2Hex(Bin2Dec(strrev(tmp)))<<"h\n";
     }
     
     ofs.flush();
     ofs.close();

}

int CheckBank(bool ab ) {
    
    if ( ab == true )
       return BANK0;
       
    if ( TmpPsw[RS1]==false&&TmpPsw[RS0]==false )
       return BANK0;
    else if ( TmpPsw[RS1]==false&&TmpPsw[RS0]==true )
       return BANK1;
    else if ( TmpPsw[RS1]==true&&TmpPsw[RS0]==false )
       return BANK2;
    else if ( TmpPsw[RS1]==true&&TmpPsw[RS0]==true )
       return BANK3;
}


void Execute ( ) {

     switch ( OC.Opcode ) {
            case MOV:
                 
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                         if ( ( OC.Operand1 <= DPTR  ) && OC.Operand2 <= DPTR ) 
                            SaveTmpReg(OC.Operand1,CheckBank(1),TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(1),TmpReg[OC.Operand2][CheckBank(0)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(0),TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(0),TmpReg[OC.Operand2][CheckBank(0)]);
                        }
                     else 
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR  ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),OC.Operand2);
                         else
                         if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),OC.Operand2);                           
                        }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand2 <= DPTR) 
                             WriteIMem(OC.Operand1,TmpReg[OC.Operand2][CheckBank(1)]);
                          else if ( OC.Operand2 > DPTR)
                             WriteIMem(OC.Operand1,TmpReg[OC.Operand2][CheckBank(0)]);
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                        if ( OC.Operand1 <= DPTR)     
                           SaveTmpReg(OC.Operand1,CheckBank(1),ReadIMem(OC.Operand2));
                        else if ( OC.Operand2 > DPTR)
                             SaveTmpReg(OC.Operand1,CheckBank(0),ReadIMem(OC.Operand2));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false )
                        WriteIMem(OC.Operand1,ReadIMem(OC.Operand2));
                     else 
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false )
                        WriteIMem(OC.Operand1,OC.Operand2);
                     else 
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == true && OC.Op2IsAdd == false ) {
                        if ( OC.Operand1 <= DPTR)
                           WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],OC.Operand2);
                        else
                        if ( OC.Operand1 > DPTR)
                           WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],OC.Operand2);
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                        if ( OC.Operand1 <= B && OC.Operand2 <= DPTR) 
                           SaveTmpReg(OC.Operand1,CheckBank(1),ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]));
                         else
                         if ( OC.Operand1 <= B && OC.Operand2 > DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(1),ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]));
                         else
                         if ( OC.Operand1 > B && OC.Operand2 <= DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(0),ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]));
                         else
                         if ( OC.Operand1 > B && OC.Operand2 > DPTR) 
                         SaveTmpReg(OC.Operand1,CheckBank(0),ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else 
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand2 <= DPTR)
                             WriteIMem(OC.Operand1,TmpReg[OC.Operand2][CheckBank(1)]);
                          else
                          if (OC.Operand2 > DPTR)
                             WriteIMem(OC.Operand1,TmpReg[OC.Operand2][CheckBank(0)]);
                             }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == true && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                            WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(0)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                            WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                            WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(0)]);
                        }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == true && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR )
                             WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(OC.Operand2));
                          else
                          if ( OC.Operand1 > DPTR )
                             WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(OC.Operand2));
                     }
                     TmpPsw[P]=CheckParity();
                     break;
                     
            case MOVX:
                      
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                        
                        if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                           SaveTmpReg(OC.Operand1,CheckBank(1),ReadXMem(TmpReg[OC.Operand2][CheckBank(1)]));
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(1),ReadXMem(TmpReg[OC.Operand2][CheckBank(0)]));
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(0),ReadXMem(TmpReg[OC.Operand2][CheckBank(1)]));
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                            SaveTmpReg(OC.Operand1,CheckBank(0),ReadXMem(TmpReg[OC.Operand2][CheckBank(0)]));
                            
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == true && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             WriteXMem(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                            WriteXMem(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(0)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                            WriteXMem(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(1)]);
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                            WriteXMem(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(0)]);
                     
                     }
                     TmpPsw[P]=CheckParity();
                     break;
            
            case ADD:
                     
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                        
                        if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                           res=TmpReg[OC.Operand1][CheckBank(1)]+TmpReg[OC.Operand2][CheckBank(1)];
                           _ADD();
                           SaveTmpReg(OC.Operand1,CheckBank(1),res);
                        }
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(1)]+TmpReg[OC.Operand2][CheckBank(0)];
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(1),res);
                         }
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+TmpReg[OC.Operand2][CheckBank(1)];
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+TmpReg[OC.Operand2][CheckBank(0)];
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }

                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                        
                        if ( OC.Operand1 <= DPTR) {
                           res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(OC.Operand2);
                           _ADD();
                           SaveTmpReg(OC.Operand1,CheckBank(1),res);
                        }
                         else
                
                         if ( OC.Operand1 > DPTR ) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(OC.Operand2);
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }

                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                        
                        
                        if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                           res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                           _ADD();
                           SaveTmpReg(OC.Operand1,CheckBank(1),res);
                        }
                         else
                         if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(1),res);
                         }
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }
                         else
                         if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }

                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR) {
                           res=TmpReg[OC.Operand1][CheckBank(1)]+OC.Operand2;
                           _ADD();
                           SaveTmpReg(OC.Operand1,CheckBank(1),res);
                        }
                         else
                
                         if ( OC.Operand1 > DPTR ) {
                            res=TmpReg[OC.Operand1][CheckBank(0)]+OC.Operand2;
                            _ADD();
                            SaveTmpReg(OC.Operand1,CheckBank(0),res);
                         }

                     }
                     TmpPsw[P]=CheckParity();
                     break;
            case ADDC:
                     
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                        
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+TmpReg[OC.Operand2][CheckBank(1)]+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+TmpReg[OC.Operand2][CheckBank(0)]+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+TmpReg[OC.Operand2][CheckBank(1)]+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+TmpReg[OC.Operand2][CheckBank(0)]+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(OC.Operand2)+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(OC.Operand2)+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR) {
                             res=TmpReg[OC.Operand1][CheckBank(1)]+OC.Operand2+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                             res=TmpReg[OC.Operand1][CheckBank(0)]+OC.Operand2+TmpPsw[CY];
                             _ADD();
                             SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     TmpPsw[P]=CheckParity();
                     break;
            case SUBB:
                      if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                           
                           if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                                if ( TmpPsw[CY] == true ) {
                                   if ( TmpReg[OC.Operand1][CheckBank(1)] > TmpReg[OC.Operand2][CheckBank(1)] )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - TmpReg[OC.Operand2][CheckBank(1)] - TmpPsw[CY];  
                                   else {
                                     res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - TmpReg[OC.Operand2][CheckBank(1)] - TmpPsw[CY];
                                     TmpPsw[CY]=true;
                                   }
                                }
                                else{
                                     if ( TmpReg[OC.Operand1][CheckBank(1)] > TmpReg[OC.Operand2][CheckBank(1)] )
                                        res=TmpReg[OC.Operand1][CheckBank(1)] - TmpReg[OC.Operand2][CheckBank(1)];
                                     else {
                                          res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - TmpReg[OC.Operand2][CheckBank(1)];
                                          TmpPsw[CY]=true;
                                     }
                                }
                                SaveTmpReg(OC.Operand1,CheckBank(1),res); 
                           }
                           else
                           if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                                if ( TmpPsw[CY] == true ) {
                                   if ( TmpReg[OC.Operand1][CheckBank(1)] > TmpReg[OC.Operand2][CheckBank(0)] )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - TmpReg[OC.Operand2][CheckBank(0)] - TmpPsw[CY];  
                                      else {
                                           res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - TmpReg[OC.Operand2][CheckBank(0)] - TmpPsw[CY];
                                           TmpPsw[CY]=true;
                                      }
                                   }
                                else{
                                     if ( TmpReg[OC.Operand1][CheckBank(1)] > TmpReg[OC.Operand2][CheckBank(0)] )
                                        res=TmpReg[OC.Operand1][CheckBank(1)] - TmpReg[OC.Operand2][CheckBank(0)];
                                     else {
                                        res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - TmpReg[OC.Operand2][CheckBank(0)];
                                        TmpPsw[CY]=true;
                                     }
                                }         
                                SaveTmpReg(OC.Operand1,CheckBank(1),res); 
                           }
                           else
                           if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                                if ( TmpPsw[CY] == true ) {
                                     if ( TmpReg[OC.Operand1][CheckBank(0)] > TmpReg[OC.Operand2][CheckBank(1)] )
                                        res=TmpReg[OC.Operand1][CheckBank(0)] - TmpReg[OC.Operand2][CheckBank(1)] - TmpPsw[CY];  
                                     else {
                                          res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - TmpReg[OC.Operand2][CheckBank(1)] - TmpPsw[CY];
                                          TmpPsw[CY]=true;
                                     }
                                }
                                else 
                                     if ( TmpReg[OC.Operand1][CheckBank(0)] > TmpReg[OC.Operand2][CheckBank(1)] )
                                        res=TmpReg[OC.Operand1][CheckBank(0)] - TmpReg[OC.Operand2][CheckBank(1)];
                                     else {
                                          res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - TmpReg[OC.Operand2][CheckBank(1)];
                                          TmpPsw[CY]=true;
                                     }
                                }
                                SaveTmpReg(OC.Operand1,CheckBank(0),res); 
                           }
                           else
                           if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                              if ( TmpPsw[CY] == true ) {
                                 if ( TmpReg[OC.Operand1][CheckBank(0)] > TmpReg[OC.Operand2][CheckBank(0)] )
                                      res=TmpReg[OC.Operand1][CheckBank(0)] - TmpReg[OC.Operand2][CheckBank(0)] - TmpPsw[CY];  
                                 else {
                                      res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - TmpReg[OC.Operand2][CheckBank(0)] - TmpPsw[CY];
                                      TmpPsw[CY]=true;
                                 }
                              }
                              else { 
                                   if ( TmpReg[OC.Operand1][CheckBank(0)] > TmpReg[OC.Operand2][CheckBank(0)] )
                                      res=TmpReg[OC.Operand1][CheckBank(0)] - TmpReg[OC.Operand2][CheckBank(0)];
                                   else {
                                        res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - TmpReg[OC.Operand2][CheckBank(0)];
                                        TmpPsw[CY]=true;
                                   }
                              }
                              SaveTmpReg(OC.Operand1,CheckBank(0),res); 
                           
                      } 
                      else if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                           if ( OC.Operand1 <= DPTR) {
                              if ( TmpPsw[CY] == true ) {
                                 if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(OC.Operand2) )
                                    res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(OC.Operand2) - TmpPsw[CY];  
                                 else {
                                      res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(OC.Operand2) - TmpPsw[CY];
                                      TmpPsw[CY]=true;
                                 }
                              }
                              else {
                                   if ( TmpReg[OC.Operand1][CheckBank(1)] > ReadIMem(OC.Operand2) )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(OC.Operand2);
                                   else {
                                        res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(OC.Operand2);
                                        TmpPsw[CY]=true;
                                   }
                              }                             
                              SaveTmpReg(OC.Operand1,CheckBank(1),res);
                           }
                           else
                           if ( OC.Operand1 > DPTR ) {
                              if ( TmpPsw[CY] == true ) {
                                 if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(OC.Operand2) )
                                      res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(OC.Operand2) - TmpPsw[CY];  
                                 else {
                                      res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(OC.Operand2) - TmpPsw[CY];
                                      TmpPsw[CY]=true;
                                 }
                              }
                              else {
                                   if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(OC.Operand2) )
                                      res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(OC.Operand2);
                                   else {
                                        res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(OC.Operand2);
                                        TmpPsw[CY]=true;
                                   }
                              }
                              SaveTmpReg(OC.Operand1,CheckBank(0),res);
                           }
                      }
                      else if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                           if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                              if ( TmpPsw[CY]=true ) {
                                 if ( TmpReg[OC.Operand1][CheckBank(1)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) - TmpPsw[CY];  
                                 else {
                                      res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) - TmpPsw[CY];
                                      TmpPsw[CY]=true;
                                 }
                              }
                              else {
                                   if ( TmpReg[OC.Operand1][CheckBank(1)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                                   else {
                                        res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                                        TmpPsw[CY]=true;
                                   }
                              }
                              SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                               if ( TmpPsw[CY]=true ) {
                                    if ( TmpReg[OC.Operand1][CheckBank(1)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) )
                                       res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) - TmpPsw[CY];  
                                    else {
                                       res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) - TmpPsw[CY];
                                       TmpPsw[CY]=true;
                                    }
                               }
                               else {
                                    if ( TmpReg[OC.Operand1][CheckBank(1)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) )
                                         res=TmpReg[OC.Operand1][CheckBank(1)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                                         TmpPsw[CY]=true;
                                    }
                               }
                               SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                               if ( TmpPsw[CY]=true ) {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) - TmpPsw[CY];  
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) - TmpPsw[CY];
                                         TmpPsw[CY]=true;
                                    }
                               }
                               else {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]) )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                                         TmpPsw[CY]=true;
                                    }
                               }
                               SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                               if ( TmpPsw[CY]=true ) {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) - TmpPsw[CY];  
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) - TmpPsw[CY];
                                         TmpPsw[CY]=true;
                                    }
                               }
                               else {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]) )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                                         TmpPsw[CY]=true;
                                    }
                               }
                               SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                          
                      }
                      else if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                           if ( OC.Operand1 <= DPTR) {
                                if ( TmpPsw[CY] == true ) {
                                   if ( TmpReg[OC.Operand1][CheckBank(1)] > OC.Operand2 )
                                      res=TmpReg[OC.Operand1][CheckBank(1)] - OC.Operand2 - TmpPsw[CY];  
                                   else {
                                        res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - OC.Operand2 - TmpPsw[CY];
                                        TmpPsw[CY]=true;
                                   }
                                }
                                else {
                                     if ( TmpReg[OC.Operand1][CheckBank(1)] > OC.Operand2 )
                                        res=TmpReg[OC.Operand1][CheckBank(1)] - OC.Operand2;
                                     else {
                                          res=(TmpReg[OC.Operand1][CheckBank(1)] + 256) - OC.Operand2;
                                          TmpPsw[CY]=true;
                                     }
                                }
                                SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                               if ( TmpPsw[CY] == true ) {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > OC.Operand2 )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - OC.Operand2 - TmpPsw[CY];  
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - OC.Operand2 - TmpPsw[CY];
                                         TmpPsw[CY]=true;
                                    }
                               }
                               else {
                                    if ( TmpReg[OC.Operand1][CheckBank(0)] > OC.Operand2 )
                                       res=TmpReg[OC.Operand1][CheckBank(0)] - OC.Operand2;
                                    else {
                                         res=(TmpReg[OC.Operand1][CheckBank(0)] + 256) - OC.Operand2;
                                         TmpPsw[CY]=true;
                                    }
                               }
                               SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                      }
                      TmpPsw[P]=CheckParity();
                      break;
            
            case INC:
                     if ( OC.Op1IsAdd == false && OC.Op1HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR ) {
                               res=TmpReg[OC.Operand1][CheckBank(1)]+1;
                               if ( OC.Operand1 == DPTR ){
                                  if ( res > 65535 )
                                     res=0;
                               }
                               else if( res> 255 )
                                  res=0;
                               SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                               res=TmpReg[OC.Operand1][CheckBank(0)]+1;
                               if( res > 255 )
                                  res=0;
                               SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     
                     if ( OC.Op1IsAdd == true && OC.Op1HoldsAdd == false ) {
                        res=ReadIMem(OC.Operand1)+1;
                        if ( res>255 )
                             res=0;
                        WriteIMem(OC.Operand1,res);
                     }
                     
                     if ( OC.Op1IsAdd == false && OC.Op1HoldsAdd == true ) {
                        
                        if ( OC.Operand1 <= DPTR ) {
                               res=ReadIMem(TmpReg[OC.Operand1][CheckBank(1)])+1;
                               if ( OC.Operand1 == DPTR ){
                                  if ( res > 65535 )
                                     res=0;
                               }
                               else if( res> 255 )
                                  res=0;
                               WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                               res=ReadIMem(TmpReg[OC.Operand1][CheckBank(0)])+1;
                               if( res > 255 )
                                  res=0;
                               WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],res);
                          }
                                            
                     }
                     TmpPsw[P]=CheckParity();
                     break;

            case DEC:
                     if ( OC.Op1IsAdd == false && OC.Op1HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR ) {
                               res=TmpReg[OC.Operand1][CheckBank(1)]-1;
                               if ( OC.Operand1 == DPTR ){
                                  if ( res < 0 )
                                     res=65535;
                               }
                               else if( res < 0 )
                                  res=255;
                               SaveTmpReg(OC.Operand1,CheckBank(1),res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                               res=TmpReg[OC.Operand1][CheckBank(0)]-1;
                               if( res < 0 )
                                  res=255;
                               SaveTmpReg(OC.Operand1,CheckBank(0),res);
                          }
                     }
                     
                     if ( OC.Op1IsAdd == true && OC.Op1HoldsAdd == false ) {
                        res=ReadIMem(OC.Operand1)-1;
                        if ( res < 0 )
                             res=255;
                        WriteIMem(OC.Operand1,res);
                     }
                     
                     if ( OC.Op1IsAdd == false && OC.Op1HoldsAdd == true ) {
                        
                        if ( OC.Operand1 <= DPTR ) {
                               res=ReadIMem(TmpReg[OC.Operand1][CheckBank(1)])-1;
                               if ( OC.Operand1 == DPTR ){
                                  if ( res < 0 )
                                     res=65535;
                               }
                               else if( res < 0 )
                                  res=255;
                               WriteIMem(TmpReg[OC.Operand1][CheckBank(1)],res);
                          }
                          else
                          if ( OC.Operand1 > DPTR ) {
                               res=ReadIMem(TmpReg[OC.Operand1][CheckBank(0)])-1;
                               if( res < 0 )
                                  res=255;
                               WriteIMem(TmpReg[OC.Operand1][CheckBank(0)],res);
                          }
                                            
                     }
                     TmpPsw[P]=CheckParity();
                     break;
            case MUL:
                     res=TmpReg[OC.Operand1][0]*TmpReg[OC.Operand2][0];
                     if ( res > 255 ) {
                          
                          char *tmp=Dec2Hex(res),t1[3];
                          int len=strlen(tmp);
                          t1[1]=tmp[len-1];
                          t1[0]=tmp[len-2];
                          t1[2]='\0';
                          TmpReg[OC.Operand1][CheckBank(0)]=Hex2Dec(t1);
                          t1[1]=tmp[len-3];
                          t1[0]=tmp[len-4];
                          t1[2]='\0';
                          TmpReg[OC.Operand2][CheckBank(0)]=Hex2Dec(t1);
                          TmpPsw[OV]=true;
                     }
                     else {
                          TmpReg[OC.Operand1][0]=TmpReg[OC.Operand1][0]*TmpReg[OC.Operand2][0];
                          TmpPsw[OV]=false;
                     }
                     TmpPsw[P]=CheckParity();
                     break;
                     
            case DIV:
                     if ( OC.Operand2 == 0 ) {
                        TmpPsw[OV]=true;
                        TmpPsw[CY]=false;
                     }
                     else {
                          res=TmpReg[OC.Operand1][CheckBank(0)]/TmpReg[OC.Operand2][CheckBank(0)];
                          TmpReg[OC.Operand1][CheckBank(0)]=res;
                     }
                     TmpPsw[P]=CheckParity();
                     break;
					 
            case ANL:
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(0)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(OC.Operand2)));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(OC.Operand2)));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ANL(TmpReg[OC.Operand1][CheckBank(1)],OC.Operand2));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ANL(TmpReg[OC.Operand1][CheckBank(0)],OC.Operand2));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand2 <= DPTR ) 
                             WriteIMem(OC.Operand1,_ANL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand2 > DPTR ) 
                             WriteIMem(OC.Operand1,_ANL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false )
                          WriteIMem(OC.Operand1,_ANL(ReadIMem(OC.Operand1),OC.Operand2));
            
                     TmpPsw[P]=CheckParity();
                     break;
                     
            case ORL:
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(0)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(OC.Operand2)));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(OC.Operand2)));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_ORL(TmpReg[OC.Operand1][CheckBank(1)],OC.Operand2));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_ORL(TmpReg[OC.Operand1][CheckBank(0)],OC.Operand2));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand2 <= DPTR ) 
                             WriteIMem(OC.Operand1,_ORL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand2 > DPTR ) 
                             WriteIMem(OC.Operand1,_ORL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false )
                          WriteIMem(OC.Operand1,_ORL(ReadIMem(OC.Operand1),OC.Operand2));
                     
                     TmpPsw[P]=CheckParity();
                     break;
            case XRL:
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],TmpReg[OC.Operand2][CheckBank(0)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(OC.Operand2)));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(OC.Operand2)));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(1)])));
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],ReadIMem(TmpReg[OC.Operand2][CheckBank(0)])));
                     }
                     else
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          
                          if ( OC.Operand1 <= DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(1),_XRL(TmpReg[OC.Operand1][CheckBank(1)],OC.Operand2));
                          else
                          if ( OC.Operand1 > DPTR ) 
                             SaveTmpReg(OC.Operand1,CheckBank(0),_XRL(TmpReg[OC.Operand1][CheckBank(0)],OC.Operand2));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                          if ( OC.Operand2 <= DPTR ) 
                             WriteIMem(OC.Operand1,_XRL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(1)]));
                          else
                          if ( OC.Operand2 > DPTR ) 
                             WriteIMem(OC.Operand1,_XRL(ReadIMem(OC.Operand1),TmpReg[OC.Operand2][CheckBank(0)]));
                     }
                     else
                     if ( OC.Op1IsAdd == true && OC.Op2IsAdd == false && OC.IVal == true && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false )
                          WriteIMem(OC.Operand1,_XRL(ReadIMem(OC.Operand1),OC.Operand2));
                     
                     TmpPsw[P]=CheckParity();
                     break;
            case CPL:
                 
                       if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 ) {
                           if ( TmpPsw[OC.Operand1-500] == false )
                              TmpPsw[OC.Operand1-500] = true;
                           else
                               TmpPsw[OC.Operand1-500] = false;
                       }
                       else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 ) {
                           if ( TmpPort[OC.Operand1-600][PORT0] == false )
                              TmpPort[OC.Operand1-600][PORT0] = true;
                           else
                               TmpPort[OC.Operand1-600][PORT0] = false;
                       }
                       else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 ) {
                           if ( TmpPort[OC.Operand1-700][PORT1] == false )
                              TmpPort[OC.Operand1-700][PORT1] = true;
                           else
                               TmpPort[OC.Operand1-700][PORT1] = false;
                       }
                       else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 ) {
                           if ( TmpPort[OC.Operand1-800][PORT2] == false )
                              TmpPort[OC.Operand1-800][PORT2] = true;
                           else
                               TmpPort[OC.Operand1-800][PORT2] = false;
                       }
                       else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 ) {
                           if ( TmpPort[OC.Operand1-900][PORT3] == false )
                              TmpPort[OC.Operand1-900][PORT3] = true;
                           else
                               TmpPort[OC.Operand1-900][PORT3] = false;
                       }
                       else if ( OC.Operand1 <= DPTR )
                            SaveTmpReg(OC.Operand1,CheckBank(1),_CPL(TmpReg[OC.Operand1][CheckBank(1)]));
                       else
                            SaveTmpReg(OC.Operand1,CheckBank(0),_CPL(TmpReg[OC.Operand1][CheckBank(0)]));
                              
                     TmpPsw[P]=CheckParity();
                     break;
            case CLR:

                       if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 )
                           TmpPsw[OC.Operand1-500]=false;
                      else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 )
                           TmpPort[OC.Operand1-600][PORT0]=false;
                      else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 )
                           TmpPort[OC.Operand1-700][PORT1]=false;
                      else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 )
                           TmpPort[OC.Operand1-800][PORT2]=false;
                      else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 )
                           TmpPort[OC.Operand1-900][PORT3]=false;
                      else if ( OC.Operand1 <= DPTR )
                             SaveTmpReg(OC.Operand1,CheckBank(1),0);
                          else
                              SaveTmpReg(OC.Operand1,CheckBank(0),0);

                     
                     break;
            case RL:
                    if ( OC.Op1IsAdd == false ) {
                          if ( OC.Operand1 <= DPTR )
                             SaveTmpReg(OC.Operand1,CheckBank(1),_RL(TmpReg[OC.Operand1][CheckBank(1)]));
                          else
                              SaveTmpReg(OC.Operand1,CheckBank(0),_RL(TmpReg[OC.Operand1][CheckBank(0)]));
                     }
                    TmpPsw[P]=CheckParity();
                    break;
            case RLC:
                     if ( OC.Op1IsAdd == false ) {
                          if ( OC.Operand1 <= DPTR )
                             SaveTmpReg(OC.Operand1,CheckBank(1),_RLC(TmpReg[OC.Operand1][CheckBank(1)]));
                          else
                              SaveTmpReg(OC.Operand1,CheckBank(0),_RLC(TmpReg[OC.Operand1][CheckBank(0)]));
                     }
                    TmpPsw[P]=CheckParity();
                    break;
            case RR:
                    if ( OC.Op1IsAdd == false ) {
                          if ( OC.Operand1 <= DPTR )
                             SaveTmpReg(OC.Operand1,CheckBank(1),_RR(TmpReg[OC.Operand1][CheckBank(1)]));
                          else
                              SaveTmpReg(OC.Operand1,CheckBank(0),_RR(TmpReg[OC.Operand1][CheckBank(0)]));
                     }
                    TmpPsw[P]=CheckParity();
                    break;
            case RRC:
                    if ( OC.Op1IsAdd == false ) {
                          if ( OC.Operand1 <= DPTR )
                             SaveTmpReg(OC.Operand1,CheckBank(1),_RRC(TmpReg[OC.Operand1][CheckBank(1)]));
                          else
                              SaveTmpReg(OC.Operand1,CheckBank(0),_RRC(TmpReg[OC.Operand1][CheckBank(0)]));
                     }
                    TmpPsw[P]=CheckParity();
                    break;
            case XCH:
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
  						  if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(1)];
                               TmpReg[OC.Operand1][CheckBank(1)]=TmpReg[OC.Operand2][CheckBank(1)];
                               TmpReg[OC.Operand2][CheckBank(1)]=tmp;
                          }
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(1)];
                               TmpReg[OC.Operand1][CheckBank(1)]=TmpReg[OC.Operand2][CheckBank(0)];
                               TmpReg[OC.Operand2][CheckBank(0)]=tmp;
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(0)];
                               TmpReg[OC.Operand1][CheckBank(0)]=TmpReg[OC.Operand2][CheckBank(1)];
                               TmpReg[OC.Operand2][CheckBank(1)]=tmp;
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(0)];
                               TmpReg[OC.Operand1][CheckBank(0)]=TmpReg[OC.Operand2][CheckBank(0)];
                               TmpReg[OC.Operand2][CheckBank(0)]=tmp;
                          }
                        
                     }
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == true && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == false ) {
                        
                        if ( OC.Operand1 <= DPTR ) {
                             int tmp=TmpReg[OC.Operand1][CheckBank(1)];
                             TmpReg[OC.Operand1][CheckBank(1)]=ReadIMem(OC.Operand2);
                             WriteIMem(OC.Operand2,tmp);
                        }
                        else 
                        if ( OC.Operand1 <= DPTR ) {
                             int tmp=TmpReg[OC.Operand1][CheckBank(0)];
                             TmpReg[OC.Operand1][CheckBank(0)]=ReadIMem(OC.Operand2);
                             WriteIMem(OC.Operand2,tmp);
                        }
                        
                     }
                     if ( OC.Op1IsAdd == false && OC.Op2IsAdd == false && OC.IVal == false && OC.Op1HoldsAdd == false && OC.Op2HoldsAdd == true ) {
                          if ( OC.Operand1 <= DPTR && OC.Operand2 <= DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(1)];
                               TmpReg[OC.Operand1][CheckBank(1)]=ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                               WriteIMem(TmpReg[OC.Operand2][CheckBank(1)],tmp);
                          }
                          else
                          if ( OC.Operand1 <= DPTR && OC.Operand2 > DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(1)];
                               TmpReg[OC.Operand1][CheckBank(1)]=ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                               WriteIMem(TmpReg[OC.Operand2][CheckBank(0)],tmp);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 <= DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(0)];
                               TmpReg[OC.Operand1][CheckBank(0)]=ReadIMem(TmpReg[OC.Operand2][CheckBank(1)]);
                               WriteIMem(TmpReg[OC.Operand2][CheckBank(1)],tmp);
                          }
                          else
                          if ( OC.Operand1 > DPTR && OC.Operand2 > DPTR) {
                               int tmp=TmpReg[OC.Operand1][CheckBank(0)];
                               TmpReg[OC.Operand1][CheckBank(0)]=ReadIMem(TmpReg[OC.Operand2][CheckBank(0)]);
                               WriteIMem(TmpReg[OC.Operand2][CheckBank(0)],tmp);
                          }
                     } 
                     
                     TmpPsw[P]=CheckParity();
                     break;
            case SETB:
                      
                      if ( OC.Operand1 >= 500  && OC.Operand1 <= 507 )
                           TmpPsw[OC.Operand1-500]=true;
                      else if ( OC.Operand1 >= 600  && OC.Operand1 <= 607 )
                           TmpPort[OC.Operand1-600][PORT0]=true;
                      else if ( OC.Operand1 >= 700  && OC.Operand1 <= 707 )
                           TmpPort[OC.Operand1-700][PORT1]=true;
                      else if ( OC.Operand1 >= 800  && OC.Operand1 <= 807 )
                           TmpPort[OC.Operand1-800][PORT2]=true;
                      else if ( OC.Operand1 >= 900  && OC.Operand1 <= 907 )
                           TmpPort[OC.Operand1-900][PORT3]=true;
                      break;
            
            case DA:
                    if ( OC.Operand1 == A )
                       _DA();
                    break;
            
            case SWAP:
                      _SWAP();
                      break;
                      
            case XCHD:
                      if ( OC.Operand1 == A && OC.Operand2 > DPTR && OC.Op1HoldsAdd == false && OC.Op1IsAdd == false && OC.Op2HoldsAdd == true )
                         _XCHD( OC.Operand2 );
                      break;
                      
                      
                      
     }

}

     
     
void _ADD(){
     if ( res > 255 ) {
        TmpPsw[CY]=true;
        TmpPsw[AC]=true;
        TmpPsw[OV]=true;
        res=res-256;
     }
     else if ( res > 127 ) {
          TmpPsw[AC]=true;
          TmpPsw[CY]=false;
          TmpPsw[OV]=false;
     }
     else {
          TmpPsw[CY]=false;
          TmpPsw[AC]=false;
          TmpPsw[OV]=false;
     }                        
}


int _ANL ( int Dest , int Src ) {
     strcpy(db,Dec2Bin(Dest));
     strcpy(sb,Dec2Bin(Src));
     for ( int i=0;i<8;i++ ) {
         if ( db[i]!=sb[i] )
           db[i]='0';
     }
     return Bin2Dec(db);
}

int _ORL ( int Dest , int Src ) {
     strcpy(db,Dec2Bin(Dest));
     strcpy(sb,Dec2Bin(Src));
     for ( int i=0;i<8;i++ ) {
         if ( db[i]=='0' && sb[i]=='0' )
           db[i]='0';
         else
             db[i]='1';
     }
     return Bin2Dec(db);
}

int _XRL ( int Dest , int Src ) {
     strcpy(db,Dec2Bin(Dest));
     strcpy(sb,Dec2Bin(Src));
     for ( int i=0;i<8;i++ ) {
         if ( db[i]==sb[i] )
           db[i]='0';
         else
             db[i]='1';
     }
     return Bin2Dec(db);
}

int _CPL ( int Dest ) {
     strcpy(db,Dec2Bin(Dest));
     for ( int i=0;i<8;i++ ) {
         if ( db[i]=='0' )
           db[i]='1';
         else
             db[i]='0';
     }
     return Bin2Dec(db);
}

int _RL( int Reg ) {
    
    strcpy(sb,Dec2Bin(Reg));
    
    char tmp1=sb[0],tmp2;
    for ( int i=7;i>=0;i-- ) {
        tmp2=sb[i];
        sb[i]=tmp1;
        tmp1=tmp2;
    }
    return Bin2Dec(sb);
}

int _RLC( int Reg ) {
    
    strcpy(sb,Dec2Bin(Reg));
    
    char tmp1=sb[0],tmp2;
    
    if ( TmpPsw[CY] == 1 )
       tmp2='1';
    else if ( TmpPsw[CY] == 0 )
        tmp2='0';
    
    if ( tmp1 == '1' )
        TmpPsw[CY]=1;
    else if ( tmp1 == '0' )
        TmpPsw[CY]=0;
        
    tmp1=tmp2;
    for ( int i=7;i>=0;i-- ) {
        tmp2=sb[i];
        sb[i]=tmp1;
        tmp1=tmp2;
    }
    return Bin2Dec(sb);
}

int _RR( int Reg ) {
    
    strcpy(sb,Dec2Bin(Reg));
    
    char tmp1=sb[7],tmp2;
    for ( int i=0;i<8;i++ ) {
        tmp2=sb[i];
        sb[i]=tmp1;
        tmp1=tmp2;
    }
    return Bin2Dec(sb);
}

int _RRC( int Reg ) {
    
    strcpy(sb,Dec2Bin(Reg));
    
    char tmp1=sb[7],tmp2;
    
    if ( TmpPsw[CY] == 1 )
       tmp2='1';
    else if ( TmpPsw[CY] == 0 )
        tmp2='0';
    
    if ( tmp1 == '1' )
        TmpPsw[CY]=1;
    else if ( tmp1 == '0' )
        TmpPsw[CY]=0;
        
    tmp1=tmp2;
    for ( int i=0;i<8;i++ ) {
        tmp2=sb[i];
        sb[i]=tmp1;
        tmp1=tmp2;
    }
    return Bin2Dec(sb);
}

void _DA() {
     
     strcpy(sb,Dec2Bin(TmpReg[A][0]));
/*
     cout<<"A (hex) = "<<Dec2Hex(TmpReg[A][0])<<"\n";
     cout<<"A (dec) = "<<TmpReg[A][0]<<"\n";
     cout<<"A (bin) = "<<sb<<"\n";
      */
     //getch();
     
     db[0]='0';
     db[1]='0';
     db[2]='0';
     db[3]='0';
     
     for ( int i= 4; i<8 ; i++ )
         db[i]=sb[i];
         
     db[8]='\0';/*
  cout<<"A lowbit (bin) = "<<db<<"\n";
  cout<<"A lowbit (dec) = "<<Bin2Dec(db)<<"\n\n";
  cout<<Hex2Dec("BE")<<"\n\n";
  */
  if ( Bin2Dec(db) > 9 )
     TmpReg[A][0]=TmpReg[A][0]+6;
     
     //cout<<"A (bin) = "<<Dec2Bin(TmpReg[A][0]);
  
     getch();
  
     if ( TmpReg[A][0] > 255 ) {
        TmpPsw[CY]=true;
        TmpPsw[AC]=true;
        TmpPsw[OV]=true;
        res=res-256;
     }
     else if ( TmpReg[A][0] > 127 ) {
          TmpPsw[AC]=true;
          TmpPsw[CY]=false;
          TmpPsw[OV]=false;
     }
     else {
          TmpPsw[CY]=false;
          TmpPsw[AC]=false;
          TmpPsw[OV]=false;
     }
  //getch();
  
  
     
}

void _SWAP ( void ) {

     strcpy(sb,Dec2Bin(TmpReg[A][0]));
     for ( int i=0;i<4;i++ )
         db[4+i]=sb[i];
     for ( int i=0;i<4;i++ )
         db[i]=sb[4+i];
     db[8]='\0';

     TmpReg[A][0]=Bin2Dec(db);
}


void _XCHD ( int Dest ) {
     
     strcpy(sb,Dec2Bin(TmpReg[A][0]));     
     strcpy(db,Dec2Bin(ReadIMem(TmpReg[Dest][CheckBank(0)])));
     
     char tmp[5];
     for ( int i=7;i>3;i-- )
         tmp[i-4]=sb[i];
     
     tmp[4]='\0';
     
     for ( int i=4;i<8;i++ )
         sb[i]=db[i];
                
     for ( int i=4;i<8;i++ )
         db[i]=tmp[i-4];
         
     TmpReg[A][0]=Bin2Dec(sb);
     WriteIMem(TmpReg[Dest][CheckBank(0)],Bin2Dec(db));

}


bool CheckParity() {
     int cnt=0;
     strcpy(sb,Dec2Bin(TmpReg[A][CheckBank(0)]));
     for ( int i=0;i<8;i++ )
         if ( sb[i] == '1')
            cnt++;
     if ( cnt%2 == 0 )
        return false;
     else
         return true;
}


