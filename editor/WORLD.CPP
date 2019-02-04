//////////////////////////////////////////////////////////////////////////
//
//  Modul zawiera procedurey sluzace do generacji i wyswietlania pola bitwy
//
///////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "mover.h"
#include "image13h.h"
int placeG[mMaxX][mMaxY];      // Grafika
int grassCounter,waterCounter,fireCounter;

        // tutaj wpisac odpowiednie wartosci
int Xe[20]={11,27,43,59,75,91,107,123,139,155,171,187,203,219,235,251,267,283,299,315};
int Ye[16]={8,22,36,50,64,78,92,106,120,134,148,162,176,190,204,218};
int maskaT[16]={2,3,4,6,7,5,8,4,3,4,8,4,2,3,3,5};
char kol[4]={LightYellow,Yellow,DarkYellow,Yellow};
char *fire[14];
char *tree[14];
extern int place[mMaxX][mMaxY];
extern char placeN[mMaxX][mMaxY];
extern char attack[mMaxX][mMaxY];
extern int color1,color2,drzewa;
extern int  WriteMode13h; //0-copy put 1-xor put
extern char MapY;
extern int MaxX;
extern int MaxY;
extern struct MMessage Msg;

extern char *picture[MaxPictures],*missiles[4][3][3],*tlo,*Mysz[13];
extern char *movers[5][13][3][3],*ramka[4];   //faza:typ:dx:dy
extern char *buttons[15];
extern char *Hit[2],*dead[3];
extern char *shadow;
extern char *face[16];//0-9 twarze, 10-15 budynki
extern char *Buttons[10];

