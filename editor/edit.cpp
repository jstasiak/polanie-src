#include "image13h.h"
#include "mouse.h"
#include "mover.h"
#include <bios.h>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//============================================
struct EditStr {
  int mode; // 0-tereny(skaly,woda,droga,sucha ziemia,palisada),
            // 1-gadgety,2-drzewa, 3-postacie, 4-budynki, 5-rodzaje decyzji i
            // konca
  int panel;
  int obj;
  int color;        // 0-niebieski 1-szary 2-zolty 3-zielony
  int xp, yp;       // pastwisko wrogow
  int xz, yz;       // zbiorka wrogow
  int xo, yo;       // postac do odnalezienia
  int xk, yk;       // krag
  int xprz, yprz;   // miejsce przemiany
  int warunekKonca; // 0-zabij wszystkich 1-uratuj 2-zbuduj budynki 4-zabij
                    // jednego
  int typPlanszy;   // 0-wioska 1-bitwa dyn 2-stat 3-nic
  int postac;
  int generator;
  int maxmilk;
  int milk;
  int X, Y;
  char pName[20];
  char name[40];
} E = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "", ""};

// 9  10 11 12 13 14 15 16 17 18 19 20 21
int kierh[13] = {1, 0, 0, 1, 1, 0, 0,
                 0, 0, 0, 0, 1, 1}; // nachylenie zbocza poziomego
int kierv[13] = {1, 1, 1, 0, 0, 0, 0,
                 0, 0, 1, 0, 0, 1}; // nachylenie zbocza pionowego

int kolor[5];
#define LAST_INDEX 333
int Tree = 113;
int gr[5][26] = {

    {332, 82, 40, 22, 266, 17, 16, 13, 11, 15, 10, 14, 9, 12,
     20,  21, 19, 18, 17,  25, 26, 27, 1,  1,  1,  1}, // skaly,woda,droga,sucha
                                                       // ziemia, palisada
    {28, 29, 30, 55, 56, 57,  58,  59,  60,  61,  62,  63,  64,
     65, 66, 67, 68, 69, 256, 301, 300, 328, 330, 333, 329, 1}, // gadgety
    {400, 113, 114, 115, 116, 117, 118, 119, 54, 70, 71, 72, 73,
     46,  47,  48,  49,  50,  51,  52,  53,  1,  1,  1,  1,  1}, // drzewa,mosty
    {302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315,
     316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327}, // postacie
    {257, 137, 157, 177, 197, 217, 237, 147, 167, 187, 207, 227,
     247, 1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1} // budynki

};
//========Zmienne=================================

int place[mMaxX][mMaxY];
char placeN[mMaxX][mMaxY];

int MaxX = 66;
int MaxY = 66;

int PlaceUsed = 0;
int drzewa;
int color1 = Red, color2 = Blue;
char xleczenie, yleczenie;

char chat;
char skroller = 0, skroller1 = 0;
int speed = 0;
int ile0 = 0, ile1 = 0;
char Mname[26][16] = {
    "graf\\graf.011", "graf\\graf.012", "graf\\graf.013", "graf\\graf.014",
    "graf\\graf.015", "graf\\graf.011", "graf\\graf.012", "graf\\graf.013",
    "graf\\graf.014", "graf\\graf.015", "graf\\graf.013", "graf\\graf.014",
    "graf\\graf.011", "graf\\graf.012", "graf\\graf.015", "graf\\graf.016",
    "graf\\graf.016", "graf\\graf.011", "graf\\graf.014", "graf\\graf.017",
    "graf\\graf.017", "graf\\graf.017", "graf\\graf.017", "graf\\graf.018",
    "graf\\graf.018", "graf\\graf.012"};
int rx1, rx2, ry1, ry2;
int mouseDisabled = 0, mouseCommand = 1;
int quitLevel = 0;
char MapY = 1;
int Map = 0;

struct Plansza {
  char decisionType; // 0-miasto 1-bitwa dyn 2-stat 3,4-nic
  char gen;          // 0-nic 1-generator
  char endType;      // 0-zabij 1-uratuj
  char typ;   // typ uratowanego /nr domu do zostawienia 20-nic nie zostawiaj
  int xw, yw; // xwyjscia,ywwyjscia -do uratowania
  int x0, y0; // miejsce faceta do uwolnienia;
  int xp, yp, tp; // miejsce przemiany,typ przemiany;
  int gTimer;
  int maxmilk;
  char name[20];
  char ide;
  char next;
} pl;
int ScreenX = 3, ScreenY = 1;

//=========== Zmienne extern ===========

extern int level;
extern char *tree[14];
extern char *picture[MaxPictures], *missiles[4][3][3], *tlo, *Mysz[13];
extern char *movers[5][13][3][3], *ramka[4]; // faza:typ:dx:dy
extern char *buttons[16];
extern char *Hit[2], *dead[3];
extern char *drewno[3];
extern char *fire[14]; // world
extern char *face[16]; // 0-9 twarze, 10-15 budynki
extern int placeG[mMaxX][mMaxY];
//////////////
extern int grassCounter, waterCounter, fireCounter;

/////////////
//===========Deklaracje funkcji=======================

void DispatchEvent(void);
void ShowSelected(void);
void InitBattle(int, int);
void SubMenu(void);

int SaveLevel(void);
void Scroll(void);
void ShowPanel();

void Rocks(int, int, int, int);
int PanelCommand();
void ShowPicture(int x, int y, int i);
//===========Deklaracje funkcji zewnetrznych===========
// world
extern int InitPlace(int level);
extern void ShowPlace(int, int);
extern void Droga(int, int, int);
extern void Woda(int, int, int);
extern void Skala(int, int, int);
extern void ShowTrees(int, int);
extern void ShowBattleMap(int, int);

