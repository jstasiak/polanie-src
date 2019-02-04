//////////////////////////////////////////////////////////////////////
//   Modul zawiera funkcje sluzace do obslugi grafiki bitmapowej
//
/////////////////////////////////////////////////////////////////////
#include "image13h.h"
#include "mouse.h"
#include "mover.h"
#include "zabezset.h"
#include <STRING.H>
#include <conio.h>
#include <dos.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
//=======Zmienne ==============================
int ExpColor[30];
int Color1 = 38;
int LightRed;
int Red;
int DarkRed;

int LightYellow;
int Yellow;
int DarkYellow;

int LightGreen;
int Green;
int DarkGreen;

int LightBlue;
int Blue;
int DarkBlue;

int LightGray;
int Gray;
int DarkGray;

int LightBrown;
int Brown;
int DarkBrown;

void *memoryBuf;
extern char *fire[14]; // world
extern char *tree[14]; // world
extern char *picture[MaxPictures], *missiles[4][3][3], *tlo, *Mysz[13];
extern char *movers[5][13][3][3], *ramka[4]; // faza:typ:dx:dy
extern char *buttons[16];
extern char *Hit[2], *dead[3];
extern char *shadow;

extern char *Buttons[4];

char *drewno[3];
//=======Zmienne extern==============================
extern int color1, color2;

//====== Deklaracje funkcji ==================
void ShowBackground(void);
void InitPicture(void);
void FreeMemory(void);
void ShowPanel(int, int, int, int, int);
void ShowMainMenu(void);
void ShowSubMenu(void);
void ShowIntro(void);
void ShowPicture(int, int);
void ShowText(int, int);
void Haslo(char *, char);
int GetMemory(void);
//=========================================
extern void PressButton(int, int);
// ====== Ciala funkcji ===================