//====================================================================
void ShowBattleMap(int,int);
void Droga(int,int,int);
void Woda(int,int,int);
void ShowPlace(int,int);
void ShowTrees(int,int);
//---------------------------------------------------------------------
void ShowPlace(int X,int Y)
{

    kol[0]=LightYellow;
    kol[1]=Yellow;
    kol[2]=DarkYellow;
    kol[3]=Yellow;
    int i,j,k=0;
    
    for(j=Y;j<Y+13;j++)
    {
        for(i=X;i<X+16;i++)
        {
                    k=i+j*5;k=k&0x0f;
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[maskaT[k]],0);// trawa do edytora
                    
                
                
                if(placeG[i][j]>8&&placeG[i][j]<22)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0);// obrazki
                if(placeG[i][j]>24&&placeG[i][j]<46)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0);// obrazki
                if(placeG[i][j]>45&&placeG[i][j]<54)
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[82+(waterCounter*13)],0); //woda
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],1);// most
                }
                if(placeG[i][j]>53&&placeG[i][j]<68)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0);// obrazki
                if(placeG[i][j]>265&&placeG[i][j]<278)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0);// palisada
                if(placeG[i][j]==68){PutImage13h(Xe[i-X],Ye[j-Y],picture[68],0);PutImage13h(Xe[i-X],Ye[j-Y],fire[8+((fireCounter+i+j)&3)],1);} //male ognisko
                if(placeG[i][j]==69){PutImage13h(Xe[i-X],Ye[j-Y],picture[68],0);PutImage13h(Xe[i-X],Ye[j-Y],fire[4+((fireCounter+i+j)&3)],1);} //duze ognisko
                if(placeG[i][j]>69&&placeG[i][j]<74)PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0); //podstawa pod rzewa wypalone
                if(placeG[i][j]>73&&placeG[i][j]<87)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]+(waterCounter*13)],0); //woda
                if(placeG[i][j]>112&&placeG[i][j]<120)//drzewo      praca!
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0);//trawa pod drzewem
                }
                if(placeG[i][j]>126&&placeG[i][j]<266)
                {
                    if(!(((placeG[i][j]-137)/10)&1))PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0); //zamek i ruina nasz
                    else PutImageChange13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]-10],0,Red,Yellow); //zamek i ruina ich
                }
                if(place[i][j]==2)PutImage13h(Xe[i-X],Ye[j-Y],picture[156],1);// pale w wodzie pod budoewe mostu
                if(placeG[i][j]==256)
                {
                     //swiete miejsce leczenie
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1); 
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[282+(waterCounter&1)],1,Red,LightBlue);
                }
                if(placeG[i][j]==301)//magiczny krag
                {
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[69],1,Red,(char)kol[(fireCounter&3)]);
                }
                if(placeG[i][j]==328)//rozjasnienie poczatkowe
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1);
                    OutText13h(Xe[i-X]+4,Ye[j-Y],"S",Yellow);
                }
                if(placeG[i][j]==329)//postac do zabicia lub uratowania
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1);
                    OutText13h(Xe[i-X]+4,Ye[j-Y]+1,"O",White);
                }
                if(placeG[i][j]==330)//pastwisko wrogow
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1);
                    OutText13h(Xe[i-X]+4,Ye[j-Y]+1,"P",LightRed);
                }
                if(placeG[i][j]==331)//swiete miejsce wrogow
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1);
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[282+(waterCounter&1)],1,Red,LightBlue);
                    OutText13h(Xe[i-X]+4,Ye[j-Y],"W",LightRed);
                }
                if(placeG[i][j]==333)//miejsce zbiorki wrogow
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],1);
                    OutText13h(Xe[i-X]+4,Ye[j-Y],"Z",LightRed);
                }
                if(placeG[i][j]==332)
                {
                    Rectangle13h(Xe[i-X]+1,Ye[j-Y]+1,Xe[i-X]+14,Ye[j-Y]+12,White);//skaly niedorobione
                    
                }
                if(placeG[i][j]==300)
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0); //swiete miejsce - przemiana
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[282+(waterCounter&1)],1,Red,LightRed);
                }    
                if(placeG[i][j]==70)PutImage13h(Xe[i-X],Ye[j-Y],tree[1],1); //lezy drzewo brazowe
                if(placeG[i][j]==72)PutImage13h(Xe[i-X],Ye[j-Y],tree[8],1); //lezy drzewo spalone
                if(placeG[i+1][j]==70)PutImage13h(Xe[i-X],Ye[j-Y],tree[0],1); //lezy drzewo brazowe
                if(placeG[i+1][j]==72)PutImage13h(Xe[i-X],Ye[j-Y],tree[7],1); //lezy drzewo spalone
                //postacie
                switch(placeG[i][j])
                {
                    case 302://nasza krowa
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][0][1][2],1,Red,Red);
                        break;
                    case 303://nasz drwal
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][1][1][2],1,Red,Red);
                        break;
                    case 304://nasz lucznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][2][1][2],1,Red,Red);
                        break;
                    case 305://nasza kaplanka
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][3][1][2],1,Red,Red);
                        break;
                    case 306://nasz kaplan
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][4][1][2],1,Red,Red);
                        break;
                    case 307://nasz woj
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][5][1][2],1,Red,Red);
                        break;
                    case 308://nasza wlocznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][6][1][2],1,Red,Red);
                        break;
                    case 309://nasz rycerz
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][7][1][2],1,Red,Red);
                        break;    
                    case 310://nasza strzyga
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][8][1][2],1,Red,Red);
                        break;
                    case 311://nasz niedzwiedz
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][9][1][2],1,Red,Red);
                        break;
                    case 312://nasz pastuszek
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][10][1][2],1,Red,Red);
                        break;
                    case 313://nasz mag
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][11][1][2],1,Red,Red);
                        break;
                    case 314://nasz kusznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][12][1][2],1,Red,Red);
                        break;    
                    case 315:// krowa
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][0][0][0],1,Red,Yellow);
                        break;
                    case 316:// drwal
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][1][0][0],1,Red,Yellow);
                        break;
                    case 317://lucznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][2][0][0],1,Red,Yellow);
                        break;
                    case 318:// kaplanka
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][3][0][0],1,Red,Yellow);
                        break;
                    case 319:// kaplan
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][4][0][0],1,Red,Yellow);
                        break;
                    case 320:// woj
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][5][0][0],1,Red,Yellow);
                        break;
                    case 321:// wlocznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][6][0][0],1,Red,Yellow);
                        break;
                    case 322://rycerz
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][7][0][0],1,Red,Yellow);
                        break;    
                    case 323://strzyga
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][8][0][0],1,Red,Yellow);
                        break;
                    case 324://niedzwiedz
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][9][0][0],1,Red,Yellow);
                        break;
                    case 325://pastuszek
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][10][0][0],1,Red,Yellow);
                        break;
                    case 326://mag
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][11][0][0],1,Red,Yellow);
                        break;
                    case 327://kusznik
                        PutImageChange13h(Xe[i-X],Ye[j-Y],movers[0][12][0][0],1,Red,Yellow);
                        break;              
                }   
    
            
        }
    }
    for(j=Y;j<Y+13;j++)
    {
        for(i=X;i<X+16;i++)
        {
            if(placeG[i][j]>21&&placeG[i][j]<25)
                PutImage13h(Xe[i-X]-3,Ye[j-Y]-2,picture[placeG[i][j]],1);// sucha ziemia
        }
    }
      
}
////////////////////////////////////////////////////////////////////
void ShowTrees(int X,int Y)
{
int i,j;
for(j=Y;j<Y+14;j++)
{
    for(i=X;i<X+16;i++)
    {
        if(placeN[i][j])
        {
            if(placeG[i][j]>112&&placeG[i][j]<120)//drzewo zywe
            {
                if(j<Y+13)PutImage13h(Xe[i-X]-8,Ye[j-Y],picture[placeG[i][j]],1); //drzewo dol
                if(j>Y)PutImage13h(Xe[i-X]-8,Ye[j-1-Y],picture[placeG[i][j]+7],1); //
                    
            }
            if(placeG[i][j]==71||placeG[i][j]==73) //drzewo spalone
            {
                if(j<Y+13)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],1); //drzewo dol
                if(j>Y)PutImage13h(Xe[i-X],Ye[j-1-Y],picture[placeG[i][j]-1],1); //
            }
            
            
        }
            
    }

}
}
//----------------------------------------------------------------------------
void ShowBattleMap(int Xe,int Ye)
{
int i,j,Y=8,X=11;
if(!MapY)return;
Bar13h(11,8,11+MaxX,MapY+10,LightBrown);
for(j=1;j<=MapY;j++)
    {
    for(i=1;i<MaxX-1;i++)
           if(placeN[i][j])
                {
                if(placeG[i][j]>265&&placeG[i][j]<277)PutPixel13h(i+X,j+Y,DarkBrown); //palisada
                if(placeG[i][j]==277)PutPixel13h(i+X,j+Y,Green); //palisada zburzona
                if(placeG[i][j]>=0&&placeG[i][j]<9)PutPixel13h(i+X,j+Y,Green); //trawa i gadgety
                if(placeG[i][j]>8&&placeG[i][j]<22)PutPixel13h(i+X,j+Y,Gray); //skaly
                if(placeG[i][j]>21&&placeG[i][j]<25)PutPixel13h(i+X,j+Y,LightBrown); //ziemia
                if(placeG[i][j]>30&&placeG[i][j]<54)PutPixel13h(i+X,j+Y,Brown); //droga i most
                if(placeG[i][j]>53&&placeG[i][j]<68)PutPixel13h(i+X,j+Y,Green); // gadgety
                if(placeG[i][j]==68||placeG[i][j]==69)PutPixel13h(i+X,j+Y,Yellow); // ogien
                if(placeG[i][j]==71||placeG[i][j]==73)PutPixel13h(i+X,j+Y,Black); // drzewo spalone
                if(placeG[i][j]==70||placeG[i][j]==72)PutPixel13h(i+X,j+Y,Green); // drzewo spalone
                if(placeG[i][j]>73&&placeG[i][j]<87)PutPixel13h(i+X,j+Y,Blue); //woda
                if(placeG[i][j]>112&&placeG[i][j]<120){PutPixel13h(i+X,j+Y,DarkGreen);} //drzewo
                if(placeG[i][j]>256&&placeG[i][j]<266)PutPixel13h(i+X,j+Y,LightBrown); //ruiny
                if(placeG[i][j]==256)PutPixel13h(i+X,j+Y,Green);//swiete miejsce
                if(place[i][j]>256&&place[i][j]<512)PutPixel13h(i+X,j+Y,White); //nasi
                if(place[i][j]<768&&place[i][j]>511)PutPixel13h(i+X,j+Y,LightRed); //oni
                }
           else PutPixel13h(i+X,j+Y,0);
        }
if(MapY==MaxX-2)PutImageChange13h(X+Xe,Y+Ye,Mysz[5],1,Red,White);
}
//---------------------------------------------------------------------
void Droga(int x,int y,int typ)    //typ=35-droga  266-palisada
{
int k=0;   // wprowadzic duzo zmian
if(placeG[x][y]>typ+10||placeG[x][y]<typ-4)return;
if(typ>200&&placeG[x][y]<typ-1)return;

if(placeG[x][y-1]<typ+11&&placeG[x][y-1]>typ-5)k=1;
if(placeG[x+1][y]<typ+11&&placeG[x+1][y]>typ-5)k+=2;
if(placeG[x][y+1]<typ+11&&placeG[x][y+1]>typ-5)k+=4;
if(placeG[x-1][y]<typ+11&&placeG[x-1][y]>typ-5)k+=8;

//if(!k&&typ<200){placeG[x][y]=0;return;}
if(k==1&&typ<200){placeG[x][y]=34;return;}
if(k==2&&typ<200){placeG[x][y]=31;return;}
if(k==4&&typ<200){placeG[x][y]=33;return;}
if(k==8&&typ<200){placeG[x][y]=32;return;}

if(k==1||k==4){placeG[x][y]=typ+1;return;}
if(k==2||k==8){placeG[x][y]=typ+5;return;}
              //////??????????????????????????????
if(k>7)k--;
if(k>3)k--;
if(k>2)k--;
if(k>1)k--;
if(!k)k=2;
placeG[x][y]=typ-1+k;
}

