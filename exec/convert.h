
#include <math.h>

#define FILE_NAME 0
#define FILE_PATH 1
#define FILE_EXT 2

using namespace std;

char __FileExt[4];
             
int Hex2Dec( char[] ); // Hex To Decimal
char* Dec2Hex( int ); // Decimal To Hex
int Bin2Dec ( char [] ); // Binary To Decimal
char* Dec2Bin ( int ); // Decimal To binary
char* SplitPath( char[], int ); // Split Path Into Different Parts


/* Body of function to convert Hex To Decimal */
int Hex2Dec(char* __Hex) {
      int __Length;
      float __Decimal;
	  char __ch;
	  
      __Length=strlen(__Hex);
      __Decimal=0;
      
      for(int __i=__Length-1, __power=0 ; __i>=0 ; __i-- ,__power++){
              __ch=__Hex[__i];        
              switch (__ch) {
                     case '0':
                          __Decimal=__Decimal+(0*pow(16,__power));
                          break;
                     case '1':
                          __Decimal=__Decimal+(1*pow(16,__power));
                          break;
                     case '2':
                          __Decimal=__Decimal+(2*pow(16,__power));
                          break;
                     case '3':
                          __Decimal=__Decimal+(3*pow(16,__power));
                          break;
                     case '4':
                          __Decimal=__Decimal+(4*pow(16,__power));
                          break;
                     case '5':
                          __Decimal=__Decimal+(5*pow(16,__power));
                          break;
                     case '6':
                          __Decimal=__Decimal+(6*pow(16,__power));
                          break;
                     case '7':
                          __Decimal=__Decimal+(7*pow(16,__power));
                          break;
                     case '8':
                          __Decimal=__Decimal+(8*pow(16,__power));
                          break;
                     case '9':
                          __Decimal=__Decimal+(9*pow(16,__power));
                          break;
                     case 'A':
                          __Decimal=__Decimal+(10*pow(16,__power));
                          break;
                     case 'B':
                          __Decimal=__Decimal+(11*pow(16,__power));
                          break;
                     case 'C':
                          __Decimal=__Decimal+(12*pow(16,__power));
                          break;
                     case 'D':
                          __Decimal=__Decimal+(13*pow(16,__power));
                          break;
                     case 'E':
                          __Decimal=__Decimal+(14*pow(16,__power));
                          break;
                     case 'F':
                          __Decimal=__Decimal+(15*pow(16,__power));
                          break;
                     case 'a':
                          __Decimal=__Decimal+(10*pow(16,__power));
                          break;
                     case 'b':
                          __Decimal=__Decimal+(11*pow(16,__power));
                          break;
                     case 'c':
                          __Decimal=__Decimal+(12*pow(16,__power));
                          break;
                     case 'd':
                          __Decimal=__Decimal+(13*pow(16,__power));
                          break;
                     case 'e':
                          __Decimal=__Decimal+(14*pow(16,__power));
                          break;
                     case 'f':
                          __Decimal=__Decimal+(15*pow(16,__power));
                          break;
              }
      }
      return (unsigned long int) __Decimal;
}

/* Body of function to convert Decimal To Hex */
char* Dec2Hex( int __Dec ) {
 	  char __Hex[5];
      int __Rem;
	  strcpy( __Hex,"");
	  
      if( __Dec == 0 )
	      return strcpy( __Hex,"0");;
	      
      
      while( __Dec > 0) {
		     __Rem=__Dec%16;
		     __Dec=__Dec/16;
		     switch (__Rem) {
		            case 0:
			             strcat(__Hex,"0");
                         break;
                    case 1:
			             strcat(__Hex,"1");
			             break;
                    case 2:
			             strcat(__Hex,"2");
                         break;
                    case 3:
			             strcat(__Hex,"3");
			             break;
                    case 4:
			             strcat(__Hex,"4");
			             break;
                    case 5:
			             strcat(__Hex,"5");
			             break;
                    case 6:
			             strcat(__Hex,"6");
			             break;
		            case 7:
			             strcat(__Hex,"7");
			             break;
		            case 8:
			             strcat(__Hex,"8");
			             break;
		            case 9:
			             strcat(__Hex,"9");
			             break;
		            case 10:
			             strcat(__Hex,"A");
			             break;
		            case 11:
			             strcat(__Hex,"B");
			             break;
                    case 12:
			             strcat(__Hex,"C");
			             break;
		            case 13:
			             strcat(__Hex,"D");
			             break;
		            case 14:
			             strcat(__Hex,"E");
			             break;
		            case 15:
			             strcat(__Hex,"F");
			             break;
             }
	 }
     return strrev(__Hex);
}

int Bin2Dec(char* __Bin) {
      float __Decimal;
	  char __ch;

      __Decimal=0;
      for(int __i=7, __power=0 ; __i>=0 ; __i-- ,__power++) {
              __ch=__Bin[__i];
              if (__ch == '1')
                 __Decimal=__Decimal+pow(2,__power);
      }
      return (int)__Decimal;
}

char* Dec2Bin( int __Dec ) {
      char __Bin[9];
      int __Rem;
      strcpy( __Bin,"");
      while( __Dec > 0) {
		     __Rem=__Dec%2;
		     __Dec=__Dec/2;
		     
		     switch (__Rem) {
		            case 0:
			             strcat(__Bin,"0");
                         break;
                    case 1:
			             strcat(__Bin,"1");
			             break;
             }
	 }
	 for (int i=strlen(__Bin);i<8;i++)
	 strcat(__Bin,"0");
     return strrev(__Bin);
}

/* Body of function to split path */
char* SplitPath( char* __Path , int __ID) {
      char __ch=' ';
      int __Len_FN=0, __Len_Path,__Count=0;
      
       __Len_Path=strlen( __Path );
      
      if ( __ID == FILE_NAME ) {
           char* __FileName=new char[__Len_FN];
           while ( __Len_FN>=0 ) {
	             __ch=__Path[__Len_Path-__Len_FN];
	             if ( __ch!='.')
                     __FileName[__Count]=__ch;
                 else
                     break;
	             __Count++;__Len_FN--;
           }           
           return __FileName;
      }
      else if ( __ID == FILE_EXT ) {
           for ( int i=(__Len_Path-3),j=0;i<=__Len_Path;i++,j++ )
               __FileExt[j]=__Path[i];
           return __FileExt;
      }
      else if ( __ID == FILE_PATH ) {
           char* __FNDIR = new char[__Len_Path];
           strcpy(__FNDIR,"");
           for ( int i=0,j=0;i<__Len_Path;i++ ) {
               __ch=__Path[i];
               if ( __ch != '.' ) {
                  __FNDIR[j]=__ch;
                  __FNDIR[j+1]='\0';
                  j++;
               }
               else
                   return __FNDIR;
           }
      }
     return "Nothing";
}

