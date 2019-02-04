/////////////////////////////////////////////////////////////////////////
//
// Modul zawiera deklaracje i implementacje klas
// Mover1
// Missile
//////////////////////////////////////////////////////////////////////////
#include "image13h.h"
#include "mover.h"
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////
//---------Zmienne----------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////
extern int xpastw;
extern int ypastw;
extern int level;
inline int abs(int x) { return x > 0 ? x : -x; }

int Place[MaxX][MaxY];

/*------------dane o postaciach-------------------------------------------------
                    |-----------------s_range
                    | |---------------a_range
                    | | |-------------damage
                    | | | |-----------armour
                    | | | |  |--------hp
                    | | | |  |  |-----maxdelay
                    | | | |  |  |   |-???*/
int Udata[13][7] = {
    {2, 0, 0, 0, 100, 12, 0},  // krowa
    {3, 1, 10, 0, 100, 10, 0}, // drwal
    {4, 3, 5, 0, 100, 8, 0},   // wysliwy
    {4, 3, 35, 0, 70, 14, 0},  // kaplanka
    {5, 5, 50, 0, 80, 16, 0},  // kaplan
    {3, 1, 20, 3, 120, 10, 0}, // woj
    {5, 4, 15, 3, 120, 10, 0}, // wlocznik
    {4, 1, 30, 5, 150, 10, 0}, // rycerz
    {4, 1, 25, 3, 300, 16, 0}, // niedzwiedz
    {4, 1, 20, 3, 120, 6, 0},  // strzyga
    {3, 1, 0, 0, 40, 12, 0},   // pastuch
    {6, 6, 10, 0, 50, 12, 0},  // mag
    {6, 5, 25, 5, 130, 8, 0}   // kusznik
};

int ddamage[15] = {1, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 7, 8, 9};
int darmour[15] = {0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 6};
int dmagic[15] = {60,  80,  85,  90,  120, 140, 150, 160,
                  170, 180, 190, 200, 220, 240, 280};
extern int ExpColor[30];
//---------------shift-------------------------------------------------
char shiftX[13][17] = {
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 16, 15, 15, 15, 15},    // 12krowa
    {0, 1, 2, 3, 4, 6, 8, 10, 12, 14, 16, 25, 15, 15, 15, 15, 15},   // 10drwal
    {0, 2, 4, 6, 8, 10, 12, 14, 16, 15, 15, 15, 15, 15, 15, 15, 15}, // 8mysliwy
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 15, 15},   // 14kaplanka
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15},     // 16kaplan
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 16, 15, 15, 15, 15, 15, 15}, // 10woj
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 16, 15, 15, 15, 15, 15, 15}, // 10wlocznik
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 15, 15, 15, 15, 15, 15, 15}, // 10rycerz
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15}, // 16niedzwiedz ?
    {0, 3, 5, 7, 9, 12, 14, 15, 15, 8, 9, 10, 11, 12, 13, 14, 15},  // 6strzyga
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 16, 15, 15, 15, 15},   // 12pastuch
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 16, 15, 15, 15, 15},   // 12Mag
    {0, 2, 4, 6, 8, 10, 12, 14, 16, 15, 15, 15, 15, 15, 15, 15, 15} // 8kusznik
};
char shiftY[13][17] = {
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 15, 15, 15, 15},   // 12krowa
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15},  // 10drwal
    {0, 1, 3, 5, 7, 9, 11, 13, 14, 15, 15, 15, 15, 15, 15, 15, 15}, // 8mysliwy
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15},    // 14kaplanka
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 14},     // 16kaplan
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15}, // 10woj
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15}, // 10wlocznik
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15}, // 10rycerz
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 14}, // 16niedzwiedz
    {0, 3, 5, 7, 9, 12, 14, 14, 14, 8, 9, 10, 11, 12, 13, 14, 14}, // 6 strzyga
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 15, 15, 15, 15},  // 12Pastuch
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 15, 15, 15, 15},  // 12Mag
    {0, 1, 3, 5, 7, 9, 11, 13, 14, 15, 15, 15, 15, 15, 15, 15, 15} // 8kusznik
};

char Phase[13][19] = {
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 6, 1, 1, 0, 8, 4},  // 12krowa
    {0, 0, 1, 1, 1, 0, 0, 2, 2, 2, 0, 6, 0, 0, 0, 0, 0, 6, 3},  // 10topor
    {0, 0, 2, 2, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 4, 2},  // 8luk
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 6, 0, 11, 8}, // 14babcia
    {0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 13, 9}, // 16kaplan
    {0, 0, 2, 2, 2, 0, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 6, 3},  // 10miecz
    {0, 0, 2, 2, 2, 0, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 6, 3},  // 10dzida
    {0, 0, 2, 2, 2, 0, 0, 1, 1, 1, 1, 6, 0, 0, 1, 1, 0, 5, 2},  // 10dow
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 8, 2},  // 16niedzwiedz
    {0, 1, 1, 0, 2, 2, 0, 6, 0, 0, 0, 2, 2, 2, 2, 0, 0, 4, 2},  // 6strzyga
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 6, 1, 1, 0, 8, 4},  // 12pastuch
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 6, 1, 1, 0, 8, 4},  // 12mag
    {0, 0, 2, 2, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 4, 2}   // 8kusznik

};
//-------Zmienne extern -----------------------------------------------
extern int PlaceUsed;
extern int diff;
extern int master, drzewa, drzewa0;
extern int color1, color2;
extern char xleczenie, yleczenie;

extern int attack[MaxX][MaxY];
extern int place[MaxX][MaxY];
extern char placeN[MaxX][MaxY];
extern int placeG[MaxX][MaxY];
extern int Xe[20], Ye[16]; // world
extern char *picture[MaxPictures], *missiles[6][3][3], *tlo, *Mysz[13];
extern char *movers[5][13][3][3]; // faza:typ:dx:dy
extern char *Hit[2], *dead[3];
extern char *shadow;

struct MMessage Msg;

extern class Castle castle[2];
extern void Droga(int, int, int);