/////////////////////////////////////////////////////
void Woda(int x,int y,int t)   //woda t=74-86
{
int k=0;
if(placeG[x][y]>t+12||placeG[x][y]<t)return;
if(placeG[x][y-1]<t+13&&placeG[x][y-1]>=t)k+=1;
if(placeG[x+1][y]<t+13&&placeG[x+1][y]>=t)k+=2;
if(placeG[x][y+1]<t+13&&placeG[x][y+1]>=t)k+=4;
if(placeG[x-1][y]<t+13&&placeG[x-1][y]>=t)k+=8;
//mosty
        if(!(k&1)&&placeG[x][y-1]<54&&placeG[x][y-1]>45)k+=1;
        if(!(k&2)&&placeG[x+1][y]<54&&placeG[x+1][y]>45)k+=2;
        if(!(k&4)&&placeG[x][y+1]<54&&placeG[x][y+1]>45)k+=4;
        if(!(k&8)&&placeG[x-1][y]<54&&placeG[x-1][y]>45)k+=8;
//placeG[x][y]=t;
switch (k)
{
        //case 0: //placeG[x][y]=60;break;
        case 3: placeG[x][y]=t;break;
        case 6: placeG[x][y]=t+1;break;
        case 7: placeG[x][y]=t+2;break;
        case 9: placeG[x][y]=t+3;break;
        case 11: placeG[x][y]=t+4;break;
        case 12: placeG[x][y]=t+5;break;
        case 13: placeG[x][y]=t+6;break;
        case 14: placeG[x][y]=t+7;break;
        case 15: placeG[x][y]=t+8;
                 int xx;
                 xx=placeG[x-1][y-1];
             if((xx<46)||(xx>53&&xx<74)||(xx>86))placeG[x][y]=t+9;
                 xx=placeG[x+1][y-1];
                 if((xx<46)||(xx>53&&xx<74)||(xx>86))placeG[x][y]=t+10;
                 xx=placeG[x+1][y+1];
                 if((xx<46)||(xx>53&&xx<74)||(xx>86))placeG[x][y]=t+11;
                 xx=placeG[x-1][y+1];
                 if((xx<46)||(xx>53&&xx<74)||(xx>86))placeG[x][y]=t+12;
         break;
        }
}
