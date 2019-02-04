/////////////////////////////////////////////////////////////////////////
//
// Modul zawiera deklaracje i implementacje klas
// Mover1
// Missile
//////////////////////////////////////////////////////////////////////////
#include "image13h.h"
#include "mover.h"
#include <stdlib.h>
#include <string.h>
/////////////////////////////////////////////////////////////////////////
//---------Zmienne----------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

int Place[mMaxX][mMaxY];

//---------------shift-------------------------------------------------
char shiftX[10][17] = {
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 16, 15, 15, 15, 15},    // 12krowa
    {0, 1, 2, 3, 4, 6, 8, 10, 12, 14, 16, 25, 15, 15, 15, 15, 15},   // 10dzida
    {0, 2, 4, 6, 8, 10, 12, 14, 16, 15, 15, 15, 15, 15, 15, 15, 15}, // 8proca
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 15, 15},     // 14babcia
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15},       // 16kaplan
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 16, 15, 15, 15, 15, 15, 15},   // 10miecz
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 16, 15, 15, 15, 15, 15, 15},   // 10luk
    {0, 1, 3, 5, 6, 8, 9, 11, 12, 14, 15, 15, 15, 15, 15, 15, 15}, // 10bohater
                                                                   // ?
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 15}, // 16smok    ?
    {0, 3, 5, 7, 9, 12, 14, 15, 15, 8, 9, 10, 11, 12, 13, 14, 15}}; // 16 ?
char shiftY[10][17] = {
    {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14, 15, 15, 15, 15, 15},   // 12krowa
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15},  // 10dzida
    {0, 1, 3, 5, 7, 9, 11, 13, 14, 15, 15, 15, 15, 15, 15, 15, 15}, // 8proca
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15},     // 14babcia
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 14},      // 16kaplan
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15},  // 10miecz
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15},  // 10luk
    {0, 1, 2, 5, 6, 7, 8, 10, 11, 12, 14, 25, 15, 15, 15, 15, 15},  // 10bohater
                                                                    // ?
    {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14, 14}, // 16smok    ?
    {0, 3, 5, 7, 9, 12, 14, 14, 14, 8, 9, 10, 11, 12, 13, 14, 14}}; // 16 ?

char Phase[10][19] = {
    {0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 2, 2, 0, 6, 1, 1, 0, 8, 4},  // 12krowa
    {0, 0, 1, 1, 1, 0, 0, 2, 2, 2, 0, 6, 0, 0, 0, 0, 0, 6, 3},  // 10topor
    {0, 0, 2, 2, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 0, 0, 4, 2},  // 8luk
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 0, 6, 0, 11, 8}, // 14babcia
    {0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 13, 9}, // 16kaplan
    {0, 0, 2, 2, 2, 0, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 6, 3},  // 10miecz
    {0, 0, 2, 2, 2, 0, 0, 0, 1, 1, 1, 6, 0, 0, 0, 0, 0, 6, 3},  // 10dzida
    {0, 0, 2, 2, 2, 0, 0, 1, 1, 1, 1, 6, 0, 0, 1, 1, 0, 5, 2},  // 10dow
    {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 8, 2},  // 16niedzwiedz
    {0, 1, 1, 0, 2, 2, 0, 6, 0, 0, 0, 2, 2, 2, 2, 0, 0, 4, 2}   // 6strzyga
};

//-------Zmienne extern -----------------------------------------------
extern int PlaceUsed;
int diff;
int master, drzewa;
extern int color1, color2;
extern char xleczenie, yleczenie;

char attack[mMaxX][mMaxY];
extern int place[mMaxX][mMaxY];
extern char placeN[mMaxX][mMaxY];
extern int placeG[mMaxX][mMaxY];
extern int Xe[20], Ye[16]; // world
extern char *picture[MaxPictures], *missiles[4][3][3], *tlo, *Mysz[13];
extern char *movers[5][10][3][3]; // faza:typ:dx:dy
extern char *Hit[2], *dead[3];
extern char *shadow;
extern int MaxX;
extern int MaxY;
struct MMessage Msg;