int GetMemory(void) {
  char *buf;
  unsigned int size;

  memoryBuf = NULL;
  memoryBuf = (char *)malloc(260000);

  if (memoryBuf == NULL) {
    FreeMemory();
    return 1;
  }

  buf = (char *)memoryBuf;
  for (int i = 0; i < MaxPictures; i++) {
    picture[i] = NULL;
  }

  for (i = 0; i < 22; i++) {
    picture[i] = (char *)buf;
    buf += 260;
  }

  for (i = 25; i < 113; i++) {
    picture[i] = (char *)buf;
    buf += 260;
  }

  // 104 obrazki  jeszcze 246-104=142 obrazki

  for (i = 127; i < 269; i++) {
    picture[i] = (char *)buf;
    buf += 260;
  }
  // koniec  bufora 0
  //-----------------------------------------

  for (i = 269; i < MaxPictures; i++) {
    picture[i] = (char *)buf;
    buf += 260;
  }
  // 64000-15*260=  60100
  size = GetImageSize13h(0, 0, 23, 18);
  // size=462
  for (i = 0; i < 3; i++) {
    picture[22 + i] = (char *)buf;
    buf += size;
  }
  // buf=58714
  size = GetImageSize13h(0, 0, 32, 14);
  // size=501
  for (i = 0; i < 7; i++) {
    picture[i + 113] = buf;
    buf += size;
  }
  for (i = 0; i < 7; i++) {
    picture[i + 120] = buf;
    buf += size;
  }
  // buf=51700
  // buf=50446
  //-----------------------------------------------
  size = GetImageSize13h(1, 1, 17, 15);
  // size=261
  for (i = 0; i < 16; i++) {
    buttons[i] = buf;
    buf += size;
  }
  for (i = 0; i < 14; i++) {
    tree[i] = buf;
    buf += size;
  }
  // buf=42616
  Hit[0] = buf;
  buf += size;
  Hit[1] = buf;
  buf += size;
  //   Myszka
  for (i = 0; i < 13; i++) {
    Mysz[i] = buf;
    buf += size;
  }
  // fire
  for (i = 0; i < 14; i++) {
    fire[i] = buf;
    buf += size;
  }

  for (i = 0; i < 3; i++) // dead[typ]
  {
    dead[i] = buf;
    buf += size;
  }
  // buf=34264

  ramka[0] = buf;
  buf += GetImageSize13h(0, 0, 11, 197);
  // buf=31882
  ramka[1] = buf;
  buf += GetImageSize13h(0, 0, 268, 8);
  // buf=29455
  ramka[2] = buf;
  buf += GetImageSize13h(267, 0, 274, 199);
  // buf=27849
  ramka[3] = buf;
  buf += GetImageSize13h(0, 190, 268, 200);
  // buf=24884
  // drewno
  drewno[0] = buf;
  buf += GetImageSize13h(272, 7, 276, 28);
  // buf=24768
  drewno[1] = buf;
  buf += GetImageSize13h(278, 7, 310, 28);
  // buf=24036
  drewno[2] = buf;
  buf += GetImageSize13h(299, 41, 314, 150);
  // buf=23890

  // shadow
  shadow = buf;
  buf += GetImageSize13h(0, 0, 32, 28);
  // buf=22927
  // buf=11215
  for (int typ = 0; typ < 4; typ++)
    for (int x = 0; x < 2;
         x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (int y = 0; y < 3; y++) // missiles [typ][x][y]
      {
        missiles[typ][x][y] = buf;
        buf += 261;
      }
  // buf=4951
  Buttons[0] = buf;
  buf += GetImageSize13h(108, 30, 219, 48);
  Buttons[1] = buf;
  buf += GetImageSize13h(108, 30, 219, 48);
  // buf=1683
  Buttons[2] = buf;
  buf += GetImageSize13h(274, 38, 292, 54);
  Buttons[3] = buf;
  buf += GetImageSize13h(274, 38, 292, 54);

  int x, y, faza;
  size = GetImageSize13h(0, 0, 16, 14); // 260

  ////////////////////////////////////////////////////////////////////////
  // postacie  pastuszek, mag, kusznik
  for (faza = 0; faza < 5; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        movers[faza][10][x][y] = buf; // pastuch
        buf += 260;
        movers[faza][11][x][y] = buf; // mag
        buf += 260;
        movers[faza][12][x][y] = buf; // kusznik
        buf += 260;
      }
  ////////////////////////////////////////////////////////////////////////
  //  postac topornik
  for (faza = 0; faza < 5; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        if (faza < 3) {
          movers[faza][1][x][y] = buf;
          buf += 260;
        }
      }
  // walka - topornik
  movers[3][1][0][0] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
  movers[3][1][0][1] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14);
  movers[3][1][0][2] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
  movers[3][1][1][0] = buf;
  buf += GetImageSize13h(0, 0, 16, 14 + 7);
  movers[3][1][1][1] = buf;
  buf += GetImageSize13h(0, 0, 16, 14);
  movers[3][1][1][2] = buf;
  buf += GetImageSize13h(0, 0, 16, 14 + 7);
  movers[4][1][0][0] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
  movers[4][1][0][1] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14);
  movers[4][1][0][2] = buf;
  buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
  movers[4][1][1][0] = buf;
  buf += GetImageSize13h(0, 0, 16, 14 + 7);
  movers[4][1][1][1] = buf;
  buf += GetImageSize13h(0, 0, 16, 14);
  movers[4][1][1][2] = buf;
  buf += GetImageSize13h(0, 0, 16, 14 + 7);

  //  postacie   kaplan - bohater
  // chod
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 5; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          if (faza < 3 || (typ != 1 && typ != 3)) {
            movers[faza][typ + 4][x][y] = buf;
            buf += 260;
          }
        }
  // walka
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    if (typ == 1 || typ == 3) {
      movers[3][typ + 4][0][0] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[3][typ + 4][0][1] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14);
      movers[3][typ + 4][0][2] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[3][typ + 4][1][0] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[3][typ + 4][1][1] = buf;
      buf += GetImageSize13h(0, 0, 16, 14);
      movers[3][typ + 4][1][2] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[4][typ + 4][0][0] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[4][typ + 4][0][1] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14);
      movers[4][typ + 4][0][2] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[4][typ + 4][1][0] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[4][typ + 4][1][1] = buf;
      buf += GetImageSize13h(0, 0, 16, 14);
      movers[4][typ + 4][1][2] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
    }
  }
  //  postacie   niedzwiedz, strzyga
  // chod
  for (typ = 0; typ < 2;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 3; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          {
            movers[faza][typ + 8][x][y] = buf;
            buf += 260;
          }
        }
  // walka
  for (typ = 0; typ < 2;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    {
      movers[3][typ + 8][0][0] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[3][typ + 8][0][1] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14);
      movers[3][typ + 8][0][2] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[3][typ + 8][1][0] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[3][typ + 8][1][1] = buf;
      buf += GetImageSize13h(0, 0, 16, 14);
      movers[3][typ + 8][1][2] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[4][typ + 8][0][0] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[4][typ + 8][0][1] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14);
      movers[4][typ + 8][0][2] = buf;
      buf += GetImageSize13h(0, 0, 16 + 8, 14 + 7);
      movers[4][typ + 8][1][0] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
      movers[4][typ + 8][1][1] = buf;
      buf += GetImageSize13h(0, 0, 16, 14);
      movers[4][typ + 8][1][2] = buf;
      buf += GetImageSize13h(0, 0, 16, 14 + 7);
    }
  }

  //  postacie   krowa, lucznik, kaplanka
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 5; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          if (typ != 1) {
            movers[faza][typ][x][y] = buf;
            buf += 260;
          }
        }
  return 0;
}

