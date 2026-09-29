#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileIO.h"

//globals
const char messageLoc[] = "DATAM.DAT";
int numSysMessages;
const char messageIndexLoc[]="IDXM.DAT";
char *sysMessages;
char message[255];

//functions
void loadSystemMessages();
void reverse(char *, int , int );
int getSizeOfFile(const char *);
void showMessage(int);
void getMessage(int);

//

int main() {
	
	//bdos(14,1); switch drive
	printf("\016");					// clear the screen
	numSysMessages = getSizeOfFile((char *)"IDXM.DAT");
	printf("Size :%d\n",numSysMessages);
	
	loadSystemMessages();
	showMessage(22);

    return 0;
}







void reverse(char *s, int start, int length)
{
  int c,i,j;

  for (i=0,j=length-1;i<j;i++,j--)
  {
    c=s[start+i];
    s[start+i]=s[start+j];
    s[start+j]=c;
  }
}

void showMessage(int number)
{
   /* load and display a message*/

   int x,xpos,slen,wl,printLoop;

   getMessage(number);
   slen=strlen(message);
   xpos=1;

   x=0;

   while (x<slen){
     wl=0;

     /* get the next word (or end of string)*/
     while (message[x+wl]!=' ' && x+wl < slen)
     {wl++;}

     if (xpos + wl > 38 )
     {putchar('\n');xpos=0;}

     /* print out the word */
     for (printLoop=0;printLoop<wl;printLoop++,x++)
     {putchar(message[x]);}

     xpos=xpos+wl+1;/* +1 for the space */

     if (xpos < 38 && xpos > 0) /* no space at end or start of line*/
	 {putchar(' ');}else{xpos--;}
     x++;
   }

   message[0]='\0';

}

