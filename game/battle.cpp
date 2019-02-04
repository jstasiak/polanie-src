#include "shar.h"
//#define bez_zegara
// to wyciac jak tylko zalozymy obsluge zegara zwiekszajaca licznik
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include <malloc.h>
#include "cd.h"
#include "mover.h"
#include "mouse.h"
#include "image13h.h"
#include "menegdma.h"
#include "zabezset.h"

//#define POMOC

#define TRACK_MENU 2
#define TRACK_TXT 3
#define TRACK_VICTORY 5
#define TRACK_DEFEAT 4
//========Zmienne=================================

char FileName[4][12]={{"save.001"},{"save.002"},{"save.003"},{"save.004"}};
int place[MaxX][MaxY];
char placeN[MaxX][MaxY];
int attack[MaxX][MaxY];
extern int track;
char endL;
int drzewa0;
int musik=1;
int debug=0;
int kody=0;
int scrollTimer=0;
struct EditStr
{
    int mode;//0-tereny(skaly,woda,droga,sucha ziemia,palisada),
             //1-gadgety,2-drzewa, 3-postacie, 4-budynki, 5-rodzaje decyzji i konca
    int panel;
    int obj;
    int color;//0-niebieski 1-szary 2-zolty 3-zielony
    int xp,yp;//pastwisko wrogow
    int xz,yz;//zbiorka wrogow
    int xo,yo;//postac do odnalezienia
    int xk,yk;//krag
    int xprz,yprz;//miejsce przemiany
    int warunekKonca;//0-zabij wszystkich 1-uratuj 2-zabij jednego
    int typPlanszy;//0-wioska 1-bitwa dyn 2-stat 3-nic
    int postac;
    int generator;
    int maxmilk;
    int milk;
    int X,Y;
    char pName[20];
    char name[40];
}E={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"",""};

int JestSB;
int irq;
int port;
int kanal;

int PlaceUsed=0;
int diff=0;
int master=0,drzewa;
int color1,color2=Blue;
char xleczenie,yleczenie;
int chat=100;
int showAll=1;
char skroller=4,skroller1=0;
int speed=1;
int ile0=0,ile1=0;
char prowintion[25]={2,2,2,3,2,2,4,4,3,3,2,4,3,3,5,4,4,3,5,5,5,5,5,5,5};
char prowintionA[25];//11,14,16,19
char prowintionInit[25]={2,2,2,3,2,2,4,4,3,3,2,4,3,3,5,4,4,3,5,5,5,5,5,5,5};

int Track[52]={
    9,6,7,8,9,6,7,8,9,6,//powrot mirka
    7,8,9,6,7,8,9,6,7,8,
    9,6,7,8,9,

    6,11,13,10,8,//przyjaciele
    9,11,6,12,14,
    7,13,12,6,10,8,
    11,14,10,8,13,
    12,6,7,14,8,10};
//plemiona: 1-Red(YOU) 2-Green 3-Blue 4-Yellow 5-Gray
char zaznaczanie=0,grupa=0;
int rx1,rx2,ry1,ry2;
struct Command Cmd[2];
class Castle castle[2];
int mouseDisabled=0,mouseCommand=1;
int quitLevel=0;
char dzwiek=1,mowa=1;
char MapY=1;
int mouseP=0;
int mouseCounter,mouseMode=0;
int Map=0;
int positioN[40][2];
char decisionFaza=0;
char cel1[70],cel2[70],showcel=0;
struct Plansza{
            char decisionType;   //0-miasto 1-bitwa dyn 2-stat 3,4-nic
            char gen; //0-nic 1-generator
            char endType;// 0-zabij 1-uratuj 2-zabij jedn
            char typ; //typ uratowanego /nr domu do zostawienia 20-nic nie zostawiaj
            int xw,yw;//xwyjscia,ywwyjscia -do uratowania
            int x0,y0;//miejsce faceta do uwolnienia;
            int xp,yp,tp;//miejsce przemiany,typ przemiany;
            int gTimer;
            int maxmilk;
            char name[20];
            char haslo;
            char nrh;
            char nrp;  // nr planszy jezeli rowny 7 14 24 to haslo
            char ide;
            char next;
            }pl;
extern struct MMessage Msg;
int ScreenX=3,ScreenY=1;

//---dane dla modulu decyzyjnego---------------------
struct Mem{
      char index;//nr budynku
      char aindex;//nr budynku do ataku
      int faza;//0-inicjacja 1-budowa 2- wyslij
               //3-czekanie
               //4-atak
               //5-powrot
      int xz,yz,x,y,xe,ye;//miejsce zbiorki;
      int xp,yp;//pastwisko;
      int distance;//odleglosc przeciwnika od grodu
      int dmilk;//przyrost mleka na runde;
      int delay; //czas do nastepnego ataku
      int atak; //nr ataku
      class Castle *c;//wskaznik na zamek
      }mem;
int xpastw;
int ypastw;

//---------------------------------------------------
struct SSelected{
    int IFF;   //0,1  2-nic nie zaznaczone
        int nrb;   //nr budynku  0-19  20=zamek
        int nrm;   //nr zolnierza w budynku nr=place[x][y]-256*(IFF+1)-10*nrb-4;
        int co;   // 0-budynek 1-zolnierz
        }select,posT[10]={{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0}};
char posTT[10][10][2];
//------------------------------------
char  *guzik[3],*lancuch[2];

//=========== Zmienne extern ===========

extern int quit=0;
extern int level;
extern int licznik;
extern char  *picture[MaxPictures],*missiles[6][3][3],*tlo,*Mysz[13];
extern char  *movers[5][10][3][3],*ramka[4];   //faza:typ:dx:dy
extern char  *buttons[16];
extern char  *Hit[2],*dead[3];
extern char  *drewno[3];
extern char  *fire[14]; //world
extern char  *face[16];//0-9 twarze, 10-15 budynki
extern int placeG[MaxX][MaxY];
//////////////
int MVol;
/////////////
extern class MENEGERDMA SND;
//===========Obiekty=================================

class Mover1 *selectM;
class Building *selectB;


//===========Deklaracje funkcji=======================

void DispatchEvent(void);
void ShowSelected(void);
void RefreshScreen(void);
void InitBattle(int,int);
void SubMenu(void);
void Options(void);
void Decision(void);
void DecisionB(void);
void DecisionS(void);
char EndLevel(void);
int  LoadGame(void);
int  SaveGame(void);
void Scroll(void);
void Generator(void);
void ZaznaczObiekt(int,int ,int ,int ,int mode);

//===========Deklaracje funkcji zewnetrznych===========
//mover1
extern int Who(int);
extern void FindEnemy(int x,int y,int* xe,int* ye,int*distance);
extern Mover1* Pointer(int nr);
extern int Udata[13][7];
// world
extern int  InitPlace(int level);
extern void ShowPlace(int,int);
extern void Droga(int,int,int);
extern void Woda(int,int,int);
extern void Skala(int,int,int);
extern void ShowTrees(int,int);
extern void ShowBattleMap(int,int);
extern void ClearAttack(void);
// mapa
extern void NextConquest(void);
//s graphics
extern void InitPicture(void);
extern void FreeMemory(void);
extern void InitGraphics(void);
extern void ShowBackground(void);
extern void ShowSubMenu(void);
extern void ShowEndMenu(void);
extern void ShowPanel(int,int,int,int,int);
extern void ShowPicture(int,int);
extern void ShowText(int,int);
// main
extern void PressButton(int B,int P);
extern void MouseEngine(void);
int licznik2=0;
///////////////////////////////////////////////
//      Battle
///////////////////////////////////////////////
void Battle(int type)   // 1-single start   0-rs   2-loaded
{
int i,j;
#ifdef SHAREWARE
 for(i=0;i<52;i++)Track[i]=2+(i&1);
#endif
pl.nrh=0;
color1=Red;

do
{
licznik2=0;
if(type<2)
    {
    PlayTrack(TRACK_TXT);
    ShowText(level,0);
    } else DownPalette(2);

StopPlaying();
kody=0;
if(type<2)
{
    InitBattle(level,0);
}
else
{
    InitBattle(level,1);
    type=1;
}
chat=100;
if(type>1)type=1;
SetScreen(1);
ShowBackground();
ShowPanel(0,0,0,0,0);
if(Map)PressButton(16,2);else PressButton(16,3);
PutImage13h(275,139,buttons[7],0);

PlayTrack(Track[level-1]);
quitLevel=0;    //   screen=1
selectM=&castle[master].m[1];
select.co=1;
select.nrb=1;
select.nrm=1;
select.IFF=2;
mouseCommand=1;
////////////////////////////////////////////////
//  to wyciac
////////////////////////////////////////////////////
//for(i=0;i<MaxX;i++)
//for(j=0;j<MaxX;j++)placeN[i][j]=1;
////////////dotad///////
castle[0].Run();
castle[1].Run();
castle[0].Prepare(ScreenX,ScreenY,1);
castle[1].Prepare(ScreenX,ScreenY,1);
ShowSelected();
LoadExtendedPalette(3);
RisePalette(1);
/////
mem.faza=0;
mouseMode=0;
Msg.ddzwiek=0;
do
{


    do
    {
        decisionFaza++;
        if(decisionFaza==6)
        {
            switch(pl.decisionType)
            {
                case 0:Decision();break;
                case 1:DecisionB();break;
                case 2:DecisionS();break;
            }
            if(pl.gen)Generator();
            decisionFaza=0;
            CheckCD();
        }
        if(grupa==1&&Cmd[0].co==1)Cmd[0].co=3;
        castle[0].SetCmd(&Cmd[0]);
        castle[1].SetCmd(&Cmd[1]);

        castle[0].Run();
        castle[1].Run();
        castle[0].Prepare(ScreenX,ScreenY,1);
        castle[1].Prepare(ScreenX,ScreenY,1);

        //clear commmand
        Cmd[1].co=5;
        Cmd[0].co=5;
        ClearAttack();
        if(pl.endType==1&&placeN[pl.x0][pl.y0]&&pl.x0)
        {
            castle[0].m[1].Init(pl.typ,pl.x0,pl.y0,0,0);
            castle[0].m[1].SetIFF(1);
            castle[0].m[1].SetNr(1+456);
            castle[0].m[1].Show();
            pl.x0=0;placeN[pl.x0][pl.y0]=1;
        }
        if(placeG[pl.xp][pl.yp]==300&&place[pl.xp][pl.yp]>256&&place[pl.xp][pl.yp]<768)//przemiana
        {
            Mover1* mm=Pointer(place[pl.xp][pl.yp]);
            if(mm!=NULL)
            {
                mm->type=8;
                mm->s_range=Udata[8][0];
                mm->a_range=Udata[8][1];
                mm->damage=Udata[8][2];
                mm->armour=Udata[8][3];
                mm->maxhp=Udata[8][4];
                mm->maxdelay=Udata[8][5];
                mm->magic=0;
                if(mm->hp>mm->maxhp)mm->hp=mm->maxhp;
            }
        }
        if(mouseP>3)mouseP=0;else mouseP++;
        if(Msg.count==6)Msg.count=0;
        if(Msg.count)Msg.count--;

        if(mouseCounter>0)mouseCounter--;
        if(mouseCounter==1||(!mouseCounter&&mouseCommand==1))       // wypisz panel
        if(select.IFF<1)
        {
            int comm=0;
            if(select.co==1)
            {
                int comm=0;
                if(selectM->type&&selectM->command==5)comm=1;
                if(selectM->type==1&&selectM->command==8)comm=3;
                if((selectM->type==3||selectM->type==4)&&selectM->command==7)comm=3;
                ShowPanel(select.IFF,selectM->type,1,selectM->magic,comm);
            }
            if(select.co==0)
            {
                if(selectB->exist==1)comm=castle[0].milk;
                ShowPanel(select.IFF,selectB->type+19,comm,selectB->maxfood-selectB->food,0);
            }
        }
        else
        {
            ShowPanel(0,0,0,0,0);
        }


      ////////////////////////////////////////////////////////////////
        if(Msg.dzwiek)
        {
            if(Msg.dzwiek<0)Msg.dzwiek=0;
            if(!SND.jest_odtwarzany)//1996.06.29
            {
                if(Msg.X<ScreenX+22&&Msg.Y<ScreenY+18&&Msg.X>ScreenX-5&&Msg.Y>ScreenY-4)
                if(dzwiek&&Msg.dzwiek<26)SND(Msg.dzwiek-1);
                if(mowa&&Msg.dzwiek>25)SND(Msg.dzwiek-1);
                Msg.dzwiek=0;
                Msg.ddzwiek++;
                if(Msg.ddzwiek>2)Msg.ddzwiek=0;
            }

        }
      ////////////////////////////////////////////////////////////////

      //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        do
        {
            if(Map&&MapY<64)MapY++;
            if(Map&&MapY<64)MapY++;
            if(!Map&&MapY)MapY--;
            if(!Map&&MapY)MapY--;
            if(!zaznaczanie)Scroll();
            ShowSelected();
            i=mouse.GetMsg13h();
            ile0=mouse.Ile(0);
            ile1=mouse.Ile(1);
            if(i||ile0||ile1)DispatchEvent();
            if(zaznaczanie&&!mouse.Button)
            {
                zaznaczanie=0;
                ZaznaczObiekt(rx1,ry1,rx2,ry2,0);rx2=rx1;ry2=ry1;
            }
            #ifdef bez_zegara
            licznik++;
            #endif

        }while(licznik-licznik2<speed);
        showAll=1;
      //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        if(licznik>10000)
        {
            licznik=0;
            scrollTimer=licznik;
        }
        licznik2=licznik;
        
        if(pl.ide)pl.ide--;
        if(Msg.licznik)Msg.licznik--;
        if(decisionFaza==2&&!quitLevel)endL=EndLevel();
    }while(!quitLevel&&!endL);

}while(!quitLevel&&!endL);
castle[0].FreeUnits();
castle[1].FreeUnits();
SetScreen(0);
if(endL==1)
{
    PlayTrack(TRACK_VICTORY);
    ShowText(level,1);
    if(level<26)
    {
        #ifdef SHAREWARE
        quitLevel=1;//koniec scenariusza
        #else
        prowintion[level-1]=1;
        prowintionA[level-1]=0;
        if(pl.next)prowintionA[pl.next-1]=1;
        j=0;
        for(i=0;i<25;i++)j+=prowintion[i];
        if(j==25)quitLevel=2;
        else NextConquest();
        #endif
    }
    else
    {
      #ifdef SHAREWARE
      quitLevel=1;//koniec scenariusza
      #else
      if(level==30||level==35||level==41||level==46||level==52)
      quitLevel=1;//koniec scenariusza
      else level++;
      #endif

    }
}

if(endL==2)
{
    PlayTrack(TRACK_DEFEAT);
    ShowText(level,2);
    if(level==15||level>25)
    {}
    else NextConquest();
}
if(quitLevel==2)
{
          PlayTrack(TRACK_VICTORY);
          ShowText(2,3);
          ShowText(3,3);
          ShowText(4,3);
          ShowText(5,3);
          quitLevel=1;
}  //zakonczenie gry
}while(!quitLevel);
PlayTrack(TRACK_MENU);
};
//============koniec procedury Battle===================


