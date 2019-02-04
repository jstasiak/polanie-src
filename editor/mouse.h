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
class Mouse
      {
       public:

    int X;
    int Y;
    int oldX;
    int oldY;
    int Button;
    int Key;

    int Ile(int);
    int  GetMsg(void);
    void ButtonUp(void);
    int  MouseInit(void);
    void ReadMouse(void);
    void MoveCursor(int,int);
    int  MWindow(int,int,int,int);
    };
extern Mouse mouse;

#endif

