/* Tatung functions */

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

unsigned char pos(void) {
    #asm
        // On the Tatung Einstein, the MOS explicitly records the current 
        // cursor column (0-39 or 0-79) directly at memory address 0xFB4A
        ld a, ($FB4A)
        
        // Pass the 8-bit result safely back into C using the L register
        ld l, a
        ld h, 0
    #endasm
}

void disable_einstein_cursor_flag(void) {
    #asm
        // 1. Set the cursor's logical tracking positions to 0.
        // This ensures the next printf() starts exactly at column 0.
        ld a, 0
        ld ($FB4A), a   ; Column X = 0
        ld ($FB4B), a   ; Row Y = 0

        // 2. Clear out the primary VRAM tracking pointer ($02FB).
        // The base VRAM offset for the 80-column display grid on row 0 
        // is exactly 9 bytes. This forces alignment back to the left edge.
        ld hl, 9
        ld ($02FB), hl

        // 3. THE MAME FIX: Change the 6845 hardware cursor base address registers.
        // By changing Registers 14 and 15 (Cursor Address High/Low) inside the 
        // system shadow maps to an out-of-bounds page index (like 0x3FFF), 
        // MAME renders the cursor block off-screen.
        
        ld a, $3F
        ld ($0329), a   ; Overwrite 6845 Register 14 Shadow (Cursor Address High)
        ld a, $FF
        ld ($032A), a   ; Overwrite 6845 Register 15 Shadow (Cursor Address Low)
    #endasm
}



void mode(int m)
{
	//mode 0 is 80 column mode
	//mode 7 is 40 column mode
	if (m==0){
		//printf("\x10");
		printf("\x14");
	}
	else
	{
		printf("\016"); //40 column mode
	}	
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