///////////////////////////////////////////////////////////////////////////
extern char *RealVirtualScreen;
void InitPicture() {
  int typ, faza, x, y;

  ShowPicture(3, 0);
  ShowPicture(18, 100);

  ///////////////////////////////////////////////////////////////////////////
  // petla do wyeliminowania koloru 1, ktory powstal w wyniku blednej konwersji
  // palet
  for (int ii = 0; ii < 63999; ii++)
    if (RealVirtualScreen[ii] == 1)
      RealVirtualScreen[ii] = 0;
  ///////////////////////////////////////////////////////////////////////////
  Color1 = RealVirtualScreen[1];
  LightRed = RealVirtualScreen[0];
  Red = RealVirtualScreen[1];
  DarkRed = RealVirtualScreen[2];

  LightYellow = RealVirtualScreen[3];
  Yellow = RealVirtualScreen[4];
  DarkYellow = RealVirtualScreen[5];

  LightBlue = RealVirtualScreen[6];
  Blue = RealVirtualScreen[7];
  DarkBlue = RealVirtualScreen[8];

  LightGreen = RealVirtualScreen[9];
  Green = RealVirtualScreen[10];
  DarkGreen = RealVirtualScreen[11];

  LightBrown = RealVirtualScreen[12];
  Brown = RealVirtualScreen[13];
  DarkBrown = RealVirtualScreen[14];

  LightGray = RealVirtualScreen[15];
  Gray = RealVirtualScreen[16];
  DarkGray = RealVirtualScreen[17];

  for (ii = 0; ii < 30; ii++)
    ExpColor[ii] = RealVirtualScreen[ii + 18];
  for (ii = 0; ii < 100; ii++)
    RealVirtualScreen[ii] = RealVirtualScreen[ii + 110];

  //-----------------------------------------------

  for (x = 1; x < 13; x++) // 1-3
  {
    if (Mysz[x] != NULL)
      GetImage13h(11 + (x - 1) * 16, 8, 11 + x * 16, 22, Mysz[x]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed.Mouse.");
      exit(0);
    }
  }

  //    przyciski
  for (x = 0; x < 14; x++) {
    if (buttons[x] != NULL)
      GetImage13h(11 + x * 16, 22, 27 + x * 16, 22 + 14, buttons[x]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - buttons (%d).", x);
      exit(0);
    }
  }

  //   tree
  for (x = 0; x < 2; x++) {
    if (tree[0 + x * 7] != NULL)
      GetImage13h(235, 22 + x * 56, 235 + 16, 36 + x * 56, tree[0 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[1 + x * 7] != NULL)
      GetImage13h(235 + 16, 22 + x * 56, 235 + 32, 36 + x * 56,
                  tree[1 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[2 + x * 7] != NULL)
      GetImage13h(235, 36 + x * 56, 235 + 16, 50 + x * 56, tree[2 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[3 + x * 7] != NULL)
      GetImage13h(235 + 16, 36 + x * 56, 235 + 32, 50 + x * 56,
                  tree[3 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[4 + x * 7] != NULL)
      GetImage13h(235, 50 + x * 56, 235 + 16, 64 + x * 56, tree[4 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[5 + x * 7] != NULL)
      GetImage13h(235 + 16, 50 + x * 56, 235 + 32, 64 + x * 56,
                  tree[5 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
    if (tree[6 + x * 7] != NULL)
      GetImage13h(235 + 16, 64 + x * 56, 235 + 32, 78 + x * 56,
                  tree[6 + x * 7]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - tree (%d).", x);
      exit(0);
    }
  }

  if (buttons[14] != NULL)
    GetImage13h(235 + 16, 8, 235 + 32, 8 + 14, buttons[14]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed - buttons (14).");
    exit(0);
  }

  if (buttons[15] != NULL)
    GetImage13h(235, 8, 235 + 16, 8 + 14, buttons[15]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed - buttons (15).");
    exit(0);
  }

  // Hit

  if (Hit[0] != NULL)
    GetImage13h(11 + 12 * 16, 8, 27 + 12 * 16, 21, Hit[0]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Hit 0");
    exit(0);
  }
  if (Hit[1] != NULL)
    GetImage13h(11 + 13 * 16, 8, 27 + 13 * 16, 21, Hit[1]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Hit 1");
    exit(0);
  }

  // dead - zwloki
  for (x = 0; x < 3; x++) // dead[typ]
  {
    if (dead[x] != NULL)
      GetImage13h(11 + 13 * 16, 8 + (3 + x) * 14, 27 + 13 * 16,
                  8 + (4 + x) * 14, dead[x]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - dead%d.", x);
      exit(0);
    }
  }
  //*/

  // - obrazki -
  // dry earth
  for (x = 0; x < 3; x++) {
    if (picture[x + 22] != NULL)
      GetImage13h(235, 134 + (x * 19), 257, 152 + (x * 19), picture[x + 22]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Dry Earth.");
      exit(0);
    }
  }
  // trawa  [0-8]
  for (x = 0; x < 3; x++)
    if (picture[x] != NULL)
      GetImage13h(11 + 16 * x, 36, 27 + 16 * x, 50, picture[x]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Grass.");
      exit(0);
    }

  // gadgety   [54-64]
  for (x = 3; x < 14; x++)
    if (picture[x + 54 - 3] != NULL)
      GetImage13h(11 + 16 * x, 36, 27 + 16 * x, 50, picture[x + 54 - 3]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - gadgets.");
      exit(0);
    }
  // gadgety [65-67]
  for (x = 0; x < 3; x++)
    if (picture[x + 65] != NULL)
      GetImage13h(11 + 16 * (x + 11), 8 + 140, 27 + 16 * (x + 11), 22 + 140,
                  picture[x + 65]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - gadgets.");
      exit(0);
    }

  // gadgety [68-73]
  for (x = 0; x < 6; x++)
    if (picture[x + 68] != NULL)
      GetImage13h(11 + 16 * (x + 8), 8 + 154, 27 + 16 * (x + 8), 22 + 154,
                  picture[x + 68]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - gadgets.");
      exit(0);
    }

  // water
  for (x = 0; x < 13; x++)
    if (picture[x + 74] != NULL)
      GetImage13h(11 + 16 * x, 50, 27 + 16 * x, 64, picture[x + 74]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - water.");
      exit(0);
    }

  for (x = 0; x < 13; x++)
    if (picture[x + 87] != NULL)
      GetImage13h(11 + 16 * x, 64, 27 + 16 * x, 78, picture[x + 87]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - water.");
      exit(0);
    }

  for (x = 0; x < 13; x++)
    if (picture[x + 100] != NULL)
      GetImage13h(11 + 16 * x, 78, 27 + 16 * x, 92, picture[x + 100]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - water.");
      exit(0);
    }

  // rocks
  for (x = 0; x < 13; x++)
    if (picture[x + 9] != NULL)
      GetImage13h(11 + 16 * x, 92, 27 + 16 * x, 106, picture[x + 9]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - rocks.");
      exit(0);
    }

  // kamienie + road  [22-34]
  for (x = 3; x < 13; x++)
    if (picture[x + 22] != NULL)
      GetImage13h(11 + 16 * (x), 8 + 126, 27 + 16 * (x), 8 + 140,
                  picture[x + 22]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed -  road.");
      exit(0);
    }

  /*
  for(x=0;x<4;x++)
    if(picture[x+31]!=NULL)
          GetImage13h(11+16*(x+9),8+126,27+16*(x+9),8+140,picture[x+31]);
          else {Close13h();cprintf("ERROR 3. Bitmap allocation failed -
  road.");exit(0);}
  */

  //   [35-45]
  for (x = 0; x < 11; x++)
    if (picture[x + 35] != NULL)
      GetImage13h(11 + 16 * x, 8 + 140, 27 + 16 * x, 8 + 154, picture[x + 35]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed -  road.");
      exit(0);
    }

  // bridge  [46-53]
  for (x = 0; x < 8; x++)
    if (picture[x + 46] != NULL)
      GetImage13h(11 + 16 * x, 8 + 154, 27 + 16 * x, 8 + 168, picture[x + 46]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - bridge.");
      exit(0);
    }

  // trees [113-119] dograc sprawe wielkosci drzew
  for (x = 0; x < 7; x++) {
    if (picture[x + 113] != NULL)
      GetImage13h(11 + 32 * x, 8 + (8 * 14), 43 + 32 * x, 8 + (9 * 14),
                  picture[x + 113]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - trees.");
      exit(0);
    }
  }
  for (x = 0; x < 7; x++) {
    if (picture[x + 120] != NULL)
      GetImage13h(11 + 32 * x, 8 + (7 * 14), 43 + 32 * x, 8 + (8 * 14),
                  picture[x + 120]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - trees.");
      exit(0);
    }
  }

  // fire
  for (x = 0; x < 14; x++)
    if (fire[x] != NULL)
      GetImage13h(11 + 16 * x, 8 + 168, 27 + 16 * x, 8 + 168 + 14, fire[x]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed - fire.");
      exit(0);
    }

  // ramka
  if (ramka[0] != NULL)
    GetImage13h(0, 0, 11, 197, ramka[0]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Frame 0");
    exit(0);
  }
  if (ramka[1] != NULL)
    GetImage13h(0, 0, 268, 8, ramka[1]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Frame 1");
    exit(0);
  }
  if (ramka[2] != NULL)
    GetImage13h(267, 0, 274, 199, ramka[2]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Frame 2");
    exit(0);
  }
  if (ramka[3] != NULL)
    GetImage13h(0, 190, 268, 200, ramka[3]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Frame 3");
    exit(0);
  }

  // drewno
  if (drewno[0] != NULL)
    GetImage13h(272, 7, 276, 28, drewno[0]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Wood 0");
    exit(0);
  }
  if (drewno[1] != NULL)
    GetImage13h(278, 7, 310, 28, drewno[1]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Wood 0");
    exit(0);
  }
  if (drewno[2] != NULL)
    GetImage13h(299, 41, 314, 150, drewno[2]);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed. Wood 0");
    exit(0);
  }

  // przyciski
  if (Buttons[2] != NULL)
    GetImage13h(274, 38, 292, 54, Buttons[2]);
  if (Buttons[3] != NULL)
    GetImage13h(274, 58, 292, 74, Buttons[3]);

  //////////// tlo 2///////////////////////////////////////////////

  ShowPicture(4, 0);
  ShowPicture(19, 100);
  ///////////////////////////////////////////////////////////////////////////
  // petla do wyeliminowania koloru 1, ktory powstal w wyniku blednej konwersji
  // palet
  for (ii = 0; ii < 63999; ii++)
    if (RealVirtualScreen[ii] == 1)
      RealVirtualScreen[ii] = 0;
  ///////////////////////////////////////////////////////////////////////////

  // trawa
  for (x = 0; x < 6; x++)
    if (picture[x + 3] != NULL)
      GetImage13h(303, (2 + x) * 14, 319, (3 + x) * 14, picture[x + 3]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Grass.");
      exit(0);
    }
  // tarcza
  for (x = 0; x < 4; x++)
    if (picture[x + 278] != NULL)
      GetImage13h(303, (8 + x) * 14, 319, (9 + x) * 14, picture[x + 278]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Shield.");
      exit(0);
    }
  // leczenie
  for (x = 0; x < 2; x++)
    if (picture[x + 282] != NULL)
      GetImage13h(303, (x)*14, 319, (1 + x) * 14, picture[x + 282]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Heald.");
      exit(0);
    }

  // palisada
  for (x = 0; x < 12; x++)
    if (picture[x + 266] != NULL)
      GetImage13h(303 - 16, (x)*14, 319 - 16, (x + 1) * 14, picture[x + 266]);
    else {
      Close13h();
      cprintf("ERROR 3. Bitmap allocation failed. Wall.");
      exit(0);
    }
  //  postacie   krowa-babcia
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 5; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          if (faza < 3 || (typ != 1)) {
            if (movers[faza][typ][x][y] != NULL)
              GetImage13h(x * 16 + (faza * 32), y * 14 + (typ * 42),
                          16 + x * 16 + (faza * 32), 14 + y * 14 + (typ * 42),
                          movers[faza][typ][x][y]);
            else {
              Close13h();
              cprintf("ERROR 3. Bitmap allocation failed: cow - girl.");
              exit(0);
            }
          }
        }
  // walka - topornik
  GetImage13h(96, (42) + 7, 120, (42) + 28, movers[3][1][0][0]);
  GetImage13h(96, (42) + 28, 120, (42) + 42, movers[3][1][0][1]);
  GetImage13h(120, (42) + 7, 136, (42) + 28, movers[3][1][1][0]);
  GetImage13h(120, (42) + 28, 136, (42) + 42, movers[3][1][1][1]);
  GetImage13h(136, (42), 152, (42) + 21, movers[3][1][1][2]);
  GetImage13h(152, (42), 176, (42) + 21, movers[3][1][0][2]);
  GetImage13h(176, (42) + 7, 200, (42) + 28, movers[4][1][0][0]);
  GetImage13h(176, (42) + 28, 200, (42) + 42, movers[4][1][0][1]);
  GetImage13h(200, (42) + 7, 216, (42) + 28, movers[4][1][1][0]);
  GetImage13h(200, (42) + 28, 216, (42) + 42, movers[4][1][1][1]);
  GetImage13h(136, (42) + 21, 152, (42) + 42, movers[4][1][1][2]);
  GetImage13h(152, (42) + 21, 176, (42) + 42, movers[4][1][0][2]);

  //////postac pastuch
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  for (faza = 0; faza < 3; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        if (movers[faza][10][x][y] != NULL)
          GetImage13h(160 + x * 16 + (faza * 32), y * 14,
                      160 + 16 + x * 16 + (faza * 32), 14 + y * 14,
                      movers[faza][10][x][y]);
      }
  // pastuch atak krowy pierwszy pasek
  for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
  {
    if (movers[3][10][0][y] != NULL)
      GetImage13h(256, y * 14, 256 + 16, 14 + y * 14, movers[3][10][0][y]);
  }
  // pastuch atak krowy drugi pasek
  for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
  {
    if (movers[3][10][1][y] != NULL)
      GetImage13h(160, y * 14 + 84, 160 + 16, 84 + 14 + y * 14,
                  movers[3][10][1][y]);
  }
  for (x = 0; x < 2; x++)   // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
    {
      if (movers[4][10][x][y] != NULL)
        GetImage13h(176 + x * 16, 84 + y * 14, 176 + 16 + x * 16,
                    84 + 14 + y * 14, movers[4][10][x][y]);
    }
  //////postac Mag /////////////////////////////////////

  for (faza = 0; faza < 3; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        if (movers[faza][11][x][y] != NULL)
          GetImage13h(160 + x * 16 + (faza * 32), 126 + y * 14,
                      160 + 16 + x * 16 + (faza * 32), 126 + 14 + y * 14,
                      movers[faza][11][x][y]);
      }
  /// Mag atak
  for (faza = 0; faza < 2; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        if (movers[faza + 3][11][x][y] != NULL)
          GetImage13h(208 + x * 16 + (faza * 32), 84 + y * 14,
                      208 + 16 + x * 16 + (faza * 32), 84 + 14 + y * 14,
                      movers[faza + 3][11][x][y]);
      }

  //////////// tlo 3 ///////////////////////////////////////////////

  ShowPicture(5, 0);
  ShowPicture(20, 100);
  ///////////////////////////////////////////////////////////////////////////
  // petla do wyeliminowania koloru 1, ktory powstal w wyniku blednej konwersji
  // palet
  for (ii = 0; ii < 63999; ii++)
    if (RealVirtualScreen[ii] == 1)
      RealVirtualScreen[ii] = 0;
  ///////////////////////////////////////////////////////////////////////////

  //  postacie   kaplan - bohater
  // chod
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 5; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          if (faza < 3 || (typ != 1 && typ != 3)) {
            if (movers[faza][typ + 4][x][y] != NULL)
              GetImage13h(x * 16 + (faza * 32), y * 14 + (typ * 42),
                          16 + x * 16 + (faza * 32), 14 + y * 14 + (typ * 42),
                          movers[faza][typ + 4][x][y]);
            else {
              Close13h();
              cprintf("ERROR 3. Bitmap allocation failed: cleric - hero.");
              exit(0);
            }
          }
        }
  // walka
  for (typ = 0; typ < 4;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    if (typ == 1 || typ == 3) {
      GetImage13h(96, (typ * 42) + 7, 120, (typ * 42) + 28,
                  movers[3][typ + 4][0][0]);
      GetImage13h(96, (typ * 42) + 28, 120, (typ * 42) + 42,
                  movers[3][typ + 4][0][1]);
      GetImage13h(120, (typ * 42) + 7, 136, (typ * 42) + 28,
                  movers[3][typ + 4][1][0]);
      GetImage13h(120, (typ * 42) + 28, 136, (typ * 42) + 42,
                  movers[3][typ + 4][1][1]);
      GetImage13h(136, (typ * 42), 152, (typ * 42) + 21,
                  movers[3][typ + 4][1][2]);
      GetImage13h(152, (typ * 42), 176, (typ * 42) + 21,
                  movers[3][typ + 4][0][2]);
      GetImage13h(176, (typ * 42) + 7, 200, (typ * 42) + 28,
                  movers[4][typ + 4][0][0]);
      GetImage13h(176, (typ * 42) + 28, 200, (typ * 42) + 42,
                  movers[4][typ + 4][0][1]);
      GetImage13h(200, (typ * 42) + 7, 216, (typ * 42) + 28,
                  movers[4][typ + 4][1][0]);
      GetImage13h(200, (typ * 42) + 28, 216, (typ * 42) + 42,
                  movers[4][typ + 4][1][1]);
      GetImage13h(136, (typ * 42) + 21, 152, (typ * 42) + 42,
                  movers[4][typ + 4][1][2]);
      GetImage13h(152, (typ * 42) + 21, 176, (typ * 42) + 42,
                  movers[4][typ + 4][0][2]);
    }
  }

  if (shadow != NULL)
    GetImage13h(6 * 32, 12 * 14, 6 * 32 + 32, 12 * 14 + 28, shadow);
  else {
    Close13h();
    cprintf("ERROR 3. Bitmap allocation failed - Shadow.");
    exit(0);
  }

  //    pociski
  for (typ = 0; typ < 4; typ++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // missiles [typ][x][y]
      {
        if (missiles[typ][x][y] != NULL)
          GetImage13h((15 + x) * 16, y * 14 + (typ * 42), (16 + x) * 16,
                      14 + y * 14 + (typ * 42), missiles[typ][x][y]);
        else {
          Close13h();
          cprintf("ERROR 3. Bitmap allocation failed - missiles.");
          exit(0);
        }
      }

  //////////// tlo 4 ///////////////////////////////////////////////

  ShowPicture(7, 0);
  ShowPicture(22, 100);
  ///////////////////////////////////////////////////////////////////////////
  // petla do wyeliminowania koloru 1, ktory powstal w wyniku blednej konwersji
  // palet
  for (ii = 0; ii < 63999; ii++)
    if (RealVirtualScreen[ii] == 1)
      RealVirtualScreen[ii] = 0;
  ///////////////////////////////////////////////////////////////////////////

  //  postacie   bear - something
  // chod
  for (typ = 0; typ < 2;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for (faza = 0; faza < 3; faza++)
      for (x = 0; x < 2;
           x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
        {
          {
            if (movers[faza][typ + 4][x][y] != NULL)
              GetImage13h(x * 16 + (faza * 32), y * 14 + (typ * 42),
                          16 + x * 16 + (faza * 32), 14 + y * 14 + (typ * 42),
                          movers[faza][typ + 8][x][y]);
            else {
              Close13h();
              cprintf("ERROR 3. Bitmap allocation failed: cleric - hero.");
              exit(0);
            }
          }
        }
  // walka
  for (typ = 0; typ < 2;
       typ++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    {
      GetImage13h(96, (typ * 42) + 7, 120, (typ * 42) + 28,
                  movers[3][typ + 8][0][0]);
      GetImage13h(96, (typ * 42) + 28, 120, (typ * 42) + 42,
                  movers[3][typ + 8][0][1]);
      GetImage13h(120, (typ * 42) + 7, 136, (typ * 42) + 28,
                  movers[3][typ + 8][1][0]);
      GetImage13h(120, (typ * 42) + 28, 136, (typ * 42) + 42,
                  movers[3][typ + 8][1][1]);
      GetImage13h(136, (typ * 42), 152, (typ * 42) + 21,
                  movers[3][typ + 8][1][2]);
      GetImage13h(152, (typ * 42), 176, (typ * 42) + 21,
                  movers[3][typ + 8][0][2]);
      GetImage13h(176, (typ * 42) + 7, 200, (typ * 42) + 28,
                  movers[4][typ + 8][0][0]);
      GetImage13h(176, (typ * 42) + 28, 200, (typ * 42) + 42,
                  movers[4][typ + 8][0][1]);
      GetImage13h(200, (typ * 42) + 7, 216, (typ * 42) + 28,
                  movers[4][typ + 8][1][0]);
      GetImage13h(200, (typ * 42) + 28, 216, (typ * 42) + 42,
                  movers[4][typ + 8][1][1]);
      GetImage13h(136, (typ * 42) + 21, 152, (typ * 42) + 42,
                  movers[4][typ + 8][1][2]);
      GetImage13h(152, (typ * 42) + 21, 176, (typ * 42) + 42,
                  movers[4][typ + 8][0][2]);
    }
  }
  ////////postac Kusznik
  for (faza = 0; faza < 2; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        GetImage13h(224 + x * 16 + (faza * 32), y * 14,
                    224 + 16 + (x * 16) + (faza * 32), 14 + y * 14,
                    movers[faza][12][x][y]);
      }
  for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
  {
    GetImage13h(224 + 32, y * 14, 224 + 32 + 16, 14 + y * 14,
                movers[2][12][0][y]);
  }
  for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
  {
    GetImage13h(224, 42 + y * 14, 224 + 16, 42 + 14 + y * 14,
                movers[2][12][1][y]);
  }
  for (faza = 0; faza < 2; faza++)
    for (x = 0; x < 2; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
      for (y = 0; y < 3; y++) // movers [faza][typ][x][y]
      {
        GetImage13h(224 + 16 + x * 16 + (faza * 32), 42 + y * 14,
                    224 + 16 + 16 + x * 16 + (faza * 32), 42 + 14 + y * 14,
                    movers[faza + 3][12][x][y]);
      }
  //////////////koniec kusznika/////////////

  // --------ok ------------------------------------------------------
  // buildings
  for (y = 0; y < 6; y++)
    for (x = 0; x < 20; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    {
      if (picture[137 + x + (y * 20)] != NULL)
        GetImage13h(x * 16, (y + 6) * 14, (x + 1) * 16, (y + 7) * 14,
                    picture[137 + x + (y * 20)]);
    }
  // budowa
  for (x = 0; x < 9; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    if (picture[127 + x] != NULL)
      GetImage13h(x * 16, 12 * 14, (x + 1) * 16, 13 * 14, picture[127 + x]);
  }
  // ruina
  for (x = 0; x < 9; x++) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  {
    if (picture[257 + x] != NULL)
      GetImage13h(x * 16, 13 * 14, (x + 1) * 16, 14 * 14, picture[257 + x]);
  }

  // przyciski
  ShowPicture(1, 0);
  ShowPicture(16, 100);
  Bar13h(110, 116, 217, 129, 0);
  Bar13h(110, 144, 217, 158, 0);
  if (Buttons[0] != NULL)
    GetImage13h(108, 142, 219, 160, Buttons[0]);
  if (Buttons[1] != NULL)
    GetImage13h(108, 114, 219, 132, Buttons[1]);
}
//------------------------------------------------
void FreeMemory() {
  if (memoryBuf == NULL)
    free(memoryBuf);
}
////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////
void ShowPanel(int iff, int co, int max, int food,
               int button) // kto 0-oni 1-myb 2-mypost
{
  int i;

  // PutImage13h(278,7,drewno[1],0);
  for (i = 0; i < 6; i++) {
    PutImage13h(274, 18 + 20 * i, Buttons[3], 1);
  }
  if (button)
    PutImage13h(274, -2 + 20 * button, Buttons[2], 1);
  if (iff + co + max == 0)
    return;

  switch (co) {
  case 10:
    if (max >= 1050)
      PutImage13h(275, 39, buttons[11], 1); // swiatynia
    if (max >= 850)
      PutImage13h(275, 59, buttons[10], 1); // koszary1
    if (max >= 650)
      PutImage13h(275, 79, buttons[9], 1); // stajnia
    if (max >= 250)
      PutImage13h(275, 119, picture[40], 1); // droga
    break;
  case 11: // odbudowa +krowa
    if (max >= 450 && food)
      PutImage13h(275, 99, movers[0][0][1][1], 1); // krowa
    break;
  case 12: // odbudowa +procarz+dzida+koszary2
    if (max >= 1050)
      PutImage13h(275, 39, buttons[12], 1); // koszary2
    if (max >= 650 && food)
      PutImage13h(275, 79, movers[0][2][1][1], 1); // luk
    if (max >= 450 && food)
      PutImage13h(275, 99, movers[0][1][1][1], 1); // topor
    break;
  case 13: // odbudowa +babcia+kaplan
    if (max >= 1050 && food)
      PutImage13h(275, 39, movers[0][4][1][1], 1); // kaplan
    if (max >= 650 && food)
      PutImage13h(275, 79, movers[0][3][1][1], 1); // druidka
    break;
  case 14: // odbudowa +miecz+luk+akademia
    if (max >= 1050)
      PutImage13h(275, 39, buttons[13], 1); // dom bohatera
    if (max >= 850 && food)
      PutImage13h(275, 59, movers[0][6][1][1], 1); // luk
    if (max >= 650 && food)
      PutImage13h(275, 79, movers[0][5][1][1], 1); // miecz
    if (max >= 250)
      PutImage13h(275, 119, buttons[14], 1); // palisada
    break;
  case 15: // odbudowa +wodz
    if (max >= 1050 && food)
      PutImage13h(275, 39, movers[0][7][1][1], 1); // bohater
    break;
  }

  if (co < 10) {
    if (co)
      PutImage13h(275, 19, buttons[0], 1); // tarcza
    if (co == 4 && food > 79) {
      PutImage13h(275, 61, missiles[2][1][2], 1);
      PutImage13h(271, 59, missiles[2][1][2], 1); // deszcz
      PutImage13h(279, 59, missiles[2][1][2], 1);
    }
    if (co == 4 && food > 25)
      PutImage13h(275, 79, buttons[15], 0); // widzenie
    if (co == 3 && food > 79) {
      PutImage13h(275, 60, missiles[1][1][2], 1); // deszcz
      PutImage13h(271, 59, missiles[1][1][2], 1); // deszcz
      PutImage13h(279, 59, missiles[1][1][2], 1);
    } // deszcz
    if (co == 3 && food > 49)
      PutImage13h(275, 79, picture[278], 1); // tarcza
    if (co == 1 && button == 2)
      PutImage13h(275, 59, buttons[8], 1); // topornik-odbudowa
  }
}
///////////////////////////////////////////////////////////////////////
//   POKAZANIE MENU GLOWNEGO
//////////////////////////////////////////////////////////////////////
void ShowMainMenu(void) {
  DownPalette(6);
  LoadExtendedPalette(2);
  ShowPicture(2, 0);
  ShowPicture(17, 100);
  RisePalette(0);
}
//////////////////////////////////////////////////////////////////////////
//  Pokazanie menu w trakcie gry
//////////////////////////////////////////////////////////////////////////
void ShowSubMenu(void) {
  int Ty[5] = {33, 60, 90, 117, 145};
  char *txt[5] = {"Zapisz gr$", "Wczytaj gr$", "Powt%rz wypraw$",
                  "Powr%t do gry", "Koniec gry"};
  // unsigned char *txt[5]={"Spiel speichern","Spiel laden","Wiederhole
  // Mission","Zur^ck zum Spiel","Spiel beenden"};

  DownPalette(2);
  LoadExtendedPalette(1);
  ShowPicture(1, 0);
  ShowPicture(16, 100);
  PressButton(1, 1);
  PressButton(2, 1);
  PressButton(3, 1);
  PressButton(4, 1);
  PressButton(5, 1);
  for (char i = 0; i < 5; i++) {
    CenterText13h(99, Ty[i] - 1, 99 + 126, Ty[i] + 13, txt[i], 1);
    CenterText13h(100, Ty[i], 100 + 126, Ty[i] + 14, txt[i], 1);
    CenterText13h(100, Ty[i] - 1, 100 + 126, Ty[i] + 13, txt[i], 79);
  }
  RisePalette(0);
}
//////////////////////////////////////////////////////////////////////////
//  Pokazanie menu przy wychodzeniu
//////////////////////////////////////////////////////////////////////////
void ShowEndMenu(void) {
  int Ty[5] = {33, 60, 90, 117, 145};
  // char *txt[5]={" "," "," ","Ja","Nein"};
  char *txt[5] = {" ", " ", " ", "Tak", "Nie"};

  DownPalette(2);
  LoadExtendedPalette(1);
  ShowPicture(1, 0);
  ShowPicture(16, 100);
  PressButton(5, 1);
  for (char i = 0; i < 5; i++) {
    CenterText13h(109, Ty[i] - 1, 109 + 106, Ty[i] + 13, txt[i], 1);
    CenterText13h(110, Ty[i], 110 + 106, Ty[i] + 14, txt[i], 1);
    CenterText13h(110, Ty[i] - 1, 110 + 106, Ty[i] + 13, txt[i], 79);
  }
  // CenterText13h(10,Ty[1]-1,10+300,Ty[1]+13,"Wollen Sie das Spiel wirklich
  // beenden?",79);
  CenterText13h(10, Ty[1] - 1, 10 + 300, Ty[1] + 13,
                "Naprawde chcesz skonczyc gre?", 79);
  RisePalette(0);
}

/////////////////////////////////////////////////////////
void ShowIntro(void) {
  // BlackPalette();
  LoadExtendedPalette(9);
  ShowPicture(9, 0);
  ShowPicture(25, 100);
  RisePalette(15);
  delay(1500);
  /*
  DownPalette(10);
  LoadExtendedPalette(0);
  ShowPicture(0,0);
  ShowPicture(15,100);
  RisePalette(10);*/
}
//////////////////////////////////////////////////////////
//============================================

void ShowBackground(void) {
  ShowPicture(3, 0);
  ShowPicture(18, 100);
}

//////////////////////////////////////////////////
void ShowText(int level, int t)

{ // t=0 wstep t=1 zwyciestwo t=2 porazka t=3 koniec)
  FILE *plik;
  char name[20] = "graf\\level.ini", z, l, line[150], endd = 0;
  int kolort = 230, kolorb = 1, k;
  // odswiez_Polan(generuj_graf(18)+1);
  DownPalette(0);
  // ShowPicture(0,0);   //plik intra
  // ShowPicture(15,100);

  if (!t) // wstep
  {
    ShowPicture(8, 0);
    ShowPicture(23, 100);
    LoadExtendedPalette(8);
    kolort = 248;
    kolorb = 18;
  }
  if (t == 1) { // zwyciestwo
    ShowPicture(14, 0);
    ShowPicture(29, 100);
    kolort = 203;
    LoadExtendedPalette(13);
  }
  if (t == 2) { // przegrana
    ShowPicture(11, 0);
    ShowPicture(26, 100);
    kolorb = 29;
    kolort = 1;
    LoadExtendedPalette(11);
  }
  if (t == 3) { // koniec gry
    ShowPicture(0, 0);
    ShowPicture(15, 100);
    kolorb = 1;
    kolort = 233;
    LoadExtendedPalette(0);
  }
  if (t == 3 && level == 1) // wstep do gry
  {
    ShowPicture(8, 0);
    ShowPicture(23, 100);
    LoadExtendedPalette(8);
    kolort = 248;
    kolorb = 18;
  }

  RisePalette(0);

  plik = fopen(name, "r");
  if (plik == NULL)
    return;
  k = 0;
  do {
    z = getc(plik);
    if (z == '$')
      k++;
    if (z == '@')
      return;
  } while (k != level + (t * 30));
  l = 0;
  k = 0;
  mouse.Key = 0;
  do {
  } while (mouse.GetMsg());
  mouse.Key = 0;
  endd = 0;
  do {
    do {
      z = getc(plik);
    } while (z != '!');
    k = 0;
    do {
      line[k] = getc(plik);
      k++;
    } while (line[k - 1] != '%');
    k--;
    line[k] = 0;
    if (k && line[k - 1] != '~')
      OutTextDelay13h(45, 10 * l + 5, line, kolort, kolorb, 30);
    l++;
    mouse.GetMsg();
    if (mouse.Key == 283)
      endd = 1; // esc
    if (mouse.Key == 27)
      endd = 1; // esc
  }

  while (!endd && line[k - 1] != '~');

  if (mouse.Key != 283)
    do {
    } while (!mouse.GetMsg());

  fclose(plik);
  DownPalette(2);
}

/////////////////////////////////////////////////////////////////
void ShowPicture(int nr, int b) {
  int t = LoadToScreen13h(nr, b);
  // if(t){Close13h();cprintf("Fehler 303. Zu wenig Arbeitsspeicher.\n\r
  // Ben”tigt werden 586 KB freien Arbeitsspeicher.\n\r");exit(0);}
}
///////////////////////////////////////////////////////////////
void Haslo(char *haslo, char nr) {}