extern void Droga(int, int, int);

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
      if (place[i][j] > 255 && place[i][j] < 500)
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
    break; // kamien
  case 3:
    exist += 3;
    view = missiles[1][dx + 1][dy + 1];
    break; // maly ogien
  case 4:
    exist += 3;
    view = missiles[2][dx + 1][dy + 1];
    break; // duzy ogien
  case 6:
    exist += 2;
    view = missiles[3][dx + 1][dy + 1];
    break; // strzala
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
  if (exist >= 112) {
    exist = 0;
    return;
  }
  if (exist > 99) {
    exist++;
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
    case 3:
      kk = 4;
      break;
    case 4:
      kk = 5;
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

  if (visible > 0) {
    x += dx;
    y += dy;
  } else {
    exist = 100;
    attack[xt][yt] += damage;
    if (type == 4 || type == 9)
      exist = 112;
    if (type == 4 && (placeN[xt][yt] < 50 || placeG[xt][yt] < 112) &&
        (placeG[xt][yt] < 74 || placeG[xt][yt] > 86)) {
      placeN[xt][yt] = 100;
    }
    if (type == 4 && damage > 15) {
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
        attack[xt][yt] += damage - 2;
      }
      xt += dx;
      yt += dy;
      if (placeG[xt][yt] < 74 || placeG[xt][yt] > 86) {
        placeN[xt][yt] = 75;
        attack[xt][yt] += damage - 4;
      }
    }
    if (type == 8 && Msg.dzwiek < 6) {
      Msg.dzwiek = 6;
      Msg.X = xt;
      Msg.Y = yt;
    }
    if (type == 9 && Msg.dzwiek < 7) {
      Msg.dzwiek = 7;
      Msg.X = xt;
      Msg.Y = yt;
    }
    if (type == 7 || type == 5 || type == 1) // sprawdzic czy na ekranie
    {
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
    dx = 0;
    dy = 0;
    view = NULL;
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
      if (dx > 0)
        PutImageChangeInverse13h(a, b, view, 1, 1, 1);
      else
        PutImage13h(a, b, view, 1);
    }
  }
}
/////////////////////////////////////////////////////////////////////
//
//            Mover1
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
  wybrany = 0;
  switch (t) {
  case 0:
    type = 0; // krowa
    s_range = 2;
    a_range = 0;
    damage = 0;
    armour = 0;
    hp = 30;
    maxdelay = 12;
    break;
  case 1:
    type = 1; // topor
    s_range = 3;
    a_range = 1;
    damage = 3;
    armour = 0;
    hp = 30;
    maxdelay = 10;
    break;
  case 2:
    type = 2; // luk
    s_range = 4;
    a_range = 3;
    damage = 2;
    armour = 0;
    hp = 25;
    maxdelay = 8;
    break;
  case 3:
    type = 3; // druidka
    s_range = 4;
    a_range = 3;
    damage = 6;
    armour = 0;
    hp = 18;
    maxdelay = 14;
    magic = 50;
    break;
  case 4:
    type = 4; // Kaplan
    s_range = 4;
    a_range = 4;
    damage = 16;
    armour = 0;
    hp = 25;
    maxdelay = 16;
    magic = 50;
    break;
  case 5:
    type = 5; // miecz
    s_range = 4;
    a_range = 1;
    damage = 5;
    armour = 1;
    hp = 30;
    maxdelay = 10;
    break;
  case 6:
    type = 6; // wlucznia
    s_range = 5;
    a_range = 4;
    damage = 4;
    armour = 1;
    hp = 25;
    maxdelay = 10;
    break;
  case 7:
    type = 7; // dowodca
    s_range = 4;
    a_range = 1;
    damage = 5;
    armour = 2;
    hp = 40;
    maxdelay = 10;
    break;
  case 8:
    type = 8; // niedzwiedz
    s_range = 4;
    a_range = 1;
    damage = 6;
    armour = 1;
    hp = 60;
    maxdelay = 16;
    break;
  case 9:
    type = 9; // strzyga
    s_range = 4;
    a_range = 1;
    damage = 4;
    armour = 1;
    hp = 35;
    maxdelay = 6;
    magic = 100;
    break;
  }

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
}
//---------------------------------

