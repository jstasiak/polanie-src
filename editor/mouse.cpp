////////////////////////////////////////////////////////////////////////
// mouse.cpp
//
////////////////////////////////////////////////////////////////////////////////
#include "mouse.h"
#include <bios.h>
#include <conio.h>
Mouse mouse;
//--------------------------------------------------------------
int Mouse::Ile(int button) {
  union REGS r;
  r.h.ah = 0x0;
  r.h.al = 0x05;
  r.w.bx = (short)button;
  int386(0x33, &r, &r);

  return (int)r.w.bx;
}
//-----------------------------------------------------------------
void Mouse::ButtonUp(void) {
  while (Button > 0)
    ReadMouse();
}
//--------------------------------------------------
//-------------------------------------------
int Mouse::GetMsg() {
  Button = 0;
  if (kbhit()) {
    Key = getch();
    if (!Key)
      Key = getch();
    return 1;
  }
  Key = 0;
  ReadMouse();
  // if(!Button)Button=Ile(1);
  return Button;
}
//-------------------------------------------

//---------------------------
int Mouse::MWindow(int x1, int y1, int x2, int y2) {
  if ((Button) && (X >= x1) && (X <= x2) && (Y >= y1) && (Y <= y2))
    return 1;
  return 0;
}

int Mouse::MouseInit() {
  union REGS r;

  r.w.ax = 0;
  int386(0x33, &r, &r);
  r.h.ah = 0x0;
  return (int)r.w.ax;
}

void Mouse::ReadMouse() {
  union REGS r;

  r.w.ax = 3;
  int386(0x33, &r, &r);
  Button = (int)r.w.bx;
  X = (int)(r.w.cx >> 1);
  Y = (int)r.w.dx;
}

void Mouse::MoveCursor(int x, int y) {
  union REGS r;
  r.w.cx = (short)x;
  r.w.dx = (short)y;
  r.w.ax = 4;
  int386(0x33, &r, &r);
}

//////////////////////////////////////////////