// s graphics
extern void InitPicture(void);
extern void FreeMemory(void);
extern void InitGraphics(void);
extern void ShowBackground(void);
extern void ShowSubMenu(void);
extern void ShowEndMenu(void);
extern void ShowPicture(int, int);
extern void ShowText(int, int);
// main
extern void PressButton(int B, int P);
extern void MouseEngine(void);
///////////////////////////////////////////////
//      Battle
///////////////////////////////////////////////
void Edit(int type) {
  int i, j;
  kolor[0] = Blue;
  kolor[1] = Gray;
  kolor[2] = DarkYellow;
  kolor[3] = DarkGreen;
  DownPalette(2);
  InitBattle(level, 0);
  SetScreen(1);

  Bar13h(1, 1, 319, 199, Black);
  ShowPanel();
  quitLevel = 0; //   screen=1
  LoadExtendedPalette(3);
  RisePalette(0);
  /////
  waterCounter = 0;
  fireCounter = 0;

  do {
    fireCounter++;
    if (!(fireCounter & 3))
      waterCounter++;
    if (waterCounter > 2)
      waterCounter = 0;

    if (Map && MapY < MaxY - 2)
      MapY++;
    if (Map && MapY < MaxY - 2)
      MapY++;
    if (!Map && MapY)
      MapY--;
    if (!Map && MapY)
      MapY--;
    Scroll();
    ShowSelected();

    i = mouse.GetMsg();
    ile0 = mouse.Ile(0);
    ile1 = mouse.Ile(1);
    if (i) {
      DispatchEvent();
    }

  } while (!quitLevel);

  SetScreen(0);
}
//============koniec procedury edit===================

