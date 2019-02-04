////////////////////////////////////////////////////////////////////////
//mouse.cpp
//
////////////////////////////////////////////////////////////////////////////////
#include <bios.h>
#include <conio.h>
#include "mouse.h"
Mouse mouse;
//--------------------------------------------------------------
int Mouse::Ile(int button)
{
    union REGS r;
    r.h.ah = 0x0;
    r.h.al = 0x05;
    r.w.bx = (short)button;
    int386(0x33, &r, &r);    

    return (int)r.w.bx;
}
//-------------------------------------------------------------
void Mouse::TButtonUp()
{
while(Button>0)TReadMouse();
}
//-----------------------------------------------------------------
void Mouse::GButtonUp()
{
while(Button>0)GReadMouse();
}
//--------------------------------------------------
int Mouse::GGetMsg()
{
if(kbhit())
{
    Key=getch();
    if(!Key)Key=getch();
    return 1;
}
Key=0;
GReadMouse();
return Button;
}
//-------------------------------------------
int Mouse::GetMsg13h()
{
if(kbhit())
{
    Key=getch();
    if(!Key)Key=getch();
    return 1;
}
Key=0;
ReadMouse13h();
if(!Button)Button=Ile(1);
return Button;
}
//-------------------------------------------

int Mouse::TGetMsg()
{
if(kbhit())
{
    Key=getch();
    if(!Key)Key=getch();
    return 1;
}
Key=0;
TReadMouse();
return Button;
}

//---------------------------
int Mouse::MWindow(int x1,int y1,int x2,int y2)
{
if((Button)&&(X>=x1)&&(X<=x2)&&(Y>=y1)&&(Y<=y2))
                    return 1;
return 0;
}


int Mouse::MouseInit()
{
    union REGS r;
    
    r.w.ax = 0;
    int386(0x33, &r, &r);    
    r.h.ah = 0x0;
    return (int)r.w.ax;    
}

void Mouse::TextCursor()
{
/*    
asm{
mov ax,0ah
xor bx,bx
mov cx,0ffffh
mov dx,7700h
int 33h
}*/
}

void Mouse::ShowCursor()
{
    /*
asm{
mov ax,01h
int 33h
}*/
}

void Mouse::HideCursor()
{/*
asm{
mov ax,02h
int 33h
}*/
}

void Mouse::TReadMouse()
{
        union REGS r;
    
    r.w.ax = 3;
    int386(0x33, &r, &r);    
    Button=(int)r.w.bx;
    X= (int)(r.w.cx>>3);
    Y= (int)(r.w.dx>>3);
}
//1234567890
void Mouse::GReadMouse()
{
    union REGS r;
    
    r.w.ax = 3;
    int386(0x33, &r, &r);    
    Button=(int)r.w.bx;
    X= (int)(r.w.cx);
    Y= (int)r.w.dx;
}
void Mouse::ReadMouse13h()
{
    union REGS r;
    
    r.w.ax = 3;
    int386(0x33, &r, &r);    
    Button=(int)r.w.bx;
    X= (int)(r.w.cx>>1);
    Y= (int)r.w.dx;
}

void Mouse::TMoveCursor(int x,int y)
{
    /*
_CX=x;
_DX=y;
asm{
dec cx
shl cx,03h
dec dx
shl dx,03h
mov ax,04h
int 33h
}*/
}

void Mouse::GMoveCursor(int x, int y)
{
    union REGS r;
    r.w.cx = (short)x;
    r.w.dx = (short)y;
    r.w.ax = 4;
    int386(0x33, &r, &r);    

}

void Mouse::TMWindow(int x1,int y1,int x2,int y2)
{/*
_CX=x1;
_DX=x2;
asm{
dec cx
shl cx,03h
dec dx
shl dx,03h
mov ax,07h
int 33h
}
_CX=y1;
_DX=y2;
asm{
dec cx
shl cx,03h
dec dx
shl dx,03h
mov ax,08h
int 33h
}*/
}

void Mouse::GMWindow(int x1,int y1,int x2,int y2)
{/*
_CX=x1;
_DX=x2;

asm{
 mov ax,07h
 int 33h
 }
_CX=y1;
_DX=y2;
asm{
    mov ax,08h
    int 33h
    }*/
}

// Przesuniecie poziome i pionowe
void Mouse::TMCounter(int &dx ,int &dy)
{/*
asm{
mov ax,0bh
int 33h
shr cx,03h
inc cx
shr dx,03h
inc dx
}
dx=_CX;
dy=_DX;*/
}
//-------------------------------------
void Mouse::GMCounter(int &dx ,int &dy)
{/*
asm{
mov ax,0bh
int 33h
}
dx=_CX;
dy=_DX;*/
}
//-----------------------------------
void Mouse::TMouseTrap(int x1,int y1, int x2 ,int y2)
{/*
_CX=x1;
_SI=x2;
_DX=y1;
_DI=y2;
asm{
dec cx
shl cx,03h
dec dx
shl dx,03h
dec si
shl si,03h
dec di
shl di,03h
mov ax,10h
int 33h
}*/
}
void Mouse::GMouseTrap(int x1,int y1, int x2 ,int y2)
{/*
_AX=0x10;
_CX=x1;
_SI=x2;
_DX=y1;
_DI=y2;
asm{
   mov ax,10h
   int 33h
   }*/
}


void Mouse::SetMouseSpeed(int speed)
{/*
_AX=0x13;
_DX=speed;
asm{mov ax,13h
    int 33h
    }*/
}
//////////////////////////////////////////////
void Mouse::SetMouseSensitiv(int dx,int dy)
{/*
_AX=0xf;
_CX=dx;
_DX=dy;
asm{mov ax,0fh
    int 33h
    }*/
}
///////////////////////////////////////////////////////////
void Mouse::SetCursorMask(int *tab,int x, int y)
{/*
 unsigned sg = FP_SEG(tab);
 unsigned of = FP_OFF(tab);

 asm pusha
 asm push es

 asm mov ax,9
 asm mov bx,x
 asm mov cx,y
 asm mov es,sg
 asm mov dx,of
 asm int 0x33

 asm pop es
 asm popa*/
}
