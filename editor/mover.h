#if !defined(__MOVER_H)
#define __MOVER_H

#define mMaxX 66
#define mMaxY 66
#define MaxUnitsInCastle 40
#define MaxPictures 284

//////// definicje kolorow /////////////////
extern int ExperienceColor[10];
extern int Color1;

extern int LightRed;
extern int Red;
extern int DarkRed;

extern int LightYellow;
extern int Yellow;
extern int DarkYellow;

extern int LightGreen;
extern int Green;
extern int DarkGreen;

extern int LightBlue;
extern int Blue;
extern int DarkBlue;

extern int LightGray;
extern int Gray;
extern int DarkGray;

extern int LightBrown;
extern int Brown;
extern int DarkBrown;

#define Black 0
#define White 255
#define FontColor 181
#define MsgFontColor 255
///////////////////////////////////////////////////////////
struct MMessage {
  int licznik, count;
  int X, x;
  int Y, y;
  int dzwiek;
  char msg[40];
};
#endif

//================================================
struct memory {
  int dx, dy;
  struct memory *next;
};
//-----------------------------------------------
class Missile {
public:
  int x, y, xt, yt;
  int dx, dy;
  char type;
  char *view;
  char visible;
  char exist;
  char damage;
  void Init(int, int, int, int, int, int);
  void Move();
  void Show(int, int);
};
//----------------------------------------------
class Mover1 {
public:
  int xr, yr;
  char wybrany;
  int nr; // nr bity 0-7 nr bity 8-9 IFF bity 10-15 typ
  int hp, maxhp;
  int mainTarget;
  int visible;
  int command, commandN; // 0-nic 1-go 2-fight
  int type;              // 0-krowa 1-miecz 2-luk 3-mag
  class Missile missile;

  Mover1(void);
  ~Mover1(void);
  void Move(void);
  void Show(void);
  void Hide(void);
  void SetEnd(int, int);
  void SetStart(int, int);
  void Prepare(int, int, int);
  void ShowS(void);
  void Disable();
  void Repare(void);
  void SetNr(int);
  void SetIFF(int);
  void Init(int, int, int, int, int);
  void SetCommand(int);
  void SetTarget(int);
  void Run(void);
  int Milk(void);
  int OK(void) { return exist; };

  void FindGrass();
  void Labeling(void);
  void Run1(void);
  void Run2(void);
  int LookAround(void);
  int LokateTarget(void);
  void Graze(void); // pasienie sie
  void Attack(void);
  int Distance(void);

  int exist;

  int IFF; // zawsze 1 lub 2
  int exp;
  int x, y;
  int xe, ye;
  int dx, dy;
  int phase; // 0-faza ruch  1-faza ruch 2-atak
  int faza;
  int magic; // 100
  int xp, yp, xm, ym;
  int armour, marmour;
  int inmove;
  int inattack;
  int drange;
  int damage;
  int udder;
  int s_range;
  int a_range;
  int target;
  int ShowHit;
  int delay, maxdelay;
  int ispath;
  int path[6][2];
};
/////////////////////////////////////////////////
struct Command {
  int co;
  int command;
  int nrb;
  int nrm;
  int x;
  int y;
};
//////////////////////////////////////////////////////
//  class Building
///////////////////////////////////////////////////////

class Building {
public:
  int exist;
  int x, y;
  int faza;
  int type;
  int hp;
  int armour;
  int maxhp;
  int food, maxfood;
  int IFF;
  int nr;
  class Mover1 m[6];
  void Prepare(int, int, int);
  void ShowS(int, int, int);
  void Run();
  int Wynik(int);
  int NewMan(int);
  int Milk();
  void Rebuild();
  void Init(int, int, int, int, int);
};
//////////////////////////////////////////////////////
//  class Castle
///////////////////////////////////////////////////////
class Castle {
public:
  int IFF;
  int faza;
  int milk, maxmilk;
  class Mover1 m[MaxUnitsInCastle];
  class Building b[20];
  struct Command command;
  void Init(int, int);
  void GetCmd(struct Command *Cmd);
  void SetCmd(struct Command *Cmd);
  void Prepare(int, int, int);
  void ShowS(int, int, int);
  void Run(void);
  void DisableUnits(void);
  void FreeUnits(void);
};
