//////////////////////////////////////////
//
//    Modul glowny
/////////////////////////////////////////
#include "ems.h"
#include "image13h.h"
#include "instrum.h"
#include "menegdma.h"
#include "mouse.h"
#include "mover.h"
#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <malloc.h>
#include <process.h>
#include <stdio.h>

//========zmienne=====================

int endGame = 0;
int level = 26;
char *picture[MaxPictures], *missiles[4][3][3], *tlo, *Mysz[13];
char *movers[5][13][3][3], *ramka[4]; // faza:typ:dx:dy
char *buttons[16];
char *dead[3], *Hit[2];
char *shadow;
char *face[16];   // 0-9 twarze, 10-15 budynki
char *Buttons[4]; // moze byc [8] bo save i smenu to te same przyciski

//======zmienne extern===========
extern int licznik;
extern char prowintion[25];
extern char prowintionInit[25];
extern char hasla[10];
extern unsigned char MVol;
//=======Deklaracje funkcji=============

void MainMenuDispatchEvent(void);
void ShowMainEditMenu();
void PressButton(int B, int P);
void MouseEngine(void);
//====== Funkcje extern ======================
extern void InitPicture(void);
extern void FreeMemory(void);
extern void ShowMainMenu(void);
extern void Edit(int);
extern void ShowSubMenu(void);
extern void ShowIntro(void);
extern void ShowPicture(int, int);
extern void ShowText(int, int);
extern void Haslo(char *, char);
extern int GetMemory(void);
//////////////////////////////////
//////////////////////////////////////////
extern int ladowanie(void);
////////////////////////////////////////////////////////////////////////////
////////// Funkcje z Mirkowego Adliba///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
extern void UstawAdliba(void);
extern void WylaczAdliba(void);
extern void MusicOn(void);
extern int GetPaternMemory(void);
extern int FreePaternMemory();

////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
void main() {
  if (InitBuffers13h()) {
    cprintf(
        "BLAD !!!\n\rBrak pamieci operacyjnej. \n\rProgram wymaga 586kB RAM\n");
    return;
  }
  Init13h();
  BlackPalette();
  // LoadExtendedPalette(1);
  // SetExtendedPalette();
  if (InitText13h()) {
    FreeBuffers13h();
    Close13h();
    cprintf("Blad inicjacji pamieci na czcionki, lub brak pliku FONT.DAT");
    return;
  }
  // getch();
  Close13h();
  if (!mouse.MouseInit()) {
    FreeBuffers13h();
    ClearText13h();
    cprintf("BLAD !!!\n\r----------- Brak sterownika myszy.---------");
    return;
  }
  //------------------------------------------
  // cprintf("Alokacja pamieci EMS ..................................  \n\r");
  // if(!EMS::SegmentRamki()){FreeBuffers13h();ClearText13h();cprintf("BLAD
  // !!!\n\r----------- Brak pamieci EMS ! Patrz
  // instrukcja.-------------");return;} if(ladowanie())
  // {FreeBuffers13h();ClearText13h(); cprintf("BLAD !!!\n\r----------- Brak
  // pliku SETUP.INI.  Uruchom program SETUP.EXE.-----------\n"); return; }
  //----------------------------------------------------------------------------

  //--------------------------------------------

  cprintf("\n\rAlokacja pamieci niskiej - "
          "grafika..................................  \n\r");
  if (GetMemory()) {
    FreeBuffers13h();
    ClearText13h();
    cprintf(
        "BLAD !!!\n\rBrak pamieci operacyjnej. \n\rProgram wymaga 586kB RAM\n");
    return;
  }

  cprintf("Alokacja ekranu wirtualnego......................  \n\r");
  if (!InitVirtualScreen()) {
    ClearText13h();
    FreeBuffers13h();
    cprintf("BLAD !!!\n\rBrak pamieci operacyjnej. Program wymaga 586kB RAM\n");
    return;
  }

  //<<<<  muzyka
  // UstawAdliba();
  cprintf("Alokacja pamieci niskiej - muzyka .........  \n\r");
  // if(GetPaternMemory()){FreeBuffers13h();WylaczAdliba();FreeMemory();ClearText13h();cprintf("BLAD
  // !!!\n\rBrak pamieci operacyjnej na muzyke.\n\rProgram wymaga 586kB
  // RAM\n\rZwieksz ilosc wolnej pamieci lub wylacz muzyke
  // (SETUP.EXE).");return;} cprintf("Ladowanie muzyki .........  \n\r");
  //-----

  cprintf("Otwarcie plikow ........\n\r");
  OpenPaletteFile();
  OpenGraphicFile();

  Init13h();

  SetScreen(0);
  // ShowIntro();

  SetScreen(1);

  InitPicture(); // screen=1
  ClearScreen13h();
  SetScreen(0); // screen=0
  // delay(500);

  ClearScreen13h();
  LoadExtendedPalette(1);
  SetExtendedPalette();
  do {
    ShowMainEditMenu();
    MouseEngine();
    MainMenuDispatchEvent();

  } while (!endGame);

  // ShowVirtualScreen();

  DownPalette(7);

  //=======Zakonczenie programu===========

  // WylaczAdliba();
  // FreePaternMemory();
  FreeBuffers13h();
  // ClearText13h();
  FreeMemory();
  FreeVirtualScreen();
  ClosePaletteFile();
  CloseGraphicFile();
  Close13h();
}
/////////////////////////////////////////////////////////////////////////
//
//    Obsluga menu
////////////////////////////////////////////////////////////////////////

