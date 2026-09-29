/* Beeb functions */


void vdu25(int,int,int);
void mode(int);
void moveCursor(int,int);
void tab(int,int);   
void moveXY(int,int);
void origin(int,int);
void line(int,int);
void drawLine(int,int,int,int);       
void drawBox(int,int,int,int);
void gcol(int,int);
void lcol(int,int);
int inkey(int,int);
void cursorEdit(int);
void cursorOff(void);
void textWindow(int,int,int,int);
void graphWindow(int,int,int,int);
void mode(int m);

#define LINECOLOUR 2
#define HIGHLIGHTCOLOUR 3     

int curX,curY;

void mode(int m)
{
/* direct call to oswrch via kernel.h */

}

void moveCursor(int x, int y)
{

}

void oscli(char s[])
{


}

void cursorEdit(int on)
{

}

void vdu(int code)
{

}

void vdu25(int t,int x,int y)
{
}  

int pos(int xy)
{
 int retVal;
 retVal=1;
 return retVal;
}

int inkey(int lsb,int msb)
{  
   int result;

}

void tab(int x, int y)
{

}

void cursorOff()
{

}

void moveXY(int x,int y)
{
 vdu25(4,x,y);
}             

void origin(int x,int y)
{
  moveXY(x,y);
  curX=x;
  curY=y;
}

void movePen(int angle, int length)
{
   if (angle==0){curY=curY+length;}
   if (angle==90){curX=curX+length;}
   if (angle==180){curY=curY-length;}
   if (angle==270){curX=curX-length;}
}

void line(int angle,int length)
{   
 /*int l;*/
 moveXY(curX,curY);

 if (angle==0)
  {
    vdu25(1,0,length);
    curY=curY+length;
  }
 if (angle==90)
  {
    vdu25(1,length,0);
    curX=curX+length;
  }                   
 if (angle==180)
  {    
    moveXY(curX,curY-length);vdu25(1,0,length);
    curY=curY-length;
   }
 if (angle==270)
  {
    moveXY(curX-length,curY);vdu25(1,length,0);
    curX=curX-length;
  }
}            

void drawLine(int x,int y,int ex,int ey)
{
 vdu25(4,x,y);vdu25(5,ex,ey);
}                    

void drawBox(int x,int y,int w, int h)
{
 moveXY(x,y);
 vdu25(5,w+x,y);vdu25(5,w+x,h);vdu25(5,x,h);vdu25(5,x,y);
}    

void lcol(int a, int c)
{

}

void gcol(int a,int c)
{

}

void clg()
{

}   