int Who(int nr) // 0 nikt 1-nasz wojownik 2-nasz budynek 3-ich wojownik 4-ich
                // budynek
{
  int aa = nr & 0x00ff;
  int bb = nr >> 8;
  if (bb < 1 || bb > 2)
    return 0;
  bb--;
  int nrb = aa / 10;       // nr budynku
  int nrp = aa - nrb * 10; // nr postaci
  if (nrp > 3 || nrb > 19) // jeden z zolnierzy
  {
    if (bb)
      return 3; // ich zolnierz
    else
      return 1; // nasz zolnierz
  }
  if (bb)
    return 4; // ich budynek
  else
    return 2; // nasz budynek
}
int Type(int nr) // 0 nikt 1-nasz wojownik 3-nasz budynek 2-ich wojownik 4-ich
                 // budynek
{
  int aa = nr & 0x00ff;
  int bb = nr >> 8;
  if (bb < 1 || bb > 2)
    return 0;
  bb--;
  int nrb = aa / 10;       // nr budynku
  int nrp = aa - nrb * 10; // nr postaci
  if (nrp > 3 || nrb > 19) // jeden z zolnierzy
  {
    if (nrb < 20)
      return castle[bb].b[nrb].m[nrp - 4].type;
    else
      return castle[bb].m[(nr & 0xff) - 200].type;
  }
  return castle[bb].b[nrb].type + 20;
}
Mover1 *Pointer(int nr) {
  int aa = nr & 0x00ff;
  int bb = nr >> 8;
  if (bb < 1 || bb > 2)
    return NULL;
  bb--;
  int nrb = aa / 10;       // nr budynku
  int nrp = aa - nrb * 10; // nr postaci
  if (nrp > 3 || nrb > 19) // jeden z zolnierzy
  {
    if (nrb < 20) {
      // char ss[20];
      // sprintf(ss,"Pointer c:%d, b:%d, p:%d,
      // u:%d",bb,nrb,nrp-4,castle[bb].b[nrb-1].m[nrp-4].udder);
      // strcpy(Msg.msg,ss);
      // Msg.licznik=20;
      return &castle[bb].b[nrb].m[nrp - 4];
    } else
      return &castle[bb].m[(nr & 0xff) - 200];
  }
  return NULL;
}
/////////////////////////////////////////////////////////////////
//
//  NewCow
/////////////////////////////////////////////////////////////////
void NewCow(int nr) {
  Mover1 *oldcow = Pointer(nr);
  if (oldcow == NULL)
    return;
  // oldcow->wybrany=1;
  int side = 1;
  if (nr > 512 && nr < 768)
    side = 0;

  for (int i = 2; i < 38; i++) {
    if (!(castle[side].m[i].exist)) {
      place[oldcow->x][oldcow->y] = 0;
      oldcow->exist = 0;
      castle[side].m[i].Init(oldcow->type, oldcow->x, oldcow->y, 1, 10);
      castle[side].m[i].SetIFF(side + 1);
      castle[side].m[i].SetNr(i + ((side + 1) << 8) + 200);
      castle[side].m[i].hp = oldcow->hp;
      castle[side].m[i].maxhp = oldcow->maxhp;
      castle[side].m[i].udder = oldcow->udder;
      castle[side].m[i].xm = castle[side].m[i].x + 3;
      castle[side].m[i].ym = castle[side].m[i].y + 3;
      castle[side].m[i].xp = castle[side].m[i].x;
      castle[side].m[i].yp = castle[side].m[i].y;
      castle[side].m[i].xe = castle[side].m[i].x;
      castle[side].m[i].ye = castle[side].m[i].y;
      castle[side].m[i].dx = castle[side].m[i].dx;
      castle[side].m[i].dy = castle[side].m[i].dy;
      castle[side].m[i].Show();
      if (side) {
        castle[side].m[i].xp = xpastw;
        castle[side].m[i].yp = ypastw;
        castle[side].m[i].xe = xpastw;
        castle[side].m[i].ye = ypastw;
      }
      return;
    }
  }
  for (i = 0; i < 20; i++)
    for (int j = 0; j < 6; j++) {
      if (castle[side].b[i].type == 2 && castle[side].b[i].exist == 1)
        if (!(castle[side].b[i].m[j].exist)) {
          // strcpy(Msg.msg,"Przepisuje krowe do obory");
          // Msg.licznik=200;

          place[oldcow->x][oldcow->y] = 0;
          oldcow->exist = 0;
          castle[side].b[i].m[j].Init(oldcow->type, oldcow->x, oldcow->y, 1,
                                      10);
          castle[side].b[i].m[j].SetIFF(side + 1);
          castle[side].b[i].m[j].SetNr(i * 10 + j + 4 + ((side + 1) << 8));
          castle[side].b[i].m[j].hp = oldcow->hp;
          castle[side].b[i].m[j].maxhp = oldcow->maxhp;
          castle[side].b[i].m[j].udder = oldcow->udder;
          castle[side].b[i].m[j].xm = castle[side].m[i].x + 3;
          castle[side].b[i].m[j].ym = castle[side].m[i].y + 3;
          castle[side].b[i].m[j].xp = castle[side].m[i].x;
          castle[side].b[i].m[j].yp = castle[side].m[i].y;
          castle[side].b[i].m[j].xe = castle[side].m[i].x;
          castle[side].b[i].m[j].ye = castle[side].m[i].y;
          castle[side].b[i].m[j].dx = castle[side].m[i].dx;
          castle[side].b[i].m[j].dy = castle[side].m[i].dy;
          castle[side].b[i].m[j].Show();
          if (side) {
            castle[side].b[i].m[j].xp = xpastw;
            castle[side].b[i].m[j].yp = ypastw;
            castle[side].b[i].m[j].xe = xpastw;
            castle[side].b[i].m[j].ye = ypastw;
          }
          return;
        }
    }
}
/////////////////////////////////////////////////////////////////
//
//  Find Cow
/////////////////////////////////////////////////////////////////
void FindCow(int x, int y, int *xe, int *ye) {
  int i, j, startk, stopk;
  int ii, jj;
  unsigned char kolejka[2000];
  for (j = 0; j < MaxY; j++)
    for (i = 0; i < MaxX; i++) {
      Place[i][j] = 1;
      if ((i > 0) && (j > 0) && (i < MaxX - 1) && (j < MaxY - 1)) {
        if (!place[i][j])
          Place[i][j] = 1000;
        if (place[i][j] > 255 && place[i][j] < 500) {
          ii = Who(place[i][j]);
          if (ii == 1) // nasz wojownik
          {
            ii = Type(place[i][j]);
            if (ii == 0)
              Place[i][j] = 1001;
          }
        }
      }
    }
  startk = 0;
  stopk = 0;
  i = x;
  j = y;
  Place[i][j] = 1000;
  do {
    /////////////////pozar prerii///////////////////////////
    if (stopk != startk) {
      i = kolejka[stopk];
      j = kolejka[stopk + 1];
      if (stopk < 1950)
        stopk += 2;
      else
        stopk = 0;
    }

    for (jj = j - 1; jj < j + 2; jj++)
      for (ii = i - 1; ii < i + 2; ii++) {
        if (Place[ii][jj] == 1001) {
          *xe = ii;
          *ye = jj;
          return;
        }
        if (Place[ii][jj] == 1000)
          if (ii > 0 && ii < MaxX - 1 && jj > 0 && jj < MaxY - 1) {
            Place[ii][jj] = 1;
            kolejka[startk] = ii;
            kolejka[startk + 1] = jj;
            if (startk < 1950)
              startk += 2;
            else
              startk = 0;
          }
      }

  } while (stopk != startk);
  //-------------------
  *xe = x;
  *ye = y;
}
/////////////////////////////////////////////////////////////////
//////
/////////////////////////////////////////////////////////////////
void FindEnemy(int x, int y, int *xe, int *ye, int *distance) {
  int i, j, startk, stopk;
  int ii, jj;
  unsigned char kolejka[2000];
  for (j = 0; j < MaxY; j++)
    for (i = 0; i < MaxX; i++) {
      Place[i][j] = 1;
      if (!place[i][j])
        Place[i][j] = 1000;
      if (place[i][j] > 255 && place[i][j] < 500 && !(*distance))
        Place[i][j] = 1001;
      if (place[i][j] > 511 && place[i][j] < 512 + 255 && (*distance == 1))
        Place[i][j] = 1001;
      if ((place[i][j] > drzewa0) && ((*distance) == 2))
        Place[i][j] = 1001;
    }

  startk = 0;
  stopk = 0;
  i = x;
  j = y;
  Place[i][j] = 1000;
  do {
    /////////////////pozar prerii///////////////////////////
    if (stopk != startk) {
      i = kolejka[stopk];
      j = kolejka[stopk + 1];
      if (stopk < 1950)
        stopk += 2;
      else
        stopk = 0;
    }

    for (jj = j - 1; jj < j + 2; jj++)
      for (ii = i - 1; ii < i + 2; ii++) {
        if (Place[ii][jj] == 1001) {
          *xe = ii;
          *ye = jj;
          return;
        }
        if (Place[ii][jj] == 1000)
          if (ii > 0 && ii < MaxX - 1 && jj > 0 && jj < MaxY - 1) {
            Place[ii][jj] = 1;
            kolejka[startk] = ii;
            kolejka[startk + 1] = jj;
            if (startk < 1950)
              startk += 2;
            else
              startk = 0;
          }
      }

  } while (stopk != startk);
  //-------------------
  *xe = 32;
  *ye = 32;
}
/////////////////////////////////////////////////////////////////
//  FindHolyPlace
/////////////////////////////////////////////////////////////////
void FindHolyPlace(int *xe, int *ye) {
  int i, j, startk, stopk;
  int ii, jj;
  if (placeG[*xe][*ye] == 256)
    return;
  int x = *xe;
  int y = *ye;
  unsigned char kolejka[2000];
  for (j = 0; j < MaxY; j++)
    for (i = 0; i < MaxX; i++) {
      Place[i][j] = 1; // zajete
      if ((i > 0) && (j > 0) && (i < MaxX - 1) && (j < MaxY - 1)) {
        if (!(place[i][j]))
          Place[i][j] = 1000; // wolne
        if (placeG[i][j] == 256)
          Place[i][j] = 1234;
      }
    }

  startk = 0;
  stopk = 0;
  i = x;
  j = y;
  Place[i][j] = 1000;
  do {
    /////////////////pozar prerii///////////////////////////
    if (stopk != startk) {
      i = kolejka[stopk];
      j = kolejka[stopk + 1];
      if (stopk < 1950)
        stopk += 2;
      else
        stopk = 0;
    }

    for (jj = j - 1; jj < j + 2; jj++)
      for (ii = i - 1; ii < i + 2; ii++) {
        if (Place[ii][jj] == 1234) {
          *xe = ii;
          *ye = jj;
          return;
        }
        if (Place[ii][jj] == 1000) {
          if (ii > 0 && ii < MaxX - 1 && jj > 0 && jj < MaxY - 1) {
            Place[ii][jj] = 1;
            kolejka[startk] = ii;
            kolejka[startk + 1] = jj;
            if (startk < 1950)
              startk += 2;
            else
              startk = 0;
          }
        }
      }

  } while (stopk != startk);
  //-------------------
  *xe = x;
  *ye = y;
}
//////////////////////////////////////////////////////////////////////////////
//          Missile
//////////////////////////////////////////////////////////////////////////////
void Missile::Init(int x1, int y1, int x2, int y2, int d, int t) {
  int max;
  if (exist)
    return;

  damage = d;
  xt = x2;
  yt = y2;
  target = place[xt][yt];
  dx = x2 - x1;
  dy = y2 - y1;
  if (abs(dx) - abs(dy) > 1)
    dy = 0;
  if (abs(dy) - abs(dx) > 1)
    dx = 0;
  if (dx > 0)
    dx = -1;
  if (dx < 0)
    dx = -1;
  if (dy > 0)
    dy = 1;
  if (dy < 0)
    dy = -1;
  exist = 6;
  type = (char)t;
  switch (t) {
  case 9: // strzyga
  case 8: // niedzwiedz
  case 7: // miecz -bohater
  case 5: // miecz
  case 1:
    view = NULL;
    break; // topor

  case 2:
    exist++;
    view = missiles[0][dx + 1][dy + 1];
    break; // luk
  case 12:
    exist++;
    view = missiles[5][dx + 1][dy + 1];
    break; // kusznik
  case 3:
    exist += 3;
    view = missiles[1][dx + 1][dy + 1];
    break; // grom
  case 13:
    exist += 3;
    view = missiles[1][dx + 1][dy + 1];
    break; // grom
  case 11:
    exist += 3;
    view = missiles[4][dx + 1][dy + 1];
    break; // magowy
  case 4:
    exist += 3;
    view = missiles[2][dx + 1][dy + 1];
    break; // duzy ogien
  case 14:
    exist += 3;
    view = missiles[2][dx + 1][dy + 1];
    break; // duzy ogien
  case 6:
    exist += 2;
    view = missiles[3][dx + 1][dy + 1];
    break; // wlocznia
  }
  //------------obliczenie przyrostow na ekranie
  max = 2;
  if (view != NULL) {
    dx = x2 - x1;
    dy = y2 - y1;
    max = abs(dx);
    if (max < abs(dy))
      max = abs(dy);
    max = max << 1;
    if (max < 1)
      max = 2;
    dx = (int)(dx << 4) / max;
    dy = (int)dy * 14 / max;
    x = x1 << 4;
    y = y1 * 14;
    if (type == 12) {
      dx *= 2;
      dy *= 2;
      max /= 2;
    }
    x += dx;
    y += dy;
  }
  //-------------------------
  visible = max;
  visible--;
  Move();
}
//-----------------------------------------
void Missile::Move() {
  if (!exist)
    return;
  if (exist == 100) {
    exist = 0;
    return;
  }
  if (exist == 2) // dzwiek wystrzalu
  {
    int kk = 0;
    switch (type) {
    case 2:
    case 6:
      kk = 3;
      break;
    case 13:
    case 3:
      kk = 4;
      break;
    case 4:
    case 14:
      kk = 5;
      break;
    case 11:
      kk = 6;
      break; // duch
    case 12:
      kk = 7;
      break; // kusza
    }
    if (Msg.dzwiek < kk) {
      Msg.dzwiek = kk;
      Msg.X = xt;
      Msg.Y = yt;
    }
  }
  if (exist > 1) {
    exist--;
    return;
  }

  if (visible)
    visible--;
  if (visible == 100) {
    exist = 0;
    return;
  }
  if (visible > 100) {
    attack[xt][yt] += damage;
    return;
  }
  if (visible > 0) {
    x += dx;
    y += dy;
  } else {
    exist = 120; // opoznienie przed nastepnym strzalem
    if (type == 4 || type == 11 || type == 12) {
      exist = 130;
    }
    attack[xt][yt] += damage;
    // gromobicie
    if (type == 13) {
      attack[xt + 1][yt] += damage;
      attack[xt - 1][yt] += damage;
      exist = 103;
    }
    // deszcz ognia
    if (type == 14) {
      exist = 103;
      attack[xt + 1][yt] += damage;
      attack[xt - 1][yt] += damage;
      if ((placeN[xt][yt] < 50 || placeG[xt][yt] < 112) &&
          (placeG[xt][yt] < 74 || placeG[xt][yt] > 86)) {
        placeN[xt][yt] = 100;
      }
      if ((placeN[xt + 1][yt] < 50 || placeG[xt + 1][yt] < 112) &&
          (placeG[xt + 1][yt] < 74 || placeG[xt + 1][yt] > 86)) {
        placeN[xt + 1][yt] = 100;
      }
      if ((placeN[xt - 1][yt] < 50 || placeG[xt - 1][yt] < 112) &&
          (placeG[xt - 1][yt] < 74 || placeG[xt - 1][yt] > 86)) {
        placeN[xt - 1][yt] = 100;
      }
    }
    /////kaplan i ognie
    if (type == 4 && (placeN[xt][yt] < 50 || placeG[xt][yt] < 112) &&
        (placeG[xt][yt] < 74 || placeG[xt][yt] > 86)) {
      placeN[xt][yt] = 100;
    }
    if (type == 4) { // rozrzut ognia po strzale
      if (dx > -3 && dx < 3)
        dx = 0;
      if (dx > 2)
        dx = 1;
      if (dx < -2)
        dx = -1;
      if (dy > -3 && dy < 3)
        dy = 0;
      if (dy > 2)
        dy = 1;
      if (dy < -2)
        dy = -1;
      xt += dx;
      yt += dy;
      if (placeG[xt][yt] < 74 || placeG[xt][yt] > 86) {
        placeN[xt][yt] = 90;
        attack[xt][yt] += (damage - 10);
      }
      xt += dx;
      yt += dy;
      if (placeG[xt][yt] < 74 || placeG[xt][yt] > 86) {
        placeN[xt][yt] = 80;
        attack[xt][yt] += (damage - 20);
      }
    }
    if ((type != 4) && (type != 11) && (type < 13)) {
      if (!place[xt][yt]) {
        Mover1 *tar = Pointer(target);
        if (tar != NULL) {
          if (abs(xt - tar->x) < 2 && abs(yt - tar->y) < 2) {
            xt = tar->x;
            yt = tar->y;
            if (diff)
              attack[xt][yt] += (damage >> 1);
            else
              attack[xt][yt] += (damage >> 2);
          }
        }
      }
    }
    // dzwieki uderzen
    if (type == 8 && Msg.dzwiek < 8) {
      Msg.dzwiek = 8;
      Msg.X = xt;
      Msg.Y = yt;
    }
    if (type == 9 && Msg.dzwiek < 9) {
      Msg.dzwiek = 9;
      Msg.X = xt;
      Msg.Y = yt;
    }
    if (type == 7 || type == 5 || type == 1) {
      if (place[xt][yt] > 768)
        dx = 1;
      else
        dx = 2;
      if (Msg.dzwiek < dx) {
        Msg.dzwiek = dx;
        Msg.X = xt;
        Msg.Y = yt;
      }
    }
    // duch
    if (type == 11) {
      visible = 140;
      exist = 1;
      x = xt * 16;
      y = yt * 14;
    } else {
      view = NULL;
      dx = 0;
      dy = 0;
    }
  }
}
//-------------------------------------------------------------
void Missile::Show(int xe, int ye) {
  if (exist != 1)
    return;

  if (view != NULL) {
    int a, b;
    a = x - xe;
    b = y - ye;
    if (a > 10 && a <= 259 && b > 7 && b < 185) {
      if (visible > 100) // duszacy duszek
      {
        if (visible & 0x04) // duszacy duszek
        {
          PutImage13h(a, b, missiles[4][1][1], 1);
        } else {
          if (dx > 0)
            PutImageChangeInverse13h(a, b, view, 1, 1, 1);
          else
            PutImage13h(a, b, view, 1);
        }
      } else {
        if (dx > 0)
          PutImageChangeInverse13h(a, b, view, 1, 1, 1);
        else
          PutImage13h(a, b, view, 1);
        if (type == 13 || type == 14) {
          int k = (6 + (y / 14) - yt) * 2;
          PutImage13h(a - k, b - 5, view, 1);
          PutImage13h(a + k, b - 5, view, 1);
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////
//
//            Mover1
//
/////////////////////////////////////////////////////////////////////

void Mover1::SetStart(int x0, int y0) {
  x = x0;
  y = y0;
}

void Mover1::Disable() {
  exist = 0;
  missile.exist = 0;
  wybrany = 0;
  ispath = 0;
}

void Mover1::Init(int t, int x1, int y1, int c, int d) {
  magic = 0;
  if (t == 3 || t == 4 || t == 11)
    magic = dmagic[0] / 2;
  wybrany = 0;
  exp = 0;
  type = t;

  s_range = Udata[t][0];
  a_range = Udata[t][1];
  damage = Udata[t][2];
  armour = Udata[t][3];
  hp = Udata[t][4];
  maxdelay = Udata[t][5];

  commandN = c;
  command = c;
  SetStart(x1, y1);
  xm = x1;
  ym = y1;
  maxhp = hp;
  exist = 1;
  dx = 0;
  dy = 0;
  phase = 0;
  udder = 0;
  target = 0;
  visible = 0;
  inmove = 0;
  delay = d;
  marmour = 0;
}

//----------------------------------------------------------------
Mover1::Mover1() {
  ispath = 0;
  dx = 0;
  dy = 0;
  exist = 0;
  wybrany = 0;
  missile.exist = 0;
  maxdelay = 5;
}
//------------------------------------------------------------------
Mover1::~Mover1() {
  exist = 0;
  missile.exist = 0;
  wybrany = 0;
}

//--------------------------------------------------------------------
void Mover1::SetNr(int Nr) { nr = Nr; }
//---------------------------------
void Mover1::SetTarget(int Nr) {
  target = Nr;
  mainTarget = 1;

  if (type == 11) {
    int d = Who(target);
    if (d != 1 && d != 3) {
      target = 0;
      mainTarget = 0;
    }
  }
}
//---------------------------------

void Mover1::SetIFF(int Nr) {
  IFF = Nr;
  if (IFF == 2 && diff > 1) {
    hp += 15;
    maxhp += 15;
  }
  if (IFF == 2 && !diff) {
    hp -= 25;
    maxhp -= 25;
  }
}
//---------------------------------
void Mover1::SetCommand(int Nr) {
  if (type != 1 && (Nr == 8 || Nr == 11))
    return;
  if (Nr == 8) {
    xm = xe;
    ym = ye;
  } // budowa
  if (Nr == 2)
    ispath = 0;
  if (Nr == 5)
    mainTarget = 0;
  if (type && Nr == 123 && (x & 1))
    return;
  commandN = Nr;
}
//----------------------------------
//----------------------------------
void Mover1::Labeling() {
  int i, j, mini, minj, min, startk, stopk;

  unsigned char ii, jj;
  unsigned char kolejka[2000];
  delay = maxdelay;
  if (PlaceUsed > 5 && type)
    return;
  PlaceUsed++;
  for (j = 0; j < MaxY; j++)
    for (i = 0; i < MaxX; i++)
      if (place[i][j] == 0) {
        Place[i][j] = 1000;
        if (placeN[i][j] > 10 && placeN[i][j] < 100)
          Place[i][j] = 9999;
      } else
        Place[i][j] = 9999;

  Place[xe][ye] = 0;
  startk = 0;
  stopk = 0;
  i = xe;
  j = ye;
  Place[i][j] = 0;
  do {
    /////////////////pozar prerii///////////////////////////
    if (stopk != startk) {
      i = kolejka[stopk];
      j = kolejka[stopk + 1];
      if (stopk < 1950)
        stopk += 2;
      else
        stopk = 0;
    }

    min = Place[i][j];
    for (jj = j - 1; jj < j + 2; jj++)
      for (ii = i - 1; ii < i + 2; ii++)
        if (Place[ii][jj] < 1001 && Place[ii][jj] > min + 1) {
          if (ii > 0 && ii < MaxX - 1 && jj > 0 && jj < MaxY - 1) {
            Place[ii][jj] = min + 1;
            kolejka[startk] = ii;
            kolejka[startk + 1] = jj;
            if (startk < 1950)
              startk += 2;
            else
              startk = 0;
          }
        }
  } while ((Place[x][y] == 1000) && (stopk != startk));
  //-------------------
  if (Place[x][y] == 1000) // 1 ruch w kierunku
  {
    if (!type)
      return;
    int ddx, ddy;
    ddx = x - xe;
    if (ddx < 0)
      ddx = -1;
    if (ddx > 0)
      ddx = 1;
    ddy = y - ye;
    if (ddy < 0)
      ddy = -1;
    if (ddy > 0)
      ddy = 1;
    path[0][0] = ddx;
    path[0][1] = ddy;
    ispath = 1;
    xe += ddx; // przesun punkt docelowy
    ye += ddy;
    // if(!type&&udder<100){xp=xe;yp=ye;}
    // if(!type){xp=x;yp=y;}
    return;
  }
  //-------------------------
  int Path[6][2];
  i = x;
  j = y;
  Place[i][j] = 1000;
  min = 1000;
  if (Place[i - 1][j] < min) {
    min = Place[i - 1][j];
    mini = i - 1;
    minj = j;
  }
  if (Place[i + 1][j] < min) {
    min = Place[i + 1][j];
    mini = i + 1;
    minj = j;
  }
  if (Place[i][j - 1] < min) {
    min = Place[i][j - 1];
    mini = i;
    minj = j - 1;
  }
  if (Place[i][j + 1] < min) {
    min = Place[i][j + 1];
    mini = i;
    minj = j + 1;
  }
  if (Place[i - 1][j - 1] < min) {
    min = Place[i - 1][j - 1];
    mini = i - 1;
    minj = j - 1;
  }
  if (Place[i - 1][j + 1] < min) {
    min = Place[i - 1][j + 1];
    mini = i - 1;
    minj = j + 1;
  }
  if (Place[i + 1][j - 1] < min) {
    min = Place[i + 1][j - 1];
    mini = i + 1;
    minj = j - 1;
  }
  if (Place[i + 1][j + 1] < min) {
    min = Place[i + 1][j + 1];
    mini = i + 1;
    minj = j + 1;
  }

  if (min >= 1000)
    return;
  Path[0][0] = i - mini;
  Path[0][1] = j - minj;
  ispath = 1;
  if (min == 0) {
    path[0][0] = Path[0][0];
    path[0][1] = Path[0][1];
    return;
  }
  do {
    i = mini;
    j = minj;
    min = 1000;
    Place[i][j] = 1000;
    if (Place[i - 1][j] < min) {
      min = Place[i - 1][j];
      mini = i - 1;
      minj = j;
    }
    if (Place[i + 1][j] < min) {
      min = Place[i + 1][j];
      mini = i + 1;
      minj = j;
    }
    if (Place[i][j - 1] < min) {
      min = Place[i][j - 1];
      mini = i;
      minj = j - 1;
    }
    if (Place[i][j + 1] < min) {
      min = Place[i][j + 1];
      mini = i;
      minj = j + 1;
    }
    if (Place[i - 1][j - 1] < min) {
      min = Place[i - 1][j - 1];
      mini = i - 1;
      minj = j - 1;
    }
    if (Place[i - 1][j + 1] < min) {
      min = Place[i - 1][j + 1];
      mini = i - 1;
      minj = j + 1;
    }
    if (Place[i + 1][j - 1] < min) {
      min = Place[i + 1][j - 1];
      mini = i + 1;
      minj = j - 1;
    }
    if (Place[i + 1][j + 1] < min) {
      min = Place[i + 1][j + 1];
      mini = i + 1;
      minj = j + 1;
    }

    if (min >= 1000) {
      for (i = 0; i < ispath; i++) {
        path[i][0] = Path[ispath - i - 1][0];
        path[i][1] = Path[ispath - i - 1][1];
      }
      return;
    }
    Path[ispath][0] = i - mini;
    Path[ispath][1] = j - minj;
    ispath++;
  } while ((min != 0) && (ispath < 6));
  for (i = 0; i < ispath; i++) {
    path[i][0] = Path[ispath - i - 1][0];
    path[i][1] = Path[ispath - i - 1][1];
  }
}
//----------------------------------
//----------------------------------
//-----------------------------------
void Mover1::Move() {
  inmove = 0;
  if (xe < 1)
    xe = 1;
  if (xe >= MaxX)
    xe = MaxX - 1;
  if (ye < 1)
    ye = 1;
  if (ye >= MaxY)
    ye = MaxY - 1;
  if ((x == xe) && (y == ye)) {
    commandN = 0;
    if (!type && command == 3) {
      command = 1;
      commandN = 1;
      delay = 0;
    }
    return;
  }
  if (type == 11 && magic > 10 && Distance() > 6) {
    commandN = 10;
    return;
  }
  place[x][y] = 0;
  if (!ispath)
    Labeling();
  place[x][y] = nr;
  if (!ispath) {
    if (!type) {
      Mover1 *mov;
      if (place[x - 1][y - 1]) {
        mov = Pointer(place[x - 1][y - 1]);
        if (mov != NULL) {
          mov->SetEnd(x - 5, y - 5);
          mov->SetCommand(1);
          // strcpy(Msg.msg,"Posun sie z -1 -1 !!!");
          /// Msg.licznik=20;
        }
      }
      if (place[x + 1][y + 1]) {
        mov = Pointer(place[x + 1][y + 1]);
        if (mov != NULL) {
          mov->SetEnd(x + 5, y + 5);
          mov->SetCommand(1);
          // strcpy(Msg.msg,"Posun sie z 1 1 !!!");
          // Msg.licznik=20;
        }
      }
      if (place[x - 1][y + 1]) {
        mov = Pointer(place[x - 1][y + 1]);
        if (mov != NULL) {
          mov->SetEnd(x - 5, y + 5);
          mov->SetCommand(1);
          // strcpy(Msg.msg,"Posun sie z -1 1 !!!");
          // Msg.licznik=20;
        }
      }
      drange++;
      if (drange > 14) {
        if (placeG[x][y] != 163) {
          xe = x;
          ye = y;
          if (udder < 100) {
            xp = x;
            yp = y;
          }
        }
        drange = 0;
      }
      delay = maxdelay;
      return;
    } else {
      delay = maxdelay;
      return;
    }
  }

  dx = path[ispath - 1][0];
  dy = path[ispath - 1][1];

  if (place[x - dx][y - dy] != 0) {
    if ((type) && (x - xe < 2) && (x - xe > -2) && (y - ye < 2) &&
        (y - ye > -2)) {
      commandN = 0;
    }
    if (!type) {
      // if(Msg.dzwiek<8){Msg.dzwiek=8;Msg.X=x;Msg.Y=y;}
      if (placeG[x][y] != 163) {
        xe = x;
        ye = y;
        if (udder < 100) {
          xp = x;
          yp = y;
        }
      }
      ispath = 0;
      return;
    }
    if (udder > 3) {
      udder = 0;
      ispath = 0;
      return;
    }
    udder++;
    delay = maxdelay;
    return;
  }
  ispath--;
  Hide();
  x -= dx;
  y -= dy;
  Show();
  delay = maxdelay;
  // if(placeG[x][y]>34&&placeG[x][y]<46)delay-=2; //przyspieszenie na drodze
  inmove = 1;
  if (x == xe && y == ye) {
    ispath = 0;
  }
}
//--------------------------------------------
int Mover1::LookAround() {
  if (!type) {
    command = 2;
    return 1;
  }
  int i, j, k = 1, d = 0, iff;
  int NoOfEnemies = 0;
  int NoOfOurs = 0;
  if (IFF == 2 && type == 10) // pastuch
  {
    FindCow(x, y, &xe, &ye);
    if (xe != x || y != ye) {
      target = place[xe][ye];
      commandN = 2;
      return 1;
    } else {
      if (x != xm || y != ym) {
        xe = xm;
        ye = ym;
        commandN = 1;
      }
    }
    return 0;
  }
  /*
  if(IFF==1&&type==10)//pastuch
  {
      return 0;
  }*/

  iff = IFF << 8;
  ispath = 0;
  target = 0;
  if (IFF == 2 && hp > (maxhp - 20)) // komputerowy
  {
    k = s_range + drange;
    for (i = x - k; i <= x + k && i < MaxX - 1; i++)
      for (j = y - k + 1; j <= y + k - 1 && j < MaxY - 1; j++) {
        switch (Who(place[i][j])) {
        case 1:
          NoOfEnemies++;
          break;
        case 3:
          NoOfOurs++;
          break;
        }
      }
    if (NoOfEnemies > 2 && NoOfOurs == 1) {
      int distance = 1;
      FindEnemy(x, y, &xe, &ye, &distance);
      commandN = 1; // move
      return 0;
    }
  }
  for (k = 1; k <= s_range + drange; k++) {
    for (i = x - k; i <= x + k && i < MaxX - 1; i++)
      for (j = y - k; j <= y + k && j < MaxY - 1; j++) {
        if (i > 0 && i < MaxX - 1 && j > 0 && j < MaxY - 1) {
          if (place[i][j] && place[i][j] != nr) {
            d = place[i][j];
            int dd = Who(d);
            if (((d & 0xff00) != iff) && (dd == 1 || dd == 3)) { // postac
              target = d;
              if (type != 10)
                return 1;
              Mover1 *mm = Pointer(target);
              if ((!mm->type) || (mm->type == 9))
                return 1;
            }
          }
        }
      }
  }
  if (!d || IFF == 1)
    return 0; // nasi nie atakuja budynkow
  if (type == 11)
    return 0; // Mag tez nie
  for (k = 1; k <= s_range + drange; k++) {
    for (i = x - k; i <= x + k && i < MaxX - 1; i++)
      for (j = y - k + 1; j <= y + k - 1 && j < MaxY - 1; j++) {
        if (i > 0 && i < MaxX - 1 && j > 0 && j < MaxY - 1) {
          if (place[i][j] && place[i][j] != nr) {
            d = place[i][j];
            if (d < 256 + 512 && d > 255 && (d & 0xff00) != iff) {
              target = d;
              return 1;
            }
          }
        }
      }
  }

  return 0;
}
//-----------------------------------------------
int Mover1::LokateTarget() {
  int i, j;
  if (!target)
    return 0;
  if (place[xe][ye] == target)
    return 1;
  for (i = 1; i < MaxX - 1; i++)
    for (j = 1; j < MaxY - 1; j++) {
      if (place[i][j] == target) {
        xe = i;
        ye = j;
        return 1;
      }
    }
  return 0;
}
//-------------------------------------------
int Mover1::Distance() {
  int maxx, maxy;
  maxx = x - xe;
  maxy = y - ye;
  if (maxx < 0)
    maxx = -maxx;
  if (maxy < 0)
    maxy = -maxy;
  if (maxy < maxx)
    return maxx;
  else
    return maxy;
}
//------------------------------------------------
void Mover1::Attack() {
  inmove = 0;
  if (target < 768)
    ispath = 0;
  if (target > 768 && type != 1 && type != 4) {
    target = 0;
    mainTarget = 0;
  }
  if (!target) {
    phase = 0;
    delay = maxdelay;
    return;
  }

  if (!LokateTarget()) {
    target = 0;
    mainTarget = 0;
    phase = 0;
    if (command == 2) {
      commandN = 0;
    } else {
      if (command != 11)
        commandN = 5;
      else
        commandN = 11;
    }
    delay = maxdelay;
    return;
  }
  if (Distance() <= a_range) {
    if (type == 10) {
      ispath = 0;
      /// wstaznik na krowe

      inattack = 1;
      Mover1 *oldcow = Pointer(target);
      if (oldcow != NULL)
        oldcow->SetCommand(123); // zmiana
      dx = x - xe;
      dy = y - ye;
      if (dx > 1)
        dx = 1;
      if (dx < 0)
        dx = -1;
      if (dy > 1)
        dy = 1;
      if (dy < 0)
        dy = -1;
      delay = 10;
      target = 0;
      commandN = 0;
      return;
    }

    ispath = 0;
    if (missile.exist) {
      delay = maxdelay;
      return;
    }
    dx = x - xe;
    dy = y - ye;
    if (IFF == 2 && type == 3 && magic < 11) {
      SetEnd(x + (dx * 3), y + (dy * 3));
      Move();
      return;
    }
    if (IFF == 2 && type == 11 && magic < 11) {
      SetEnd(x + dx * 3, y + dy * 3);
      Move();
      return;
    }
    if (IFF == 2 && type == 4 && magic < 21) {
      SetEnd(x + dx * 3, y + dy * 3);
      Move();
      return;
    } // ucieczka
    delay = maxdelay;

    if (type == 3 && magic < 11) {
      phase = 0;
      dx = 0;
      dy = 0;
      return;
    }
    if (type == 4 && magic < 21) {
      phase = 0;
      dx = 0;
      dy = 0;
      return;
    }
    if (type == 11 && magic < 21) {
      phase = 0;
      dx = 0;
      dy = 0;
      return;
    }
    if (IFF == 2 && (type == 4 || type == 3) && magic > 80) {
      phase = 0;
      dx = 0;
      dy = 0;
      commandN = 7;
      delay = 0;
      return;
    }

    if (type == 3)
      magic -= 10;
    if (type == 4)
      magic -= 20;
    if (type == 11)
      magic -= 20;
    if (IFF == 2 && place[xe][ye] > 800)
      damage += 20;
    missile.Init(x, y, xe, ye, damage + ddamage[exp >> 4], type);
    if (IFF == 2 && place[xe][ye] > 800)
      damage -= 20;
    if ((type == 4 || type == 11) && target > 768) {
      mainTarget = 0;
    }
    if (target < 768 && exp < (15 * 16) - 5) {
      int kkl = Who(target);
      if (kkl == 3 || IFF == 2) // jezeli nie budynek
      {
        exp++;
        if (type == 3 || type == 4 || type == 11)
          exp += 3;
      }
    }
    inattack = 1;
    if (dx > 1)
      dx = 1;
    if (dx < 0)
      dx = -1;
    if (dy > 1)
      dy = 1;
    if (dy < 0)
      dy = -1;
    if (!placeN[x][y])
      placeN[x][y] = 1;
    if (!placeN[xe][ye])
      placeN[xe][ye] = 1;

    if (!mainTarget) {
      target = 0;
      if (command == 2)
        commandN = 0;
      else
        commandN = 5;
    }
    return;
  }
  if (command == 2 || command == 11) {
    Move();
  } else {
    target = 0;
    commandN = 5;
    delay = maxdelay;
  }
}
//--------------------------------------------
void Mover1::Repare() {
  inmove = 0;
  inattack = 0;
  if (placeN[xe][ye] > 226 || placeG[xe][ye] == 47 ||
      (placeG[xe][ye] > 265 && placeG[xe][ye] < 277)) {
    commandN = 0;
    placeN[xe][ye] = 1;
    delay = maxdelay;
    return;
  }
  if (Distance() == 1) {
    ispath = 0;
    if (placeG[xe][ye] == 277 && place[xe][ye]) {
      delay = maxdelay;
      return;
    }
    dx = x - xe;
    dy = y - ye;
    delay = maxdelay;
    inattack = 1;
    if (dx > 1)
      dx = 1;
    if (dx < 0)
      dx = -1;
    if (dy > 1)
      dy = 1;
    if (dy < 0)
      dy = -1;
    if (placeN[xe][ye] < 220)
      placeN[xe][ye] = 220;
    else
      placeN[xe][ye]++;
    if (Msg.dzwiek < 2) {
      Msg.dzwiek = 1;
      Msg.X = x;
      Msg.Y = y;
    }
    return;
  }
  xe = xm;
  ye = ym;
  Move();
}
//--------------------------------------------

void Mover1::Run() {
  if (type)
    Run1();
  else
    Run2();
}
//////////////////////////////////////////////////
void Mover1::Run1() // rycerze
{
  if (!exist)
    return;
  if (exist == 2 && delay < 1) {
    exist = 0;
    return;
  }
  if (exist == 2) {
    delay--;
    return;
  } // trup

  if (attack[x][y]) {
    int dem;
    if (marmour)
      attack[x][y] = 0;
    if (attack[x][y] > 0) {
      dem = (int)attack[x][y] - armour - darmour[exp >> 4];
      attack[x][y] = 0;
      if (dem <= 0)
        dem = 1;
      if (!mainTarget)
        if (command == 2)
          commandN = 0;
      if ((command == 1 || command == 8) && type != 10 &&
          placeG[xe][ye] != 256 && (hp > (maxhp >> 1)))
        commandN = 0;        // zatrzymaj idacych!
      if (placeN[x][y] > 60) // uciekaj z ognia
      {
        for (int ii = x - 1; ii <= x + 1; ii++) {
          for (int jj = y - 1; jj <= y + 1; jj++) {
            if (ii > 0 && jj > 0 && ii < MaxX - 1 && jj < MaxY - 1) {
              if (!(place[ii][jj]) && (placeN[ii][jj] < 2)) {
                xe = ii;
                ye = jj;
                SetCommand(1);
              }
            }
          }
        }
      }
      if (type == 3 && magic > 49) {
        commandN = 9; // magiczna tarcza
        if (exp < 100)
          exp += 4;
      }
      if (hp > dem) {
        hp -= dem;
        ShowHit = 5;
        if (IFF == 2)
          drange = 6;
      } else {
        exist = 2;
        hp = 0;
        dx = 0;
        dy = 0;
        ispath = 0;
        delay = 400;
        Hide();
        inmove = 0;
        int kk = 35 + type * 11;
        if (Msg.dzwiek < kk) {
          Msg.dzwiek = kk;
          Msg.X = x;
          Msg.Y = y;
        }
        return;
      }
      if (hp < (diff >> 4) && IFF == 2 && command != 1) //&&diff
      { // jezeli chory to udaj sie do swiatyni na leczenie
        int xl = x, yl = y;
        FindHolyPlace(&xl, &yl);
        if (xl != x || yl != y) {
          SetEnd(xl, yl);
          SetCommand(1);
        }
      }
    }
  }

  if (marmour)
    marmour--;

  if (delay)
    return; // wyjdz jezeli nie koniec ruchu
  if (drange)
    drange--;

  if (IFF == 2 && !command && placeG[x][y] == 163) //??? nie blokuj obory
  {
    SetEnd(x + 2, y - 3);
    SetCommand(1);
  }
  /*   if(hp<5+(diff>>4)&&IFF==2&&!drange&&command!=1)//&&diff
     {// jezeli chory to udaj sie do swiatyni na leczenie
         int xl=x,yl=y;
         if(placeG[xe][ye]!=256)
         {
             FindHolyPlace(&xl,&yl);
             if(xl!=x||yl!=y)
             {
                 SetEnd(xl,yl);
                 SetCommand(1);
             }
         }
         else
         {
             SetCommand(1);
         }
     }*/
  if (hp == maxhp && IFF == 2 && !command &&
      placeG[x][y] == 256) { // wyleczony odsun sie z miejsca leczenia
    SetEnd(x + 5, y + 5);
    SetCommand(1);
  }
  inattack = 0;
  phase = 0;
  if (placeG[x][y] == 256 && hp < maxhp) {
    hp += 4; // leczenie w swietym miejscu
    if (hp > maxhp)
      hp = maxhp;
    if (type == 3 || type == 4 || type == 9 || type == 11)
      magic += 5;
  }
  if (type == 3 || type == 4 || type == 9 || type == 11) {
    magic++;
    if (type == 11)
      magic++;
    if (magic > dmagic[exp >> 4])
      magic = dmagic[exp >> 4];
    if (type == 9 && magic > 9 && hp < maxhp) { // autoleczenie strzygi
      hp += 10;
      magic -= 10;
    }
  }

  if (command == 8 && commandN != 8) {
    placeN[xe][ye] = 1; // jezeli konczysz robote to wyczysc po sobie
  }
  if (command == 10) // teleport
  {
    commandN = 0;
    Hide();
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      xe += 3; //=0
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      ye += 3; // 1
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      xe -= 3; // 2
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      xe -= 3; // 3
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      ye -= 3; // 4
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      ye -= 3; // 5
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      xe += 3; // 6
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2)
      xe += 3; // 7
    if (place[xe][ye] || xe < 1 || ye < 1 || xe > MaxX - 2 || ye > MaxX - 2) {
      xe = x;
      ye = y;
    }
    x = xe;
    y = ye;
    Show();
    inattack = 1;
    delay = maxdelay;
    command = commandN;
    return;
  }
  command = commandN;

  switch (command) {
  case 123:
    NewCow(nr);
    return;
  case 0:
    if (LookAround()) {
      commandN = 2;
      return;
    } // jezeli nic nie robi to rozejzec sie
    break;
  case 1:
    if (x == xe && y == ye) {
      commandN = 0;
      inmove = 0;
      return;
    } else {
      Move();
      return;
    } // jezeli idzie to idz

  case 2:
  case 6:
    Attack();
    return; // jezeli atak to atakuj
  case 5:
    if (LookAround()) {
      commandN = 6;
      return;
    } // jezeli nic nie robi to rozejzec sie
    break;
  case 7:
    if (magic >= 80) {
      inattack = Distance();
      if (inattack > 9) {
        inattack = 0;
        Move();
        return;
      }
      if (missile.exist)
        return;
      inmove = 0;
      inattack = 1;
      dx = 0;
      dy = 0;
      int Mtype = 14;
      if (type == 3)
        Mtype = 13;
      int xxe = xe + ((magic & 2) >> 1) - ((magic & 4) >> 2);
      int yye = ye + ((magic & 2) >> 1) - ((magic & 4) >> 2);
      missile.Init(xxe + 1, yye - 4, xxe, yye, damage - (type - 3) * 20, Mtype);
      magic -= 5;
      delay = maxdelay;
      if (!place[xe][ye])
        commandN = 0;
    } else
      commandN = 0;
    break;
  case 8:
    Repare();
    return;
  case 9:
    commandN = 0;
    if (magic > 49 && type == 3) {
      inattack = 1;
      magic -= 50;
      marmour = 100;
      if (IFF == 1)
        marmour = 200;
      if (Msg.dzwiek < 16) {
        Msg.dzwiek = 10;
        Msg.X = x;
        Msg.Y = y;
      }
    }
    if (magic > 24 && type == 4) { // widzenie
      inattack = 1;
      magic -= 25;
      marmour = 5;
      if (Msg.dzwiek < 12) {
        Msg.dzwiek = 11;
        Msg.X = x;
        Msg.Y = y;
      }
      int xx, yy, i, j;
      for (i = 1; i < MaxX; i++)
        for (j = 1; j < MaxY; j++) {
          xx = abs(x - i);
          yy = abs(y - j);
          if (xx + yy < 18 && yy < 14 && xx < 14 && !placeN[i][j])
            placeN[i][j] = 1;
        }
    }
    break;
  case 10: // teleportacja
    if (magic > 10) {
      inmove = 0;
      inattack = 0;
      inattack = 1;
      dx = 0;
      dy = 0;
      magic -= 10;
      delay = maxdelay;
    }
    commandN = 0;
    break;
  case 11: // rabanie palisady
    if (type != 1 && type != 4) {
      command = 0;
      commandN = 0;
      return;
    }
    target = 2;
    FindEnemy(x, y, &xe, &ye, &target);
    target = place[xe][ye];
    if (xe == 32 && ye == 32)
      target = 0;
    if (target == nr)
      target = 0;
    if (!target)
      LookAround();
    if (target)
      Attack();
    commandN = 11;
    target = 0;
    return;
  }
  inmove = 0;
  delay = maxdelay;
}
//////////////////////////////////////////////////////////////
int Mover1::Milk(void) {
  if (command != 4)
    return 0;
  if (exist != 1)
    return 0;
  if (placeG[x][y - 1] != 160)
    return 0;

  udder--;
  if (!udder) {
    xe = xp;
    ye = yp;
    commandN = 1;
    return 0;
  }
  return 1; // wymie
}
///////////////////////////////////////////////////////////
void Mover1::FindGrass() {
  int i, j, max, maxx, maxy, range = 1;

  max = 0;
  for (range = 1; range < 5; range++) {
    max = 0;
    if (x & 1) {
      for (i = x - range; i <= x + range; i++)
        for (j = y - range; j <= y + range; j++)
          if (i > 0 && j > 0 && i < MaxX - 1 && j < MaxY - 1)
            if (place[i][j] == 0 && placeG[i][j] > max && placeG[i][j] < 9) {
              max = placeG[i][j];
              maxx = i;
              maxy = j;
            }
    } else {
      for (i = x + range; i >= x - range; i--)
        for (j = y + range; j >= y - range; j--)
          if (i > 0 && j > 0 && i < MaxX - 1 && j < MaxY - 1)
            if (place[i][j] == 0 && placeG[i][j] > max && placeG[i][j] < 9) {
              max = placeG[i][j];
              maxx = i;
              maxy = j;
            }
    }

    if (max) {
      commandN = 1;
      SetEnd(maxx, maxy);
      return;
    }
  }
}
///////////////////////////////////////////////
//
//
///////////////////////////////////////////////
void FindSheed(int k, int x, int y, int *xm, int *ym) {
  int i;
  for (i = 0; i < 20; i++) {
    if (castle[k].b[i].type == 2 && castle[k].b[i].exist == 1) {
      *xm = castle[k].b[i].x;
      *ym = castle[k].b[i].y + 2;
    }
  }
  for (i = 0; i < 20; i++) {
    if (!place[castle[k].b[i].x][castle[k].b[i].y + 1] &&
        castle[k].b[i].type == 2 && castle[k].b[i].exist == 1) {
      *xm = castle[k].b[i].x;
      *ym = castle[k].b[i].y + 2;
    }
  }
}
/////////////////////////////////////////////////////////////
void Mover1::Graze() {
  int grass = placeG[x][y];
  inmove = 0;

  if (!grass || grass > 8) {
    FindGrass();
    delay = maxdelay;
    return;
  }
  inattack = 1;
  udder += grass;
  if (udder > 100)
    udder = 100;
  placeG[x][y]--;
  if (udder >= 100) {
    int k = 0;
    if (IFF == 2)
      k = 1;

    if (placeG[xm][ym - 1] != 160) {
      FindSheed(k, x, y, &xm, &ym);
    }
    xe = xm;
    ye = ym;
    commandN = 3;
  } // wracaj
  delay = maxdelay;
}
///////////////////////////////////////////////////////////////
void Mover1::Run2() // krowy
{                   // command 1-idz na pastwisko 2-harwestuj
  // 3-wracaj 4- oddaj mleko 5-uciekaj
  if (!exist)
    return;
  if (exist == 2 && delay < 1) {
    exist = 0;
    return;
  }
  if (exist == 2) {
    delay--;
    return;
  }

  if (attack[x][y]) {
    int dem;
    commandN = 5; // ucieczka
    dem = attack[x][y] - armour;
    attack[x][y] = 0;
    if (dem > 0)
      hp -= dem;
    if (dem > 4)
      ShowHit = 4;
    else
      ShowHit = 3;
    if (hp <= 0) {
      exist = 2;
      dx = 0;
      dy = 0;
      delay = 200;
      ispath = 0;
      Hide();
      inmove = 0;
      if (Msg.dzwiek < 8) {
        Msg.dzwiek = 8;
        Msg.X = x;
        Msg.Y = y;
      }
      return;
    }
  }

  if (delay) {
    return; // wyjdz jezeli nie koniec ruchu
  }

  inattack = 0;
  phase = 0;
  if (placeG[x][y] == 256 && hp < maxhp)
    hp++; // leczenie w swietym miejscu
  command = commandN;
  if (command == 1 && x == xe && y == ye) {
    if (x == xm && y == ym) {
      commandN = 4; // daj sie doic
      inmove = 0;
      return;
    } else {
      if (udder < 100) {
        commandN = 2;
        inmove = 0;
        return;
      } else {
        if (placeG[xm][ym - 1] != 160) {
          int k = 0;
          if (IFF == 2)
            k = 1;
          FindSheed(k, x, y, &xm, &ym);
        }
        xe = xm;
        ye = ym;
        commandN = 1;
        inmove = 0;
        return;
      }
    } // doszedles to graze
  }

  if (command == 123) {
    NewCow(nr);
    return;
  }
  if (command == 1) {
    Move();
    return;
  }
  if (command == 2) {
    Graze();
    return;
  } // jezeli nic nie robi to rozejzec sie
  if (command == 3 && x == xm && y == ym) {
    commandN = 4;
    inmove = 0;
    return;
  } // oddawaj
  if (command == 3) {
    Move();
    return;
  } // wracaj
  if (command == 4)
    return;
  if (command == 5) {
    // ucieczka do obory
    int k = 0;
    if (IFF == 2)
      k = 1;

    if (placeG[xm][ym - 1] != 160) {
      for (int i = 0; i < 20; i++) {
        if (castle[k].b[i].type == 2 && castle[k].b[i].exist == 1) {
          xm = castle[k].b[i].x;
          ym = castle[k].b[i].y + 2;
        }
      }
    }
    xe = xm;
    ye = ym;
    commandN = 1;
    if (x == xm && y == ym) {
      xe = xp;
      ye = yp;
    }
    return;
  } // uciekaj
  inmove = 0;
  commandN = 2;
  delay = maxdelay;
}

//---------------------------------------------------
void Mover1::SetEnd(int x0, int y0) {
  if (x0 == 0)
    x0 = 1;
  if (y0 == 0)
    y0 = 1;
  if (y0 > MaxY - 2)
    y0 = MaxY - 2;
  if (x0 > MaxX - 2)
    x0 = MaxX - 2;

  if (!type) {
    if ((placeG[x0][y0] == 163 && place[x0][y0 - 1] < 512) ||
        (place[x0][y0] < 512 && placeG[x0][y0] > 157 && placeG[x0][y0] < 166)) {
      for (int i = x0 - 3; i < x0 + 1; i++)
        for (int j = y0; j < y0 + 3; j++) {
          if (placeG[i][j] == 163) {
            xm = i;
            ym = j;
            ispath = 0;
            strcpy(Msg.msg, "Muuu!!!");
            Msg.licznik = 20;
            return;
          }
        }
      return;
    } else {
      xp = x0;
      yp = y0;
    }
  }
  xe = x0;
  ye = y0;
  if (commandN == 8 || command == 8) {
    xm = x0;
    ym = y0;
  }
  ispath = 0;
}
//----------------------------------
void Mover1::Prepare(int x0, int y0, int typ) {
  missile.Move();
  if (!dx && !dy)
    dy = 1;
  if (phase > 2 && !inattack)
    phase = 0;
  if (inmove) {
    phase = Phase[type][delay];
  } else {
    if (inattack) {
      if (delay > maxdelay - 2) {
        phase = 3;
      }
      if (delay == Phase[type][17]) {
        phase = 4;
      }
      if (delay == Phase[type][18]) {
        phase = 1;
        if (!type)
          phase = 0;
      }
    }
  }

  if (!command)
    inmove = 0;
  if ((delay == 3) && (!command || command == 5)) // obracanie
  {
    if (faza < 4) {
      faza++;
    } else {
      faza = 0;
      dx++;
      if (dx > 1) {
        dx = -1;
        dy--;
      }
      if (dy < -1)
        dy = 1;
    }
  }
  if (x < (x0 - 1) || y < (y0 - 1) || x > (x0 + 17) || y > (y0 + 14))
    visible = 0; // ze wzgledu na LookUpTable
  else
    visible = 1;
  if (visible) {
    if (inmove == 1) {
      if (dx > 0)
        xr = Xe[x - x0 + 1] + (int)shiftX[type][delay] - 16;
      if (dx < 0)
        xr = Xe[x - x0 + 1] - (int)shiftX[type][delay] - 16;
      if (!dx)
        xr = Xe[x - x0 + 1] - 16;
      if (dy > 0)
        yr = Ye[y - y0 + 1] + (int)shiftY[type][delay] - 14;
      if (dy < 0)
        yr = Ye[y - y0 + 1] - (int)shiftY[type][delay] - 14;
      if (!dy)
        yr = Ye[y - y0 + 1] - 14;
    } else {
      xr = Xe[x - x0 + 1] - 16;
      yr = Ye[y - y0 + 1] - 14;
    }
    if (inattack && phase > 2 &&
        (type == 1 || type == 5 || (type > 6 && type < 10))) {
      if (dy > 0)
        yr -= 7;
      if (dx > 0)
        xr -= 8;
    }
    if (xr > 0 && xr < 282 && yr > 0 && yr < 194) {
    } else
      visible = 0;
  }
  if (typ) {
    if (ShowHit)
      ShowHit--;
    if (delay)
      delay--;
  }
}
//------------------------------------------------------------------------------
void Mover1::ShowS() {
  int k;
  if (!exist || !visible || !placeN[x][y])
    return;

  if (exist == 2) {
    k = 2;
    if (delay < 200)
      k = 1;
    if (delay < 70)
      k = 0; // k-faza rozkladu typ-typ postaci
    if (k) {
      if (IFF == 1)
        PutImage13h(xr, yr, movers[5 - k][type][1][1], 1);
      else
        PutImageChange13h(xr, yr, movers[5 - k][type][1][1], 1, color1, color2);
    } else {
      k = 0;
      if (type == 8)
        k = 2;
      if (!type)
        k = 1;
      if (IFF == 1)
        PutImage13h(xr, yr, dead[k], 1);
      else
        PutImageChange13h(xr, yr, dead[k], 1, color1, color2);
    }
    return;
  }
  if (!dx && !dy)
    dy = 1;
  if (wybrany) {
    int a = 0, b = 0;
    if (inattack && phase > 2 &&
        (type == 1 || type == 5 || type == 7 || type == 8 || type == 9)) {
      if (dy > 0)
        b = 7;
      if (dx > 0)
        a = 8;
    }
    if (IFF == 1)
      PutImageChange13h(xr + a, yr + b, Mysz[5], 1, color1, ExpColor[exp >> 4]);
    else
      PutImageChange13h(xr + a, yr + b, Mysz[5], 1, color1,
                        ExpColor[(exp >> 4) + 15]);
    int color = LightGreen;
    if (hp < (maxhp >> 1))
      color = Yellow;
    if (hp < (maxhp >> 2))
      color = LightRed;
    Bar13h(xr + a + 1, yr + b - 2, xr + a + 15, yr + b, Black);
    int hh = (hp * 14 / maxhp);
    if (!hh)
      hh = 1;
    Bar13h(xr + a + 1, yr + b - 2, xr + a + 1 + hh, yr + b, color);
    if (magic && IFF == 1) {
      hh = (magic * 14 / dmagic[exp >> 4]);
      Bar13h(xr + a + 16, yr + b + 14 - hh, xr + a + 17, yr + b + 14,
             LightBlue);
    }
    if (!type && IFF == 1) {
      hh = ((udder * 14) / 100);
      if (udder < 4)
        hh = 0;

      if (hh)
        Bar13h(xr + a + 16, yr + b + 14 - hh, xr + a + 17, yr + b + 14, White);
    }
  }
  if (!dx && !dy)
    dy = 1;
  if (dx < 0) {
    if (movers[phase][type][0][1 - dy] != NULL) {
      if (IFF == 1)
        PutImageChangeInverse13h(xr, yr, movers[phase][type][0][1 - dy], 1,
                                 color1, color1);
      else
        PutImageChangeInverse13h(xr, yr, movers[phase][type][0][1 - dy], 1,
                                 color1, color2);
    }
    if (marmour)
      PutImage13h(xr, yr, picture[278 + (marmour & 3)], 1);
    if (placeG[x][y] == 256 && hp < maxhp)
      PutImageChange13h(xr, yr, picture[282 + (delay & 1)], 1, Red, LightBlue);
  } else {
    if (movers[phase][type][1 - dx][1 - dy] != NULL) {
      if (IFF == 1)
        PutImage13h(xr, yr, movers[phase][type][1 - dx][1 - dy], 1);
      else
        PutImageChange13h(xr, yr, movers[phase][type][1 - dx][1 - dy], 1,
                          color1, color2);
    }
    if (marmour)
      PutImage13h(xr, yr, picture[278 + (marmour & 3)], 1);
    if (placeG[x][y] == 256 && hp < maxhp)
      PutImageChange13h(xr, yr, picture[282 + (delay & 1)], 1, Red, LightBlue);
  }

  if (ShowHit > 2)
    PutImage13h(xr, yr, Hit[1], 1);
  else if (ShowHit > 0)
    PutImage13h(xr, yr, Hit[0], 1);
}
//-----------------------------------------------------------------------------
void Mover1::Show() {
  int i, j;
  place[x][y] = nr;
  if (IFF == 1) {
    for (i = -s_range; i <= s_range; i++) {
      if (x + i > 0 && x + i < MaxX)
        for (j = -s_range; j <= s_range; j++) {
          if (y + j > 0 && y + j < MaxY && !(placeN[x + i][y + j])) {
            if ((i != -s_range || j != -s_range) &&
                (i != -s_range || j != s_range) &&
                (i != s_range || j != -s_range) &&
                (i != s_range || j != s_range)) // bez rogow
            {
              placeN[x + i][y + j] |= 1;
            }
          }
        }
    }
  }
}

//------------------------------------------------------------------------------
void Mover1::Hide() { place[x][y] = 0; }
/////////////////////////////////////////////////////////////
///
///  Building
///
///////////////////////////////////////////////////////////////
void Building::Init(int x0, int y0, int t, int iff, int Nr) {
  int i, j, k = 127;
  exist = 4;
  if (t > 10) {
    t -= 10;
    Nr += 300;
  }
  type = t;
  switch (type) {
  case 1:
    maxfood = 0;
    maxhp = 400;
    break;
  case 6:
    maxfood = 1;
    maxhp = 400;
    break;
  case 2:
  case 4:
    maxfood = 3;
    maxhp = 350;
    break;
  case 3:
    maxfood = 6;
    maxhp = 350;
    break;
  case 5:
    maxfood = 4;
    maxhp = 350;
    break;
  }
  x = x0 - 2;
  y = y0 - 2;

  armour = 10;
  if (iff == 1)
    maxhp = maxhp >> 1;
  hp = 30;
  IFF = iff;
  if (Nr > 300) // od razu wybudowany
  {
    Nr -= 300;
    exist = 1;
    hp = maxhp;
    k = 137 + (type - 1) * 20;
  }
  nr = iff * 256 + Nr * 10;
  faza = 0;
  for (i = 0; i < 6; i++) {
    m[i].exist = 0;
    m[i].missile.exist = 0;
  }
  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++) {
      placeG[x + i][y + j] = k + i + j * 3;
    }
  if (IFF == 1) {
    for (j = -2; j < 5; j++)
      for (i = -2; i < 5; i++) {
        if (x + i > 0 && y + j > 0 && x + i < MaxX && y + j < MaxY)
          if (!placeN[x + i][y + j])
            placeN[x + i][y + j] = 1;
      }
  }
  place[x][y] = nr;
  place[x + 1][y] = nr;
  place[x + 2][y] = nr + 1;
  place[x + 2][y + 1] = nr + 1;
  place[x][y + 1] = nr + 2;
  place[x + 1][y + 1] = nr + 2;
  if (type != 6)
    place[x + 1][y + 2] = nr + 3;
  place[x + 2][y + 2] = nr + 3;
  place[x][y + 2] = 0; // miejsce gdzie rodza sie krowy
}

////////////////////////////////////////////////////////////////////////////

void Building::Run() {
  int i, j, dd = 0;
  if (!exist)
    return;
  // odbudowa
  if (hp < maxhp)
    for (i = x; i < x + 3; i++)
      for (j = y; j < y + 3; j++) {
        if (placeN[i][j] > 219) {
          dd += (placeN[i][j] - 219) * 2;
          if (IFF == 2)
            dd += 3;
          placeN[i][j] = 1;
        }
      }

  if (dd) {
    hp += dd;
    if (hp >= maxhp)
      hp = maxhp;
  }

  if (exist == 4 && hp > (maxhp / 2)) // polowa budowy
  {
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++) {
        placeG[x + i][y + j] = 147 + (type - 1) * 20 + i + j * 3;
      }
    exist = 3;
  }

  if (exist == 3 && hp >= maxhp) // koniec budowy
  {
    exist = 1;
    for (j = 0; j < 3; j++)
      for (i = 0; i < 3; i++) {
        placeG[x + i][y + j] = 137 + (type - 1) * 20 + i + j * 3;
      }
    place[x + 1][y + 1] = nr;
    if (Msg.dzwiek < 18) {
      Msg.dzwiek = 18;
      Msg.X = x;
      Msg.Y = y;
    }
  }

  food = 0;
  for (i = 0; i < 6; i++) {
    m[i].Run(); // uruchom podlegle postacie
    if (m[i].exist)
      food++;
  }

  if (exist == 2 && !food) {
    exist = 0;
    return;
  } // jezeli zburzony i brak postaci to zniknij
  if (exist != 1)
    food = maxfood;
  //--- sprawdzenie atakow
  dd = 0;
  if (exist == 1 || exist > 2) {
    attack[x + 1][y + 1] = 0;
    for (i = x; i < x + 3; i++)
      for (j = y; j < y + 3; j++)
        if ((i != x) || j < 2 + y) {
          dd += attack[i][j];
          attack[i][j] = 0;
        }

    if (dd > armour)
      dd -= armour;
    else
      dd = 0;
  }
  if (exist == 2)
    dd = 0; // zabitego nie rabac
  if (dd) {
    if (IFF == 1) {
      strcpy(Msg.msg, "Wr%g w wiosce !!!");
      Msg.licznik = 20;
    }
    if (IFF == 2) {
      for (int post = 4; post < MaxUnitsInCastle; post++) {
        if (castle[1].m[post].type && castle[1].m[post].type < 10)
          castle[1].m[post].drange = 7;
      }
      for (int build = 0; build < 20; build++) {
        for (post = 0; post < 6; post++) {
          if (castle[1].b[build].m[post].type)
            castle[1].b[build].m[post].drange = 7;
        }
      }
    }
    hp -= dd;
    if (hp <= 0) {
      hp = 0;
      j = 0;
      do {
        for (i = 0; i < 3; i++) {
          placeG[x + i][y + j] = 257 + i + j * 3; // ruina
          if (type != 6 || i != 1 || j != 2)
            if (i || j != 2)
              place[x + i][y + j] = 0;
          placeN[x + i][y + j] = 88;
        }
        j++;
      } while (j < 3);
      placeN[x + 2][y + 2] = 100;
      placeN[x][y] = 81;
      placeN[x][y + 1] = 95;
      placeN[x + 2][y + 2] = 83;
      placeN[x + 2][y] = 81;
      placeN[x][y + 2] = 79;
      exist = 2;
      if (Msg.dzwiek < 17) {
        Msg.dzwiek = 17;
        Msg.X = x;
        Msg.Y = y;
      }
    }
  }

  if (exist != 1)
    return;
  if (hp < maxhp)
    placeN[x + 1][y + 1] = 1;
  if (hp < maxhp / 2)
    placeN[x + 1][y + 1] = 74;
  if (hp < maxhp / 4)
    placeN[x + 1][y + 1] = 78;
  if (hp < maxhp / 8)
    placeN[x + 1][y + 1] = 100;
  faza++;
  if (faza > 4)
    faza = 0;
  // flaga
  if (exist == 1 && faza == 1)
    switch (type) {
    case 1:
      placeG[x + 1][y] = 137 + 9;
      break; //?????
    case 2:
      placeG[x + 2][y + 1] = 157 + 9;
      break; // obora
    case 3:
      placeG[x + 2][y] = 177 + 9;
      break; // koszary1
    case 4:
      placeG[x + 1][y + 1] = 197 + 9;
      break; // swiatynia
    case 5:
      placeG[x + 1][y + 1] = 217 + 9;
      break; // koszary1
    case 6:
      placeG[x + 2][y] = 237 + 9;
      break; // bohater
    }
  if (exist == 1 && faza == 3)
    switch (type) {
    case 1:
      placeG[x + 1][y] = 137 + 1;
      break; //?????
    case 2:
      placeG[x + 2][y + 1] = 157 + 5;
      break; // obora
    case 3:
      placeG[x + 2][y] = 177 + 2;
      break; // koszary1
    case 4:
      placeG[x + 1][y + 1] = 197 + 4;
      break; // swiatynia
    case 5:
      placeG[x + 1][y + 1] = 217 + 4;
      break; // koszry1
    case 6:
      placeG[x + 2][y] = 237 + 2;
      break; // bohater
    }
}

////////////////////////////////////////////////////////////////////////

void Building::Prepare(int X, int Y, int typ) {
  int i;
  for (i = 0; i < 6; i++)
    m[i].Prepare(X, Y, typ);
}

///////////////////////////////////////////////////////////////////////

void Building::ShowS(int Xs, int Ys, int what) {
  int i;
  if (what == 1) //////////// pokaz obiekty  martwe
  {
    for (i = 0; i < 6; i++) {
      if (m[i].exist == 2)
        m[i].ShowS();
    }
    return;
  }
  if (what == 2) //////////// pokaz obiekty  zywe
  {
    for (i = 0; i < 6; i++) {
      if (m[i].exist == 1)
        m[i].ShowS();
    }
    return;
  }

  if (what == 3) /////////// pokaz pociski
  {
    for (i = 0; i < 6; i++)
      m[i].missile.Show(Xs, Ys);
  }
}

////////////////////////////////////////////////////////////////////////

void Building::Rebuild() {
  // if(hp<maxhp)hp+=30;
  // if(hp>maxhp)hp=maxhp;
}

////////////////////////////////////////////////////////////////////////

int Building::Wynik(int r) {
  if (type == 1) {
    if (r > 2)
      return r + 1; // obora,koszary 1,swiatynia
    if (r == 1)
      return 2; // droga
    return 1;
  }
  if (type == 2 && r == 2) // krowa
    return 0;
  if (type == 2 && (r == 6 && level > PASTUCH_LEV))
    return 0; // pastuch
  if (type == 3) {
    if (r == 5)
      return 7; // koszary 2
    if (r == 2 || r == 3)
      return 0; // topor,luk
    return 1;   // palisada
  }
  if (type == 4) {
    if (r == 5 || r == 3 || r == 6)
      return 0; // druidka,druid,mag
  }
  if (type == 5) {
    if (r == 5)
      return 8; // dom wodza
    if (r == 4 || r == 3)
      return 0; // miecz,wlocznia
    if (r == 1)
      return 3; // palisada
    return 1;   // odbudowa
  }
  if (type == 6 && (r == 5 || r == 6))
    return 0;
  return 1;
}

///////////////////////////////////////////////////////////////////////
int Building::Milk() {
  int i;
  if (type != 2)
    return 0;
  i = m[0].Milk() + m[1].Milk() + m[2].Milk() + m[3].Milk() + m[4].Milk() +
      m[5].Milk();
  return i;
}
////////////////////////////////////////////////////////////////////////
int Building::NewMan(int Nr) {
  int i, j, p;
  if (exist != 1)
    return 0;
  j = 20;
  if (food >= maxfood)
    return 0;
  for (i = 5; i >= 0; i--)
    if (!m[i].exist)
      j = i; // znajdz wolny
  // if(place[x][y+2]!=0)return 0;  // nie mozna utorzyc goscia
  i = 20;
  if (type == 2 && Nr == 2) {
    i = 0;
    p = 450;
  } // krowa
  if (type == 2 && Nr == 6 && level > PASTUCH_LEV) {
    i = 10;
    p = 1250;
  } // pastuch
  if (type == 3 && Nr == 2) {
    i = 1;
    p = 450;
  } // topor
  if (type == 3 && Nr == 3) {
    i = 2;
    p = 650;
  } // luk
  if (type == 4 && Nr == 3) {
    i = 3;
    p = 650;
  } // druidka   //i=typ
  if (type == 4 && Nr == 5) {
    i = 4;
    p = 1050;
  } // kaplan
  if (type == 4 && Nr == 6 && level > MAG_LEV) {
    i = 11;
    p = 1250;
  } // mag
  if (type == 5 && Nr == 3) {
    i = 5;
    p = 650;
  } // miecz
  if (type == 5 && Nr == 4) {
    i = 6;
    p = 850;
  } // wlocznia
  if (type == 6 && Nr == 5) {
    i = 7;
    p = 1050;
  } // dowodca
  if (type == 6 && Nr == 6 && level > KUSZNIK_LEV) {
    i = 12;
    p = 1250;
  } // kusznik
  if (i == 20)
    return 0; // zla kolejnosc nie mozna zbudowac takiego goscia w tym budynku
  int xx = 0, yy = 0;
  if (place[x][y + 2]) {
    for (int i1 = x - 1; i1 < x + 4; i1++)
      for (int j1 = y - 1; j1 < y + 4; j1++)
        if (!place[i1][j1]) {
          xx = i1;
          yy = j1;
        }
  } else {
    xx = x;
    yy = y + 2;
  }
  if (!xx || !yy)
    return 0;
  m[j].Init(i, xx, yy, 0, 0);
  m[j].SetNr(nr + j + 4);
  m[j].SetIFF(IFF);

  if (!i) {
    m[j].xm = x;
    m[j].ym = y + 2;

  } // stajnia dla krowy
  if (IFF == 1) {
    m[j].SetCommand(1);
    m[j].SetEnd(x - 2, y + 4);
  } else {
    m[j].exp = diff * 30;
  }
  m[j].Show();
  food--;
  return p;
}

///////////////////////////////////////////////////////////////////////
//
// class Castle
//
/////////////////////////////////////////////////////////////////////////
void Castle::Init(int iff, int mlk) {
  int i, j;

  IFF = iff;
  milk = mlk;
  maxmilk = mlk;
  for (i = 0; i < MaxUnitsInCastle; i++)
    m[i].Disable();
  for (j = 0; j < 20; j++) {
    b[j].exist = 0;
    for (i = 0; i < 6; i++)
      b[j].m[i].Disable();
  }
}
/////////////////////////////////////////////////////////
void Castle::ShowS(int Xs, int Ys, int what) {
  int i;
  //////////// pokaz budynki i ich rycerzy
  for (i = 0; i < 20; i++)
    b[i].ShowS(Xs, Ys, what);

  if (what == 1) //////////// pokaz obiekty  martwe
  {
    for (i = 1; i < MaxUnitsInCastle; i++) {
      if (m[i].exist == 2)
        m[i].ShowS();
    }
  }
  if (what == 2) //////////// pokaz obiekty  zywe
  {
    for (i = 1; i < MaxUnitsInCastle; i++) {
      if (m[i].exist == 1)
        m[i].ShowS();
    }
  }

  if (what == 3) /////////// pokaz pociski
  {
    for (i = 1; i < MaxUnitsInCastle; i++)
      m[i].missile.Show(Xs, Ys);
  }
}
/////////////////////////////////////////////////////////
void Castle::Prepare(int X, int Y, int typ) {
  int i;
  // przygotuj budynki i ich rycerzy
  for (i = 0; i < 20; i++) {
    b[i].Prepare(X, Y, typ);
  }

  // przygotuj obiekty
  for (i = 1; i < MaxUnitsInCastle; i++) {
    m[i].Prepare(X, Y, typ);
  }
}
/////////////////////////////////////////////////////////
void Castle::GetCmd(struct Command *Cmd) {
  Cmd->nrm = command.nrm;
  Cmd->nrb = command.nrb;
  Cmd->command = command.command;
  Cmd->x = command.x;
  Cmd->y = command.y;
  Cmd->co = command.co;
}
///////////////////////////////////////////////////
void Castle::SetCmd(struct Command *Cmd) {
  command.nrb = Cmd->nrb;
  command.nrm = Cmd->nrm;
  command.command = Cmd->command;
  command.x = Cmd->x;
  command.y = Cmd->y;
  command.co = Cmd->co;
}
//////////////////////////////////////////////////////////////////////////
void Castle::Run() {
  int i;
  if (command.co == 2) // komendy ogolne
  {
    if (command.command == 2 && milk >= 45 &&
        !place[command.x][command.y]) // droga
    {
      placeG[command.x][command.y] = 35;
      Droga(command.x, command.y, 35);
      Droga(command.x - 1, command.y, 35);
      Droga(command.x + 1, command.y, 35);
      Droga(command.x, command.y - 1, 35);
      Droga(command.x, command.y + 1, 35);
      milk -= 45;
      if (Msg.dzwiek < 20) {
        Msg.dzwiek = 20;
        Msg.X = command.x;
        Msg.Y = command.y;
      }
    }
    if (command.command == 2 && milk >= 45 &&
        place[command.x][command.y] == 1) // most
    {
      place[command.x][command.y] = 2;
      milk -= 80;
      if (Msg.dzwiek < 20) {
        Msg.dzwiek = 20;
        Msg.X = command.x;
        Msg.Y = command.y;
      }
    }
    if (command.command == 3 && milk >= 50) // palisada
    {
      placeG[command.x][command.y] = 277;
      milk -= 60;
      Droga(command.x - 1, command.y, 35);
      Droga(command.x + 1, command.y, 35);
      Droga(command.x, command.y - 1, 35);
      Droga(command.x, command.y + 1, 35);
      if (Msg.dzwiek < 20) {
        Msg.dzwiek = 20;
        Msg.X = command.x;
        Msg.Y = command.y;
      }
    }
    if (command.command > 3) // budowa budynku
    {
      int w;
      w = 20;
      for (i = 0; i < 20; i++)
        if (!b[i].exist)
          w = i;
      i = command.command - 1;
      if (i > 5)
        i = 5;
      if (milk < i * 200 + 50)
        return;
      if (w < 20) {
        b[w].Init(command.x, command.y, command.command - 2, IFF, w);
        milk -= i * 200 + 50;
        if (Msg.dzwiek < 20) {
          Msg.dzwiek = 20;
          Msg.X = command.x;
          Msg.Y = command.y;
        }
        command.y -= 2;
        command.x -= 2;
        for (int ii = -1; ii <= 3; ii++)
          for (int jj = -1; jj <= 3; jj++)
            Droga(command.x + ii, command.y + jj, 35);
      }
    }
  }
  if (command.co == 0) // komendy dla budynkow (odbudowa)
  {
    if (command.command == 1 && milk >= 250) {
      if (b[command.nrb].hp < b[command.nrb].maxhp)
        milk -= 250;
      b[command.nrb].Rebuild();
    };
    if (command.command > 1) // budowa postaci
    {
      i = command.command;
      if (milk >= i * 200 + 50) {
        milk -= b[command.nrb].NewMan(i);
      }
    }
  }
  if (command.co == 1) // komendy dla modulow
  {
    class Mover1 *mm;
    if (command.nrb < 20)
      mm = &b[command.nrb].m[command.nrm];
    else
      mm = &m[command.nrm];

    mm->SetCommand(command.command);
    if (command.command == 1 || command.command == 7 || command.command == 8)
      mm->SetEnd(command.x, command.y);
    if (command.command == 2)
      mm->SetTarget(place[command.x][command.y]);
  }
  class Mover1 *mm;
  if (command.co == 3) // komendy dla modulow wszystkich
  {
    int post, bud;
    for (post = 1; post < MaxUnitsInCastle; post++) {
      mm = &m[post];
      if (mm->wybrany) {
        mm->SetCommand(command.command);
        if (command.command == 8 || command.command == 7 ||
            command.command == 1)
          mm->SetEnd(command.x, command.y);
        if (command.command == 2)
          mm->SetTarget(place[command.x][command.y]);
      }
    }
    for (bud = 0; bud < 20; bud++)
      for (post = 0; post < 6; post++) {
        mm = &b[bud].m[post];
        if (mm->wybrany) {
          mm->SetCommand(command.command);
          if (command.command == 8 || command.command == 7 ||
              command.command == 1)
            mm->SetEnd(command.x, command.y);
          if (command.command == 2)
            mm->SetTarget(place[command.x][command.y]);
        }
      }
  }

  // disable command
  command.co = 5;
  command.command = 0;
  // zbierz mleczko
  if (milk < maxmilk) {
    for (i = 0; i < 20; i++)
      milk += b[i].Milk();
    for (i = 1; i < MaxUnitsInCastle; i++)
      if (!m[i].type)
        milk += m[i].Milk();
  } else {
    milk = maxmilk;
  }

  // run units
  if (faza)
    faza = 0;
  else
    faza = 1;
  if (faza) // zolnierzy
  {
    for (i = 1; i < MaxUnitsInCastle / 2; i++)
      m[i].Run();
    for (i = 0; i < 10; i++)
      b[i].Run();
  } else { //  lub budynki
    for (i = MaxUnitsInCastle / 2; i < MaxUnitsInCastle; i++)
      m[i].Run();
    for (i = 10; i < 20; i++)
      b[i].Run();
  }
  PlaceUsed = 0;
}
/////////////////////////////////////////////////////////////////////
void Castle::DisableUnits() {
  class Mover1 *mm;
  int post, bud;
  for (post = 1; post < MaxUnitsInCastle; post++) {
    mm = &m[post];
    mm->wybrany = 0;
  }
  for (bud = 0; bud < 20; bud++)
    for (post = 0; post < 6; post++) {
      mm = &b[bud].m[post];
      mm->wybrany = 0;
    }
}
void Castle::FreeUnits(void) {
  int post, bud;
  for (post = 1; post < MaxUnitsInCastle; post++) {
    m[post].Disable();
  }
  for (bud = 0; bud < 20; bud++)
    for (post = 0; post < 6; post++) {
      b[bud].m[post].Disable();
    }
}