void MainMenuDispatchEvent(void) {
  if (mouse.MWindow(100, 160, 200, 180) || mouse.Key == 9579 ||
      mouse.Key == 'k') // k=9579 q=4209
  {
    endGame = 1;
    return;
  }

  if (mouse.MWindow(100, 30, 200, 50) || mouse.Key == 'e') // Edit level
  {
    Edit(1);
    endGame = 0;
    ClearScreen13h();
    LoadExtendedPalette(1);
    SetExtendedPalette();
    return;
  }

  if (mouse.MWindow(205, 30, 225, 50) || mouse.Key == 75) {
    if (level < 60)
      level++;
    return;
  }
  if (mouse.MWindow(75, 30, 95, 50) || mouse.Key == 77) {
    if (level > 26)
      level--;
    return;
  }
}
////////////////////////////////////////////////////////////////////////
//   Przyciski
////////////////////////////////////////////////////////////////////////
void PressButton(int B, int P) {
  switch (B) {
  case 1:
    PutImage13h(108, 30, Buttons[P], 1);
    break; // zachowaj  save1
  case 2:
    PutImage13h(108, 57, Buttons[P], 1);
    break; // wgraj     save2
  case 3:
    PutImage13h(108, 87, Buttons[P], 1);
    break; // restart   save3
  case 4:
    PutImage13h(108, 114, Buttons[P], 1);
    break; // continue  save4
  case 5:
    PutImage13h(108, 142, Buttons[P], 1);
    break; // koniec    anuluj
  case 11:
    PutImage13h(274, 38, Buttons[P], 1);
    break; // stop      mag
  case 12:
    PutImage13h(274, 58, Buttons[P], 1);
    break; // move      lucznik
  case 13:
    PutImage13h(274, 78, Buttons[P], 1);
    break; // atak      rycerz
  case 14:
    PutImage13h(274, 98, Buttons[P], 1);
    break; //          krowa
  case 15:
    PutImage13h(274, 118, Buttons[P], 1);
    break; //          mur
  case 16:
    PutImage13h(274, 138, Buttons[P], 1);
    break; //          mur
  }
}
////////////////////////////////////////////////////////////////////////
//    Myszka
///////////////////////////////////////////////////////////////////////

void MouseEngine() {
  mouse.ReadMouse();
  if (mouse.X > 300) {
    mouse.X = 300;
    mouse.MoveCursor(600, mouse.Y);
  }
  if (mouse.Y > 180) {
    mouse.Y = 180;
    mouse.MoveCursor(mouse.X * 2, 180);
  }

  mouse.oldX = mouse.X;
  mouse.oldY = mouse.Y;
  GetImage13h(mouse.X, mouse.Y, mouse.X + 15, mouse.Y + 14, Mysz[0]);
  PutImage13h(mouse.X, mouse.Y, buttons[6], 1);
  do {
  } while (mouse.GetMsg());
  do {
    if (mouse.oldX != mouse.X || mouse.oldY != mouse.Y) {
      PutImage13h(mouse.oldX, mouse.oldY, Mysz[0], 0);
      if (mouse.X > 300) {
        mouse.X = 300;
        mouse.MoveCursor(600, mouse.Y);
      }
      if (mouse.Y > 180) {
        mouse.Y = 180;
        mouse.MoveCursor(mouse.X * 2, 180);
      }
      mouse.oldX = mouse.X;
      mouse.oldY = mouse.Y;
      GetImage13h(mouse.X, mouse.Y, mouse.X + 15, mouse.Y + 14, Mysz[0]);
      PutImage13h(mouse.X, mouse.Y, buttons[6], 1);
    }
  } while (!mouse.GetMsg());
  PutImage13h(mouse.oldX, mouse.oldY, Mysz[0], 0);
}

void ShowMainEditMenu() {
  Bar13h(100, 30, 200, 50, 0);
  Rectangle13h(100, 30, 200, 50, 255);
  Rectangle13h(75, 30, 95, 50, 255);
  Rectangle13h(205, 30, 225, 50, 255);
  Rectangle13h(100, 160, 200, 180, 255);
  OutText13h(130, 165, "Koniec", 255);
  char ss[50];
  sprintf(ss, "Edytuj poziom %d", level - 25);
  OutText13h(110, 35, ss, 255);
}
