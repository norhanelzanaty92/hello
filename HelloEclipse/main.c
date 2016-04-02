/*
 * main.c
 *
 *  Created on: Dec 31, 2015
 *      Author: m.mamdooh
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <ctype.h>

static const long hextable[] = {
   [0 ... 255] = -1, // bit aligned access into this table is considerably
   ['0'] = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, // faster for most modern processors,
   ['A'] = 10, 11, 12, 13, 14, 15,       // for the space conscious, reduce to
   ['a'] = 10, 11, 12, 13, 14, 15        // signed char.
};

long hexdec(unsigned const char *hex);
void dectohex(long int decimalNumber ,unsigned char *hexadecimalNumber);
void bintodec( long int binaryNumber);
void dectobin(int decimalno);
void append(char* s, char c);
unsigned char sk[100]="0" ,uc,dechex[100]="0", hexstr[50]="0";
char x [100] ={0} ;
unsigned char entry_1[23],entry_2[23] , command[1000] , lmd[20] ,st1[100] ;
char  ch, lns=0 , lmod=0 , dig1=0,dig2=0;
long int cs=0 ,s=0 , count=0, c=0 ,i=0 ,lines=1,flines,hexadecimalNumber=0;
FILE *fasm , *fhex , *fi;
void init();
void ifile();
void read();
void write_rlen();
void write_add() ;
void write_rectyp();
void lins_num();
void write_data();
void write_chksum();

int main ()
{

init();

ifile();

lins_num();

for(count=0 ; count<=lns ; count++)
{

	if(flines == (count*8)) break ;

	write_rlen();
	write_add();
	write_rectyp();
	write_data();
    write_chksum();

    cs=0;
    fprintf(fhex, "\n");

}

fprintf(fhex, ":00000001FF")  ;

return 0 ;
}

void write_chksum()
{
	fhex = fopen("filehex" , "r+");
	c=count+2; // counter for hex file lines + 2
cs=0;

	while(c)
	{   // the program will  read the lines by the : , so it has to read accourding to number of lines it's write on to wriite the cs in the right place

	  while(dig1 != ':')
          dig1 = fgetc(fhex);

	dig1=0;
	c--;
	}


	while(!feof(fhex))
	{

	dig1 = fgetc(fhex);//reading the 2 digits each time
	dig2 = fgetc(fhex);
	//printf("%c  %c\n",dig1,dig2);
		sprintf(sk, "%c%c", dig1,dig2); //converting the digits to string sk

		s=hexdec(sk); // converting the string to hex dec. num
		//printf("%d\n",s);
		if(s!=-1) // making sure that it reades right hex num , if the hex is wrong the func ret -1
	  cs+=s;
		//printf("%d\n",cs);
	}

	dectohex(cs,dechex) ; //converting the number to hex to get only it's low nible "first 2 hex nums"
	//printf("%s\n",dechex);

	uc=hexdec(dechex); // converting the two digits back to dec with implicit casting to unsigned char

	uc=~uc+1 ; // taking the 2's complement

	dectohex(uc,dechex) ; //returning the num back to hex
	//printf("%c  %c",dechex[0],dechex[1]);
	fputc(dechex[0],fhex); //printing the cs in the hex file
	fputc(dechex[1],fhex);

}


void init()
{
	fasm = fopen("out.asmadd" , "r"); //opening the file and printing the first fixed line
	fhex = fopen("filehex" , "w");
	fi = fopen("filei" , "w");
	fprintf(fhex, ":020000020000FC\n");


}
void ifile()
{
	char *token ;
	while(!feof(fasm))
	{
	fgets (x, 100, fasm); // reding all the data from the asm file line by line
	//printf("%s\n",x);

	for(i=0 ; i<100;i++)
	{
		if(x[i]==',') // converting every ","  in the file to space
	      x[i]=' ';

	 if(x[i]=='\0') //  converting every NULL   in the file to space
	      x[i]=' ';

	}

	   /* get the first token */

	fwrite(x , 1 , sizeof(x) , fi ); //write the modified asm file to an intermediate file

	}

	fclose(fi);
	fclose(fasm);
}
void lins_num()
{
	fi = fopen("filei" , "r");
	fasm = fopen("out.asmadd" , "r");

	//fprintf(fhex, ":")   ;
	while(!feof(fasm))
	{
	  ch = fgetc(fasm);
	  if(ch == '\n')
	  {
	    lines++;   //counting how many lines in the asm file
	  }

	}
	//printf("%d",lines);
	flines=lines; // fixed lines number
	lns=lines/8;
	lmod=lines%8 ;

}
void read ()
{
	fscanf(fi,"%s%s%s",command,entry_1,entry_2); //reading first 3 strings from each line in the asm file
//printf("\n%s%s%s",command,entry_1,entry_2);
//	if(isdigit(atoi(command)))
//		strcpy(command," ");

}

void write_rlen()
{
	if(count<lns)
	fprintf(fhex, ":10")  ; // if there is a complete line the rec length will be 10

	else
	{

	lmod*=2 ; // if the line is not complete , each line of inst take 2 bytes
	if (lmod>9)
	{
	lmod%=10 ;
	switch(lmod)
	{
	case 0 : fprintf(fhex, ":0A")  ;
	break;
	case 1 : fprintf(fhex, ":0B")  ;
	break;
	case 2 : fprintf(fhex, ":0C")  ;
	break;
	case 3 : fprintf(fhex, ":0D")  ;
	break;
	case 4 : fprintf(fhex, ":0E")  ;
	break;
	case 5 : fprintf(fhex, ":0F")  ;
	break;
	}

	}
	else
	{
	itoa(lmod, lmd, 10);
	fprintf(fhex, ":0");
	 fprintf(fhex, lmd) ;
	}
	}
}

