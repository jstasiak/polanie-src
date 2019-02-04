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
int placeG[MaxX][MaxY];    
int grassCounter,waterCounter,fireCounter;


int Xe[20]={11,27,43,59,75,91,107,123,139,155,171,187,203,219,235,251,267,283,299,315};
int Ye[16]={8,22,36,50,64,78,92,106,120,134,148,162,176,190,204,218};
int maskaT[16]={2,3,4,6,7,5,8,4,3,4,8,4,2,3,3,5};
int kol[4]={LightYellow,Yellow,DarkYellow,Yellow};
char *fire[14];
char *tree[14];
extern int place[MaxX][MaxY];
extern char placeN[MaxX][MaxY];
extern int attack[MaxX][MaxY];
extern int color1,color2,drzewa;
extern int  WriteMode13h; //0-copy put 1-xor put
extern char MapY;
extern struct MMessage Msg;

extern char *picture[MaxPictures],*missiles[4][3][3],*tlo,*Mysz[13];
extern char *movers[5][10][3][3],*ramka[4];   //faza:typ:dx:dy
extern char *buttons[15];
extern char *Hit[2],*dead[3];
extern char *shadow;
extern char *face[16];//0-9 twarze, 10-15 budynki
extern char *Buttons[10];

//====================================================================
void ClearAttack(void);
void ShowBattleMap(int,int);
void Droga(int,int,int);
void Woda(int,int,int);
void ShowPlace(int,int);
void ShowTrees(int,int);
//---------------------------------------------------------------------
void ShowPlace(int X,int Y)
{
    
    int i,j,k=0;
    kol[0]=LightYellow;
    kol[1]=Yellow;
    kol[2]=DarkYellow;
    kol[3]=LightYellow;
    for(j=Y;j<Y+13;j++)
    {
        for(i=X;i<X+16;i++)
        {
            if(placeN[i][j])
            {
                if(placeG[i][j]<9)
                {
                    k=i+j*5;k=k&0x0f;
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[maskaT[k]],0);// trawa
                    if(placeG[i][j]<6)PutImage13h(Xe[i-X],Ye[j-Y],picture[1],1);// wyjedzona
                    if(placeG[i][j]<3)PutImage13h(Xe[i-X],Ye[j-Y],picture[0],1);// wyjedzona bardzo
                }
    
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
                if(placeG[i][j]>112&&placeG[i][j]<120)PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0);//trawa pod drzewem
                if(placeG[i][j]<266&&placeG[i][j]>126&&place[i][j]<512)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0); //zamek i ruina nasz
                if(placeG[i][j]<266&&placeG[i][j]>126&&place[i][j]>511)PutImageChange13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],0,color1,color2); //zamek i ruina ich
                if(place[i][j]==2)PutImage13h(Xe[i-X],Ye[j-Y],picture[156],1);// pale
                if(placeG[i][j]==256)PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0); //swiete miejsce
                if(placeG[i][j]==301)
                {
                   
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[69],1,Red,kol[(fireCounter&3)]);
                }
                if(placeG[i][j]==300)
                {
                    PutImage13h(Xe[i-X],Ye[j-Y],picture[2],0); //swiete miejsce - przemiana
                    PutImageChange13h(Xe[i-X],Ye[j-Y],picture[282+(waterCounter&1)],1,Red,LightRed);
                }    
                if(placeN[i][j]==3)PutImage13h(Xe[i-X],Ye[j-Y],fire[12],1); //popiul duzy
                if(placeN[i][j]==2)PutImage13h(Xe[i-X],Ye[j-Y],fire[13],1); //popiul maly
                if(placeG[i][j]==70)PutImage13h(Xe[i-X],Ye[j-Y],tree[1],1); //lezy drzewo brazowe
                if(placeG[i][j]==72)PutImage13h(Xe[i-X],Ye[j-Y],tree[8],1); //lezy drzewo spalone
                if(placeG[i+1][j]==70)PutImage13h(Xe[i-X],Ye[j-Y],tree[0],1); //lezy drzewo brazowe
                if(placeG[i+1][j]==72)PutImage13h(Xe[i-X],Ye[j-Y],tree[7],1); //lezy drzewo spalone
    
            }
        }
    }
    for(j=Y;j<Y+13;j++)
    {
        for(i=X;i<X+16;i++)
        {
            if(placeG[i][j]>21&&placeG[i][j]<25)
            if(placeN[i][j])
            PutImage13h(Xe[i-X]-3,Ye[j-Y]-2,picture[placeG[i][j]],1);// sucha ziemia
        }
    }
    for(j=Y;j<Y+13;j++)
    {
        for(i=X;i<X+16;i++)
        {
            if(!(placeN[i][j]))
            {
                PutImage13h(Xe[i-X]-9,Ye[j-Y]-7,shadow,1); //przykryj
            }
            
        }
    }
    if(Msg.x>=X&&Msg.y>=Y&&Msg.x<X+16&&Msg.y<Y+13&&Msg.count)
    {
    
        if(Msg.count==4)Rectangle13h(Xe[Msg.x-X]-7,Ye[Msg.y-Y]-2,Xe[Msg.x-X]+14,Ye[Msg.y-Y]+15,White);
        if(Msg.count==3)Rectangle13h(Xe[Msg.x-X]-4,Ye[Msg.y-Y],Xe[Msg.x-X]+11,Ye[Msg.y-Y]+13,LightGray);
        if(Msg.count==2)Rectangle13h(Xe[Msg.x-X]-1,Ye[Msg.y-Y]+2,Xe[Msg.x-X]+8,Ye[Msg.y-Y]+11,Gray);
        if(Msg.count==1)Rectangle13h(Xe[Msg.x-X]+1,Ye[Msg.y-Y]+4,Xe[Msg.x-X]+6,Ye[Msg.y-Y]+9,Gray);
        if(Msg.count==10)Rectangle13h(Xe[Msg.x-X]-7,Ye[Msg.y-Y]-2,Xe[Msg.x-X]+14,Ye[Msg.y-Y]+15,Yellow);
        if(Msg.count==9)Rectangle13h(Xe[Msg.x-X]-4,Ye[Msg.y-Y],Xe[Msg.x-X]+11,Ye[Msg.y-Y]+13,LightRed);
        if(Msg.count==8)Rectangle13h(Xe[Msg.x-X]-1,Ye[Msg.y-Y]+2,Xe[Msg.x-X]+8,Ye[Msg.y-Y]+11,Red);
        if(Msg.count==7)Rectangle13h(Xe[Msg.x-X]+1,Ye[Msg.y-Y]+4,Xe[Msg.x-X]+6,Ye[Msg.y-Y]+9,Red);
        
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
                if(placeN[i][j]<190)
                {
                    if(j<Y+13)PutImage13h(Xe[i-X]-8,Ye[j-Y],picture[placeG[i][j]],1); //drzewo dol
                    if(j>Y)PutImage13h(Xe[i-X]-8,Ye[j-1-Y],picture[placeG[i][j]+7],1); //
                    //fire gora
                    if(j>Y&&placeN[i][j]>74&&placeN[i][j]<91)PutImage13h(Xe[i-X],Ye[j-1-Y]+8,fire[(fireCounter+i+j+1)&3],1); //fire big
                    if(j>Y&&placeN[i][j]>70&&placeN[i][j]<75)PutImage13h(Xe[i-X],Ye[j-1-Y]-7,fire[8+((fireCounter+i+j)&3)],1); //fire small
                    if(j>Y&&placeN[i][j]>70&&placeN[i][j]<75)PutImage13h(Xe[i-X],Ye[j-1-Y]+4,fire[(fireCounter+i+j)&3],1); //fire big
                    
                }
                if(placeN[i][j]>194&&placeN[i][j]<201)
                {//drzewo brazowe
                    if(j>Y)
                    {
                        PutImage13h(Xe[i-1-X],Ye[j-1-Y],tree[4],1); //
                        if(i>X)PutImage13h(Xe[i-X],Ye[j-1-Y],tree[5],1); //
                    }
                    if(j<Y+13)PutImage13h(Xe[i-X],Ye[j-Y],tree[6],1); //
                }//1 faza przewrotu
                if(placeN[i][j]>190&&placeN[i][j]<195)
                {
                    if(i>X)PutImage13h(Xe[i-X],Ye[j-Y],tree[3],1); //
                    if(j<Y+13)PutImage13h(Xe[i-1-X],Ye[j-Y],tree[2],1); //
                }//2 faza przewrotu
            }
            if(placeG[i][j]==71||placeG[i][j]==73) //drzewo spalone
            {
                if(placeN[i][j]<190)
                {
                    if(j<Y+13)PutImage13h(Xe[i-X],Ye[j-Y],picture[placeG[i][j]],1); //drzewo dol
                    if(j>Y)PutImage13h(Xe[i-X],Ye[j-1-Y],picture[placeG[i][j]-1],1); //
                }
                if(placeN[i][j]>194&&placeN[i][j]<201)
                {//drzewo brazowe
                    if(j>Y)
                    {
                        PutImage13h(Xe[i-1-X],Ye[j-1-Y],tree[11],1); //
                        if(i>X)PutImage13h(Xe[i-X],Ye[j-1-Y],tree[12],1); //
                    }
                    if(j<Y+13)PutImage13h(Xe[i-X],Ye[j-Y],tree[13],1); //
                 }//1 faza przewrotu
                 if(placeN[i][j]>190&&placeN[i][j]<195)
                 {
                    if(i>X)PutImage13h(Xe[i-X],Ye[j-Y],tree[10],1); //
                    if(j<Y+13)PutImage13h(Xe[i-1-X],Ye[j-Y],tree[9],1); //
                 }//2 faza przewrotu
            }
            
            ///////////// fire /////////////

            if(j<Y+13&&placeN[i][j]>90&&placeN[i][j]<101)//fire big
            {
                PutImage13h(Xe[i-X],Ye[j-Y],fire[((fireCounter+i+j)&3)],1); 
            }
            if(j<Y+13&&placeN[i][j]>72&&placeN[i][j]<91)//fire med
            {
                if(placeG[i][j]<113||placeG[i][j]>119)PutImage13h(Xe[i-X],Ye[j-Y],fire[12],1);
                PutImage13h(Xe[i-X],Ye[j-Y],fire[4+((fireCounter+i+j)&3)],1);
            } 
            if(j<Y+13&&placeN[i][j]>=70&&placeN[i][j]<73)//fire small
            {
                if(placeG[i][j]<113||placeG[i][j]>119)PutImage13h(Xe[i-X],Ye[j-Y],fire[12],1);
                PutImage13h(Xe[i-X],Ye[j-Y],fire[8+((fireCounter+i+j)&3)],1);
            } 
            
        }
            
    }
}
}
//--------------------------------------------------------------------------
void ClearAttack(void)
{
    int i,j;
    for(j=0;j<MaxY;j++)
    for(i=0;i<MaxX;i++)
    {
        if(placeG[i][j]==277&&placeN[i][j]>225)//postaw palisade
        {
            attack[i][j]=0;
            placeG[i][j]=266;
            place[i][j]=256+512+drzewa;
            drzewa++;
            Droga(i,j,266);
            Droga(i-1,j,266);
            Droga(i+1,j,266);
            Droga(i,j-1,266);
            Droga(i,j+1,266);
            Droga(i,j,35);
            Droga(i-1,j,35);
            Droga(i+1,j,35);
            Droga(i,j-1,35);
            Droga(i,j+1,35);
            placeN[i][j]=1;
        }
        if(place[i][j]==2&&placeN[i][j]>225)//postaw most
        {
            place[i][j]=0;
            attack[i][j]=0;
            placeG[i][j]=47;            // <<<<<------------------postaw most
            placeN[i][j]=1;
        }
        if(place[i][j]<256&&attack[i][j]!=0)attack[i][j]=0;
        if(placeG[i][j]>265&&placeG[i][j]<278&&(attack[i][j]>120||attack[i][j]<0))    //jezeli palisada
        {
            attack[i][j]=0;               //to rozwal palisade
            placeG[i][j]=277;
            place[i][j]=0;
            placeN[i][j]=1;
            //if(Msg.dzwiek<15){Msg.dzwiek=15;Msg.X=i;Msg.Y=j;}
        }
        if(placeN[i][j]>70&&placeN[i][j]<101)
        {   //fire
            if(!(grassCounter&7))
            {
                attack[i][j]+=3;
                /*if((placeG[i][j]<112||placeG[i][j]>119)&&place[i][j]<0x1000)//??????
                {
                    attack[i][j]+=3;
                }*/
                placeN[i][j]--;
            }
        }
        if(placeN[i][j]>190&&placeN[i][j]<201)//drzewo walace sie
        {
            placeN[i][j]--;
        }

        if(placeG[i][j]>112&&placeG[i][j]<120&&attack[i][j]>150)    //jezeli drzewo
        {
            attack[i][j]=0;               //to obcinaj
            placeN[i][j]=200;
            if(Msg.dzwiek<15){Msg.dzwiek=15;Msg.X=i;Msg.Y=j;}
        }
        if(attack[i][j]>75&&(placeG[i][j]==71||placeG[i][j]==73))    //jezeli drzewo (spalone)
        {
            attack[i][j]=0;               //to obcinaj
            placeN[i][j]=200;
            if(Msg.dzwiek<15){Msg.dzwiek=15;Msg.X=i;Msg.Y=j;}
        }
        if(placeN[i][j]==70)
        {
            placeN[i][j]=3; //popiol
            if(placeG[i][j]>111&&placeG[i][j]<116)placeG[i][j]=73; //drzewo igl
            if(placeG[i][j]>115&&placeG[i][j]<120)placeG[i][j]=71; //drzewo lisc
        }
        if(placeN[i][j]==190&&placeG[i][j]<112)
        {
            placeN[i][j]=1;
            place[i][j]=0;
            placeG[i][j]=72;  //pieniek
            if(placeG[i-1][j]<113)attack[i-1][j]=110;
        }
        if(placeN[i][j]==190&&placeG[i][j]>111)
        {
            placeN[i][j]=1;
            place[i][j]=0;
            placeG[i][j]=70;
            if(placeG[i-1][j]<113)attack[i-1][j]=110;
        }
    }
    grassCounter++;
    if(grassCounter&1)fireCounter++;
    if(fireCounter>3){fireCounter=0;waterCounter++;}
    if(waterCounter>2)waterCounter=0;
    if(grassCounter>300)
    {
        grassCounter=0;
        for(j=1;j<MaxY-1;j++)
        for(i=1;i<MaxX-1;i++)
        {
            if(placeG[i][j]<8)placeG[i][j]++; //odrastanie trawy
            if(placeN[i][j]==2||placeN[i][j]==3)placeN[i][j]--;
        }
    }

}
//----------------------------------------------------------------------------
void ShowBattleMap(int Xe,int Ye)
{
int i,j,Y=8,X=11;
if(!MapY)return;
Bar13h(11,8,77,MapY+10,Green);
for(j=1;j<=MapY;j++)
    {
    for(i=1;i<MaxX-1;i++)
           if(placeN[i][j])
                {
                if(placeG[i][j]>265&&placeG[i][j]<277)PutPixel13h(i+X,j+Y,LightBrown); //palisada
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
if(MapY==64)PutImageChange13h(X+Xe,Y+Ye,Mysz[5],1,Red,White);
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

if(!k&&typ<200){placeG[x][y]=0;return;}
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
placeG[x][y]=8;
switch (k)
    {
        case 0: placeG[x][y]=60;break;
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
