#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//globals
const char messageLoc[] = "DATAM.DAT";
int numSysMessages;
const char messageIndexLoc[]="IDXM.DAT";
unsigned char *sysMessages;
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

//replaces getSizeOfFile();
int getSizeOfFile(const char *filename) {
	
	//size is held in the first two bytes
    FILE *f = fopen(filename, "rb");
    if (!f) return -1;

    int size = 0;
    int ch;
	ch = fgetc(f);
	//printf("%d\n",ch);
	size = ch * 256;
	ch = fgetc(f);
	//printf("%d\n",ch);
	size+= ch;
	fclose(f);
	//printf("%d\n",size);
    return size;
	
    // Read byte-by-byte to find the actual EOF marker
    while ((ch = fgetc(f)) != EOF) {
		
		printf("%d\n",ch);
        if (ch == 0x1A) { 
            break; // Stop counting if we hit the CP/M EOF pad character
        }
        size++;
    }

    fclose(f);
    return size;
}

long readIndex(int recordNumber, char *fileName)
{
 /* reads an integer from the index file */

 long index;
 int number;
 
 printf("rec n%d\n",recordNumber);

 FILE *fileptr;
 index=(((long)recordNumber-1)*2)+5;
 number=0;
 
 //printf(fileName);
 //printf("\n");
 printf("index %lu\n",index);

 fileptr=fopen(fileName,"rb");
 if (fileptr==NULL) { printf("Null file pointer in index");return 0;}

 fseek(fileptr,index,SEEK_SET);
 
 //get first byte
 number=fgetc(fileptr)*256;
 //get second byte
 number=number+fgetc(fileptr);
 printf("Pos in file %d\n",number);
 fclose(fileptr);
 return (long)number;

}



void loadSystemMessages()
{
   long index;
   int i,c,o,l;
   FILE *fileptr;
   fileptr=fopen(messageLoc,"rb");
   printf("Messages\n ");
   printf(messageLoc);
   printf("\n");

   if (fileptr==NULL) { printf("Null file pointer in system message");return;}
   /* get the index of message 19 as this will be the end of message 18 */

   index=readIndex(numSysMessages,(char *)messageIndexLoc);
   
   printf("returned index %lu\n",index);

   sysMessages=(char *)malloc(((int)index)*sizeof(char));
   fread(sysMessages,(int)index,1,fileptr);
   fclose(fileptr);
	printf("Read.");

   c=0;
   //while (c<index){
	//	i=sysMessages[c];c=c+1;l=sysMessages[c];
	//	reverse(sysMessages,c+1,l);
	//	/* step through each character of the word based on length */
	//	o=1;
	//	while (o<l+1){
	//	  c++;o++;
	//	}
	//	c++;/*start of next word*/
	//}
 printf("Messages done.\n");
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

void getMessage(int number)
{
   long index;
   FILE *fileptr;
   int msgLen,x,y,num,start,end;

   memset(&message[0],'\0',sizeof(message));

   if (number < numSysMessages){
    /* start with first message and step through */
    /* until the correct number is found */

		end=1;
		for (num=1;num<=number;num++)
		{
		   msgLen=(int)sysMessages[end]; /* len of current message */
		   printf("Message : %d len : %d \n",num,msgLen);
		   end=end+msgLen+(int)2;
		}
		start=end-msgLen;
		end--;

		for (x=0,y=start-1;x<msgLen;x++,y++)
		{
		  message[x]=sysMessages[y];
		}
		message[x]='\0';

   }else{
	printf("no message");   
   }   
}