void write_add()
{
	char j[100] ;

	   if (count>9)
		{

				switch(count)
				{
				case 10 : count='A'  ;
				break;
				case 11 : count='B'  ;
				break;
				case 12 : count='C'  ;
				break;
				case 13 : count='D'  ;
				break;
				case 14 : count='E' ;
				break;
				case 15 :count='F' ;
				break;
				}

		}

	if(count<=16)
	{

		fprintf(fhex, "00");
		itoa(count,j,10);
		fprintf(fhex, j);
	    fprintf(fhex, "0");
	}
	else if (count>(16) && count<=(32) )
	{
		fprintf(fhex, "01");
				itoa(count,j,10);
				fprintf(fhex, j);
			    fprintf(fhex, "0");
	}
	else if (count>(32) && count<=(48) )
		{
			fprintf(fhex, "02");
					itoa(count,j,10);
					fprintf(fhex, j);
				    fprintf(fhex, "0");
		}

    }

void write_rectyp(void)
{
	fprintf(fhex, "00");

}
void write_data()
{
	//printf("%d",lines);
	int count =0 ,k=0 ,l=0;
	unsigned char com[100] , obc[100] ,reg1[100],reg2[100]="NULL" , tobc[100] , hexstr1[10]="0",hexstr2[10]="0",hexstr3[10]="0",hexstr4[10]="0";

	FILE *fot;

  fot=fopen("opcodtable" , "r");
	while(lines)
		{
			read();
			//printf("\n%s",command);
		while(!feof(fot))
		{
	      fscanf(fot,"%s%s",com,obc);
		      if(!strcmp(command,com))
		    	  strcpy(tobc,obc);
		      else if(!strcmp(entry_1,com))
		          strcpy(reg1,obc);
		      else if(!strcmp(entry_2,com))
		    	  strcpy(reg2,obc);
		}
		//

		if(!strcmp(command ,"SBI")||!strcmp(command ,"CBI")||!strcmp(command ,"SBIC")||!strcmp(command ,"SBIS"))
			{
			dectobin(atoi(entry_2));

			strcat(reg1,st1);

			bintodec(atoi(reg1));

			dectohex(hexadecimalNumber,hexstr4);

			 printf("\n%s",hexstr4);
			                    printf("\n%s",tobc);
			fprintf(fhex, hexstr4);
			 fprintf(fhex, tobc);
			}
		else if(!strcmp(command ,"ADD")||!strcmp(command ,"ADC")||!strcmp(command ,"AND")||!strcmp(command ,"CP")||!strcmp(command ,"EOR")||!strcmp(command ,"MOV")||!strcmp(command ,"SUB")||!strcmp(command ,"OR")||!strcmp(command ,"SBC")||!strcmp(command ,"MUL"))
		{

          append(tobc,reg2[0]);

          strcat(tobc,reg1);
          for(k=1;k<5;k++)
        	  append(tobc,reg2[k]);


          for(k=0;k<8;k++)
          {
        	  hexstr1[k]=tobc[k];
          }
          bintodec(atoi(hexstr1));

          dectohex(hexadecimalNumber,hexstr2);

          for(k=8;k<16;k++)
              {
        	     hexstr[l]=tobc[k];
        	     l++;
              }

          bintodec(atoi(hexstr));
//          printf("\n%s",hexstr);
                    dectohex(hexadecimalNumber,hexstr3);
//                   printf("\n%s",hexstr3);
//                    printf("\n%s",hexstr2);
          fprintf(fhex, hexstr3);
          fprintf(fhex, hexstr2);
		}
		else
		{
			 fprintf(fhex, tobc);
		}

	lines--;
	count++;
	if (count==8)
		break;
l=0;
	fseek(fot, 0, SEEK_SET );
		}

	fclose(fhex);

}



long hexdec(unsigned const char *hex) {
   long ret = 0;
   while (*hex && ret >= 0) {
      ret = (ret << 4) | hextable[*hex++];
   }
   return ret;
}

void dectohex(long int decimalNumber ,unsigned char *hexadecimalNumber)
{
long int quotient;
long int i=1,temp;

	quotient = decimalNumber;

	while(quotient!=0) {
		temp = quotient % 16;
		//To convert integer into character
		if( temp < 10)
         temp =temp + 48;
    else
         temp = temp + 55;

		hexadecimalNumber[i--]= temp;
		quotient = quotient / 16;
	}

}
void dectobin(int decimalno)
{

	int i =0 ,d;
	unsigned char stt[50];
	if(decimalno==0)
			strcpy(st1,"000");
	while(decimalno>0)
	      {
	           d=decimalno%2;
	           itoa(d,stt,10);
	           if(i==0)
	           strcpy(st1,stt);
	           else
	        	   strcat(st1,stt);
	           i++;
	           decimalno=decimalno/2;
	      }
	if(strlen(st1)==1)
		strcat(st1,"00");
	else if(strlen(st1)==2)
		strcat(st1,"0");


	strrev(st1);
}
void bintodec( long int binaryNumber)
{

	long int j=1,remainder;
	hexadecimalNumber=0;

	    while(binaryNumber!=0){
	    remainder=binaryNumber%10;
	    hexadecimalNumber=hexadecimalNumber+remainder*j;
	        j=j*2;
	        binaryNumber=binaryNumber/10;

	      }

}
void append(char* s, char c)
{
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}
