///////////////////////////////////////////////////////////
// MOUS.H: Mouse class header file.
///////////////////////////////////////////////////////////

#ifndef __MOUSE_H
#define __MOUSE_H

#include <dos.h>

// Mouse button values.
#define NoButon 0
#define LeftButon 1
#define RightButon 2
#define BothButon 3

// The mouse class declaration.
class Mouse {
public:
  int X;
  int Y;
  int oldX;
  int oldY;
  int Button;
  int Key;

  int Ile(int);
  int GGetMsg(void);
  int GetMsg13h(void);
  int TGetMsg(void);
  void TButtonUp(void);
  void GButtonUp(void);
  int MouseInit(void);
  void ShowCursor(void);
  void HideCursor(void);
  void TextCursor(void);
  void TReadMouse(void);
  void GReadMouse(void);
  void ReadMouse13h(void);
  void TMoveCursor(int, int);
  void GMoveCursor(int, int);
  void TMWindow(int, int, int, int);
  void GMWindow(int, int, int, int);
  void TMCounter(int &, int &);
  void GMCounter(int &, int &);
  void TMouseTrap(int, int, int, int);
  void GMouseTrap(int, int, int, int);
  void SetMouseSpeed(int);
  void SetMouseSensitiv(int, int);
  void SetCursorMask(int *, int, int);
  int MWindow(int, int, int, int);
};
extern Mouse mouse;

#endif
