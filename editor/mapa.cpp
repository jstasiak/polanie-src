//////////////////////////////////////////////////////////////////////
//   Modul zawiera funkcje sluzace do obslugi grafiki bitmapowej
//
/////////////////////////////////////////////////////////////////////
#include "image13h.h"
#include "mouse.h"
#include "mover.h"
#include "zabezset.h"
#include <conio.h>
#include <malloc.h>
#include <stdlib.h>
/////////////zmienne//////////////////////////

char EndMap = 0;
char kolorK[5][5] = {{107, 107, 108, 109, 110},
                     {11, 29, 34, 36, 39},
                     {11, 118, 196, 198, 200},
                     {11, 87, 221, 222, 223},
                     {11, 11, 88, 91, 106}};
//          czerwony        zielony                zolty            niebieski
//          szary
int wsp1[25][4] = {
    {272, 68, 311, 97},   {246, 67, 271, 106},  {211, 67, 245, 100},
    {1, 8, 43, 48},       {46, 1, 82, 56},      {93, 2, 131, 52},
    {138, 7, 176, 37},    {185, 10, 222, 41},   {4, 68, 40, 111},
    {50, 66, 97, 110},    {105, 76, 144, 100},  {148, 76, 191, 102},
    {6, 121, 34, 145},    {41, 119, 65, 146},   {76, 121, 106, 146},
    {118, 117, 153, 147}, {163, 116, 202, 145}, {8, 169, 36, 193},
    {43, 151, 84, 197},   {86, 168, 123, 197},  {129, 161, 164, 197},
    {223, 112, 261, 147}, {174, 171, 216, 192}, {219, 162, 260, 192},
    {269, 160, 314, 195}};
//--------------------------------------------------------------------------
int wsp2[25][2] = {{110, 10},  {90, 15},   {63, 23},   {60, 49},   {86, 24},
                   {114, 35},  {142, 44},  {171, 49},  {81, 79},   {102, 73},
                   {134, 69},  {160, 76},  {94, 114},  {115, 104}, {135, 89},
                   {163, 98},  {188, 89},  {108, 128}, {130, 104}, {156, 123},
                   {191, 112}, {212, 103}, {117, 142}, {153, 145}, {182, 142}};