void ShowSelected()
{
int i,j;
//---------------------
if(showAll)
{
    showAll=0;
    SetClippingArea13h(11,8,267,190);//262,192
    ShowPlace(ScreenX,ScreenY);
    castle[0].ShowS(ScreenX,ScreenY,1);
    castle[1].ShowS(ScreenX,ScreenY,1);
    castle[1].ShowS(ScreenX,ScreenY,2);
    castle[0].ShowS(ScreenX,ScreenY,2);
    int xee=(ScreenX<<4)-11;
    int yee=ScreenY*14-8;
    castle[0].ShowS(xee,yee,3);
    castle[1].ShowS(xee,yee,3);
    ShowTrees(ScreenX,ScreenY);
    // zaznacz
    if(select.IFF<2)
    {
        int color22=LightGray;
        if(select.IFF)color22=LightRed;
        if(select.co) //zolnierz
        {
            if(selectM->exist!=1)     //jezeli nie zyje
            {
                SetClippingArea13h(0,0,319,199);
                int p=0;
                int b=0;
                if(selectM->IFF==1)
                {
                    b=20;
                    int kkk=0;
                    for(int i=1;i<MaxUnitsInCastle;i++)
                    {
                        if(castle[0].m[i].wybrany&&castle[0].m[i].exist==1){p=i;kkk++;}
                    }
                    if(p)
                    {
                        selectM=&castle[0].m[p];
                        if(kkk==1)grupa=0;  //skasowac grupe
                    }
                    else
                    {//szukaj w budynkach
                        b=20;
                        b=4;
                        int kkk=0;
                        for(int j=0;j<20;j++)
                        for(i=0;i<20;i++)
                        if(castle[0].b[j].m[i].wybrany&&castle[0].b[j].m[i].exist==1)
                        {
                            p=i;
                            b=j;
                            kkk++;
                        }
                        if(b<20)
                        {
                            selectM=&castle[0].b[j].m[i];
                            if(kkk==1)grupa=0;
                        }
                        else{b=0;p=0;}
                    }
                }
                if(!p&&!b)
                {
                    select.IFF=2;
                    ShowPanel(0,0,0,0,0);
                    mouseMode=0; //  to odznacz
                }
            }
        }
        else   // budynek
        {
            if(selectB->exist==1||selectB->exist>2)
            {
                int xr,yr;// zaznaczenie budynku
                xr=(selectB->x-ScreenX)*16+11-3;
                yr=(selectB->y-ScreenY)*14+8;
                int color=LightGreen;
                if(selectB->hp<(selectB->maxhp>>1))color=Yellow;
                if(selectB->hp<(selectB->maxhp>>2))color=LightRed;

                Bar13h(xr+1,yr-1,xr+1+(selectB->hp*52/selectB->maxhp),yr+1,color);
                if(selectB->type>1&&!select.IFF)//wypisz ilosc jedzenia w naszej chacie
                {
                    color=(selectB->maxfood-selectB->food);
                    for(i=0;i<selectB->maxfood;i++)
                    {
                        if(i<color)Bar13h(xr+3+i*5,yr+41,xr+6+i*5,yr+43,DarkYellow);
                        else Bar13h(xr+3+i*5,yr+41,xr+6+i*5,yr+43,DarkBrown);
                    }
                }
                PutImageChange13h(xr+1,yr-3,buttons[1],1,color1,color22);
                PutImageChange13h(xr+53-16,yr-3,buttons[2],1,color1,color22);
                PutImageChange13h(xr+1,yr+45-14,buttons[3],1,color1,color22);
                PutImageChange13h(xr+53-16,yr+45-14,buttons[4],1,color1,color22);
            }
            else
            {
                select.IFF=2;
                SetClippingArea13h(0,0,319,199);
                ShowPanel(0,0,0,0,0);
                mouseMode=0; // jezeli nie to odznacz
            }
        }
    }
    SetClippingArea13h(0,0,319,199);

    ///////////// wypisz mleko /////////////////////////////////
    PutImage13h(299,9,drewno[2],0);//???
    i=castle[master].milk;
    if(i>1410)i=1410;
    i=i/10;
    if(i)Bar13h(299,150-i,314,150,255);
    i=castle[master].maxmilk;
    if(i>1260)i=0;
    i=i/10;
    if(i)Bar13h(299,150-i,314,151-i,LightRed);

    ////////////////mapa//////////////////////////////////
    if(mouseCommand<2)
    {
        ShowBattleMap(ScreenX,ScreenY);
    }
    ///////////wypisz komunikat///////////////////////////
    if(Msg.licznik&&!mowa)
    {
        OutText13h(29,171,Msg.msg,1);
        OutText13h(30,170,Msg.msg,MsgFontColor);
    }
    ////////////////////////////////////////////////////////////////
    if(mouseCommand>1)
    {
        switch(mouseCommand)
        {
            case 2:OutText13h(80,23,"Budowa drogi",White);break;
            case 3:OutText13h(80,23,"Budowa palisady",White);break;
            case 4:OutText13h(80,23,"Budowa obory",White);break;
            case 5:OutText13h(80,23,"Budowa chaty drwali i my*liwych",White);break;
            case 6:OutText13h(80,23,"Budowa chaty kap`an%w",White);break;
            case 7:OutText13h(80,23,"Budowa chaty woj%w",White);break;
            case 8:OutText13h(80,23,"Budowa chaty rycerza",White);break;
          /*  case 2:OutText13h(49,24,"Weg bauen",1);OutText13h(50,23,"Weg bauen",White);break;
            case 3:OutText13h(49,24,"Zaun bauen",1);OutText13h(50,23,"Zaun bauen",White);break;
            case 4:OutText13h(49,24,"Scheune bauen",1);OutText13h(50,23,"Scheune bauen",White);break;
            case 5:OutText13h(49,24,"Haus der J[ger bauen",1);OutText13h(50,23,"Haus der J[ger bauen",White);break;
            case 6:OutText13h(49,24,"Haus der Priester bauen",1);OutText13h(50,23,"Haus der Priester bauen",White);break;
            case 7:OutText13h(49,24,"Haus der K[mpfer bauen",1);OutText13h(50,23,"Haus der K[mpfer bauen",White);break;
            case 8:OutText13h(49,24,"Haus der Ritter bauen",1);OutText13h(50,23,"Haus der Ritter bauen",White);break;
          */
        }
    }
    if(!select.co&&!select.IFF&&!mowa)
    {
        switch(selectB->type)
        {
            case 1:OutText13h(81,12,"Budynek g`%wny",1);OutText13h(80,12,"Budynek g`%wny",LightGray);break;
            case 2:OutText13h(81,12,"  Obora",1);OutText13h(80,12,"  Obora",LightGray);break;
            case 3:OutText13h(81,12,"Chata drwali i my*liwych",1);OutText13h(80,12,"Chata drwali i my*liwych",LightGray);break;
            case 4:OutText13h(81,12,"Chata kap`an%w",1);OutText13h(80,12,"Chata kap`an%w",LightGray);break;
            case 5:OutText13h(81,12,"Chata woj%w",1);OutText13h(80,12,"Chata woj%w",LightGray);break;
            case 6:OutText13h(81,12,"Chata rycerza",1);OutText13h(80,12,"Chata rycerza",LightGray);break;

           /* case 1:OutText13h(49,13,"Hauptgeb[ude",1);OutText13h(50,12,"Hauptgeb[ude",FontColor);break;
            case 2:OutText13h(49,13," Scheune",1);OutText13h(50,12," Scheune",FontColor);break;
            case 3:OutText13h(49,13,"Haus der J[ger",1);OutText13h(50,12,"Haus der J[ger",FontColor);break;
            case 4:OutText13h(49,13,"Haus der Priester",1);OutText13h(50,12,"Haus der Priester",FontColor);break;
            case 5:OutText13h(49,13,"Haus der K[mpfer",1);OutText13h(50,12,"Haus der K[mpfer",FontColor);break;
            case 6:OutText13h(49,13,"Haus der Ritter",1);OutText13h(50,12,"Haus der Ritter",FontColor);break;
          */
        }
    }
    if(select.co==1)
    {
        if(pl.endType==1&&!select.IFF&&select.nrm==1&&select.nrb==20){OutText13h(100,30,pl.name,1);OutText13h(101,30,pl.name,FontColor);}
        if(pl.endType==4&&select.IFF==1&&select.nrm==1&&select.nrb==20){OutText13h(100,30,pl.name,1);OutText13h(101,30,pl.name,LightRed);}
    }
    if(showcel)
    {
        OutText13h(19,76,cel1,1);
        OutText13h(20,75,cel1,FontColor);
        OutText13h(19,86,cel2,1);
        OutText13h(20,85,cel2,FontColor);
    }
}
/////////////// pokazanie myszki  ////////////////////////////
int M;
int x,y;

x=(int)((mouse.X-11)>>4)+ScreenX;
y=(int)(mouse.Y-8)/14+ScreenY;
M=1;
if(select.IFF<2) //jezeli zaznaczony
{
    if(!select.IFF&&select.co) //nasz zolnierz
    {
        M=12;
        if(!place[x][y]){M=2;} // jezeli wolne to idz

        if(selectM->type&&place[x][y]>255){M=5;}  //zaznacz??????
        if(selectM->type&&place[x][y]>511){M=3;}  //atakuj
        if(selectM->type==10&&M==3)
        {
            Mover1* mm=Pointer(place[x][y]);
            if(mm==NULL||(mm->type!=0&&mm->type!=8))//krowa i niedzwiedz
            {
                M=12;
            }  //nie atakuj nikogo oprocz krowy
        }
        if((selectM->type!=4&&selectM->type!=1)&&place[x][y]>768){M=12;}  //nie drzewo
        if((selectM->type==1)&&place[x][y]>768&&placeG[x][y]<119&&placeG[x][y]>112){M=12;}  //nie drzewo

        if(!selectM->type&&place[x][y]<512&&placeG[x][y]>157&&placeG[x][y]<166)M=2;//krowa idz do naszej obory
        if(!placeN[x][y])M=2; //idz
        if(!mouseMode)M=1;//strzalka
        if(!mouseMode&&place[x][y]>255&&place[x][y]<768&&placeN[x][y]){M=5;}  //nasz
        if((selectM->type==1)&&(select.co==1)&&(place[x][y]==2||placeG[x][y]==277||(place[x][y]>255&&place[x][y]<511&&placeG[x][y]>126&&placeG[x][y]<256)))M=13;//robota
        if((selectM->type==3)&&mouseCommand==13)M=16;
        if((selectM->type==4)&&mouseCommand==13)M=17;

    }
    else   //nie nasz
    {
        if(place[x][y]>255&&place[x][y]<768)M=5;
    }
}
//jezeli nikt nie zaznaczony

if(select.IFF>=2)if(place[x][y]>255&&place[x][y]<768)M=5;//zaznacz


int k=mouse.Button;
mouse.Button=1;
if(mouse.MWindow(11,8,76,74)&&Map&&mouseCommand<2) // na mapie
{
    x=mouse.X-12;
    y=mouse.Y-9;
    M=15;
    if(mouseMode)M=4;
}
mouse.Button=k;
if(mouseCommand>1&&mouseCommand<8&&!select.co)M=1;
if(mouse.X>261)M=1;//panel
if(mouse.Y>192){mouse.Y=192;M=9;if(ScreenY==MaxY-14)M=12;}
if(mouse.Y>192-7&&M==1){mouse.Y=192;M=9;if(ScreenY==MaxY-14)M=12;}
if(mouse.Y<8){mouse.Y=7;M=8;if(ScreenY==1)M=12;}
if(mouse.X<11){mouse.X=8;M=10;if(ScreenX==1)M=12;}
if(mouse.X>306){mouse.X=311;M=11;if(ScreenX==MaxX-17)M=12;}

if(zaznaczanie)
{
    if(mouse.X>261)mouse.X=261;
    if(mouse.Y>192-7)mouse.Y=192-7;
    if(mouse.Y<10)mouse.Y=10;
    if(mouse.X<12)mouse.X=12;
    Rectangle13h(rx1,ry1,rx2,ry2,LightGray);
    M=1;
}

if(mouseCommand>1&&mouseCommand<10&&!select.co)//budynek
{
    int i,j,siz=3,kol=255,dr=0;
    if(mouseCommand<4)siz=1;
                    /////  tu sprawdzic
    showAll=1;
    for(i=-1;i<=siz;i++)
        for(j=0;j<siz;j++)
        {
            if(placeG[x+i][y+j]>30&&placeG[x+i][y+j]<54)dr=1;
        }
    for(i=0;i<siz;i++)
        for(j=-1;j<=siz;j++)
        {
            if(placeG[x+i][y+j]>30&&placeG[x+i][y+j]<54)dr=1;
        }
    if(y>62||x>62)dr=0;    
    int c=0;
    if(mouseCommand==3){dr=1;}  //palisada
    if(mouseCommand==2){c=1;}   //droga

    if(dr==1)
    {
        for(i=0;i<siz;i++)
            for(j=0;j<siz;j++)
            {
                if(place[x+i][y+j]>c)kol=LightRed;
                if(!placeN[x+i][y+j])kol=LightRed;
                if((placeG[x+i][y+j]>45&&placeG[x+i][y+j]<54)||(placeG[x+i][y+j]>126&&placeG[x+i][y+j]<257))kol=LightRed;
            }
    }
    else
    {
        kol=LightRed;
    }
    siz--;
    if(x>=ScreenX&&x<ScreenX+16&&y>=ScreenY&&y<ScreenY+13)
    {
        PutImageChange13h(11+(x-ScreenX)*16,8+(y-ScreenY)*14,buttons[1],1,color1,kol);
    }
    if(x+siz>=ScreenX&&x+siz<ScreenX+16&&y>=ScreenY&&y<ScreenY+13)
    {
        PutImageChange13h(11+(x+siz-ScreenX)*16,8+(y-ScreenY)*14,buttons[2],1,color1,kol);
    }
    if(x>=ScreenX&&x<ScreenX+16&&y+siz>=ScreenY&&y+siz<ScreenY+13)
    {
        PutImageChange13h(11+(x-ScreenX)*16,8+(y+siz-ScreenY)*14,buttons[3],1,color1,kol);
    }
    if(x+siz>=ScreenX&&x+siz<ScreenX+16&&y+siz>=ScreenY&&y+siz<ScreenY+13)
    {
        PutImageChange13h(11+(x+siz-ScreenX)*16,8+(y+siz-ScreenY)*14,buttons[4],1,color1,kol);
    }
}
int colorM=White;    //LightGreen;
if(mouseP>1)
{
    colorM=LightGray;
    if(M==2)M=7;
    if(M==3)M=6;
}
int a=8,b=7;
if(M==15)
{
    GetImage13h(mouse.X-8,mouse.Y-7,mouse.X+8,mouse.Y+7,Mysz[0]);
    PutImageChange13h(mouse.X-8,mouse.Y-7,buttons[5],1,Color1,colorM);
}
else
{
    if(M==13)
    {
        GetImage13h(mouse.X-8,mouse.Y-7,mouse.X+8,mouse.Y+7,Mysz[0]);
        PutImage13h(mouse.X-8,mouse.Y-7,buttons[8],1);
    }
    else
    {
       if(M==16)
       {
            GetImage13h(mouse.X-8,mouse.Y-7,mouse.X+8,mouse.Y+7,Mysz[0]);
            PutImage13h(mouse.X-8,mouse.Y-6,missiles[1][1][2],1);
            PutImage13h(mouse.X-5,mouse.Y-7,missiles[1][1][2],1);
            PutImage13h(mouse.X-11,mouse.Y-7,missiles[1][1][2],1);
        }
        else
        {
            if(M==17)
            {
                GetImage13h(mouse.X-8,mouse.Y-7,mouse.X+8,mouse.Y+7,Mysz[0]);
                PutImage13h(mouse.X-8,mouse.Y-6,missiles[2][1][2],1);
                PutImage13h(mouse.X-5,mouse.Y-7,missiles[2][1][2],1);
                PutImage13h(mouse.X-11,mouse.Y-7,missiles[2][1][2],1);
            }
            else
            {
                if(M==1){a=2;b=2;}
                GetImage13h(mouse.X-a,mouse.Y-b,mouse.X-a+16,mouse.Y-b+14,Mysz[0]);
                mouse.Button=1;
                if(mouseCommand>1 && mouse.MWindow(11,8,267,190))
                {
                }
                else
                {
                    PutImageChange13h(mouse.X-a,mouse.Y-b,Mysz[M],1,Color1,colorM);
                }
                mouse.Button=0;
            }
        }
    }
}
ShowVirtualScreen();
PutImage13h(mouse.X-a,mouse.Y-b,Mysz[0],0);
}
//////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////
void Scroll()
{
    
if(scrollTimer+(5-skroller)>licznik)return;

scrollTimer=licznik;
//if(skroller1>skroller){return;}

if(mouse.X<10)
        {
        ScreenX--;
        if(ScreenX<1){ScreenX=1;return;}
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        skroller1++;
        RefreshScreen();
        }
if(mouse.X>310)
        {
        ScreenX++;
        if(ScreenX>MaxX-17){ScreenX=MaxX-17;return;}
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        skroller1++;
        RefreshScreen();

        }
if(mouse.Y<8)
        {
        ScreenY--;
        if(ScreenY<1){ScreenY=1;return;}
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        RefreshScreen();
        skroller1++;
        return;
        }
if(mouse.Y>192-7)
        {
        ScreenY++;
        if(ScreenY>MaxY-14){ScreenY=MaxY-14;return;}
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        skroller1++;
        RefreshScreen();
        return;
        }

}
///////////////////////////////////////////////////////////////////////////
// Rozpatrzenie zadania
///////////////////////////////////////////////////////////////////////////
void DispatchEvent()
{
int x,y,i;
if(ile0)mouse.Button=1;
if(ile1)mouse.Button=2;
if(!kody)
{
    if(mouse.Key=='k'&&!chat){chat=15;return;}
    if(mouse.Key=='o'&&chat==15){chat=14;return;}
    if(mouse.Key=='d'&&chat==14){chat=13;return;}
    if(mouse.Key=='y'&&chat==13){kody=1;chat=0;return;}
    if(mouse.Key)chat=0;
}
if(kody)
{
    if(mouse.Key==13){chat=0;return;}

    if(mouse.Key=='D'&&!chat){chat=1;return;}
    if(mouse.Key=='O'&&chat==1){chat=2;return;}
    if(mouse.Key=='S'&&chat==2){chat=3;return;}
    if(mouse.Key=='W'&&chat==3){selectM->exp=100;chat=100;return;}

    if(mouse.Key=='M'&&!chat){chat=5;return;}
    if(mouse.Key=='A'&&chat==5){chat=6;return;}
    if(mouse.Key=='G'&&chat==6){chat=7;return;}
    if(mouse.Key=='I'&&chat==7){chat=8;return;}
    if(mouse.Key=='C'&&chat==8)
    {
        if(selectM->type==3||selectM->type==4||selectM->type==9||selectM->type==11)selectM->magic=300;
        chat=100;
        return;
    }

    if(mouse.Key=='I'&&chat==5){chat=9;return;}
    if(mouse.Key=='L'&&chat==9){chat=10;return;}
    if(mouse.Key=='K'&&chat==10)
    {
        chat=100;
        castle[0].milk=castle[0].maxmilk;
    }

    if(mouse.Key=='K'&&!chat){chat=11;return;}
    if(mouse.Key=='I'&&chat==11){chat=12;return;}
    if(mouse.Key=='L'&&chat==12){chat=13;return;}
    if(mouse.Key=='L'&&chat==13)
    {
        attack[selectM->x][selectM->y]=127;
        chat=100;
        return;
    }

    if(mouse.Key=='T'&&!chat){chat=14;return;}
    if(mouse.Key=='R'&&chat==14){chat=15;return;}
    if(mouse.Key=='E'&&chat==15){chat=16;return;}
    if(mouse.Key=='E'&&chat==16)
    {
        int treex=selectM->x;
        int treey=selectM->y;
        if(placeG[treex][treey]<9)
        {
            selectM->exist=0;
            attack[selectM->x][selectM->y]=100;
            place[treex][treey]=drzewa+256+512;
            drzewa++;
            placeG[treex][treey]=113+(treex&3)+(treey&3);//113-119
        }
        chat=100;
        return;
    }

    if(mouse.Key=='E'&&!chat){chat=17;return;}
    if(mouse.Key=='N'&&chat==17){chat=18;return;}
    if(mouse.Key=='D'&&chat==18){chat=19;return;}
    if(mouse.Key=='V'&&chat==19){endL=1;chat=100;return;}
    if(mouse.Key=='L'&&chat==19){endL=2;chat=100;return;}

    if(mouse.Key=='C'&&!chat){chat=20;return;}
    if(mouse.Key=='O'&&chat==20){chat++;return;}
    if(mouse.Key=='U'&&chat==21){chat++;return;}
    if(mouse.Key=='N'&&chat==22){chat++;return;}
    if(mouse.Key=='T'&&chat==23){debug=(!debug);chat=100;return;}

    if(mouse.Key=='S'&&!chat){chat=24;return;}
    if(mouse.Key=='H'&&chat==24){chat++;return;}
    if(mouse.Key=='O'&&chat==25){chat++;return;}
    if(mouse.Key=='W'&&chat==26)
    {
        chat=100;
        for(i=0;i<MaxX;i++)
        for(int j=0;j<MaxX;j++)if(!placeN[i][j])placeN[i][j]=1;
        decisionFaza=3;
        return;
    }
    if(mouse.Key)
    {
        chat=100;
    }

}
if(mouse.MWindow(12,9,76,74)&&Map&&mouse.Button==1&&!zaznaczanie&&(mouseCommand<2))
                    {
                        ScreenX=mouse.X-12-7;
                        ScreenY=mouse.Y-9-8;
                        if(ScreenX>MaxX-17)ScreenX=MaxX-17;
                        if(ScreenY>MaxY-14)ScreenY=MaxY-14;
                        if(ScreenY<1)ScreenY=1;
                        if(ScreenX<1)ScreenX=1;
                        castle[0].Prepare(ScreenX,ScreenY,0);
                        castle[1].Prepare(ScreenX,ScreenY,0);
                        return;
                        }


if(mouse.Button==1&&!ile0&&zaznaczanie)
{
        rx2=mouse.X-2;
        ry2=mouse.Y;
        if(rx2<12)rx2=12;
        if(rx2>260)rx2=260;
        if(ry2<9)ry2=9;
        if(ry2>188)ry2=188;
        RefreshScreen();
        return;
}

if(mouse.Button==1&&ile0&&!zaznaczanie&&mouse.MWindow(14,10,260,187)&&mouseCommand==1)
    {
     //   if(Map&&mouse.MWindow(11,8,76,74)){}
     //            else
     //         {
              rx1=mouse.X-2;
              ry1=mouse.Y;
              rx2=rx1;
              ry2=ry1;
              zaznaczanie=1;
       //       }
    }
/*
if(mouse.Key=='-')//-
    {
        if(skroller>0)skroller--;
        return;
        }
if(mouse.Key=='=')//=
    {
        if(skroller<5)skroller++;
        return;
        }
*/
// Skrolling
if(mouse.MWindow(0,0,10,200)||mouse.Key==75)
        {
        ScreenX--;
        if(mouse.Key)ScreenX--;
        if(ScreenX<1)ScreenX=1;
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        RefreshScreen();
        return;
        }
if(mouse.MWindow(310,0,320,200)||mouse.Key==77)
        {
        ScreenX++;
        if(mouse.Key)ScreenX++;
        if(ScreenX>MaxX-17)ScreenX=MaxX-17;
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        RefreshScreen();
        return;
        }
if(mouse.MWindow(77,0,320,8)||mouse.Key==72)
        {
        ScreenY--;
        if(mouse.Key)ScreenY--;
        if(ScreenY<1)ScreenY=1;
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        RefreshScreen();
        return;
        }
if(mouse.MWindow(0,192,320,200)||mouse.Key==80)
        {
        ScreenY++;
        if(mouse.Key)ScreenY++;
        if(ScreenY>MaxY-14)ScreenY=MaxY-14;
        castle[0].Prepare(ScreenX,ScreenY,0);
        castle[1].Prepare(ScreenX,ScreenY,0);
        RefreshScreen();
        return;
        }

if(mouse.MWindow(268,174,305,195))// -------------- SubMenu ----------------
    {
        SubMenu();
        decisionFaza=3;
        if(quitLevel)return;
        PlayTrack(Track[level-1]);
        DownPalette(1);
        LoadExtendedPalette(3);  //<<<<<<<<<<<<<<<<<<< zmienic
        ShowSelected();
        ShowVirtualScreen();
        if(Map)PressButton(16,2);else PressButton(16,3);
        PutImage13h(275,139,buttons[7],0);
        RisePalette(0);
        licznik2=licznik+speed+1;
        RefreshScreen();
        return;
    }
if(mouse.MWindow(268,154,305,175)/*mouse.Key==12909*/)
        {
        Options();
        DownPalette(1);
        LoadExtendedPalette(3);
        ShowSelected();
        ShowVirtualScreen();
        RisePalette(0);
        licznik2=licznik+speed+1;
        RefreshScreen();
        return;
        }
/*
if((ile1&&mouse.Button==2&&mouse.MWindow(274,140,295,155)))    //cel misji
        {
        if(showcel)showcel=0;else showcel=1;
    return;
        }
*/
if(mouse.Key=='m'||(ile0&&mouse.MWindow(274,140,295,155)))    //m mapa
        {
        if(Map){Map=0;
            PressButton(16,3);//274,138
                PutImage13h(275,139,buttons[7],0);
        }
    else {Map=1;
             PressButton(16,2);
             PutImage13h(275,139,buttons[7],0);
             }
        return;
        }
// speed
if(mouse.Key=='-'&&speed<5){speed++;return;} // -
if(mouse.Key=='+'&&speed>0)         // +
{
    speed--;
    if(speed<0)speed=0;
    return;
}

// rozkazy dla zamku - odznacz
if(mouse.Button==2&&ile1&&mouseCommand>1)
{ // odznacz
        mouseCommand=1;
        if(select.IFF<2)
        {
            if(select.co==1)ShowPanel(select.IFF,selectM->type,1,selectM->magic,0);
            if(select.co==0)ShowPanel(select.IFF,selectB->type+19,castle[0].milk,selectB->maxfood-selectB->food,0);
        }else ShowPanel(0,0,0,0,0);
        return;
}

// zaznacz
if(mouse.Key=='1'||mouse.Key=='2'||mouse.Key=='3'||mouse.Key=='4'||mouse.Key=='5'||
mouse.Key=='6'||mouse.Key=='7'||mouse.Key=='8'||mouse.Key=='9'||mouse.Key=='0')
{
        int pp;
        switch(mouse.Key)
        {
           case  '1':pp=0;break;
           case  '2':pp=1;break;
           case  '3':pp=2;break;
           case  '4':pp=3;break;
           case  '5':pp=4;break;
           case  '6':pp=5;break;
           case  '7':pp=6;break;
           case  '8':pp=7;break;
           case  '9':pp=8;break;
           case  '0':pp=9;break;
        }
        if(posT[pp].IFF==2)return;
        castle[0].DisableUnits();
        castle[1].DisableUnits();
        grupa=0;
        mouseCommand=1;
        select.IFF=posT[pp].IFF;
        select.co=posT[pp].co;
        select.nrb=posT[pp].nrb;
        select.nrm=posT[pp].nrm;
        if(select.co&&select.nrb<20)selectM=&castle[select.IFF].b[select.nrb].m[select.nrm];
        if(select.co&&select.nrb==20)selectM=&castle[select.IFF].m[select.nrm];
        if(!select.co)selectB=&castle[select.IFF].b[select.nrb];
        if(!select.IFF&&select.co)mouseMode=1;
        char p,b,i;
        for(i=0;i<10;i++)
        if(posTT[pp][i][0]||posTT[pp][i][1]<20)
        {
                if(i)grupa=1;
                p=posTT[pp][i][0];
                b=posTT[pp][i][1];
                if(b<20)castle[0].b[b].m[p].wybrany=1;
                   else castle[0].m[p].wybrany=1;
        }
        return;
}

//zapamietaj
if(mouse.Key=='!'||mouse.Key=='@'||mouse.Key=='#'||mouse.Key=='$'||mouse.Key=='%'||
mouse.Key=='^'||mouse.Key=='&'||mouse.Key=='*'||mouse.Key=='('||mouse.Key==')')

{
         int pp;
        if(select.co==0)return;
        if(select.IFF)return;
        switch(mouse.Key)
        {
                case '!':pp=0;break;
                case '@':pp=1;break;
                case '#':pp=2;break;
                case '$':pp=3;break;
                case '%':pp=4;break;
                case '^':pp=5;break;
                case '&':pp=6;break;
                case '*':pp=7;break;
                case '(':pp=8;break;
                case ')':pp=9;break;
        }
        posT[pp].IFF=select.IFF;
        posT[pp].co=select.co;
        posT[pp].nrb=select.nrb;
        posT[pp].nrm=select.nrm;
        int i,b,p;
        for(i=0;i<10;i++){posTT[pp][i][1]=20;posTT[pp][i][0]=0;}
        p=0;
        for(i=1;i<MaxUnitsInCastle;i++)
        if(castle[0].m[i].wybrany)
        {
                posTT[pp][p][0]=i;
                posTT[pp][p][1]=20;
                p++;
                if(p>9)p=9;
        }
        for(b=0;b<20;b++)
         for(i=0;i<6;i++)
           if(castle[0].b[b].m[i].wybrany)
           {
                posTT[pp][p][0]=i;
                posTT[pp][p][1]=b;
                p++;
                if(p>9)p=9;
           }
}

if(select.co&&select.IFF==master&&((ile0&&mouse.Button==1)||mouse.Key))  // postac i nasza
{

if((mouse.Key==8051)||mouse.MWindow(274,20,295,35))
{ // stop tu stoj     s
        if(!selectM->type)return;
        Cmd[master].co=1;
        Cmd[master].command=5;
        Cmd[master].nrb=select.nrb;
        Cmd[master].nrm=select.nrm;
        //strcpy(Msg.msg,"Stehe !");
        strcpy(Msg.msg,"Stoj$ !");
        if(!selectM->type==8)strcpy(Msg.msg,"GROARRR !");
        if(!selectM->type==9)strcpy(Msg.msg,"Ssssss...");
        Msg.licznik=20;
        /////////////////
        int kk=34+selectM->type*11;
        if(grupa&&selectM->type)
        {
              kk=177;

              strcpy(Msg.msg,"Stoimy !");
              if(selectM->type>7){kk=0;strcpy(Msg.msg," ");}
        }
        Msg.X=ScreenX;Msg.Y=ScreenY;
        if(Msg.dzwiek<kk){Msg.dzwiek=kk;}
        return;
}
if((ile0&&mouse.MWindow(274,60,295,75))||(mouse.Key=='c')) //c =11875
{
        if((selectM->type==3||selectM->type==4)&&selectM->magic>79)//kaplan,kaplanka - deszcz ognia
        {
              ShowPanel(select.IFF,selectM->type,1,selectM->magic,3);
              mouseCommand=13;
              return;
        }
        return;
}
if((ile0&&mouse.MWindow(274,80,295,95))||(mouse.Key==' '))
{
        if(selectM->type==3||selectM->type==4)//druidka - tarcza /kaplan widzenie
        {
              ShowPanel(select.IFF,selectM->type,1,selectM->magic,4);
              mouseCounter=10;
              Cmd[master].co=1;
              Cmd[master].command=9;
              Cmd[master].nrb=select.nrb;
              Cmd[master].nrm=select.nrm;
              if(selectM->type==3)strcpy(Msg.msg,"");else strcpy(Msg.msg," ");
              Msg.licznik=20;
              return;
        }

}
}

//-----------komendy dla zamkow----------------
if(!select.co&&select.IFF==master&&ile0&&mouse.Button==1)
{  // komendy odbudowy zamku i produkcji postaci
     if(selectB->exist>2)
     {
         mouseCommand=1;
     }
     else
     for(i=0;i<6;i++)  // produkcja postaci  i budynkow
            if(mouse.MWindow(274,20+i*20,290,35+i*20))
            {
                mouseCounter=0;
                if(castle[0].milk<50+200*(6-i)){return;}
                mouseCommand=selectB->Wynik(6-i); //budowa domu
                //show panel przelozony 3 linijki nizej
                if(mouseCommand==0)             //budowa goscia
                {
                     ShowPanel(select.IFF,selectB->type+9,castle[0].milk,selectB->maxfood-selectB->food,i+1);
                     mouseCounter=10;
                     Cmd[master].co=0;
                     Cmd[master].nrb=select.nrb;
                     Cmd[master].command=6-i;
                     mouseCommand=1;
                }
                if(Msg.dzwiek<19){Msg.dzwiek=19;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
            }

}



x=(int)((mouse.X-11)>>4)+ScreenX;
y=(int)(mouse.Y-8)/14+ScreenY;
if(mouse.MWindow(11,8,76,74)&&Map&&mouseMode) // na mapie
{
    x=mouse.X-12;
    y=mouse.Y-9;
}
if(((mouse.Button==1&&ile0)||(mouse.Button==2&&ile1))&&(mouse.MWindow(11,8,266,190)))
        {
        if(!select.IFF&&select.co&&mouseMode&&mouse.Button==2)//zolnierz  nasz
            {
                if(place[x][y]>511)mouseCommand=11;
                if(!place[x][y])
                {
                    for(int ii=x-1;ii<x+2;ii++)
                    for(int jj=y-1;jj<y+2;jj++)
                    if(Who(place[ii][jj])==3)
                    {
                        x=ii;
                        y=jj;
                        mouseCommand=11;
                    }
                }
                if(selectM->type==10&&mouseCommand==11)
                {
                    Mover1* mm=Pointer(place[x][y]);
                    if(mm==NULL||(mm->type!=0&&mm->type!=8))
                    {
                        mouseCommand=1;
                    }  //nie atakuj nikogo oprocz krowy
                }
                if((selectM->type!=4&&selectM->type!=1)&&place[x][y]>768)mouseCommand=1;
                if((selectM->type==1)&&place[x][y]>768&&placeG[x][y]>112&&placeG[x][y]<119)mouseCommand=1; //drzewo zielona
                if((selectM->type!=1)&&place[x][y]>0x1000)mouseCommand=1;//budowa reperacja
                if(!place[x][y]||!placeN[x][y])mouseCommand=10;//idz
                if((selectM->type==1)&&(select.co==1)&&(placeG[x][y]==277||place[x][y]==2||(place[x][y]>255&&place[x][y]<511&&placeG[x][y]>126&&placeG[x][y]<256)))mouseCommand=12;//odbudowa
                if(!selectM->type&&place[x][y]<512&&placeG[x][y]>157&&placeG[x][y]<166)mouseCommand=10;//krowa idz do naszej obory
                if(place[x][y]&&Map&&mouse.MWindow(11,8,76,74))mouseCommand=10;
                }
        switch (mouseCommand)
            {
        case 10:  //move
                {
                if(mouse.Button==1)return;
                if(place[x][y]!=0&&x!=selectM->x&&y!=selectM->y&&selectM->type)
                {
                        int xm,ym,dx,dy;
                        xm=selectM->x;ym=selectM->y;
                        do
                         {
                         dx=xm-x;
                         dy=ym-y;
                         if(dx>0)x++;
                         if(dx<0)x--;
                         if(dy>0)y++;
                         if(dy<0)y--;
                         }while(place[x][y]!=0&&x!=xm&&y!=ym);
                }
                ShowPanel(select.IFF,selectM->type,1,selectM->magic,0);
                mouseCommand=1;
                zaznaczanie=0;
                //mouseMode=0;
                Cmd[master].co=1;
                Cmd[master].nrm=select.nrm;
                Cmd[master].nrb=select.nrb;
                Cmd[master].command=1;
                Cmd[master].x=x;
                Cmd[master].y=y;
                //strcpy(Msg.msg,"Ich gehe !");
                strcpy(Msg.msg,"Id$ !");
                if(!selectM->type)strcpy(Msg.msg,"MUUUU !");
                if(!selectM->type==8)strcpy(Msg.msg,"GROARRR !");
                if(!selectM->type==9)strcpy(Msg.msg,"Ssssss...");
                Msg.licznik=20;
                /////////////////
                Msg.count=5;
                Msg.x=x;
                Msg.y=y;
                /////////////////
                int kk=0;
                if(grupa&&selectM->type)
                {
                      kk=171;
                      if(selectM->type>7)kk=0;
                      //strcpy(Msg.msg,"Wir gehen !");
                      strcpy(Msg.msg,"Idziemy !");
                }
                else
                {
                    if(selectM->type)kk=28+selectM->type*11+Msg.ddzwiek;
                }
                if(!selectM->type)kk=0;
                if(Msg.dzwiek<kk){Msg.dzwiek=kk;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
        case 11 :  //attack
                {
                if(mouse.Button==1)return;
                if(!selectM->type){mouseCommand=1;return;}
                if(!placeN[x][y]){mouseCommand=1;return;}
                if(place[x][y]<256)return;
                ShowPanel(select.IFF,selectM->type,1,selectM->magic,0);
                mouseCommand=1;
                zaznaczanie=0;
                //mouseMode=0;
                Cmd[master].co=1;
                Cmd[master].nrm=select.nrm;
                Cmd[master].nrb=select.nrb;
                Cmd[master].command=2;
                Cmd[master].x=x;
                Cmd[master].y=y;
                //if(selectM->type)strcpy(Msg.msg,"in den Kampf!");
                if(selectM->type)strcpy(Msg.msg,"Do ataku !!!");
                if(!selectM->type)strcpy(Msg.msg,"MUUUU !");
                if(!selectM->type==8)strcpy(Msg.msg,"GROARRR !");
                if(!selectM->type==9)strcpy(Msg.msg,"Ssssss...");
                Msg.licznik=20;
                /////////////////
                Msg.count=11;
                Msg.x=x;
                Msg.y=y;
                /////////////////
                int kk=0;
                if(grupa)
                {
                    kk=174;
                }
                else
                {
                  if(selectM->type)kk=31+selectM->type*11+Msg.ddzwiek;
                }
                if(!selectM->type)kk=0;
                if(Msg.dzwiek<kk){Msg.dzwiek=kk;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
        case 12 :  // budowa/reperacja
                {
        //      ShowPanel(select.IFF,1,1,0,2);
                if(placeG[x][y]<277&&place[x][y]>256&&place[x][y]==place[x-1][y])x--;
                mouseCommand=1;
                zaznaczanie=0;
                mouseMode=1;
                Cmd[master].co=1;
                Cmd[master].nrm=select.nrm;
                Cmd[master].nrb=select.nrb;
                Cmd[master].command=8;//reperacja
                Cmd[master].x=x;
                Cmd[master].y=y;

                if(selectM->type)strcpy(Msg.msg,"Do pracy !");
                Msg.licznik=20;
                /////////////////
                Msg.count=11;
                Msg.x=x;
                Msg.y=y;
                /////////////////
                int kk=31;           //??????? co to za dzwiek???
                if(Msg.dzwiek<kk){Msg.dzwiek=kk;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
        case 13 :  // deszcz ognia
                {
                if(mouse.Button==2){mouseCommand=1;return;}
              //ShowPanel(select.IFF,selectM->type,1,selectM->magic,2);
                mouseCommand=1;
                zaznaczanie=0;
                mouseMode=1;
                Cmd[master].co=1;
                Cmd[master].nrm=select.nrm;
                Cmd[master].nrb=select.nrb;
                Cmd[master].command=7;//czar bojowy
                Cmd[master].x=x;
                Cmd[master].y=y;
                if(selectM->type)strcpy(Msg.msg,"Alartuan !");
                Msg.licznik=20;
                /////////////////
                Msg.count=11;
                Msg.x=x;
                Msg.y=y;
                /////////////////
                int kk=13;           //??????? co to za dzwiek???
                if(Msg.dzwiek<kk){Msg.dzwiek=kk;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
    case 4 :
    case 5 :
    case 6 :
    case 7 :
    case 8 : {
            //budowa budynku
            if(mouse.Button==2)return;
            int i,j,ok;
            ok=0;
            for(i=0;i<3;i++)
            for(j=0;j<3;j++)
            {
                if(place[x+i][y+j])ok=1;
            }
            if(ok)return; //musi stac na pustym miejscu
            ok=0;
            for(i=0;i<3;i++)
            for(j=0;j<3;j++)
            {
                if(!placeN[x+i][y+j])ok=1;
                if(placeG[x+i][y+j]==47|| placeG[x+i][y+j]==50)ok=1;
                if((placeG[x+i][y+j]>126&&placeG[x+i][y+j]<257))ok=1;
            }
            if(y>62||x>62)ok=1;
            if(ok)return; //musi stac na odkrytym miejscu i nie na moscie
            ok=0;
            for(i=-1;i<=3;i++)
            for(j=0;j<3;j++)      //
            if(placeG[x+i][y+j]>30&&placeG[x+i][y+j]<54)ok=1;
            for(i=0;i<3;i++)
            for(j=-1;j<=3;j++)      //
            if(placeG[x+i][y+j]>30&&placeG[x+i][y+j]<54)ok=1;
            if(!ok)return;// i obok drogi

            Cmd[master].co=2;
            Cmd[master].command=mouseCommand;
            Cmd[master].x=x+2;
            Cmd[master].y=y+2;
            mouseCommand=1;
            ShowPanel(select.IFF,selectB->type+9,castle[0].milk,selectB->maxfood-selectB->food,0);
            // if(Msg.dzwiek<19){Msg.dzwiek=19;Msg.X=ScreenX;Msg.Y=ScreenY;}
            return;
            }
         case 2:  //budowa drogi
                {
                if(mouse.Button==2)return;
                int i,j,ok;
                ok=0;
                if((place[x][y]>1)||!placeN[x][y])return;
                if((placeG[x][y]>45&&placeG[x][y]<54)||(placeG[x][y]>126&&placeG[x][y]<257))return;//nie na moscie i nie na budynku
                for(i=-1;i<=1;i++)
                if(placeG[x+i][y]>30&&placeG[x+i][y]<54)ok=1;
                for(j=-1;j<=1;j++)
                if(placeG[x][y+j]>30&&placeG[x][y+j]<54)ok=1;
                if(!ok)return;
                Cmd[master].co=2;
                Cmd[master].command=2;
                Cmd[master].x=x;
                Cmd[master].y=y;
             //   if(Msg.dzwiek<20){Msg.dzwiek=20;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
         case 3:  //budowa palisady
                {
                if(mouse.Button==2)return;
                if(place[x][y]!=0||!placeN[x][y])return;
                if((placeG[x][y]>45&&placeG[x][y]<54)||(placeG[x][y]>126&&placeG[x][y]<257))return;
                Cmd[master].co=2;
                Cmd[master].command=3;
                Cmd[master].x=x;
                Cmd[master].y=y;
               // if(Msg.dzwiek<20){Msg.dzwiek=20;Msg.X=ScreenX;Msg.Y=ScreenY;}
                return;
                }
          case 1:
                if(mouse.Button==2)return;
                ZaznaczObiekt(x,y,x,y,1);
                return;
            }
        }
}
//=================================================
extern char drive[4];
void InitBattle(int level,int type)   // type 0 zaladuj scenariusz 1-scenariusz zaladowany
{
int k=0,i,j,p0=1,p1=1;
FILE *plikPlansz;
char name[40],z;

int chatki=1,chaTki=1;
endL=0;
if(!type)
{
    if(level<26)
    {
        switch (prowintion[level-1])
        {
            case(1):
            case(4):color2=DarkBlue;break;
            case(3):color2=DarkYellow;break;
            case(2):color2=DarkGreen;break; //
            case(5):color2=LightGray;break;
        }
    }
    if(level>25)color2=Yellow;
}
if(level==15)
{
    prowintionA[18]=1;
    prowintionA[10]=1;//11,14,16,19
    prowintionA[13]=1;
    prowintionA[15]=1;
}
for(int d1=0;d1<10;d1++)
for(int d2=0;d2<10;d2++)
{
    posTT[d1][d2][0]=0;
    posTT[d1][d2][1]=20;
}
for(i=0;i<10;i++)posT[i].IFF=2;

if(!type)
{
    drzewa=1;
    pl.nrp=0;
    pl.haslo=132;
    pl.next=0;
    pl.tp=0;
    pl.x0=0;
    pl.xp=0;
    pl.yp=0;
    pl.gen=0;
    for(i=0;i<MaxX;i++)
    for(j=0;j<MaxY;j++)
    {
        placeN[i][j]=0;
        placeG[i][j]=8;
        place[i][j]=0;
    }
    sprintf(name,"%slevels\\level.dat",drive);
    castle[0].Init(1,2000);
    castle[1].Init(2,2000);
    ScreenX=10;ScreenY=10;

    if(level<26)
    {
        plikPlansz=fopen(name,"r");
        if (plikPlansz==NULL){Close13h();exit(0);}

        do
        {
            z=getc(plikPlansz);
            if(z=='$')k++;
            if(z=='@'){Close13h();exit(0);}
        }
        while(k!=level);
        for(j=0;j<MaxY;j++)
        {
            do
            {
                z=getc(plikPlansz);
                if(z=='@'){Close13h();exit(0);}
                if(z=='D'){z=getc(plikPlansz);pl.decisionType=(char)(z-48);}
                if(z=='E'){z=getc(plikPlansz);pl.endType=(char)(z-48);}
                if(z=='G')pl.gen=1;
                if(z=='P'){z=getc(plikPlansz);pl.tp=(char)(z-48);}
                if(z=='T'){z=getc(plikPlansz);i=(char)(z-48);
                z=getc(plikPlansz);pl.typ=(char)(z-48)+i*10;}
                if(z=='M'){z=getc(plikPlansz);pl.maxmilk=(int)(z-48)*200+50;if(pl.maxmilk==50)pl.maxmilk=0;}
                if(z=='N'){z=getc(plikPlansz);i=(char)(z-48);
                z=getc(plikPlansz);pl.next=(char)(z-48)+i*10;}
                if(z=='*')
                {
                    char cc=0;
                    do
                    {
                        z=getc(plikPlansz);
                        pl.name[cc]=z;
                        cc++;
                    }while(pl.name[cc-1]!='*');
                    pl.name[cc-1]=0;
                }//   'imie'

            }while(z!='!');
            if(pl.endType==4&&!p1)p1=1;


            for(i=0;i<MaxX;i++)
            {
                                // if(p0>MaxUnitsInCastle-5)p0=MaxUnitsInCastle-5;
                                        // if(p1>MaxUnitsInCastle-5)p1=MaxUnitsInCastle-5;
                z=getc(plikPlansz);
                if(z=='@')
                {
                    Close13h();
                    exit(0);
                }
                place[i][j]=0;
                if(i==0||i==MaxX-1||j==0||j==MaxY-1)place[i][j]=10;
                placeG[i][j]=8;
                if(z=='*')
                {
                    if(pl.endType==1)
                    {
                        place[i][j]=10;
                        pl.x0=i;
                        pl.y0=j;
                    }
                    if(pl.endType==4)
                    {
                        if(!p1)p1++;
                        castle[1].m[1].Init(pl.typ,i,j,0,5);
                        castle[1].m[1].SetNr(512+201);
                        castle[1].m[1].SetIFF(2);
                        castle[1].m[1].Show();
                    }
                }
                if(z=='^')
                {
                    pl.xw=i;
                    pl.yw=j;
                    placeG[i][j]=301;
                }
                if(z=='.')placeG[i][j]=256;
                if(z=='+')
                {
                    placeN[i][j]=1;
                    placeN[i+1][j]=1;
                    placeN[i+1][j+1]=1;
                    placeN[i][j+1]=1;
                }

                if(z=='a')placeG[i][j]=54;   //zejscie ze skal
                if(z=='b')placeG[i][j]=55;
                if(z=='c')placeG[i][j]=56;
                if(z=='d')placeG[i][j]=57;
                if(z=='e'){placeG[i][j]=58;place[i][j]=10;}
                if(z=='f'){placeG[i][j]=59;place[i][j]=0;}
                if(z=='g'){placeG[i][j]=60;place[i][j]=10;}
                if(z=='h'){placeG[i][j]=61;place[i][j]=10;}
                if(z=='i'){placeG[i][j]=62;place[i][j]=10;}
                if(z=='j'){placeG[i][j]=63;place[i][j]=10;}
                if(z=='k'){placeG[i][j]=64;place[i][j]=10;}
                if(z=='l'){placeG[i][j]=65;place[i][j]=10;}
                if(z=='m'){placeG[i][j]=66;place[i][j]=10;}
                if(z=='n')placeG[i][j]=67;
                if(z=='o'){placeG[i][j]=68;place[i][j]=10;}
                if(z=='p'){placeG[i][j]=69;place[i][j]=10;}
                // skaly
                if(z=='Ó'){placeG[i][j]=9;place[i][j]=10;}
                if(z=='É'){placeG[i][j]=10;place[i][j]=10;}
                if(z=='º'){placeG[i][j]=11;place[i][j]=10;}
                if(z=='Ù'){placeG[i][j]=12;place[i][j]=10;}
                if(z=='Ä'){placeG[i][j]=13;place[i][j]=10;}
                if(z=='¸'){placeG[i][j]=14;place[i][j]=10;}
                if(z=='³'){placeG[i][j]=15;place[i][j]=10;}
                if(z=='Í'){placeG[i][j]=16;place[i][j]=10;}
                if(z=='s'){placeG[i][j]=17;place[i][j]=0;}//wejscie
                if(z=='¼'){placeG[i][j]=18;place[i][j]=10;}
                if(z=='Ô'){placeG[i][j]=19;place[i][j]=10;}
                if(z=='Ú'){placeG[i][j]=20;place[i][j]=10;}
                if(z=='·'){placeG[i][j]=21;place[i][j]=10;}

                if(z=='u'){placeG[i][j]=256;xleczenie=i;yleczenie=j;}
                if(z=='/'){placeG[i][j]=300;pl.xp=i;pl.yp=j;}
                if(z=='q')placeG[i][j]=22;
                if(z=='t')placeG[i][j]=35;
                if(z=='w'){placeG[i][j]=74;place[i][j]=1;}
                if(z=='r'){placeG[i][j]=266;place[i][j]=256+512+drzewa;drzewa++;}   //??
                //--------most-------------------
                if(z=='1')placeG[i][j]=46;
                if(z=='2')placeG[i][j]=47;
                if(z=='3')placeG[i][j]=48;
                if(z=='4')placeG[i][j]=49;
                if(z=='5')placeG[i][j]=50;
                if(z=='6')placeG[i][j]=51;
                if(z=='7')placeG[i][j]=52;
                if(z=='8')placeG[i][j]=53;
                // ---------drzewa ---------------
                if(z=='A'&&drzewa<2000){placeG[i][j]=113;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='B'&&drzewa<2000){placeG[i][j]=114;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='C'&&drzewa<2000){placeG[i][j]=115;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='D'&&drzewa<2000){placeG[i][j]=116;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='E'&&drzewa<2000){placeG[i][j]=117;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='F'&&drzewa<2000){placeG[i][j]=118;place[i][j]=256+512+drzewa;drzewa++;}
                if(z=='G'&&drzewa<2000){placeG[i][j]=119;place[i][j]=256+512+drzewa;drzewa++;}
                //---- PASTWISKO I MIEJSCE ZBIORKI
                if(z=='?'){mem.xz=i;mem.yz=j;}
                if(z=='-'){mem.xp=i;mem.yp=j;
                            xpastw=i;ypastw=j;}
                 //---start
                if(z=='='){ScreenX=i;ScreenY=j;}
                 //----- zamki -----------
                if(z=='H')
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,11,1,chatki);
                        castle[0].b[chatki].exist=1;
                        castle[0].b[chatki].hp=castle[0].b[chatki].maxhp;
                        chatki++;
                    }
                }
                if(z=='I')
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,12,1,chatki);
                        chatki++;
                    }
                }
                if(z=='J')  //koszary
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,13,1,chatki);
                        chatki++;
                    }
                }
                if(z=='K')  //swiatynia
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,14,1,chatki);
                        chatki++;
                    }
                }
                if(z=='L')  //KOSZAARY 2
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,15,1,chatki);
                        chatki++;
                    }
                }
                if(z=='M')  //DOM BOH
                {
                    if(chatki<19)
                    {
                        castle[0].b[chatki].Init(i,j,16,1,chatki);
                        chatki++;
                    }
                }
                                         // KROWA NASZA
                if(z=='x'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(0,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='y'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(1,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='z'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(2,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='9'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(3,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='0'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(4,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z==':'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(5,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z==';'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(6,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='<'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(7,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='>'&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(8,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z==','&&p0<39)
                {
                    p0++;
                    castle[0].m[p0].Init(9,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='°'&&p0<39)//176  nasz Pastuch
                {
                    p0++;
                    castle[0].m[p0].Init(10,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='±'&&p0<39)//177 nasz Mag
                {
                    p0++;
                    castle[0].m[p0].Init(11,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                if(z=='²'&&p0<39)//  178  nasz Kusznik
                {
                    p0++;
                    castle[0].m[p0].Init(12,i,j,0,5);
                    castle[0].m[p0].SetNr(256+p0+200);
                    castle[0].m[p0].SetIFF(1);
                    castle[0].m[p0].Show();
                }
                //-----------------------------------------------------------------
                if(z=='N')
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,11,2,chaTki);
                        chaTki++;
                    }
                }
                if(z=='O')
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,12,2,chaTki);
                        chaTki++;
                    }
                }
                if(z=='P')
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,13,2,chaTki);
                        chaTki++;
                    }
                }
                if(z=='Q')  //swiatynia
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,14,2,chaTki);
                        chaTki++;
                    }
                }
                if(z=='R')  //koszary2
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,15,2,chaTki);
                        chaTki++;
                    }
                }
                if(z=='S')  //akademia
                {
                    if(chaTki<19)
                    {
                        castle[1].b[chaTki].Init(i,j,16,2,chaTki);
                        chaTki++;
                    }
                }
                //-----------------------------------------------------
                if(z=='T'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(0,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='U'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(1,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='W'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(2,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='X'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(3,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='Y'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(4,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='Z'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(5,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='#'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(6,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='"'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(7,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='%'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(8,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='&'&&p1<39)
                {
                    p1++;
                    castle[1].m[p1].Init(9,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='­'&&p1<39)//173          pastuch
                {
                    p1++;
                    castle[1].m[p1].Init(10,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='®'&&p1<39)//174    Mag
                {
                    p1++;
                    castle[1].m[p1].Init(11,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }
                if(z=='¯'&&p1<39)//175            kusznik
                {
                    p1++;
                    castle[1].m[p1].Init(12,i,j,0,5);
                    castle[1].m[p1].SetNr(512+p1+200);
                    castle[1].m[p1].SetIFF(2);
                    castle[1].m[p1].Show();
                }

            }
        }

    fclose(plikPlansz);
    mem.faza=0;
    castle[0].maxmilk=pl.maxmilk;
    castle[0].milk=pl.maxmilk;
    castle[1].maxmilk=1800;
    castle[1].milk=1800;
    drzewa0=drzewa+256+512;
    }

    if(level>25)
    {

    ////nowe plansze

    sprintf(name,"%slevels\\level.%d",drive,level);
   // SetScreen(0);
   // Bar13h(0,0,320,200,0);
   // OutText13h(50,5,"Otwieram plik:",255);
   /// OutText13h(150,5,name,255);

    plikPlansz=fopen(name,"rb");

    if (plikPlansz==NULL)
    {
        Close13h();
        printf("Sorry bracie ale nie znalazlem pliku %s.",name);
        exit(0);
    }
    //OutText13h(50,15,"Czytam nagl%wek",255);
    fseek(plikPlansz,MaxX*MaxY*4,SEEK_SET);
    fread((void*)&E,sizeof(EditStr),1,plikPlansz);

    //sprintf(name,"x:%d,y:%d,tp:%d,wk:%d,",E.X,E.Y,E.typPlanszy,E.warunekKonca);
    //OutText13h(50,25,name,255);
    //getch();
    ScreenX=E.X;
    ScreenY=E.Y;
    pl.decisionType=E.typPlanszy;
    pl.endType=E.warunekKonca;
    pl.xp=0;
    pl.yp=0;
    color2=E.color;
    switch (E.color)
    {
        case 0:color2=DarkBlue;break;
        case 1:color2=DarkYellow;break;
        case 2:color2=DarkGreen;break; //
        case 3:color2=LightGray;break;
    }
    pl.typ=E.postac;
    pl.gen=E.generator;
    pl.maxmilk=E.maxmilk;
    strcpy(pl.name,E.pName);
    if(pl.endType==4&&!p1)p1=1;

    ///OutText13h(50,170,"Ustawiam na zero",255);
    int typ=fseek(plikPlansz,0L,SEEK_SET);
    //if(typ)OutText13h(50,120,"Blad",255);
    //OutText13h(50,180,"Czytam dane",255);

    for(j=0;j<MaxY;j++)
    for(i=0;i<MaxX;i++)
    {
        //placeN[i][j]=1;//usunac
        typ=fread((void*)&placeG[i][j],4,1,plikPlansz);


        if(placeG[i][j]<8)placeG[i][j]=8;
        if(placeG[i][j]==329)//postac do uratowania
        {
            if(pl.endType==1)
            {
                place[i][j]=10;
                pl.x0=i;
                pl.y0=j;
            }
            if(pl.endType==4)
            {
                if(!p1)p1++;
                castle[1].m[1].Init(pl.typ,i,j,0,5);
                castle[1].m[1].SetNr(512+201);
                castle[1].m[1].SetIFF(2);
                castle[1].m[1].Show();
            }
        }
        if(placeG[i][j]==301)//krag mocy
        {
            pl.xw=i;
            pl.yw=j;
            placeG[i][j]=301;
        }
        if(placeG[i][j]==328)//rozjasnienie
        {
            placeN[i][j]=1;
            placeN[i+1][j]=1;
            placeN[i+1][j+1]=1;
            placeN[i][j+1]=1;
            placeG[i][j]=8;
        }

       if(placeG[i][j]==58)place[i][j]=10;
       if(placeG[i][j]>59&&placeG[i][j]<67)place[i][j]=10;
       if(placeG[i][j]==68)place[i][j]=10;
       if(placeG[i][j]==69)place[i][j]=10;
       // skaly
       if(placeG[i][j]>8&&placeG[i][j]<17)place[i][j]=10;
       if(placeG[i][j]>17&&placeG[i][j]<22)place[i][j]=10;

       if(placeG[i][j]==331)
       {
           placeG[i][j]=256;
           xleczenie=i;
           yleczenie=j;
       }
       if(placeG[i][j]==300)
       {
           pl.xp=i;
           pl.yp=j;
           placeG[i][j]=300;
       }

       if(placeG[i][j]>73&&placeG[i][j]<87)//woda
       {
           place[i][j]=1;
       }
       if(placeG[i][j]>265&&placeG[i][j]<278)//palisada
       {
           place[i][j]=256+512+drzewa;
           drzewa++;
       }
       // ---------drzewa ---------------
       if((placeG[i][j]>112&&placeG[i][j]<120)||placeG[i][j]==71||placeG[i][j]==73)
       {
           place[i][j]=256+512+drzewa;
           drzewa++;
       }

       //---- PASTWISKO I MIEJSCE ZBIORKI
       if(placeG[i][j]==333)//zbiorka
       {
           mem.xz=i;
           mem.yz=j;
           placeG[i][j]=8;
       }
       if(placeG[i][j]==330)//pastwisko
       {
           mem.xp=i;
           mem.yp=j;
           xpastw=i;
           ypastw=j;
           placeG[i][j]=8;
       }
       //----- zamki I POSTACIE -----------

       switch(placeG[i][j])
       {
           case 145://nasz budynek glowny
           case 165://nasza obora
           case 185:
           case 205:
           case 225:
           case 245:
                 typ=1+(placeG[i][j]-145)/20;
                 if(chatki<19&&typ>0&&typ<7)
                 {
                    castle[0].b[chatki].Init(i,j,10+typ,1,chatki);
                    castle[0].b[chatki].exist=1;
                    castle[0].b[chatki].hp=castle[0].b[chatki].maxhp;
                    chatki++;
                 }
                 else
                 {
                     for(int xx=-2;xx<1;xx++)
                     for(int yy=-2;yy<1;yy++)
                        placeG[i+xx][j+yy]=8;
                 }
                 break;

            case 302:// KROWA NASZA
            case 303:
            case 304:
            case 305:
            case 306:
            case 307:
            case 308:
            case 309:
            case 310:
            case 311:
            case 312:
            case 313:
            case 314://kusznik
                     if(p0<39)
                     {
                         p0++;
                         castle[0].m[p0].Init(placeG[i][j]-302,i,j,0,5);
                         castle[0].m[p0].SetNr(256+p0+200);
                         castle[0].m[p0].SetIFF(1);
                         castle[0].m[p0].Show();

                     }
                     placeG[i][j]=8;
                     break;

           case 155://ich budynek glowny
           case 175://ich obora
           case 195:
           case 215:
           case 235:
           case 255:
                     typ=1+(placeG[i][j]-155)/20;
                     if(chaTki<19)
                     {
                        castle[1].b[chaTki].Init(i,j,10+typ,2,chaTki);
                        chaTki++;

                     }
                     else
                     {
                        for(int xx=-2;xx<1;xx++)
                        for(int yy=-2;yy<1;yy++)
                            placeG[i+xx][j+yy]=8;
                     }
                     break;
           case 315://ich krowa
           case 316:
           case 317:
           case 318:
           case 319:
           case 320:
           case 321:
           case 322:
           case 323:
           case 324:
           case 325:
           case 326:
           case 327:
                    if(p1<39)
                    {
                        p1++;
                        castle[1].m[p1].Init(placeG[i][j]-315,i,j,0,5);
                        castle[1].m[p1].SetNr(512+p1+200);
                        castle[1].m[p1].SetIFF(2);
                        castle[1].m[p1].Show();
                    }
                    placeG[i][j]=8;
                    break;
       }

       if(placeG[i][j]>301)placeG[i][j]=8;

    }
    //OutText13h(50,190,"Zamykam plik",255);
    fclose(plikPlansz);
    mem.faza=0;
    castle[0].maxmilk=E.maxmilk;
    castle[0].milk=E.milk;
    castle[1].maxmilk=1800;
    castle[1].milk=1800;
    }
}
if(ScreenX<1)ScreenX=1;
if(ScreenY<1)ScreenY=1;
if(ScreenX>MaxX-16)ScreenX=MaxX-16;
if(ScreenY>MaxY-14)ScreenY=MaxY-14;
if(level<26)
{
    for(i=1;i<MaxX-1;i++)
    for(j=1;j<MaxY-1;j++)
    {
        if(placeG[i][j]>30&&placeG[i][j]<46)Droga(i,j,35);//droga
        if(placeG[i][j]>265&&placeG[i][j]<277)Droga(i,j,266);//palisada
        if(placeG[i][j]>73&&placeG[i][j]<87)Woda(i,j,74);
        if(placeG[i][j]==22)placeG[i][j]=22+(i&1)+(j&1);//sucha ziemia
    }
}
drzewa0=drzewa+256+512;
placeG[xleczenie][yleczenie]=256;
strcpy(Msg.msg,"Zaczynajmy !");
Msg.licznik=50;
Msg.dzwiek=0;
}

//////////////////////////////////////////////////////////////////
//  SubMenu
//////////////////////////////////////////////////////////////////
void SubMenu(void)
{
int quitMenu=0;

mouseCommand=1;
SetScreen(0);
            //screen=0
ShowSubMenu();
           //    ?????????????????????????????????????/

do{
 MouseEngine();
  if(mouse.MWindow(100,30,220,50)||mouse.Key==11386)    //Save Game/ Zapisz gre
        {   // s=8051  z=11386
        PressButton(1,1);
        delay(300);
        PressButton(1,0);
        SaveGame();
        quitMenu=1;
        }
  if(mouse.MWindow(100,57,220,74)||mouse.Key==4471)    //Load Game/  Wczytaj gre
        {    // l=9836  w=4471// s=8051  z=11386
        mouseMode=0;
        PressButton(2,1);
        delay(300);
        PressButton(2,0);
        LoadGame();
        Map=0;
        quitMenu=1;
        }
  if(mouse.MWindow(100,87,220,104)||mouse.Key==6512)   //Restart Level/ Powtorz poziom
        {   // r=4978   p=6512  // l=9836  w=4471// s=8051  z=11386
        mouseMode=0;
        PressButton(3,1);
        castle[0].FreeUnits();
        castle[1].FreeUnits();
        delay(300);
        PressButton(3,0);
        if(level!=15&&level<26)NextConquest();
        StopPlaying();
        PlayTrack(TRACK_TXT);
        ShowText(level,0);
        DownPalette(1);
        SetScreen(1);
        StopPlaying();
        InitBattle(level,0);
        SetScreen(1);
        ShowBackground();
        ShowSelected();
        quitMenu=1;
        }
  if(mouse.MWindow(100,114,220,131)||mouse.Key==8807)  //Continue/ Powrot do Gry
        {      // c=11875   g=8807 // r=4978   p=6512  // l=9836  w=4471// s=8051  z=11386
        PressButton(4,0);
        delay(300);
        PressButton(4,1);
        quitMenu=1;
        }
  if(mouse.MWindow(100,142,220,159)||mouse.Key==9579)  //End Game /koniec
        {   // quit=4209  k=9579  // c=11875   g=8807 // r=4978   p=6512  // l=9836  w=4471// s=8051  z=11386
        mouseMode=0;
        PressButton(5,0);
        delay(300);
        PressButton(5,1);
        ShowEndMenu();

        do
        {
        MouseEngine();
        }while(!mouse.MWindow(100,114,220,131)&&!(mouse.MWindow(100,142,220,159)));
        quitMenu=1;
        if(mouse.MWindow(100,114,220,131))quitLevel=1;
        }

}while(!quitMenu);
mouse.GButtonUp();
SetScreen(1);
      //  screen=1
}
////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//  Options
//////////////////////////////////////////////////////////////////
void Options(void)
{

int kolorRamki=225,kolorTekstu=225;
int i,quitMenu=0;
int YY[3]={79,113,146};
int XX[6]={224,235,245,256,267,276};




SetScreen(0);

DownPalette(1);
LoadExtendedPalette(12);

ShowPicture(13,0);

ShowPicture(28,100);



for(i=0;i<3;i++)
{
    if(guzik[i]!=NULL)GetImage13h(224+i*8,113,232+i*8,120,guzik[i]);
}
for(i=0;i<2;i++)
{
    if(lancuch[i]!=NULL)GetImage13h(224,79+i*67,286,86+i*67,lancuch[i]);
}
for(i=1;i<3;i++)
{
    if(lancuch[0]!=NULL)PutImage13h(224,YY[i],lancuch[0],0);
}
/*
OutText13h(19,9,"Mowa                                   Utw%r",1);
OutText13h(19,42,"Dzwi$ki",1);
OutText13h(19,76,"Muzyka",1);
OutText13h(19,108,"Szybkosc przewijania",1);
OutText13h(19,142,"Szybkosc gry",1);

OutText13h(20,9,"Mowa                                   Utw%r",kolorTekstu);
OutText13h(20,42,"Dzwi$ki",kolorTekstu);
OutText13h(20,76,"Muzyka",kolorTekstu);
OutText13h(20,108,"Szybkosc przewijania",kolorTekstu);
OutText13h(20,142,"Szybkosc gry",kolorTekstu);
*/
MVol=getVolume();
PutImage13h(XX[MVol],YY[0],guzik[0],0);
PutImage13h(XX[skroller],YY[1],guzik[1],0); //skroll
PutImage13h(XX[5-speed],YY[2],guzik[2],0); //game speed
if(GetCurrentTrack())Rectangle13h(125,72,151,92,kolorRamki);else Rectangle13h(170,72,196,92,kolorRamki);
if(mowa)Rectangle13h(125,6,151,26,kolorRamki);else Rectangle13h(170,6,196,26,kolorRamki);
if(dzwiek)Rectangle13h(125,39,151,59,kolorRamki);else Rectangle13h(170,39,196,59,kolorRamki);
Rectangle13h(215,39,241,59,0);
Rectangle13h(260,39,286,59,0);
//zmienne odpowiedzialne za mowe i dzwieki
RisePalette(0);

do
{
  MouseEngine();
#ifdef POMOC
  switch(mouse.Key)
  {
      case '0': setVolume(0); break;
      case '1': setVolume(1); break;
      case '2': setVolume(2); break;
      case '3': setVolume(3); break;
      case '4': setVolume(4); break;
      case '5': setVolume(5); break;
  }
#endif
  if(mouse.MWindow(125,72,151,92))    //MusikOn
        {
        musik=1;
        OnCDAudio();
        PlayTrack(track);

        Rectangle13h(125,72,151,92,kolorRamki);
        Rectangle13h(170,72,196,92,0);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(170,72,196,92))    //MusikOff
        {
        musik=0;
        StopPlaying();
        OffCDAudio();
        Rectangle13h(125,72,151,92,0);
        Rectangle13h(170,72,196,92,kolorRamki);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(215,39,241,59)&&musik)    //Previous
        {


        Rectangle13h(215,39,241,59,kolorRamki);
        //tu wpisac next CD
        PlayPrevious();
        delay(400);
        Rectangle13h(215,39,241,59,0);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(260,39,286,59)&&musik)    //Next
        {
        Rectangle13h(260,39,286,59,kolorRamki);
        //tu wpisac next CD
        delay(400);
        PlayNext();
        Rectangle13h(260,39,286,59,0);
        mouse.GButtonUp();
        }

  if(mouse.MWindow(125,7,151,27))    //mowaOn
  {
        mowa=1;
        Rectangle13h(125,6,151,26,kolorRamki);
        Rectangle13h(170,6,196,26,0);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(170,7,196,27))    //mowaOff
        {
        mowa=0;
        Rectangle13h(125,6,151,26,0);
    Rectangle13h(170,6,196,26,kolorRamki);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(125,39,151,59))    //dzwiekOn
        {
        dzwiek=1;
        Rectangle13h(125,39,151,59,kolorRamki);
        Rectangle13h(170,39,196,59,0);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(170,39,196,59))    //dzwiekOff
        {
        dzwiek=0;
        Rectangle13h(125,39,151,59,0);
    Rectangle13h(170,39,196,59,kolorRamki);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(200,144,224,155))   //Zwolnij gre
        {
        if(speed<5)speed++;
        if(lancuch[(speed&1)]!=NULL)PutImage13h(224,YY[2],lancuch[(speed&1)],0);
        PutImage13h(XX[5-speed],YY[2],guzik[2],0);
        mouse.GButtonUp();
        }
   if(mouse.MWindow(283,144,298,155)) //Przyspiesz gre
        {
        if(speed)speed--;
        if(lancuch[(speed&1)]!=NULL)PutImage13h(224,YY[2],lancuch[(speed&1)],0);
        PutImage13h(XX[5-speed],YY[2],guzik[2],0);
        mouse.GButtonUp();
        }
   if(mouse.MWindow(200,111,224,121))   //zwolnij skrolling
        {
        if(skroller>0)skroller--;
        if(lancuch[(skroller&1)]!=NULL)PutImage13h(224,YY[1],lancuch[(skroller&1)],0);
        PutImage13h(XX[skroller],YY[1],guzik[1],0);
        mouse.GButtonUp();
        }
   if(mouse.MWindow(283,111,298,121)) //Przyspiesz skrolling
        {
        if(skroller<5)skroller++;
        if(lancuch[(skroller&1)]!=NULL)PutImage13h(224,YY[1],lancuch[(skroller&1)],0);
        PutImage13h(XX[skroller],YY[1],guzik[1],0);
        mouse.GButtonUp();
        }

  if(mouse.MWindow(283,77,298,87)) //Zglosnij muzyke
        {
        if(MVol<5)MVol++;
        setVolume(MVol);
        if(lancuch[(MVol&1)]!=NULL)PutImage13h(224,YY[0],lancuch[(MVol&1)],0);
        PutImage13h(XX[MVol],YY[0],guzik[0],0);
        mouse.GButtonUp();
        }
   if(mouse.MWindow(200,77,224,87))   //zcisz muzyke
        {
        if(MVol)MVol--;
        setVolume(MVol);
        if(lancuch[(MVol&1)]!=NULL)PutImage13h(224,YY[0],lancuch[(MVol&1)],0);
        PutImage13h(XX[MVol],YY[0],guzik[0],0);
        mouse.GButtonUp();
        }
  if(mouse.MWindow(110,172,207,192)||mouse.Key==9579)  //koniec
        {   // quit=4209  k=9579
        quitMenu=1;
        }

}while(!quitMenu);
mouse.GButtonUp();
SetScreen(1);
      //  screen=1
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////
char EndLevel(void)
{int i,x,y;
if(!pl.endType||pl.endType==3) //zabij i zniszcz budynki
{

       x=0;
       y=0;
       if(!pl.endType)
       for(i=1;i<MaxUnitsInCastle;i++)
       {
           if(castle[0].m[i].exist)x=1;
           if(castle[1].m[i].exist)y=1;
       }
       for(i=1;i<20;i++)
       {
        if(castle[0].b[i].exist)x=1;
        if(castle[1].b[i].exist)y=1;
       }

       if(y==0)return 1;   //wygral gracz
       if(x==0)return 2;   //wygral komputer
       return 0;
}
if(pl.endType==1)    //uratuj
{
     if(placeN[pl.x0][pl.y0]&&castle[0].m[1].exist==0)return 2;//niestety zabili
     if(place[pl.xw][pl.yw]==456+1)return 1;   //256+201
     x=0;
     for(i=1;i<MaxUnitsInCastle;i++)
       {
           if(castle[0].m[i].exist)x=1;
       }
       for(i=1;i<20;i++)
       {
        if(castle[0].b[i].exist)x=1;
       }
     if(x==0)return 2;
     return 0;
}
if(pl.endType==4)    //zabij
{
     if(castle[1].m[1].exist==0)return 1;//zabiles dziada
     x=0;
     for(i=1;i<MaxUnitsInCastle;i++)
     {
           if(castle[0].m[i].exist)x=1;
     }
     for(i=1;i<20;i++)
     {
        if(castle[0].b[i].exist)x=1;
     }

     if(x==0)return 2;
     return 0;
}

if(pl.endType==2) // zbuduj "pl.typ" budynkow
{
     x=0;
     for(i=0;i<20;i++)
           if(castle[0].b[i].exist)x++;
     if(x>=pl.typ)return 1;
      x=0;
       for(i=1;i<MaxUnitsInCastle;i++)
           {
           if(castle[0].m[i].exist)x=1;
           }
       for(i=1;i<20;i++)
        {
        if(castle[0].b[i].exist)x=1;
        }
     if(x==0)return 2;
     return 0;
     }

return 0;
}
//8888888888888888888888888888888888888888888888888888888888888888888888888
//////////////////////////////////////////////////////////////////////////
//       RS sectionn
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//   SubMenuSection
///////////////////////////////////////////////////////////////////////
int sSubMenu()
{
int i;
int Ty[5]={33,60,90,117,145};
FILE *file;
char* name;

name=(char*)malloc(12);

mouse.GButtonUp();
DownPalette(2);
LoadExtendedPalette(1);
ShowPicture(1,0);
ShowPicture(16,100);
PressButton(1,1);
PressButton(2,1);
PressButton(3,1);
PressButton(4,1);
PressButton(5,1);

for(i=0;i<4;i++)
{
    strcpy(name,"Pusty");
    file=fopen(FileName[i],"rb");
    if(file!=NULL)
    {
        fseek(file,0,0);
        fread(name,1,12,file);
        fclose(file);
    }
    CenterText13h(109,Ty[i]-1,109+106,Ty[i]+13,name,1);
    CenterText13h(110,Ty[i],110+106,Ty[i]+14,name,1);
    CenterText13h(110,Ty[i]-1,110+106,Ty[i]+13,name,255);
}
CenterText13h(109,Ty[4],109+106,Ty[4]+14,"Koniec",1);
CenterText13h(110,Ty[4]-1,110+106,Ty[4]+13,"Koniec",255);
free(name);
RisePalette(1);
mouse.X=0;
mouse.Y=0;
do
{
  MouseEngine();
  if(mouse.MWindow(100,30,220,47))    //Save 1
        {
        PressButton(1,0);
        delay(300);
        return 0;
        }
  if(mouse.MWindow(100,57,220,74))    //save 2
        {
        PressButton(2,0);
        delay(300);
        return 1;
        }
  if(mouse.MWindow(100,87,220,104))   //save 3
        {
        PressButton(3,0);
        delay(300);
        return 2;
        }
  if(mouse.MWindow(100,114,220,131))  //save 4
        {
        PressButton(4,0);
        delay(300);
        return 3;
        }
  if(mouse.MWindow(100,142,220,159)||mouse.Key==11875)  //cancel
        {
        PressButton(5,0);
        delay(300);
        PressButton(5,1);
        return 4;
        }
}while(1);
}

////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
int SaveGame()
{
        FILE *file;
        int i,j;
        int Ty[4]={32,59,89,116};
        char name[20];

        i=sSubMenu();
        if(i==4)
        {
            mouse.GButtonUp();
            return 1;
        }
        file=fopen(FileName[i],"rb");
        if(file!=NULL)
        {
            fread(name,1,12,file);
            fclose(file);
        }
        else
        {
            strcpy(name,"Pusty");
        }
        j=Write13h(110,Ty[i],107,11,name,255,0);
        if(j==27){mouse.GButtonUp();return 1;}
        file=fopen(FileName[i],"wb");
        if(file==NULL){return 1;}
        fwrite(name,1,12,file);
        fwrite(&pl,sizeof(Plansza),1,file);
        fwrite(&color2,4,1,file);
        fwrite(&level,4,1,file);
        fwrite(&diff,4,1,file);
        for(j=0;j<25;j++)
               fwrite(&prowintion[j],1,1,file);
        for(j=0;j<25;j++)
               fwrite(&prowintionA[j],1,1,file);
        for(j=0;j<40;j++)
        {
               fwrite(&positioN[j][0],4,1,file);
               fwrite(&positioN[j][1],4,1,file);
        }
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fwrite(&place[i][j],4,1,file);
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fwrite(&placeG[i][j],4,1,file);
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fwrite(&placeN[i][j],4,1,file);
        fwrite(&ScreenX,4,1,file);
        fwrite(&ScreenY,4,1,file);
        fwrite(&drzewa,4,1,file);
        fwrite(&mem,sizeof(Mem),1,file);
        for(j=0;j<2;j++)
        {
        fwrite(&castle[j].milk,4,1,file);
        fwrite(&castle[j].maxmilk,4,1,file);
        fwrite(&castle[j].IFF,4,1,file);
        fwrite(&castle[j].faza,4,1,file);
        for(i=1;i<MaxUnitsInCastle;i++)
                {
                fwrite(&castle[j].m[i].nr,4,1,file);
                fwrite(&castle[j].m[i].x,4,1,file);
                fwrite(&castle[j].m[i].y,4,1,file);
                fwrite(&castle[j].m[i].type,4,1,file);
                fwrite(&castle[j].m[i].command,4,1,file);
                fwrite(&castle[j].m[i].commandN,4,1,file);
                fwrite(&castle[j].m[i].xe,4,1,file);
                fwrite(&castle[j].m[i].ye,4,1,file);
                fwrite(&castle[j].m[i].xp,4,1,file);
                fwrite(&castle[j].m[i].yp,4,1,file);
                fwrite(&castle[j].m[i].xm,4,1,file);
                fwrite(&castle[j].m[i].ym,4,1,file);
                fwrite(&castle[j].m[i].delay,4,1,file);
                fwrite(&castle[j].m[i].maxdelay,4,1,file);
                fwrite(&castle[j].m[i].hp,4,1,file);
                fwrite(&castle[j].m[i].maxhp,4,1,file);
                fwrite(&castle[j].m[i].target,4,1,file);
                fwrite(&castle[j].m[i].exist,4,1,file);
                fwrite(&castle[j].m[i].inmove,4,1,file);
                fwrite(&castle[j].m[i].damage,4,1,file);
                fwrite(&castle[j].m[i].udder,4,1,file);
                fwrite(&castle[j].m[i].magic,4,1,file);
                fwrite(&castle[j].m[i].s_range,4,1,file);
                fwrite(&castle[j].m[i].a_range,4,1,file);
                fwrite(&castle[j].m[i].armour,4,1,file);
                fwrite(&castle[j].m[i].ShowHit,4,1,file);
                fwrite(&castle[j].m[i].nr,4,1,file);
                fwrite(&castle[j].m[i].IFF,4,1,file);
                fwrite(&castle[j].m[i].mainTarget,4,1,file);
                fwrite(&castle[j].m[i].exp,4,1,file);
                }
        for(i=0;i<20;i++)
                {
                int k;
                fwrite(&castle[j].b[i].exist,4,1,file);
                fwrite(&castle[j].b[i].IFF,4,1,file);
                fwrite(&castle[j].b[i].nr,4,1,file);
                fwrite(&castle[j].b[i].x,4,1,file);
                fwrite(&castle[j].b[i].y,4,1,file);
                fwrite(&castle[j].b[i].type,4,1,file);
                fwrite(&castle[j].b[i].hp,4,1,file);
                fwrite(&castle[j].b[i].maxhp,4,1,file);
                fwrite(&castle[j].b[i].food,4,1,file);
                fwrite(&castle[j].b[i].maxfood,4,1,file);
                fwrite(&castle[j].b[i].faza,4,1,file);
                for(k=0;k<6;k++)
                    {
                         fwrite(&castle[j].b[i].m[k].nr,4,1,file);
                         fwrite(&castle[j].b[i].m[k].x,4,1,file);
                         fwrite(&castle[j].b[i].m[k].y,4,1,file);
                         fwrite(&castle[j].b[i].m[k].type,4,1,file);
                         fwrite(&castle[j].b[i].m[k].command,4,1,file);
                         fwrite(&castle[j].b[i].m[k].commandN,4,1,file);
                         fwrite(&castle[j].b[i].m[k].xe,4,1,file);
                         fwrite(&castle[j].b[i].m[k].ye,4,1,file);
                         fwrite(&castle[j].b[i].m[k].xp,4,1,file);
                         fwrite(&castle[j].b[i].m[k].yp,4,1,file);
                         fwrite(&castle[j].b[i].m[k].xm,4,1,file);
                         fwrite(&castle[j].b[i].m[k].ym,4,1,file);
                         fwrite(&castle[j].b[i].m[k].delay,4,1,file);
                         fwrite(&castle[j].b[i].m[k].maxdelay,4,1,file);
                         fwrite(&castle[j].b[i].m[k].hp,4,1,file);
                         fwrite(&castle[j].b[i].m[k].maxhp,4,1,file);
                         fwrite(&castle[j].b[i].m[k].target,4,1,file);
                         fwrite(&castle[j].b[i].m[k].exist,4,1,file);
                         fwrite(&castle[j].b[i].m[k].inmove,4,1,file);
                         fwrite(&castle[j].b[i].m[k].damage,4,1,file);
                         fwrite(&castle[j].b[i].m[k].udder,4,1,file);
                         fwrite(&castle[j].b[i].m[k].magic,4,1,file);
                         fwrite(&castle[j].b[i].m[k].s_range,4,1,file);
                         fwrite(&castle[j].b[i].m[k].a_range,4,1,file);
                         fwrite(&castle[j].b[i].m[k].armour,4,1,file);
                         fwrite(&castle[j].b[i].m[k].ShowHit,4,1,file);
                         fwrite(&castle[j].b[i].m[k].nr,4,1,file);
                         fwrite(&castle[j].b[i].m[k].IFF,4,1,file);
                         fwrite(&castle[j].b[i].m[k].mainTarget,4,1,file);
                         fwrite(&castle[j].b[i].m[k].exp,4,1,file);
                        }
                }
        }
        fwrite(&drzewa0,4,1,file);
        fclose(file);
        mouse.GButtonUp();
        return 0;
}

////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
int LoadGame()
{

  {
        FILE *file;
        int i,j;
        char name[15];

        i=sSubMenu();
        if(i==4)
        {
            mouse.GButtonUp();
            return 1;
        }
        file=fopen(FileName[i],"rb");
        if(file==NULL){return 1;}
        for(int d1=0;d1<10;d1++)
        for(int d2=0;d2<10;d2++)
        {
            posTT[d1][d2][0]=0;
            posTT[d1][d2][1]=20;
        }
        for(i=0;i<10;i++)posT[i].IFF=2;
        castle[0].FreeUnits();
        castle[1].FreeUnits();

        fread(name,1,12,file);
        fread(&pl,sizeof(Plansza),1,file);
        fread(&color2,4,1,file);
        fread(&level,4,1,file);
        fread(&diff,4,1,file);
        for(j=0;j<25;j++)
               fread(&prowintion[j],1,1,file);
        for(j=0;j<25;j++)
               fread(&prowintionA[j],1,1,file);
        for(j=0;j<40;j++)
        {
               fread(&positioN[j][0],4,1,file);
               fread(&positioN[j][1],4,1,file);
        }
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fread(&place[i][j],4,1,file);
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fread(&placeG[i][j],4,1,file);
        for(i=0;i<MaxX;i++)
        for(j=0;j<MaxY;j++)
               fread(&placeN[i][j],4,1,file);
        fread(&ScreenX,4,1,file);
        fread(&ScreenY,4,1,file);
        fread(&drzewa,4,1,file);
        fread(&mem,sizeof(Mem),1,file);  mem.c=&castle[1];
        for(j=0;j<2;j++)
        {
        fread(&castle[j].milk,4,1,file);
        fread(&castle[j].maxmilk,4,1,file);
        fread(&castle[j].IFF,4,1,file);
        fread(&castle[j].faza,4,1,file);
        for(i=1;i<MaxUnitsInCastle;i++)
                {
                fread(&castle[j].m[i].nr,4,1,file);
                fread(&castle[j].m[i].x,4,1,file);
                fread(&castle[j].m[i].y,4,1,file);
                fread(&castle[j].m[i].type,4,1,file);
                fread(&castle[j].m[i].command,4,1,file);
                fread(&castle[j].m[i].commandN,4,1,file);
                fread(&castle[j].m[i].xe,4,1,file);
                fread(&castle[j].m[i].ye,4,1,file);
                fread(&castle[j].m[i].xp,4,1,file);
                fread(&castle[j].m[i].yp,4,1,file);
                fread(&castle[j].m[i].xm,4,1,file);
                fread(&castle[j].m[i].ym,4,1,file);
                fread(&castle[j].m[i].delay,4,1,file);
                fread(&castle[j].m[i].maxdelay,4,1,file);
                fread(&castle[j].m[i].hp,4,1,file);
                fread(&castle[j].m[i].maxhp,4,1,file);
                fread(&castle[j].m[i].target,4,1,file);
                fread(&castle[j].m[i].exist,4,1,file);
                fread(&castle[j].m[i].inmove,4,1,file);
                fread(&castle[j].m[i].damage,4,1,file);
                fread(&castle[j].m[i].udder,4,1,file);
                fread(&castle[j].m[i].magic,4,1,file);
                fread(&castle[j].m[i].s_range,4,1,file);
                fread(&castle[j].m[i].a_range,4,1,file);
                fread(&castle[j].m[i].armour,4,1,file);
                fread(&castle[j].m[i].ShowHit,4,1,file);
                fread(&castle[j].m[i].nr,4,1,file);
                fread(&castle[j].m[i].IFF,4,1,file);
                fread(&castle[j].m[i].mainTarget,4,1,file);
                fread(&castle[j].m[i].exp,4,1,file);
                castle[j].m[i].missile.exist=0;
                castle[j].m[i].ispath=0;
                }
        for(i=0;i<20;i++)
                {
                int k;
                fread(&castle[j].b[i].exist,4,1,file);
                fread(&castle[j].b[i].IFF,4,1,file);
                fread(&castle[j].b[i].nr,4,1,file);
                fread(&castle[j].b[i].x,4,1,file);
                fread(&castle[j].b[i].y,4,1,file);
                fread(&castle[j].b[i].type,4,1,file);
                fread(&castle[j].b[i].hp,4,1,file);
                fread(&castle[j].b[i].maxhp,4,1,file);
                fread(&castle[j].b[i].food,4,1,file);
                fread(&castle[j].b[i].maxfood,4,1,file);
                fread(&castle[j].b[i].faza,4,1,file);
                for(k=0;k<6;k++)
                    {
                         fread(&castle[j].b[i].m[k].nr,4,1,file);
                         fread(&castle[j].b[i].m[k].x,4,1,file);
                         fread(&castle[j].b[i].m[k].y,4,1,file);
                         fread(&castle[j].b[i].m[k].type,4,1,file);
                         fread(&castle[j].b[i].m[k].command,4,1,file);
                         fread(&castle[j].b[i].m[k].commandN,4,1,file);
                         fread(&castle[j].b[i].m[k].xe,4,1,file);
                         fread(&castle[j].b[i].m[k].ye,4,1,file);
                         fread(&castle[j].b[i].m[k].xp,4,1,file);
                         fread(&castle[j].b[i].m[k].yp,4,1,file);
                         fread(&castle[j].b[i].m[k].xm,4,1,file);
                         fread(&castle[j].b[i].m[k].ym,4,1,file);
                         fread(&castle[j].b[i].m[k].delay,4,1,file);
                         fread(&castle[j].b[i].m[k].maxdelay,4,1,file);
                         fread(&castle[j].b[i].m[k].hp,4,1,file);
                         fread(&castle[j].b[i].m[k].maxhp,4,1,file);
                         fread(&castle[j].b[i].m[k].target,4,1,file);
                         fread(&castle[j].b[i].m[k].exist,4,1,file);
                         fread(&castle[j].b[i].m[k].inmove,4,1,file);
                         fread(&castle[j].b[i].m[k].damage,4,1,file);
                         fread(&castle[j].b[i].m[k].udder,4,1,file);
                         fread(&castle[j].b[i].m[k].magic,4,1,file);
                         fread(&castle[j].b[i].m[k].s_range,4,1,file);
                         fread(&castle[j].b[i].m[k].a_range,4,1,file);
                         fread(&castle[j].b[i].m[k].armour,4,1,file);
                         fread(&castle[j].b[i].m[k].ShowHit,4,1,file);
                         fread(&castle[j].b[i].m[k].nr,4,1,file);
                         fread(&castle[j].b[i].m[k].IFF,4,1,file);
                         fread(&castle[j].b[i].m[k].mainTarget,4,1,file);
                         fread(&castle[j].b[i].m[k].exp,4,1,file);
                         castle[j].b[i].m[k].missile.exist=0;
                         castle[j].b[i].m[k].ispath=0;
                        }
                }
        }
        fread(&drzewa0,4,1,file);
        fclose(file);
        mem.c=&castle[1];
        endL=0;
        mouse.GButtonUp();
        return(0);
        }
}
////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void FindGrass(int x,int y,int* xe,int* ye)
{
int i,j,k=1;
*xe=0;
for(k=2;k<=15;k++)
    {
        for(i=x-k;i<=x+k&&i<MaxX-1;i++)
        if(i>0)for(j=y-k;j<=y+k&&j<MaxY-1;j++)
           if(j>0)
           if(i==x-k||i==x+k||j==y-k||j==y+k)
               {
               if(placeG[i][j]<9&&!place[i][j])
                {
                *xe=i;
                *ye=j;
                return;
                }
               }
       }

*xe=x+5;
*ye=y+3;
}

/*{
      int obora[5];//nr budynku(20==brak)
      int koszaryo[5];//nr budynku(20==brak)
      int koszarya[5];//nr budynku(20==brak)
      int swiatynia[5];//nr budynku(20==brak)
      int akademia[5];//nr budynku(20==brak)
      int faza;//0-inicjacja 1-krowy budowa 2-krowy wyslij
               //3-koszaryo budowa i zozeslanie
               //4-swiatynia wybuduj i wyslij
               //5-atak koszarya i swiatynia
      int xe,ye;//ostatnie polozenie modulu przeciwnika
      int x,y;  //polozenie naszego grodu;
      int distance;//odleglosc przeciwnika od grodu
      int iloscZ;//ilosc zolnierzy w ataku z jednego budynku;
      int iloscB;//ilosc budynkow w ataku;
      int dmilk;//przyrost mleka na runde;
      int command;//komenda z ostatniego przejscia 2-budowa krowy;
      int nrb; // budynek
      int nrm; // i postac ktorego dotyczy ostatni rozkaz
      class Castle *c;
      }mem;   */
void DecisionB()
{
int i,j;

if(mem.faza==5)return;
if(mem.faza==0)
    {
        mem.c=&castle[1];
    mem.faza=1;
        mem.delay=0;
        }

if(mem.faza==1)
    {
        j=0;
        for(i=39;i>0;i--)if(mem.c->m[i].exist==1)j=i;//znajdz zywego zolnierza
        if(!j){mem.faza=5;return;}
        mem.x=mem.c->m[j].x;
        mem.y=mem.c->m[j].y;
        mem.distance=0;
        FindEnemy(mem.x,mem.y,&mem.xe,&mem.ye,&mem.distance);
        mem.x=j;
        mem.y=0;
        mem.faza=2;
        return;
        }
if(mem.faza==2) //wyslanie 5-ciu zolnierzy
    {
        if(mem.y>5||mem.x==40){mem.delay=300;mem.faza=3;return;}
        for(i=mem.x;i<40;i++)
            if(mem.c->m[i].exist==1)
            {
                mem.y++;
                mem.x++;
                Cmd[1].co=1;
        Cmd[1].command=1;
        Cmd[1].x=mem.xe;
        Cmd[1].y=mem.ye;
        Cmd[1].nrb=20;
        Cmd[1].nrm=i;
                return;
                }
        }
if(mem.faza==3)  //odczekanie
    {if(mem.delay>0){mem.delay--;return;}
        mem.faza=1;
        return;
        }
}
///////////////////////////
void DecisionS()
{
int i;
if(mem.faza==6)return;

if(mem.faza==0) //zapamietanie pozycji poczatkowych
    {
        mem.c=&castle[1];
        mem.faza=1;
        mem.delay=0;
        for(i=2;i<40;i++)if(mem.c->m[i].exist==1)
        {
            positioN[i][0]=mem.c->m[i].x;
            positioN[i][1]=mem.c->m[i].y;}
            else
            {
                positioN[i][0]=10;
                positioN[i][1]=10;
            }
        mem.x=3;
        return;
        }
if(mem.faza==1) //odwolanie zolnierzy na pozycje
    {
        if(mem.x>=39)
        {
            mem.delay=20;
            mem.faza=2;
            return;
        }
        if(mem.c->m[mem.x].exist==1)
        {
            Cmd[1].co=1;
            Cmd[1].command=1;
            Cmd[1].x=positioN[mem.x][0];
            Cmd[1].y=positioN[mem.x][1];
            Cmd[1].nrb=20;
            Cmd[1].nrm=mem.x;
            mem.c->m[mem.x].exp++;
            mem.x++;
            return;
        }
        mem.x++;
        return;
        }
if(mem.faza==2)  //odczekanie
{
    if(mem.delay>0){mem.delay--;return;}
    mem.faza=3;
    mem.x=3;
    return;
}
if(mem.faza==3)
    {
        mem.faza=1;
        return;
        }
}
//===========================================================
void Generator()
{
if(pl.gTimer==25&&castle[1].m[2].exist==1)
{
        int x,y,d;
        d=0;
        FindEnemy(1,5,&x,&y,&d);
        castle[1].m[2].SetCommand(1);
        castle[1].m[2].SetEnd(x,y);

}
if(pl.gTimer<150-diff*50)
{
    pl.gTimer++;
    return;
}
pl.gTimer=0;
if(!castle[1].m[2].exist&&!place[1][5])
{
        int x,y,d;
        castle[1].m[2].Init((mouse.X&7)+1,1,5,0,5);
        castle[1].m[2].SetNr(512+2+200);//+1???
        castle[1].m[2].SetIFF(2);
        castle[1].m[2].Show();
        d=0;
        FindEnemy(1,5,&x,&y,&d);
        castle[1].m[2].SetCommand(1);
        castle[1].m[2].SetEnd(x,y);
        castle[1].m[2].exp=20+diff*20;
}
}
/*
///////////////////////////
void Decision1()
{
int i;
int position[4][2]={{-1,-1},{3,-1},{3,3},{-1,2}};
if(mem.faza==0) //inicjalizacja
{
    mem.dmilk=4+diff*3;
    mem.faza=1;
    mem.c=&castle[1];
    mem.delay=0;
    mem.index=0;
    for(i=0;i<20;i++)
    {
        if(!mem.index&&mem.c->b[i].type==2&&mem.c->b[i].exist==1)mem.index=i;//dobra obora
    }
}
//-----przyrost mleka------
mem.c->milk+=mem.dmilk;
//-----odbudowa-----------------------
for(i=0;i<20;i++)
{
    if(mem.c->b[i].hp&&mem.c->b[i].hp<mem.c->b[i].maxhp)
    {
        int xx,yy,k=0;

        FindEnemy(castle[1].b[i].x, castle[1].b[i].y,&xx,&yy,&k);
        for(int j=0;j<6;j++)
        {
            if(castle[1].m[j+1].type==1&&castle[1].m[j+1].command!=8)
            { //ekipa remontowa
                castle[1].m[j+1].SetEnd(castle[1].b[i].x+(j&2),castle[1].b[i].y);
                castle[1].m[j+1].SetCommand(8);
            }
            if(castle[1].m[j+1].type>1&&!castle[1].m[j+1].command&&(j>1))
            { //straznicy
                castle[1].m[j+1].SetEnd(xx,yy);
                castle[1].m[j+1].SetCommand(1);
            }
            if(castle[1].b[i].m[j].type==1&&castle[1].b[i].m[j].command!=8)
            { //wlasna ekipa remontowa
                castle[1].b[i].m[j].SetEnd(castle[1].b[i].x+(j&2),castle[1].b[i].y);
                castle[1].b[i].m[j].SetCommand(8);
            }
            if(castle[1].b[i].m[j].type>1&&!castle[1].b[i].m[j].command&&!k)
            { //wlasni straznicy
                castle[1].b[i].m[j].SetTarget(place[xx][yy]);
                castle[1].b[i].m[j].SetCommand(2);
            }
        }
    }
}

if(mem.faza==1) //budowa   postaci krow
{
    char typ;
    if(mem.index>19)mem.index=0;
    if(mem.c->b[mem.index].exist!=1)
    {
        mem.faza=2;
        return;
    }//budynek w zlym stanie
    if(mem.c->milk<1250)return;
    switch(mem.c->b[mem.index].type)
    {
        case 1:typ=1;break;//???????? droga????
        case 2:typ=2;
               if(level>PASTUCH_LEV&&mem.c->b[mem.index].food==2)
               {
                   typ=6;
                   mem.c->milk+=200*diff;
               }
               break;//krowa,pastuch
        case 3:typ=2+(mouse.X&1);break;
        case 4:typ=3+(mouse.X&2);
               if(level>MAG_LEV&&mem.c->b[mem.index].food==2)
               {
                   typ=6;
                   mem.c->milk+=300;
               }break;//kaplanka, kaplan, mag
        case 5:typ=3+(mouse.X&1);break;//woj, wlucznik
        case 6:typ=5+(mouse.X&1);break;//rycerz, kusznik
    }
    if(mem.c->b[mem.index].type>1)
    {

        Cmd[1].co=0;
        Cmd[1].nrb=mem.index;
        Cmd[1].command=typ;
        //if(mem.c->b[mem.index].type==2&&mem.c->b[mem.index].food<1)mem.faza=1;//aby po dwie krowy robil
        //else
        mem.faza=2;
        return;
    }
    else
    {
        int j=0;
        for(i=1;i<6;i++)//?????????????
        {
            if(!mem.c->m[i].exist)j=i;
        }
        if(j&&!place[mem.c->b[mem.index].x][mem.c->b[mem.index].y+2])
        {
            mem.c->m[j].Init(1,mem.c->b[mem.index].x,mem.c->b[mem.index].y+2,0,0);
            mem.c->m[j].SetNr(712+j);
            mem.c->m[j].SetIFF(2);
            mem.c->m[j].SetCommand(1);
            mem.c->m[j].SetEnd(mem.c->b[mem.index].x+1,mem.c->b[mem.index].y-1);
            mem.c->m[j].Show();
        }
        mem.faza=2;
        return;
    }
  }
if(mem.faza==2)//wyslanie postaci
    {
        if(!mem.c->b[mem.index].exist){mem.faza=3;return;}//brak budynku
        if(mem.c->b[mem.index].type==1){mem.faza=3;return;}//glowny
        if(mem.c->b[mem.index].type==2)//obora
        {
                int xt,yt;
                FindGrass(mem.xp,mem.yp,&xt,&yt);
                for(i=0;i<3;i++)
                {
                   if(!mem.c->b[mem.index].m[i].command||mem.c->b[mem.index].m[i].command==6)
                   {
                                        mem.c->b[mem.index].m[i].SetEnd(xt,yt);
                                        mem.c->b[mem.index].m[i].SetCommand(1);
                   }
                }
                mem.faza=3;
                return;
        }
        if(mem.c->b[mem.index].type>2)//koszary swiatynie itp.
            {
                int xt,yt;
                for(i=0;i<6;i++)
                   if(!mem.c->b[mem.index].m[i].command&&(mem.c->b[mem.index].m[i].hp>5))
                   {
                        if(i==1||i==3||i==5)
                        {
                            xt=mem.xz;
                            yt=mem.yz;
                        }
                        else
                        {
                            xt=mem.c->b[mem.index].x+position[i/2][0];
                            yt=mem.c->b[mem.index].y+position[i/2][1];
                        }
                        if(diff&&i==3&&(mem.index&1))
                        {
                            xt=mem.c->b[mem.index].m[i].x;
                            yt=mem.c->b[mem.index].m[i].y;
                            mem.distance=0;
                            FindEnemy(xt,yt,&xt,&yt,&mem.distance);
                        }
                        mem.c->b[mem.index].m[i].SetEnd(xt,yt);
                        mem.c->b[mem.index].m[i].SetCommand(1);
                        if(mem.c->b[mem.index].m[i].type==1)mem.c->b[mem.index].m[i].SetCommand(11);
                    }

               }
        mem.faza=3;
        return;
        }
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
if(mem.faza==3) //attack
{
int i,j=0;
for(i=0;i<20;i++)
{
    if(mem.c->b[i].exist==1)j+=(mem.c->b[i].maxfood-mem.c->b[i].food);
}
mem.delay++;
if(j>5||mem.delay<80-(diff*20))//jezeli nie mina zadany czas lub za malo postaci to nic
{
    mem.index++;
    mem.faza=1;
    return;
}
mem.delay=0;
mem.faza=4;
mem.index=0;
mem.delay=0;
return;
}
/////////////////////   tu pracowac   ////////////////////////////////////////////
// wyslac zolnierzy o numerach>2  kaplanow o nr>1 i wodzow
if(mem.faza==4) //attack
    {
        if(mem.index>19)
        {
           mem.faza=5;
           return;
        }
        if(!mem.c->b[mem.index].exist){mem.index++;return;}//brak budynku
        if(mem.c->b[mem.index].type<3){mem.index++;return;}
        for(i=0;i<6;i++)
        if(i<2||i==3||i==5)
        {

                         int x,y;
                         x=mem.c->b[mem.index].m[i].x;
                         y=mem.c->b[mem.index].m[i].y;
                         mem.distance=0;
                         FindEnemy(x,y,&x,&y,&mem.distance);
                         if(x!=32||y!=32)
                         {
                            mem.c->b[mem.index].m[i].SetEnd(x,y);
                            mem.c->b[mem.index].m[i].SetCommand(1);
                         }
                         //strcpy(Msg.msg,"Atak na Polan");
                         //Msg.licznik=50;
                         if(x==32&&y==32)
                         {
                             if(mem.c->b[mem.index].m[i].type==1)
                             {
                                 mem.c->b[mem.index].m[i].SetCommand(11);
                             }
                             else
                             {
                                if(mem.c->b[mem.index].m[i].type==4)
                                {
                                    mem.distance=2;
                                    x=mem.c->b[mem.index].m[i].x;
                                    y=mem.c->b[mem.index].m[i].y;
                                    FindEnemy(x,y,&x,&y,&mem.distance);
                                    if(x!=32||y!=32)
                                    {
                                        mem.c->b[mem.index].m[i].SetCommand(2);//rozwal palisade
                                        mem.c->b[mem.index].m[i].SetTarget(place[x][y]);
                                    }
                                }

                            }

                         }
        }

        mem.index++;
        return;
    }
///////////////////////////////////////////////////////////////////////
if(mem.faza==5) //powrot
    {
        mem.delay++;
        if(mem.delay<500)return;
        mem.delay=0;
        mem.index=0;
        mem.faza=1;
        return;
        }
}



*/

#include "decision.cpp"

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ZaznaczObiekt(int x1,int y1,int x2,int y2,int mode)//1-budynki obcy itp
{
    int x,y,wybrany1=0;
    int nrp,nrb,aa,bb;

if(mode)
{
    x=x1;
    y=y1;
    if(place[x][y]<256||place[x][y]>768)
    {
        int i,j,xn=x,yn=y;
        for(i=x-1;i<=x+1;i++)
        for(j=y-1;j<=y+1;j++)
            if(place[i][j]>255&&place[x][y]<769)
            {
                xn=i;
                yn=j;
            }
        if(place[xn][yn]<256||place[xn][yn]>768)return;
        x=xn;
        y=yn;
    }
    if(placeN[x][y])
    {
        aa=place[x][y]&0x00ff;
        bb=(place[x][y]>>8);
        if(bb<1||bb>2)return;
        {
            bb--;
            nrb=aa/10; //nr budynku
            nrp=aa-nrb*10;// nr postaci
            castle[0].DisableUnits();
            castle[1].DisableUnits();
            grupa=0;
            select.IFF=bb;   // przynaleznosc
            if(nrp>3||nrb>19) //jeden z zolnierzy
            {
                if(!bb)mouseMode=1;//tylko dla naszych
                select.co=1;
                if(nrb<20)
                {
                    selectM=&castle[bb].b[nrb].m[nrp-4];
                    select.nrm=nrp-4;
                    selectM->wybrany=1;
                }
                else
                {
                    nrp=(place[x][y]&0x00ff)-200;
                    nrb=20;
                    select.nrm=nrp;
                    selectM=&castle[bb].m[nrp];
                    selectM->wybrany=1;
                }
                select.nrb=nrb;
                //komunikat
                int kk=0;
                if(select.IFF==master)
                {
                    switch (selectM->type)
                    {
                        case 0:strcpy(Msg.msg,"Muuu ?");break;
                        case 1:strcpy(Msg.msg,"Tak ?");break;
                        case 10:
                        case 12:
                        case 2:strcpy(Msg.msg,"Co ?");break;
                        case 3:strcpy(Msg.msg,"Tak panie ?");break;
                        case 11:
                        case 4:strcpy(Msg.msg,"Tak ?");break;
                        case 5:strcpy(Msg.msg,"Na rozkaz");break;
                        case 6:strcpy(Msg.msg,"Obecny");break;
                        case 7:strcpy(Msg.msg,"Czekam na rozkazy.");break;
                        case 8:strcpy(Msg.msg,"Grrh?");break;
                        case 9:strcpy(Msg.msg,"");break;
                    }
                    Msg.licznik=20;
                    kk=25+selectM->type*11+Msg.ddzwiek;
                    if(!selectM->type)kk=25;

                }
                else
                {
                    kk=180+(x&3); //obcy goscie
                    if(selectM->type==3)kk=179; //obca druidka
                    if(!selectM->type||selectM->type>7)kk=0;
                }
                if(Msg.dzwiek<kk)
                {
                    Msg.dzwiek=kk;
                    Msg.X=ScreenX;
                    Msg.Y=ScreenY;
                }
                if(select.co==1&&select.IFF==master)
                {
                    int comm=0;
                    if(selectM->type&&selectM->command==5)comm=1;
                    if(selectM->type==1&&selectM->command==8)comm=2;
                    ShowPanel(select.IFF,selectM->type,1,selectM->magic,comm);
                }
                else
                    ShowPanel(0,0,0,0,0);
                
                return;
            }
            else
            {
                mouseMode=0;
                select.co=0;    // budynek
                if(nrb>19)return; //nie powinno nigdy wystapic
                select.nrb=nrb; // nr budynku
                selectB=&castle[bb].b[nrb];
                if(Msg.dzwiek<19)
                {
                    Msg.dzwiek=19;
                    Msg.X=ScreenX;
                    Msg.Y=ScreenY;
                }
                if(select.IFF==master)
                {
                    ShowPanel(select.IFF,selectB->type+19,castle[0].milk,selectB->maxfood-selectB->food,0);
                }
                else
                    ShowPanel(0,0,0,0,0);
                return;
            }
        }
    }
return;
}

if(!mode)
{
if(x2<x1){x=x1;x1=x2;x2=x;}
if(y2<y1){y=y1;y1=y2;y2=y;}
x1=(int)((x1-11)>>4)+ScreenX;
y1=(int)((y1-8)/14)+ScreenY;
x2=(int)((x2-11)>>4)+ScreenX;
y2=(int)((y2-8)/14)+ScreenY;
if(x2==x1&&y2==y1)return;

wybrany1=0;
for(x=x1;x<=x2;x++)
  for(y=y1;y<=y2;y++)
     if(place[x][y]>255&&place[x][y]<768)
          {
          aa=place[x][y]&0x00ff;
          bb=(place[x][y]>>8);
          if(bb==1)  //tylko nasi
                {
                nrb=aa/10; //nr budynku
                nrp=aa-nrb*10;// nr postaci

                if(nrp>3||nrb>19) //jeden z zolnierzy
                        {
                        if(!wybrany1)
                {
                castle[0].DisableUnits();
                        castle[1].DisableUnits();

                }

                        if(wybrany1)grupa=1;
                        wybrany1++;
                        select.IFF=0;
                        mouseMode=1;//tylko dla naszych
                        select.co=1;
                        if(nrb<20)
                              {
                              selectM=&castle[0].b[nrb].m[nrp-4];
                              select.nrm=nrp-4;
                              selectM->wybrany=1;
                              }
                           else
                              {
                              nrp=aa-200;
                  nrb=20;
                  select.nrm=nrp;
                              selectM=&castle[0].m[nrp];
                              selectM->wybrany=1;
                              }
                        select.nrb=nrb;
                        }

                  }
   }
if(grupa&&selectM->type&&selectM->type<8)
{
    Msg.dzwiek=168+Msg.ddzwiek;
    Msg.X=ScreenX;
    Msg.Y=ScreenY;
}
if(select.co==1)
        {int comm=0;
            if(selectM->type&&selectM->command==5)comm=1;
            if(selectM->type==1&&selectM->command==8)comm=2;
            if(select.IFF!=master)comm=0;
        ShowPanel(select.IFF,selectM->type,1,selectM->magic,comm);
            }
     }
}
///////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////
void RefreshScreen(void)
{
    showAll=1;
}