void ShowSelected() {
  int i, j;

  //---------------------
  SetClippingArea13h(11, 8, 267, 192);
  ShowPlace(ScreenX, ScreenY);
  ShowTrees(ScreenX, ScreenY);
  SetClippingArea13h(0, 0, 319, 199);
  ///////////////// ramka ////////////////////////////////
  PutImage13h(0, 0, ramka[0], 0);
  PutImage13h(0, 0, ramka[1], 0);
  PutImage13h(267, 0, ramka[2], 0);
  PutImage13h(0, 190, ramka[3], 0);
  ShowPanel();

  ////////////////mapa//////////////////////////////////
  if (mouseCommand < 2) {
    ShowBattleMap(ScreenX, ScreenY);
  }
  /////////////// pokazanie myszki  ////////////////////////////
  int M;
  int x, y;

  x = (int)((mouse.X - 11) >> 4) + ScreenX;
  y = (int)(mouse.Y - 8) / 14 + ScreenY;
  M = 1;
  // jezeli nikt nie zaznaczony

  int colorM = White; // LightGreen;
  int a = 8, b = 7;
  GetImage13h(mouse.X - 8, mouse.Y - 7, mouse.X + 8, mouse.Y + 7, Mysz[0]);
  PutImageChange13h(mouse.X - 8, mouse.Y - 7, buttons[5], 1, Color1, colorM);

  if (mouse.X > 11 && mouse.X < 265 && mouse.Y > 8 && mouse.Y < 191) {
    int x = ((mouse.X - 11) / 16);
    int y = ((mouse.Y - 8) / 14);
    if (E.mode != 4) {
      if (gr[E.mode][E.obj] == 82 && mouse.X < 248)
        Rectangle13h(11 + x * 16, 8 + y * 14, 11 + 31 + x * 16, 8 + 25 + y * 14,
                     Blue);
      else
        Rectangle13h(11 + x * 16, 8 + y * 14, 11 + 15 + x * 16, 8 + 13 + y * 14,
                     LightGreen);
    } else {
      if (mouse.X > 16 + 11 && mouse.X < 248 && mouse.Y > 14 + 8)
        Rectangle13h(11 - 16 + x * 16, 8 - 14 + y * 14, 11 + 31 + x * 16,
                     8 + 25 + y * 14, White);
    }
  }

  ShowVirtualScreen();
  PutImage13h(mouse.X - a, mouse.Y - b, Mysz[0], 0);
}
//////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void Scroll() {
  if (skroller1) {
    skroller1--;
    return;
  }

  if (mouse.X < 10) {
    ScreenX--;
    if (ScreenX < 1) {
      ScreenX = 1;
      return;
    }
    skroller1 = skroller;
  }
  if (mouse.X > 310) {
    ScreenX++;
    if (ScreenX > MaxX - 17) {
      ScreenX = MaxX - 17;
      return;
    }
    skroller1 = skroller;
  }
  if (mouse.Y < 8) {
    ScreenY--;
    if (ScreenY < 1) {
      ScreenY = 1;
      return;
    }
    skroller1 = skroller;
  }
  if (mouse.Y > 192 - 7) {
    ScreenY++;
    if (ScreenY > MaxY - 14) {
      ScreenY = MaxY - 14;
      return;
    }
    skroller1 = skroller;
    return;
  }
}
///////////////////////////////////////////////////////////////////////////
// Rozpatrzenie zadania
///////////////////////////////////////////////////////////////////////////
void DispatchEvent() {
  int x, y, i;
  if (ile0)
    mouse.Button = 1;
  if (ile1)
    mouse.Button = 2;
  if (mouse.Key == 13) {
    SetScreen(0);
    OutText13h(60, 26, "Wpisz imie postaci:", 255);
    Write13h(60, 40, 140, 19, E.pName, 255, 0);
    SetScreen(1);
    return;
  }
  if (mouse.Key == 'c') //
  {
    E.color++;
    if (E.color > 3)
      E.color = 0;
    return;
  }
  if (mouse.Key == '-') //-
  {
    if (skroller < 5)
      skroller++;
    return;
  }
  if (mouse.Key == '=') //=
  {
    if (skroller)
      skroller--;
    return;
  }

  // Skrolling
  if (mouse.MWindow(0, 0, 10, 200) || mouse.Key == 75) {
    ScreenX--;
    if (mouse.Key)
      ScreenX--;
    if (ScreenX < 1)
      ScreenX = 1;
    return;
  }
  if (mouse.MWindow(316, 0, 320, 200) || mouse.Key == 77) {
    ScreenX++;
    if (mouse.Key)
      ScreenX++;
    if (ScreenX > MaxX - 17)
      ScreenX = MaxX - 17;
    return;
  }
  if (mouse.MWindow(77, 0, 320, 8) || mouse.Key == 72) {
    ScreenY--;
    if (mouse.Key)
      ScreenY--;
    if (ScreenY < 1)
      ScreenY = 1;
    return;
  }
  if (mouse.MWindow(0, 192, 320, 200) || mouse.Key == 80) {
    ScreenY++;
    if (mouse.Key)
      ScreenY++;
    if (ScreenY > MaxY - 14)
      ScreenY = MaxY - 14;
    return;
  }

  if (mouse.MWindow(11, 8, 11 + MaxX, 8 + MapY)) {
    ScreenX = mouse.X - 12 - 9;
    ScreenY = mouse.Y - 8 - 7;
    if (ScreenX > MaxX - 16)
      ScreenX = MaxX - 16;
    if (ScreenX < 1)
      ScreenX = 1;
    if (ScreenY > MaxY - 14)
      ScreenY = MaxY - 14;
    if (ScreenY < 1)
      ScreenY = 1;
    return;
  }
  if (mouse.MWindow(268, 0, 315, 195) &&
      ile0) // -------------- SubMenu ----------------
  {
    PanelCommand();
    return;
  }
  if (mouse.MWindow(11, 8, 268, 191)) {
    int x = ((mouse.X - 11) / 16) + ScreenX;
    int y = ((mouse.Y - 8) / 14) + ScreenY;

    if (mouse.Button == 1) {
      placeG[x][y] = gr[E.mode][E.obj];
      if (gr[E.mode][E.obj] == 400) {
        placeG[x][y] = Tree;
        Tree++;
        if (Tree > 118)
          Tree = 113;
      }
      if (gr[E.mode][E.obj] == 82) {
        placeG[x + 1][y] = 82;
        placeG[x][y + 1] = 82;
        placeG[x + 1][y + 1] = 82;
        if (x == 1) {
          placeG[x - 1][y] = 82;
          placeG[x - 1][y + 1] = 82;
        }
        if (y == 1) {
          placeG[x][y - 1] = 82;
          placeG[x + 1][y - 1] = 82;
        }
      }
      if (E.mode == 1) {
        switch (gr[E.mode][E.obj]) {
        case 301:
          placeG[E.xk][E.yk] = 8;
          E.xk = x;
          E.yk = y;
          placeG[x][y] = 301;
          break;  // krag
        case 329: // postac
          placeG[E.xo][E.yo] = 8;
          E.xo = x;
          E.yo = y;
          break;
        case 333:
          placeG[E.xz][E.yz] = 8;
          E.xz = x;
          E.yz = y; // miejsce zbiorki wrogow
          break;
        case 330:
          placeG[E.xp][E.yp] = 8;
          E.xp = x;
          E.yp = y; // pastwisko wrogow
          break;
        case 300:
          placeG[E.xprz][E.yprz] = 8;
          E.xprz = x;
          E.yprz = y; // przemiana
          break;
        }
        placeG[x][y] = gr[E.mode][E.obj];
      }
      if (E.mode == 0 && (gr[E.mode][E.obj] == 16 || gr[E.mode][E.obj] == 13)) {
        if (placeG[x + 1][y] == 332) {
          Rocks(x + 1, y, x, y);
        } else {
          if (placeG[x - 1][y] == 332)
            Rocks(x - 1, y, x, y);
        }
      }

      if (E.mode == 4) {
        for (int j = 0; j < 3; j++)
          for (int i = 0; i < 3; i++)
            placeG[x + i - 1][y + j - 1] = gr[E.mode][E.obj] + i + j * 3;
      }

    } else {
      placeG[x][y] = 8;
      if (E.mode == 4) {
        for (int j = 0; j < 3; j++)
          for (int i = 0; i < 3; i++)
            placeG[x + i - 1][y + j - 1] = 8;
      }
      if (!E.mode && E.obj == 1) {
        for (int j = 0; j < 2; j++)
          for (int i = 0; i < 2; i++)
            placeG[x + i][y + j] = 8;
      }
    }

    for (int i = 1; i < MaxX - 1; i++)
      for (int j = 1; j < MaxY - 1; j++) {
        if (placeG[i][j] > 30 && placeG[i][j] < 46)
          Droga(i, j, 35); // droga
        if (placeG[i][j] > 265 && placeG[i][j] < 277)
          Droga(i, j, 266); // palisada
        if (placeG[i][j] > 73 && placeG[i][j] < 87)
          Woda(i, j, 74);
        if (placeG[i][j] == 22)
          placeG[i][j] = 22 + (i & 1) + (j & 1); // sucha ziemia
      }
  }
}
//=================================================
void InitBattle(int level,
                int type) // type 0 zaladuj scenariusz 1-scenariusz zaladowany
{
  int k = 0, i, j, p0 = 1, p1 = 1;
  FILE *plikPlansz;
  char name[20], z;
  // baronie: 1-Red(YOU) 2-Green 3-Blue 4-Yellow 5-Gray
  int chatki = 1, chaTki = 1;
  E.warunekKonca = 0;
  E.typPlanszy = 0;
  E.xo = 0;
  E.yo = 0;
  E.xk = 0;
  E.yk = 0;
  E.xp = 0;
  E.yp = 0;
  E.xz = 0;
  E.yz = 0;

  if (level > 25) {
    sprintf(name, "levels\\level.%d", level);
    plikPlansz = fopen(name, "rb");
    if (plikPlansz == NULL) {
      for (i = 0; i < mMaxX; i++)
        for (j = 0; j < mMaxY; j++) {
          placeG[i][j] = 8;
          place[i][j] = 0;
          placeN[i][j] = 1;
        }

      return;
    } else {
      for (j = 0; j < mMaxY; j++)
        for (i = 0; i < mMaxX; i++) {
          fread(&placeG[i][j], 4, 1, plikPlansz);
          place[i][j] = 0;
          placeN[i][j] = 1;
          switch (placeG[i][j]) {
          case 301:
            E.xk = i;
            E.yk = j;
            break;  // krag
          case 329: // postac do odnalezienia
            E.xo = i;
            E.yo = j;
            break;
          case 333:
            E.xz = i;
            E.yz = j; // miejsce zbiorki wrogow
            break;
          case 330:
            E.xp = i;
            E.yp = j; // pastwisko wrogow
            break;
          case 300:
            E.xprz = i;
            E.yprz = j; // przemiana
            break;
          }
        }
      fread((void *)&E, sizeof(EditStr), 1, plikPlansz);
      ScreenX = E.X;
      ScreenY = E.Y;
      // E.X=ScreenX;
      // E.Y=ScreenY;

      fclose(plikPlansz);
      return;
    }
  }

  /*
      case(1):
      case(4):color2=DarkBlue;break;
      case(3):color2=DarkYellow;break;
      case(2):color2=DarkGreen;break; //
      case(5):color2=LightGray;break;
      */
  if (!type) {
    drzewa = 1;
    pl.next = 0;
    pl.tp = 0;
    pl.x0 = 0;
    pl.gen = 0;
    for (i = 0; i < MaxX; i++)
      for (j = 0; j < MaxY; j++)
        placeN[i][j] = 0;
    for (j = 0; j < MaxY; j++)
      for (i = 0; i < MaxX; i++)
        place[i][j] = 0;
    strcpy(name, "graf\\level.dat");
    ScreenX = 10;
    ScreenY = 10;
    plikPlansz = fopen(name, "r");
    if (plikPlansz == NULL) {
      Close13h();
      exit(0);
    }
    {
      do {
        z = getc(plikPlansz);
        if (z == '$')
          k++;
        if (z == '@') {
          Close13h();
          exit(0);
        }
      } while (k != level);
      for (j = 0; j < MaxY; j++) {
        do {
          z = getc(plikPlansz);
          if (z == '@') {
            Close13h();
            exit(0);
          }
          if (z == 'D') {
            z = getc(plikPlansz);
            pl.decisionType = (char)(z - 48);
          }
          if (z == 'E') {
            z = getc(plikPlansz);
            pl.endType = (char)(z - 48);
          }
          if (z == 'G')
            pl.gen = 1;
          if (z == 'P') {
            z = getc(plikPlansz);
            pl.tp = (char)(z - 48);
          }
          if (z == 'T') {
            z = getc(plikPlansz);
            i = (char)(z - 48);
            z = getc(plikPlansz);
            pl.typ = (char)(z - 48) + i * 10;
          }
          if (z == 'M') {
            z = getc(plikPlansz);
            pl.maxmilk = (int)(z - 48) * 200 + 50;
            if (pl.maxmilk == 50)
              pl.maxmilk = 0;
          }
          if (z == 'N') {
            z = getc(plikPlansz);
            i = (char)(z - 48);
            z = getc(plikPlansz);
            pl.next = (char)(z - 48) + i * 10;
          }
          if (z == '*') {
            char cc = 0;
            do {
              z = getc(plikPlansz);
              pl.name[cc] = z;
              cc++;
            } while (pl.name[cc - 1] != '*');
            pl.name[cc - 1] = 0;
          } //   'imie'

        } while (z != '!');
        if (pl.endType == 4 && !p1)
          p1 = 1;
        for (i = 0; i < MaxX; i++) {
          z = getc(plikPlansz);
          if (z == '@') {
            Close13h();
            exit(0);
          }
          place[i][j] = 0;
          if (i == 0 || i == MaxX - 1 || j == 0 || j == MaxY - 1)
            place[i][j] = 10;
          i = 8;
          if (z == '*') {
            if (pl.endType == 1) {
              place[i][j] = 10;
              pl.x0 = i;
              pl.y0 = j;
            }
            if (pl.endType == 4) {
              if (!p1)
                p1++;
            }
          }
          if (z == '^') {
            pl.xw = i;
            pl.yw = j;
            i = 301;
          }
          if (z == '.')
            i = 256;
          if (z == '+') {
            placeN[i][j] = 1;
            placeN[i + 1][j] = 1;
            placeN[i + 1][j + 1] = 1;
            placeN[i][j + 1] = 1;
          }
          if (z == 'a')
            i = 54; // pniak
          if (z == 'b')
            i = 55;
          if (z == 'c')
            i = 56;
          if (z == 'd')
            i = 57;
          if (z == 'e') {
            i = 58;
            place[i][j] = 10;
          }
          if (z == 'f') {
            i = 59;
            place[i][j] = 0;
          }
          if (z == 'g') {
            i = 60;
            place[i][j] = 10;
          }
          if (z == 'h') {
            i = 61;
            place[i][j] = 10;
          }
          if (z == 'i') {
            i = 62;
            place[i][j] = 10;
          }
          if (z == 'j') {
            i = 63;
            place[i][j] = 10;
          }
          if (z == 'k') {
            i = 64;
            place[i][j] = 10;
          }
          if (z == 'l') {
            i = 65;
            place[i][j] = 10;
          }
          if (z == 'm') {
            i = 66;
            place[i][j] = 10;
          }
          if (z == 'n')
            i = 67;
          if (z == 'o') {
            i = 68;
            place[i][j] = 10;
          }
          if (z == 'p') {
            i = 69;
            place[i][j] = 10;
          }
          // skaly
          if (z == 'Ó') {
            i = 9;
            place[i][j] = 10;
          }
          if (z == 'É') {
            i = 10;
            place[i][j] = 10;
          }
          if (z == 'º') {
            i = 11;
            place[i][j] = 10;
          }
          if (z == 'Ù') {
            i = 12;
            place[i][j] = 10;
          }
          if (z == 'Ä') {
            i = 13;
            place[i][j] = 10;
          }
          if (z == '¸') {
            i = 14;
            place[i][j] = 10;
          }
          if (z == '³') {
            i = 15;
            place[i][j] = 10;
          }
          if (z == 'Í') {
            i = 16;
            place[i][j] = 10;
          }
          if (z == 's') {
            i = 17;
            place[i][j] = 0;
          } // wejscie
          if (z == '¼') {
            i = 18;
            place[i][j] = 10;
          }
          if (z == 'Ô') {
            i = 19;
            place[i][j] = 10;
          }
          if (z == 'Ú') {
            i = 20;
            place[i][j] = 10;
          }
          if (z == '·') {
            i = 21;
            place[i][j] = 10;
          }

          if (z == 'u') {
            i = 256;
            xleczenie = i;
            yleczenie = j;
          }
          if (z == '/') {
            i = 300;
            pl.xp = i;
            pl.yp = j;
          }
          if (z == 'q')
            i = 22;
          if (z == 't')
            i = 35;
          if (z == 'w') {
            i = 74;
            place[i][j] = 1;
          }
          if (z == 'r') {
            i = 266;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          } //??
          //--------most-------------------
          if (z == '1')
            i = 46;
          if (z == '2')
            i = 47;
          if (z == '3')
            i = 48;
          if (z == '4')
            i = 49;
          if (z == '5')
            i = 50;
          if (z == '6')
            i = 51;
          if (z == '7')
            i = 52;
          if (z == '8')
            i = 53;
          // ---------drzewa ---------------
          if (z == 'A' && drzewa < 2000) {
            i = 113;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'B' && drzewa < 2000) {
            i = 114;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'C' && drzewa < 2000) {
            i = 115;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'D' && drzewa < 2000) {
            i = 116;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'E' && drzewa < 2000) {
            i = 117;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'F' && drzewa < 2000) {
            i = 118;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          if (z == 'G' && drzewa < 2000) {
            i = 119;
            place[i][j] = 256 + 512 + drzewa;
            drzewa++;
          }
          //---- PASTWISKO I MIEJSCE ZBIORKI
          if (z == '?') {
          }
          if (z == '-') {
          }
          //---start
          if (z == '=') {
            ScreenX = i;
            ScreenY = j;
          }
          //----- zamki -----------
          if (z == 'H') {
          }

          if (z == 'I') {
          }

          if (z == 'J') // koszary
          {
          }

          if (z == 'K') // swiatynia
          {
          }

          if (z == 'L') // KOSZAARY 2
          {
          }

          if (z == 'M') // DOM BOH
          {
          }

          // KROWA NASZA
          if (z == 'x' && p0 < 39) {
          }
          if (z == 'y' && p0 < 39) {
            p0++;
          }
          if (z == 'z' && p0 < 39) {
            p0++;
          }
          if (z == '9' && p0 < 39) {
            p0++;
          }

          if (z == '0' && p0 < 39) {
            p0++;
          }
          if (z == ':' && p0 < 39) {
            p0++;
          }
          if (z == ';' && p0 < 39) {
          }
          if (z == '<' && p0 < 39) {
          }
          if (z == '>' && p0 < 39) {
          }
          if (z == ',' && p0 < 39) {
          }
          //-----------------------------------------------------------------
          if (z == 'N') {
          }
          if (z == 'O') {
          }

          if (z == 'P') {
          }
          if (z == 'Q') // swiatynia
          {
          }
          if (z == 'R') // koszary2
          {
          }
          if (z == 'S') // akademia
          {
          }
          //-----------------------------------------------------
          if (z == 'T' && p1 < 39) {
          }
          if (z == 'U' && p1 < 39) {
          }

          if (z == 'W' && p1 < 39) {
          }
          if (z == 'X' && p1 < 39) {
          }
          if (z == 'Y' && p1 < 39) {
          }
          if (z == 'Z' && p1 < 39) {
          }
          if (z == '#' && p1 < 39) {
          }
          if (z == '"' && p1 < 39) {
          }

          if (z == '%' && p1 < 39) {
          }
          if (z == '&' && p1 < 39) {
          }
        }
      }
    }
    fclose(plikPlansz);
  }
  if (ScreenX < 1)
    ScreenX = 1;
  if (ScreenY < 1)
    ScreenY = 1;
  if (ScreenX > MaxX - 16)
    ScreenX = MaxX - 16;
  if (ScreenY > MaxY - 14)
    ScreenY = MaxY - 14;
  for (i = 1; i < MaxX - 1; i++)
    for (j = 1; j < MaxY - 1; j++) {
      if (placeG[i][j] > 30 && placeG[i][j] < 46)
        Droga(i, j, 35); // droga
      if (placeG[i][j] > 265 && placeG[i][j] < 277)
        Droga(i, j, 266); // palisada
      if (placeG[i][j] > 73 && placeG[i][j] < 87)
        Woda(i, j, 74);
      if (placeG[i][j] == 22)
        placeG[i][j] = 22 + (i & 1) + (j & 1); // sucha ziemia
    }
  placeG[xleczenie][yleczenie] = 256;
}

//////////////////////////////////////////////////////////////////
//  SubMenu
//////////////////////////////////////////////////////////////////
void SubMenu(void) {
  int quitMenu = 0;

  mouseCommand = 1;
  SetScreen(0);
  ShowSubMenu();

  do {
    MouseEngine();
    if (mouse.MWindow(100, 30, 220, 50) || mouse.Key == 11386) // Save Level
    {
      PressButton(1, 1);
      delay(300);
      PressButton(1, 0);
      SaveLevel();
      quitMenu = 1;
    }
    if (mouse.MWindow(100, 57, 220, 74) || mouse.Key == 4471) // Load Level
    {
      PressButton(2, 1);
      delay(300);
      PressButton(2, 0);
      InitBattle(level, 0);
      quitMenu = 1;
    }
    if (mouse.MWindow(100, 87, 220, 104) || mouse.Key == 6512) // Clear Level
    {
      quitMenu = 1;
    }
    if (mouse.MWindow(100, 114, 220, 131) || mouse.Key == 8807) // Cancel
    {
      PressButton(4, 0);
      delay(300);
      PressButton(4, 1);
      quitMenu = 1;
    }
    if (mouse.MWindow(100, 142, 220, 159) ||
        mouse.Key == 9579) // End Game /koniec
    { // quit=4209  k=9579  // c=11875   g=8807 // r=4978   p=6512  // l=9836
      // w=4471// s=8051  z=11386
      PressButton(5, 0);
      delay(300);
      PressButton(5, 1);
      quitMenu = 1;
      quitLevel = 1;
    }

  } while (!quitMenu);
  mouse.ButtonUp();
  SetScreen(1);
}
////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//  Options
//////////////////////////////////////////////////////////////////
void Options(void) {}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//   SubMenuSection
///////////////////////////////////////////////////////////////////////
int sSubMenu() { return 0; }

////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
int SaveLevel() {
  FILE *file;
  int i, j;
  int Ty[4] = {32, 59, 89, 116};
  char name[30];
  E.X = ScreenX;
  E.Y = ScreenY;
  sprintf(name, "levels\\level.%d", level);
  file = fopen(name, "wb");
  if (file != NULL) {
    for (j = 0; j < mMaxY; j++)
      for (i = 0; i < mMaxX; i++) {
        fwrite(&placeG[i][j], 4, 1, file);
      }
    fwrite((void *)&E, sizeof(EditStr), 1, file);
    fclose(file);
  }

  return 0;
}
////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////

void ShowPanel() {
  int X = 0;
  int Y = 0;
  int Xe[2] = {280, 300};
  int Ye[11];
  for (int j = 0; j < 11; j++)
    Ye[j] = 8 + (j * 16);

  for (j = 0; j < 3; j++)
    for (int i = 0; i < 2; i++) {
      if (E.mode == i + j * 2)
        Rectangle13h(Xe[i], Ye[j], Xe[i] + 15, Ye[j] + 13, LightGreen);
      else
        Rectangle13h(Xe[i], Ye[j], Xe[i] + 15, Ye[j] + 13, Green);
    }
  Bar13h(277, 0, 319, 8, kolor[E.color]);
  if (E.mode < 5) {
    for (j = 0; j < 5; j++)
      for (int i = 0; i < 2; i++) {
        Bar13h(Xe[i], Ye[j + 3], Xe[i] + 16, Ye[j + 3] + 14, 0);

        if (E.panel + ((j * 2) + i) < 26) {
          ShowPicture(Xe[i], Ye[j + 3], gr[E.mode][E.panel + (j * 2) + i]);
        }
      }
  }
  if (E.mode == 5) {
    if (!E.panel) {
      Rectangle13h(Xe[0], Ye[4], Xe[1] + 16, Ye[4] + 14, LightGray);
      Rectangle13h(Xe[0], Ye[6], Xe[1] + 16, Ye[6] + 14, LightGray);
      if (E.warunekKonca == 1 || E.warunekKonca == 4) {
        Rectangle13h(Xe[0], Ye[7], Xe[1] + 16, Ye[7] + 14, LightGray);
        PutImageChange13h(Xe[0] + 10, Ye[7], movers[0][E.postac][1][2], 1, Red,
                          Blue);
      }
      if (E.warunekKonca == 2) {
        Rectangle13h(Xe[0], Ye[7], Xe[1] + 16, Ye[7] + 14, LightGray);
        char ss[10];
        sprintf(ss, "%d chat", E.postac);
        OutText13h(Xe[0], Ye[7], ss, White);
      }
      OutText13h(Xe[0], Ye[3], "Typ", LightGray);
      OutText13h(Xe[0], Ye[5], "End", LightGray);
      switch (E.warunekKonca) {
      case 0:
        OutText13h(Xe[0], Ye[6], "zniszcz", White);
        break; // zabij wszystkich
      case 1:
        OutText13h(Xe[0], Ye[6], "uratuj", White);
        break; // uratuj
      case 2:
        OutText13h(Xe[0], Ye[6], "zbuduj", White);
        break; // zbuduj kilka budynkow
      case 4:
        OutText13h(Xe[0], Ye[6], "zabij", White);
        break; // zabij jednego
      }
      switch (E.typPlanszy) {
      case 0:
        OutText13h(Xe[0], Ye[4], "wioska", White);
        break; // wioska
      case 1:
        OutText13h(Xe[0], Ye[4], "atak", White);
        break; // atak
      case 2:
        OutText13h(Xe[0], Ye[4], "stoja", White);
        break; // stanie
      case 3:
        OutText13h(Xe[0], Ye[4], "nic", White);
        break; // nic nie robia
      }
    } else {
      Rectangle13h(Xe[0], Ye[4], Xe[1] + 16, Ye[4] + 14, LightGray);
      Rectangle13h(Xe[0], Ye[6], Xe[1] + 16, Ye[6] + 14, LightGray);
      if (E.warunekKonca) {
        Rectangle13h(Xe[0], Ye[7], Xe[1] + 16, Ye[7] + 14, LightGray);
        PutImageChange13h(Xe[0] + 10, Ye[7], movers[0][E.postac][1][2], 1, Red,
                          Blue);
      }
      OutText13h(Xe[0], Ye[3], "Mleko", LightGray);
      OutText13h(Xe[0], Ye[5], "Maxmleko", LightGray);

      char ss[10];
      sprintf(ss, "%d", E.milk);
      OutText13h(Xe[0], Ye[4], ss, White); // ilosc mleka
      sprintf(ss, "%d", E.maxmilk);
      OutText13h(Xe[0], Ye[6], ss, White); // ilosc mleka
      switch (E.generator) {
      case 1:
        OutText13h(Xe[0], Ye[7], "Gen", White);
        break; // generator
      case 0:
        OutText13h(Xe[0], Ye[7], "Nie Gen", White);
        break; // nie
      }
    }
  }
  if (E.mode <= 5) {
    Rectangle13h(Xe[0], Ye[8], Xe[0] + 16, Ye[8] + 14, 255);
    Rectangle13h(Xe[1], Ye[8], Xe[1] + 16, Ye[8] + 14, 255);
    OutText13h(Xe[0], Ye[8] + 2, " A    V", 255);
    if (E.mode < 5) {
      if (E.obj >= E.panel && E.obj < E.panel + 10) {
        Rectangle13h(Xe[(E.obj - E.panel) & 1], Ye[3 + (E.obj - E.panel) / 2],
                     Xe[(E.obj - E.panel) & 1] + 15,
                     Ye[3 + (E.obj - E.panel) / 2] + 13, 255);
      }
    }
  }
  Rectangle13h(Xe[0], Ye[9], Xe[1] + 16, Ye[9] + 14, 255);
  OutText13h(Xe[0], Ye[9] + 2, " Map", 255);
  Rectangle13h(Xe[0], Ye[10], Xe[1] + 16, Ye[10] + 14, 255);
  OutText13h(Xe[0], Ye[10] + 2, " Menu", 255);
}
//////////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////////
int PanelCommand() {
  int Xe[2] = {280, 300};
  int Ye[11];
  for (int j = 0; j < 11; j++)
    Ye[j] = 8 + (j * 16);

  if (mouse.MWindow(Xe[0], Ye[0], Xe[1] + 20, Ye[3])) // wybierz mod
  {
    int i = (mouse.X - 280) / 20;
    int j = (mouse.Y - 8) / 16;

    E.mode = i + j * 2;
    Bar13h(280, 0, 319, 199, Black);
    return 0;
  }
  if (mouse.MWindow(Xe[0], Ye[3], Xe[1] + 20, Ye[8]) &&
      E.mode < 5) // wybierz obj
  {
    int i = (mouse.X - 280) / 20;
    int j = (mouse.Y - 8) / 16;
    E.obj = E.panel + i + (j - 3) * 2;
    return 0;
  }
  if (mouse.MWindow(Xe[0], Ye[3], Xe[1] + 20, Ye[8]) && E.mode == 5) {
    if (!E.panel) {
      Bar13h(280, 0, 319, 199, Black);
      int j = (mouse.Y - Ye[4]) / 16;
      if (!j)
        E.typPlanszy++;
      if (j == 2)
        E.warunekKonca++;
      if (j == 3)
        E.postac++;
      if (E.typPlanszy > 3)
        E.typPlanszy = 0;
      if (E.warunekKonca == 3)
        E.warunekKonca = 4;
      if (E.warunekKonca > 4)
        E.warunekKonca = 0;
      if (E.postac > 12)
        E.postac = 0;
      return 0;
    } else {
      Bar13h(280, 0, 319, 199, Black);
      int j = (mouse.Y - Ye[4]) / 16;
      if (!j)
        E.milk += 200;
      if (E.milk > E.maxmilk)
        E.milk = 0;
      if (j == 2)
        E.maxmilk += 200;
      if (E.maxmilk && E.maxmilk < 250)
        E.maxmilk = 250;
      if (E.milk && E.milk < 250)
        E.milk = 250;
      if (E.maxmilk > 2000)
        E.maxmilk = 0;
      if (j == 3)
        E.generator++;
      if (E.generator > 1)
        E.generator = 0;
      return 0;
    }
  }
  if (mouse.MWindow(Xe[0], Ye[8], Xe[0] + 16, Ye[8] + 15)) // Up
  {
    if (E.panel > 1)
      E.panel -= 2;
    if (E.mode == 5) {
      E.panel = 0;
      Bar13h(280, 0, 319, 199, Black);
    }
    return 0;
  }
  if (mouse.MWindow(Xe[1], Ye[8], Xe[1] + 16, Ye[8] + 15)) // down
  {
    if (E.panel < 24)
      E.panel += 2;
    if (E.mode == 5) {
      E.panel = 1;
      Bar13h(280, 0, 319, 199, Black);
    }
    return 0;
  }

  if (mouse.MWindow(Xe[0], Ye[9], Xe[0] + 33, Ye[9] + 15)) // mapa
  {
    if (Map) {
      Map = 0;
    } else {
      Map = 1;
    }
    return 0;
  }
  if (mouse.MWindow(Xe[0], Ye[10], Xe[0] + 33, Ye[10] + 15)) // menu
  {
    SubMenu();
    if (quitLevel)
      return 0;
    DownPalette(1);
    LoadExtendedPalette(3); //<<<<<<<<<<<<<<<<<<< zmienic
    ShowSelected();
    ShowVirtualScreen();
    RisePalette(0);
    return 0;
  }
  return 0;
}
/////////////////////////////////////////////////////////////////////////////////////
//
//   ShowPicture
//
/////////////////////////////////////////////////////////////////////////////////////
void ShowPicture(int x, int y, int i) {
  if (i < 9)
    PutImage13h(x, y, picture[2], 0); // trawa
  if (i > 8 && i < 22)
    PutImage13h(x, y, picture[i], 0); // obrazki
  if (i > 24 && i < 46)
    PutImage13h(x, y, picture[i], 0); // obrazki
  if (i > 45 && i < 54) {
    PutImage13h(x, y, picture[82 + (waterCounter * 13)], 0); // woda
    PutImage13h(x, y, picture[i], 1);                        // most
  }
  if (i > 53 && i < 68)
    PutImage13h(x, y, picture[i], 0); // obrazki
  if (i > 265 && i < 278)
    PutImage13h(x, y, picture[i], 0); // palisada
  if (i == 68) {
    PutImage13h(x, y, picture[68], 0);
    PutImage13h(x, y, fire[8 + ((fireCounter)&3)], 1);
  } // male ognisko
  if (i == 69) {
    PutImage13h(x, y, picture[68], 0);
    PutImage13h(x, y, fire[4 + ((fireCounter)&3)], 1);
  } // duze ognisko
  if (i > 69 && i < 74)
    PutImage13h(x, y, picture[2], 0); // podstawa pod rzewa wypalone
  if (i > 73 && i < 87)
    PutImage13h(x, y, picture[i + (waterCounter * 13)], 0); // woda
  if (i > 112 && i < 120) // drzewo      praca!
  {
    PutImage13h(x, y, picture[2], 0); // trawa pod drzewem
  }
  if (i > 126 && i < 300)
    PutImage13h(x, y, picture[i], 0); // zamek i ruina nasz
  if (i > 126 && i < 300)
    PutImageChange13h(x, y, picture[i], 0, color1,
                      color2); // zamek i ruina ich
  // if(place[i][j]==2)PutImage13h(x,y,picture[156],1);// pale w wodzie pod
  // budoewe mostu
  ///////////////////////

  if (i == 256) {
    // swiete miejsce leczenie
    PutImage13h(x, y, picture[2], 1);
    PutImageChange13h(x, y, picture[282 + (waterCounter & 1)], 1, Red,
                      LightBlue);
  }
  if (i == 300) {
    // swiete miejsce przemiana
    PutImage13h(x, y, picture[2], 1);
    PutImageChange13h(x, y, picture[282 + (waterCounter & 1)], 1, Red,
                      LightRed);
  }
  if (i == 301) // magiczny krag
  {
    PutImageChange13h(x, y, picture[69], 1, Red, Yellow);
  }
  if (i == 328) // rozjasnienie poczatkowe
  {
    PutImage13h(x, y, picture[2], 1);
    OutText13h(x + 4, y, "S", Yellow);
  }
  if (i == 329) // postac do zabicia lub uratowania
  {
    PutImage13h(x, y, picture[2], 1);
    OutText13h(x + 4, y, "O", White);
  }
  if (i == 330) // pastwisko wrogow
  {
    PutImage13h(x, y, picture[2], 1);
    OutText13h(x + 4, y, "P", LightRed);
  }
  if (i == 331) // swiete miejsce wrogow
  {
    PutImage13h(x, y, picture[2], 1);
    PutImageChange13h(x, y, picture[282 + (waterCounter & 1)], 1, Red,
                      LightBlue);
    OutText13h(x + 4, y, "W", LightRed);
  }
  if (i == 333) // miejsce zbiorki wrogow
  {
    PutImage13h(x, y, picture[2], 1);
    OutText13h(x + 4, y, "Z", LightRed);
  }
  ///////////////////////
  if (i == 332) {
    Rectangle13h(x + 1, y + 1, x + 14, y + 12, White); // skaly niedorobione
  }
  if (i == 70)
    PutImage13h(x, y, tree[1], 1); // lezy drzewo brazowe
  if (i == 72)
    PutImage13h(x, y, tree[8], 1); // lezy drzewo spalone
  // postacie
  switch (i) {
  case 302: // nasza krowa
    PutImageChange13h(x, y, movers[0][0][0][0], 1, Red, Red);
    break;
  case 303: // nasz drwal
    PutImageChange13h(x, y, movers[0][1][0][0], 1, Red, Red);
    break;
  case 304: // nasz lucznik
    PutImageChange13h(x, y, movers[0][2][0][0], 1, Red, Red);
    break;
  case 305: // nasza kaplanka
    PutImageChange13h(x, y, movers[0][3][0][0], 1, Red, Red);
    break;
  case 306: // nasz kaplan
    PutImageChange13h(x, y, movers[0][4][0][0], 1, Red, Red);
    break;
  case 307: // nasz woj
    PutImageChange13h(x, y, movers[0][5][0][0], 1, Red, Red);
    break;
  case 308: // nasza wlocznik
    PutImageChange13h(x, y, movers[0][6][0][0], 1, Red, Red);
    break;
  case 309: // nasz rycerz
    PutImageChange13h(x, y, movers[0][7][0][0], 1, Red, Red);
    break;
  case 310: // nasza strzyga
    PutImageChange13h(x, y, movers[0][8][0][0], 1, Red, Red);
    break;
  case 311: // nasz niedzwiedz
    PutImageChange13h(x, y, movers[0][9][0][0], 1, Red, Red);
    break;
  case 312: // nasz pastuszek
    PutImageChange13h(x, y, movers[0][10][0][0], 1, Red, Red);
    break;
  case 313: // nasz mag
    PutImageChange13h(x, y, movers[0][11][0][0], 1, Red, Red);
    break;
  case 314: // nasz kusznik
    PutImageChange13h(x, y, movers[0][12][0][0], 1, Red, Red);
    break;

  case 315: // krowa
    PutImageChange13h(x, y, movers[0][0][0][0], 1, Red, Yellow);
    break;
  case 316: // drwal
    PutImageChange13h(x, y, movers[0][1][0][0], 1, Red, Yellow);
    break;
  case 317: // lucznik
    PutImageChange13h(x, y, movers[0][2][0][0], 1, Red, Yellow);
    break;
  case 318: // kaplanka
    PutImageChange13h(x, y, movers[0][3][0][0], 1, Red, Yellow);
    break;
  case 319: // kaplan
    PutImageChange13h(x, y, movers[0][4][0][0], 1, Red, Yellow);
    break;
  case 320: // woj
    PutImageChange13h(x, y, movers[0][5][0][0], 1, Red, Yellow);
    break;
  case 321: // wlocznik
    PutImageChange13h(x, y, movers[0][6][0][0], 1, Red, Yellow);
    break;
  case 322: // rycerz
    PutImageChange13h(x, y, movers[0][7][0][0], 1, Red, Yellow);
    break;
  case 323: // strzyga
    PutImageChange13h(x, y, movers[0][8][0][0], 1, Red, Yellow);
    break;
  case 324: // niedzwiedz
    PutImageChange13h(x, y, movers[0][9][0][0], 1, Red, Yellow);
    break;
  case 325: // pastuszek
    PutImageChange13h(x, y, movers[0][10][0][0], 1, Red, Yellow);
    break;
  case 326: // mag
    PutImageChange13h(x, y, movers[0][11][0][0], 1, Red, Yellow);
    break;
  case 327: // kusznik
    PutImageChange13h(x, y, movers[0][12][0][0], 1, Red, Yellow);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////
//
// Rocks()
//
////////////////////////////////////////////////////////////////////////////
// 9  10 11 12 13 14 15 16 17 18 19 20 21
// int kierh[13]={ 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};//nachylenie zbocza
// poziomego int kierv[13]={ 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1};//nachylenie
// zbocza pionowego
void Rocks(int x, int y, int ox, int oy) {
  // if(placeG[x][y]!=332) return;
  if (placeG[x][y] == 332) {
    placeG[x][y] = placeG[ox][oy];
  } else {
    return;
  }
  int k = 0;

  if (placeG[x][y - 1] == 332 || ((ox == x) && (oy == y - 1)))
    k += 1;
  if (placeG[x + 1][y] == 332 || ((ox == x + 1) && (oy == y)))
    k += 2;
  if (placeG[x][y + 1] == 332 || ((ox == x) && (oy == y + 1)))
    k += 4;
  if (placeG[x - 1][y] == 332 || ((ox == x - 1) && (oy == y)))
    k += 8;

  switch (k) {
  case 3:
    placeG[x][y] = 19;
    if (x != ox) {
      if (kierh[placeG[ox][oy] - 9])
        placeG[x][y] = 9;
    } else {
      if (kierv[placeG[ox][oy] - 9])
        placeG[x][y] = 9;
    }

    break;
  case 6:
    placeG[x][y] = 10;
    if (x != ox) {
      if (kierh[placeG[ox][oy] - 9])
        placeG[x][y] = 20;
    } else {
      if (!kierv[placeG[ox][oy] - 9])
        placeG[x][y] = 20;
    }

    break;
  case 1:
  case 4:
  case 5:
    placeG[x][y] = 15;
    if (kierv[placeG[ox][oy] - 9])
      placeG[x][y] = 11;
    break;
  case 2:
  case 8:
  case 10:
    placeG[x][y] = 16;
    if (kierh[placeG[ox][oy] - 9])
      placeG[x][y] = 13;
    break;
  case 9:
    placeG[x][y] = 18;
    if (x != ox) {
      if (kierh[placeG[ox][oy] - 9])
        placeG[x][y] = 12;
    } else {
      if (!kierv[placeG[ox][oy] - 9])
        placeG[x][y] = 12;
    }

    break;
  case 12:
    placeG[x][y] = 14;
    if (x != ox) {
      if (kierh[placeG[ox][oy] - 9])
        placeG[x][y] = 21;
    } else {
      if (kierv[placeG[ox][oy] - 9])
        placeG[x][y] = 21;
    }

    break;
  }

  if (placeG[x - 1][y] == 332)
    Rocks(x - 1, y, x, y);
  if (placeG[x][y - 1] == 332)
    Rocks(x, y - 1, x, y);
  if (placeG[x + 1][y] == 332)
    Rocks(x + 1, y, x, y);
  if (placeG[x][y + 1] == 332)
    Rocks(x, y + 1, x, y);
}