////////////zmienne zewnetrzne/////////////////// 87,142
extern int level, diff;
extern char prowintion[25];
extern char prowintionA[25];
extern char placeN[mMaxX][mMaxY];
extern int licznik;
extern char *Mysz[13], *buttons[16];
extern int MaxX;
extern int MaxY;
////////////funkcje//////////////////////////////
void NextConquest(void);
extern void ShowPicture(int, int);
//////////// ciala //////////////////////////////////
void DispatchMapEvent(void) {
  int i;
  for (i = 0; i < 25; i++) {
    if (mouse.MWindow(wsp2[i][0] + 5, wsp2[i][1] + 5, wsp2[i][0] + 30,
                      wsp2[i][1] + 25) &&
        prowintionA[i]) {
      level = i + 1;
      EndMap = 1;
      return;
    }
  }
  if (mouse.MWindow(272, 148, 317, 160)) {
    diff = 0;
    Rectangle13h(272, 148, 317, 160, 201);
    Rectangle13h(272, 163, 317, 175, 1);
    Rectangle13h(272, 178, 317, 190, 1);
  }
  if (mouse.MWindow(272, 163, 317, 175)) {
    diff = 1;
    Rectangle13h(272, 148, 317, 160, 1);
    Rectangle13h(272, 163, 317, 175, 201);
    Rectangle13h(272, 178, 317, 190, 1);
  }
  if (mouse.MWindow(272, 178, 317, 190)) {
    diff = 2;
    Rectangle13h(272, 148, 317, 160, 1);
    Rectangle13h(272, 163, 317, 175, 1);
    Rectangle13h(272, 178, 317, 190, 201);
  }
}
///////////////////////////////////////////////////////
void NextConquest(void) {
  int i, sw1 = 0;
  char *kraina = (char *)placeN;

  DownPalette(0);
  LoadExtendedPalette(6);
  SetScreen(1);
  ShowPicture(12, 0);
  ShowPicture(27, 100);
  SetScreen(0);
  ShowPicture(6, 0);
  ShowPicture(21, 100);
  if (kraina != NULL)
    for (i = 0; i < 25; i++) {
      SetScreen(1);
      GetImage13h(wsp1[i][0], wsp1[i][1], wsp1[i][2] + 1, wsp1[i][3] + 1,
                  kraina);
      SetScreen(0);
      PutImageChange13h(wsp2[i][0], wsp2[i][1], kraina, 1, 233,
                        kolorK[(prowintion[i] - 1)][0]);
    }
  if (!diff)
    Rectangle13h(272, 148, 317, 160, 201);
  if (diff == 1)
    Rectangle13h(272, 163, 317, 175, 201);
  if (diff == 2)
    Rectangle13h(272, 178, 317, 190, 201);
  mouse.GMoveCursor(1, 1);
  // CenterText13h(10,180,270,199,"W[hle Dein n[chstes Ziel.",1);
  // CenterText13h(11,180,271,199,"W[hle Dein n[chstes Ziel.",255);
  CenterText13h(10, 180, 270, 199, "Wybierz cel nastepnej wyprawy.", 1);
  CenterText13h(11, 180, 271, 199, "Wybierz cel nastepnej wyprawy.", 255);
  RisePalette(0);

  do {
    mouse.ReadMouse13h();
    if (mouse.X > 300) {
      mouse.X = 300;
      mouse.GMoveCursor(600, mouse.Y);
    }
    if (mouse.Y > 180) {
      mouse.Y = 180;
      mouse.GMoveCursor(mouse.X * 2, 180);
    }
    mouse.oldX = mouse.X;
    mouse.oldY = mouse.Y;
    GetImage13h(mouse.X, mouse.Y, mouse.X + 15, mouse.Y + 14, Mysz[0]);
    PutImage13h(mouse.X, mouse.Y, buttons[6], 1);
    do {
      if (licznik == 4 || licznik == 16 || licznik == 8 || licznik == 12 ||
          licznik == 20 || licznik == 24) {
        if (kraina != NULL)
          sw1 = licznik / 4;
        if (sw1 == 5)
          sw1 = 3;
        if (sw1 == 6)
          sw1 = 2;
        for (i = 0; i < 25; i++) {
          SetScreen(1);
          GetImage13h(wsp1[i][0], wsp1[i][1], wsp1[i][2] + 1, wsp1[i][3] + 1,
                      kraina);
          SetScreen(0);
          if (mouse.oldX > wsp2[i][0] - 16 && mouse.oldY > wsp2[i][1] - 14 &&
              mouse.oldX < wsp2[i][0] + 40 && mouse.oldY < wsp2[i][1] + 40) {
            PutImage13h(mouse.oldX, mouse.oldY, Mysz[0], 0);
          }
          if (prowintionA[i])
            PutImageChange13h(wsp2[i][0], wsp2[i][1], kraina, 1, 233,
                              kolorK[(prowintion[i] - 1)][sw1]);

          if (mouse.oldX > wsp2[i][0] - 16 && mouse.oldY > wsp2[i][1] - 14 &&
              mouse.oldX < wsp2[i][0] + 40 && mouse.oldY < wsp2[i][1] + 40) {
            GetImage13h(mouse.oldX, mouse.oldY, mouse.oldX + 15,
                        mouse.oldY + 14, Mysz[0]);
            PutImage13h(mouse.oldX, mouse.oldY, buttons[6], 1);
          }
        }
      }
      if (licznik > 23) {
        licznik = 0;
      }

      if (mouse.oldX != mouse.X || mouse.oldY != mouse.Y) {
        PutImage13h(mouse.oldX, mouse.oldY, Mysz[0], 0);
        if (mouse.X > 300) {
          mouse.X = 300;
          mouse.GMoveCursor(600, mouse.Y);
        }
        if (mouse.Y > 180) {
          mouse.Y = 180;
          mouse.GMoveCursor(mouse.X * 2, 180);
        }
        mouse.oldX = mouse.X;
        mouse.oldY = mouse.Y;
        GetImage13h(mouse.X, mouse.Y, mouse.X + 15, mouse.Y + 14, Mysz[0]);
        PutImage13h(mouse.X, mouse.Y, buttons[6], 1);
      }

    } while (!mouse.GetMsg13h());
    PutImage13h(mouse.oldX, mouse.oldY, Mysz[0], 0);
    DispatchMapEvent();
  } while (!EndMap);
  Bar13h(10, 70, 270, 90, 0);
  Rectangle13h(10, 70, 270, 90, 255);
  switch (prowintion[level - 1]) {
  case 5:
    CenterText13h(10, 70, 270, 90, "Atakujemy Wislan", 1);
    CenterText13h(11, 70, 271, 90, "Atakujemy Wislan", 209);
    break; // szary
  case 3:
    CenterText13h(10, 70, 270, 90, "Atakujemy Wieletow", 1);
    CenterText13h(11, 70, 271, 90, "Atakujemy Wieletow", 209);
    break; // zolty
  case 4:
    CenterText13h(10, 70, 270, 90, "Atakujemy Mazurow", 1);
    CenterText13h(11, 70, 271, 90, "Atakujemy Mazurow", 209);
    break; // niebieski
  case 2:
    CenterText13h(10, 70, 270, 90, "Atakujemy Pomorzan", 1);
    CenterText13h(11, 70, 271, 90, "Atakujemy Pomorzan", 209);
    break; // zielony
  }        // zielony
  delay(2000);
  do {
  } while (!mouse.GetMsg13h());
  EndMap = 0;
}