void Mover1::SetIFF(int Nr) {
  IFF = Nr;
  if (IFF == 2 && diff > 1) {
    hp += 5;
    maxhp += 5;
  }
  if (IFF == 2 && !diff) {
    hp -= 5;
    maxhp -= 5;
  }
}
//---------------------------------
void Mover1::SetCommand(int Nr) {
  if (type != 1 && Nr == 8)
    return;
  if (Nr == 8) {
    xm = xe;
    ym = ye;
  }
  if (Nr == 2)
    ispath = 0;
  if (Nr == 5)
    mainTarget = 0;
  commandN = Nr;
}
//----------------------------------
//----------------------------------
void Mover1::Labeling() {
  int i, j, mini, minj, min, startk, stopk;

  unsigned char ii, jj;
  unsigned char kolejka[2000];
  delay = maxdelay;
  if (PlaceUsed > 10 && type)
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
    if (!type) {
      xp = xe;
      yp = ye;
    }
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
  if ((x == xe) && (y == ye)) {
    commandN = 0;
    return;
  }
  place[x][y] = 0;
  if (!ispath)
    Labeling();
  place[x][y] = nr;
  if (!ispath) {
    return;
  }

  dx = path[ispath - 1][0];
  dy = path[ispath - 1][1];

  if (place[x - dx][y - dy] != 0) //||(IFF==1&&placeN[x-dx][y-dy]>60))
  {
    if ((type) && (x - xe < 2) && (x - xe > -2) && (y - ye < 2) &&
        (y - ye > -2)) {
      commandN = 0;
    }
    if (!type) {
      // if(Msg.dzwiek<8){Msg.dzwiek=8;Msg.X=x;Msg.Y=y;}
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
  int i, j, k = 1, d, iff;
  iff = IFF << 8;
  ispath = 0;
  target = 0;
  for (k = 1; k <= s_range + drange; k++) {
    drange = 0;
    for (i = x - k; i <= x + k && i < MaxX - 1; i++)
      if (i > 0) {
        if (y - k > 0) {
          d = place[i][y - k];
          if (d < 256 + 512 && d > 255 && (d & 0xff00) != iff) {
            target = d;
            return 1;
          }
        }
        if (y + k < MaxY - 1) {
          d = place[i][y + k];
          if (d < 256 + 512 && d > 255 && (d & 0xff00) != iff) {
            target = d;
            return 1;
          }
        }
      }
    for (j = y - k + 1; j <= y + k - 1 && j < MaxY - 1; j++)
      if (j > 0) {
        if (x - k > 0) {
          d = place[x - k][j];
          if (d < 256 + 512 && d > 255 && (d & 0xff00) != iff) {
            target = d;
            return 1;
          }
        }
        if (x + k < MaxX - 1) {
          d = place[x + k][j];
          if (d < 256 + 512 && d > 255 && (d & 0xff00) != iff) {
            target = d;
            return 1;
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
    if (command == 2)
      commandN = 0;
    else
      commandN = 5;
    delay = maxdelay;
    return;
  }

  if (Distance() <= a_range) {
    ispath = 0;
    if (missile.exist) {
      delay = maxdelay;
      return;
    }
    dx = x - xe;
    dy = y - ye;
    if (IFF == 2 && type == 3 && magic < 11) {
      SetEnd(x + dx, y + dy);
      Move();
      return;
    }
    if (IFF == 2 && type == 4 && magic < 21) {
      SetEnd(x + dx, y + dy);
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
    missile.Init(x, y, xe, ye, damage, type);
    if (type == 4 && target > 768) {
      mainTarget = 0;
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

    if (type == 7 && placeG[xe][ye] < 137 && magic < 100)
      magic++;
    if (!mainTarget) {
      target = 0;
      if (command == 2)
        commandN = 0;
      else
        commandN = 5;
    }
    return;
  }
  if (command == 2) {
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
    dem = attack[x][y] - armour - marmour;
    attack[x][y] = 0;
    if (dem > 0) {
      if (!mainTarget)
        if (command == 2)
          commandN = 0;
      if (command == 1 || command == 8)
        commandN = 0;        // zatrzymaj idacych!
      if (placeN[x][y] > 60) // uciekaj z ognia
      {
        for (int ii = -1; ii <= 1; ii++)
          for (int jj = -1; jj <= 1; jj++) {
            if (!place[x + ii][y + jj] && placeN[x + ii][y + jj] < 2) {
              SetEnd(x + ii, y + jj);
              SetCommand(1);
            }
          }
      }
      if (type == 3 && magic > 49 && IFF > 1)
        commandN = 9;
      hp -= dem;
      ShowHit = 5;
      drange = 4;
      if (hp <= 0) {
        exist = 2;
        dx = 0;
        dy = 0;
        ispath = 0;
        delay = 200;
        Hide();
        inmove = 0;
        int kk = 9;
        switch (type) {
        case 0:
          kk = 12;
          break;
        case 4:
          kk = 10;
          break;
        case 3:
          kk = 11;
          break;
        case 8:
          kk = 13;
          break;
        case 9:
          kk = 14;
          break;
        }
        if (Msg.dzwiek < kk) {
          Msg.dzwiek = kk;
          Msg.X = x;
          Msg.Y = y;
        }
        return;
      }
    }
  }

  if (marmour)
    marmour--;
  if (delay)
    return; // wyjdz jezeli nie koniec ruchu

  // zmiana charakterystyk
  if (type == 7) {
    if (magic > 30)
      damage = 7;
    if (magic > 55)
      armour = 3;
    if (magic > 70)
      damage = 8;
    if (magic > 99)
      armour = 4;
    if (magic > 100)
      magic = 100;
  }

  if (IFF == 2 && !command && placeG[x][y] == 163) {
    SetEnd(x + 2, y - 3);
    SetCommand(1);
  }
  if (hp < 6 && IFF == 2 && diff && command != 1) {
    SetEnd(xleczenie, yleczenie);
    SetCommand(1);
  }
  if (hp == maxhp && IFF == 2 && !command && x == xleczenie && y == yleczenie) {
    SetEnd(xleczenie + 5, yleczenie + 5);
    SetCommand(1);
  }
  inattack = 0;
  phase = 0;
  if (placeG[x][y] == 256 && hp < maxhp)
    hp++; // leczenie w swietym miejscu
  if ((type == 3 || type == 4 || type == 9) && magic < 100)
    magic++;
  if (type == 9 && magic > 9 && hp < 25) {
    hp++;
    magic -= 10;
  }
  if (command == 8 && commandN != 8) {
    placeN[xe][ye] = 1;
  }
  command = commandN;

  switch (command) {
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
    break;
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
    if (magic > 80) {
      inmove = 0;
      inattack = 0;
      if (Distance() > 6) {
        Move();
        return;
      }
      inattack = 1;
      dx = 0;
      dy = 0;
      missile.Init(xe + 1, ye - 4, xe, ye, damage - 3, type);
      magic -= 4;
      if (magic & 1)
        xe += 1;
      else
        xe -= 1;
      if (magic & 2)
        ye -= 1;
      if (magic & 4)
        ye += 1;
      delay = maxdelay;
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
        Msg.dzwiek = 16;
        Msg.X = x;
        Msg.Y = y;
      }
    }
    if (magic > 24 && type == 4) { // widzenie
      inattack = 1;
      magic -= 25;
      marmour = 5;
      if (Msg.dzwiek < 12) {
        Msg.dzwiek = 12;
        Msg.X = x;
        Msg.Y = y;
      }
      int xx, yy, i, j;
      for (i = 1; i < MaxX; i++)
        for (j = 1; j < MaxY; j++) {
          xx = abs(x - i);
          yy = abs(y - j);
          if (xx + yy < 14 && yy < 10 && xx < 10 && !placeN[i][j])
            placeN[i][j] = 1;
        }
    }
    break;
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
  udder--;
  if (!udder) {
    xe = xp;
    ye = yp;
    commandN = 1;
  }
  return 1; // wymie
}
///////////////////////////////////////////////////////////
void Mover1::FindGrass() {
  int i, j, max, maxx, maxy;
  max = 0;
  if (x & 1) {
    for (i = x - 1; i < x + 2; i++)
      for (j = y - 1; j < y + 2; j++)
        if (place[i][j] == 0 && placeG[i][j] > max && placeG[i][j] < 9) {
          max = placeG[i][j];
          maxx = i;
          maxy = j;
        }
  } else {
    for (i = x + 1; i > x - 2; i--)
      for (j = y + 1; j > y - 2; j--)
        if (place[i][j] == 0 && placeG[i][j] > max && placeG[i][j] < 9) {
          max = placeG[i][j];
          maxx = i;
          maxy = j;
        }
  }

  if (max) {
    commandN = 1;
    SetEnd(maxx, maxy);
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

  if (delay)
    return; // wyjdz jezeli nie koniec ruchu
  inattack = 0;
  phase = 0;
  if (placeG[x][y] == 256 && hp < maxhp)
    hp++; // leczenie w swietym miejscu

  command = commandN;
  if (command == 1 && x == xe && y == ye)
    if (x == xm && y == ym) {
      commandN = 6;
      inmove = 0;
      return;
    } else {
      commandN = 2;
      inmove = 0;
      return;
    } // doszedles to graze
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
  if (command == 5) {
    xe = xm;
    ye = ym;
    commandN = 1;
    return;
  } // uciekaj
  inmove = 0;
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
    if (placeG[x0][y0] == 163 && place[x0][y0 - 1] < 512) {
      xm = x0;
      ym = y0;
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

  if (phase > 2 && !inattack)
    phase = 0;
  if (inmove)
    phase = Phase[type][delay];
  else if (inattack) {
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
    if (!dx && !dy)
      dy = -1;
  }

  // if(command==5&&!inattack){dx=0;dy=0;}
  if (!command)
    inmove = 0;
  if ((delay == 3) && (!command || command == 5)) // obracanie
  {
    if (faza < 4)
      faza++;
    else {
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
    if (inattack && phase > 2 && (type == 1 || type == 5 || type > 6)) {
      if (dy > 0)
        yr -= 7;
      if (dx > 0)
        xr -= 8;
    }
    if (xr > 6 && xr < 252 && yr > 0 && yr < 192) {
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
    if (delay < 130)
      k = 1;
    if (delay < 70)
      k = 0; // k-faza rozkladu typ-typ postaci
    if (k) {
      if (IFF == 1)
        PutImage13h(xr, yr, movers[5 - k][type][1][1], 1);
      else
        PutImageChange13h(xr, yr, movers[5 - k][type][1][1], 1, color1, color2);
    } else {
      if (type && type < 8)
        k = 0;
      else
        k = 1;
      if (IFF == 1)
        PutImage13h(xr, yr, dead[k], 1);
      else
        PutImageChange13h(xr, yr, dead[k], 1, color1, color2);
    }
    return;
  }
  if (wybrany) {
    int a = 0, b = 0;
    if (inattack && phase > 2 && (type == 1 || type == 5 || type > 6)) {
      if (dy > 0)
        b = 7;
      if (dx > 0)
        a = 8;
    }

    if (IFF == 1)
      PutImageChange13h(xr + a, yr + b, Mysz[5], 1, color1, LightGray);
    else
      PutImageChange13h(xr + a, yr + b, Mysz[5], 1, color1, LightRed);
    int color = LightGreen;
    if (hp < (maxhp >> 1))
      color = Yellow;
    if (hp < (maxhp >> 2))
      color = LightRed;
    Bar13h(xr + a + 1, yr + b + 1, xr + a + 1 + (hp * 14 / maxhp), yr + b + 3,
           color);
  }

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
  int i, j, c = 2;
  place[x][y] = nr;
  if (IFF == master + 1) {
    if (type == 2)
      c = 4; // lucznik
    if (type == 6 || type == 3)
      c = 3; // kaplan,wlocznik
    for (i = -c; i <= c; i++) {
      if (x + i > 0 && x + i < MaxX)
        for (j = -c; j <= c; j++) {
          if (y + j > 0 && y + j < MaxY && !(placeN[x + i][y + j] & 1)) {
            if ((i != -c || j != -c) && (i != -c || j != c) &&
                (i != c || j != -c) && (i != c || j != c)) // bez rogow
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
    break;
  case 6:
    maxfood = 1;
    break;
  case 2:
  case 4:
    maxfood = 3;
    break;
  case 3:
    maxfood = 6;
    break;
  case 5:
    maxfood = 4;
    break;
  }
  x = x0 - 2;
  y = y0 - 2;
  maxhp = 200 - type * 10;
  if (iff == 2)
    maxhp += 100;
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
  if (IFF == 1)
    for (j = -2; j < 5; j++)
      for (i = -2; i < 5; i++) {
        if (x + i > 0 && y + j > 0 && x + i < MaxX && y + j < MaxY)
          if (!placeN[x + i][y + j])
            placeN[x + i][y + j] = 1;
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
  }
  if (exist == 2)
    dd = 0; // zabitego nie rabac
  if (dd) {
    if (IFF == 1) {
      strcpy(Msg.msg, "Wrog w wiosce !!!");
      Msg.licznik = 20;
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
  if (hp < 120)
    placeN[x + 1][y + 1] = placeN[x + 1][y + 1] = 74;
  if (hp < 70)
    placeN[x + 1][y + 1] = placeN[x + 1][y + 1] = 78;
  if (hp < 35)
    placeN[x + 1][y + 1] = placeN[x + 1][y + 1] = 100;
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
  if (hp < maxhp)
    hp += 30;
  if (hp > maxhp)
    hp = maxhp;
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
  if (type == 2 && r == 2)
    return 0;
  if (type == 3) {
    if (r == 5)
      return 7; // koszary 2
    if (r == 2 || r == 3)
      return 0; // topor,luk
    return 1;   // odbudowa
  }
  if (type == 4) {
    if (r == 5 || r == 3)
      return 0; // druidka,druid
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
  if (type == 6 && r == 5)
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
  for (i = 0; i < 20; i++)
    milk += b[i].Milk();
  for (i = 1; i < MaxUnitsInCastle; i++)
    if (!m[i].type)
      milk += m[i].Milk();
  if (milk > maxmilk)
    milk = maxmilk;

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
