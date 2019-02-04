/////////////////////////////////////////////////////////////////////
//   image13h.cpp
//    kompilowac w modelu Large
////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <dos.h>
#include <mem.h>
#include <string.h>
#include "image13h.h"
//////////////////////////////////////////////////////////////////
// Zmienne srodowiskowe
//////////////////////////////////////////////////////////////////
FILE *palettefile,*graphicfile;
char  *VirtualScreen;
char  *RealVirtualScreen;

char  *rgb=NULL,*Buffer330=NULL,*Rgb=NULL;
int length[91]={5,3,5,7,7,7,7,7,5,5,7,7,3,5,3,7,7,6,7,7,7,7,7,7,7,7,3,3,9,7,9,7,8,
        9,8,8,8,7,7,9,8,3,6,8,7,9,8,9,7,9,8,8,7,8,9,12,8,9,8,8,8,8,7,8,
        5,7,7,7,7,7,5,7,7,3,3,6,3,9,7,7,7,7,5,7,4,7,7,11,7,7,7};
char * index[91];
int lineLength=320;

int ClipX1=0,ClipX2=319,ClipY1=0,ClipY2=199;
///////////////////////////////////////////////////////////////////////
// inicjowanie ekranu virtualnego
///////////////////////////////////////////////////////////////////////
int InitVirtualScreen(void)
{
RealVirtualScreen=(char  *)malloc(64000);
if(RealVirtualScreen==NULL)return 0;
memset(RealVirtualScreen,0,64000);
return 1;
}
///////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////
void ClearScreen13h()
{
    memset(VirtualScreen,0,64000);             
}
//////////////////////////////////////////////////////////////////////
//kopiowanie virtualnego obrazu na ekran
//////////////////////////////////////////////////////////////////////
void ShowVirtualScreen(void)
{
if(RealVirtualScreen==NULL)return;
memcpy((void*)0xA0000,(void*)RealVirtualScreen,63680);

}
///////////////////////////////////////////////////////////////////////////
// Zwalnianie pamieci zajmowanej przez ekran
//////////////////////////////////////////////////////////////////////
void FreeVirtualScreen(void)
{
if(RealVirtualScreen==NULL)return;
free(RealVirtualScreen);
RealVirtualScreen=NULL;
}
////////////////////////////////////////////////////////////////////////////
// ustawienie aktualnego ekranu dla funkcji graficznych
//   0-rzeczywisty       1-wirtualny
//////////////////////////////////////////////////////////////////////////
void SetScreen(int Screen)
{
if((Screen)&&(RealVirtualScreen!=NULL))VirtualScreen=RealVirtualScreen;
     else VirtualScreen=(char *)0xA0000;
}
///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void Init13h(void)
{
    union REGS r;
    
    r.h.ah = 0x0;
    r.h.al = 0x13;
    int386(0x10, &r, &r);
    SetScreen(0);
    memset((void*)0xA0000,0,64000);             
   
}
///////////////////////////////////////////////////////////
int InitBuffers13h()
{
    if((rgb=(char*)malloc(768))==NULL)return 1;
    if((Rgb=(char*)malloc(768))==NULL)return 1;
    if((Buffer330=(char  *)malloc(330))==NULL)return 1;
    return 0;
}
//////////////
void FreeBuffers13h()
{
    if(rgb!=NULL)free(rgb);
    if(Rgb!=NULL)free(Rgb);
    if(Buffer330!=NULL)free(Buffer330);
}

/////////////////////////////////////////////////////////////////
//END13H
///////////////////////////////////////////////////////////////
void Close13h(void)
{
    union REGS r;
    r.h.ah = 0x0;
    r.h.al = 0x03;
    int386(0x10, &r, &r);
}
///////////////////////////////////////////////////////////
//
//   SetClippingArea
//
///////////////////////////////////////////////////////////
void SetClippingArea13h(int x1, int y1, int x2,int y2)
{
    ClipX1=x1;
    ClipX2=x2;
    ClipY1=y1;
    ClipY2=y2;
}

///////////////////////////////////////////////////////////////////////
//  rysowanie punktu
///////////////////////////////////////////////////////////////////////
void PutPixel13h(int x,int y,int color)
{
    VirtualScreen[x+y*320]=(unsigned char )color;
}


//////////////////////////////////////////////////////////////////
// Load image from file "name" to picture
//////////////////////////////////////////////////////////////////

char* LoadImage13h(char*name)
{//return adres do obrazka lub NULL w przypadku wystapienia bledu
    FILE *file;
    char *picture;
    short int sizex,sizey;
    
    file=fopen(name,"rb");
    if(file==NULL)return NULL;
    fread(&sizex,2,1,file);
    fread(&sizey,2,1,file);
    picture=(char*)malloc(GetImageSize13h(0,0,sizex,sizey));
    if(picture==NULL){fclose(file);return NULL;}
    fseek(file,0,0);
    fread(picture,1,GetImageSize13h(0,0,sizex-1,sizey-1),file);
    fclose(file);
    return picture;    
}
      
////////////////////////////////////////////////////////////////////
int LoadToScreen13h(int offset,int line)
{
    int i=0,j=1;
    int Offset=offset*33000;
    short size;

    if(graphicfile==NULL)return 1;
         
    fseek(graphicfile,Offset,0);
    fread(&size,2,1,graphicfile);
    fread(&size,2,1,graphicfile);
    fread(&size,2,1,graphicfile);
    if(line)j=0;
    for(i=0;i<99+j;i++)
    {
        size=fread((void*)(VirtualScreen+(line*320)+(i*320)),1,319,graphicfile);
        if(size!=319)j=2;
    }    
    
    return j;
}

////////////////////////////////////////////////////////////////////////////
// Save picture image to file "name"
//////////////////////////////////////////////////////////////////////////
int SaveImage256(char*name,char*picture)
{//return 0 if OK
    FILE *file;
    
    short int sizex,sizey,*buf=(short int*)picture;
    file=fopen(name,"wb");
    if(file==NULL)return 1;
    sizex=buf[0];
    sizey=buf[1];
    fwrite(picture,1,GetImageSize13h(0,0,sizex-1,sizey-1),file);
    fclose(file);
    return 0;
}

//////////////////////////////////////////////////////////////////
//  laduje bitmape pod adres map
//////////////////////////////////////////////////////////////////
void GetImage13h(int x1,int y1,int x2,int y2,char* picture)
{
    int i,j;
    short int *Buf=(short*)picture;
    
    
    Buf[0]=(short)(x2-x1);
    Buf[1]=(short)(y2-y1);    
    picture[4]=1;
    picture[5]=0;
    for(j=0;j<y2-y1;j++)
    {
       for(i=0;i<x2-x1;i++)picture[6+i+j*(x2-x1)]=VirtualScreen[i+x1+(j+y1)*320];
    }
}
////////////////////////////////////////////////////////////////////////
//
//       GetImageSize256
//
////////////////////////////////////////////////////////////////////////
int GetImageSize13h(int x1,int y1,int x2,int y2)
{
    return (x2-x1+1)*(y2-y1+1)+6;
}
//////////////////////////////////////////////////////////////////
//  wyswietla bitmape
//////////////////////////////////////////////////////////////////
void PutImage13h(int x,int y,char *picture,int how)
{
    int length,height,j,line,picturePos;
    short int *Buf=(short int *)picture;
    char color;
    if(x<0||y<0)return;
    if(!how)
    {
        length=Buf[0];
        height=Buf[1];
        if(y+height>199)height=199-y;
        line=(y*320)+x;//line poczatek linii na ekranie
        picturePos=6;
        for(j=0;j<height;j++)
        {
                memcpy((void*)&VirtualScreen[line],(void*)&picture[picturePos],length);         
                picturePos+=length;   
                line+=320;
        }
    }
    else
    {
        length=Buf[0];
        height=Buf[1];
        if(y+height>479)height=479-y;
        picturePos=6;
        line=(y)*320+x;//line poczatek linii na ekranie
        for(j=0;j<height;j++)
        {
                for(int i=0;i<length;i++)
                {
                         color=picture[picturePos];
                         picturePos++;       
                         if(color)
                         {
                             VirtualScreen[i+line]=color;
                         }
                }
                line+=320;
        }
    }    
}


//////////////////////////////////////////////////////////////////
//  wyswietla bitmape i zamienia kolor c1 na c2
//////////////////////////////////////////////////////////////////
void PutImageChange13h(int x,int y,char *picture,int how,char c1,char c2)
{
    int length,height,j,line,picturePos;
    short int *Buf=(short int *)picture;
    char color;
    
    length=Buf[0];
    height=Buf[1];
    if(y+height>479)height=479-y;
    picturePos=6;
    line=(y)*320+x;//line poczatek linii na ekranie
    for(j=0;j<height;j++)
    {
            for(int i=0;i<length;i++)
            {
                     color=picture[picturePos];
                     picturePos++;       
                     if(color||!how)
                     {
                         if(color==c1)
                         {
                             VirtualScreen[i+line]=c2;
                         }
                         else
                         {
                             VirtualScreen[i+line]=color;
                         }
                     }
            }
            line+=320;
    }
    
}




//////////////////////////////////////////////////////////////////
//  wyswietla bitmape inwersyjnie i zamienia kolor c1 na c2
//////////////////////////////////////////////////////////////////
void PutImageChangeInverse13h(int x,int y,char  *picture,int how,char c1,char c2)
{
int length,height,j,line,picturePos;
    short int *Buf=(short int *)picture;
    char color;
    
    length=Buf[0];
    height=Buf[1];
    if(y+height>479)height=479-y;
    picturePos=5;
    line=(y)*320+x;//line poczatek linii na ekranie
    for(j=0;j<height;j++)
    {
            picturePos+=length;        
            for(int i=0;i<length;i++)
            {
                     color=picture[picturePos];
                     picturePos--;       
                     if(color||!how)
                     {
                         if(color==c1)
                         {
                             VirtualScreen[i+line]=c2;
                         }
                         else
                         {
                            VirtualScreen[i+line]=color;
                         }
                     }
            }
            line+=320;
            picturePos+=length;        
    }
        
}

//////////////////////////////////////////////////////////////////
//  wyswietla kwadrat
//////////////////////////////////////////////////////////////////
void Rectangle13h(int x1,int y1,int x2,int y2, int color)
{
    int a;
    if(x1>x2)
    {
        a=x1;
        x1=x2;
        x2=a;
    }
    if(y1>y2)
    {
        a=y1;
        y1=y2;
        y2=a;
    } 
    int Y1=y1*lineLength;
    int Y2=y2*lineLength;
    int b=1,c=1,d=1; 
    a=1;
    

    if(x1>=ClipX2||x2<=ClipX1||y1>=ClipY2||y2<=ClipY1) return;
    

    
   
    if(x1<=ClipX1){x1=ClipX1+1;a=0;}
    if(x2>=ClipX2){x2=ClipX2-1;b=0;}
    if(y1<=ClipY1){y1=ClipY1+1;c=0;}
    if(y2>=ClipY2){y2=ClipY2-1;d=0;}

    //SetAreas(x1,y1,x2,y2);
    
    if(c)
    for(int i=x1; i<=x2; i++)
    {
        VirtualScreen[i+Y1]=(short)color;
     
    }
    
    if(d)
    for(int i=x1; i<=x2; i++)
    {
        VirtualScreen[i+Y2]=(short)color;
    }
    
    Y1=y1*lineLength+x1;
    Y2=y1*lineLength+x2;
    
    if(a)
    for(int i=y2-y1; i>0; i--)
    {
        VirtualScreen[Y1+=lineLength]=(short)color;
    }
    
    if(b)
    for(int i=y2-y1; i>0; i--)
    {
       VirtualScreen[Y2+=lineLength]=(short)color;
    }
}

//////////////////////////////////////////////////////////////////
//  wyswietla bar'a
//////////////////////////////////////////////////////////////////
void Bar13h(int x1,int y1,int x2,int y2, int color)
{
    int a;
    int length;
    if((x1==x2)||(y1==y2))return;
    if(x1>x2)
    {
        a=x1;
        x1=x2;
        x2=a;
    }
    if(y1>y2)
    {
        a=y1;
        y1=y2;
        y2=a;
    }
    if(x1>ClipX2||x2<ClipX1||y1>ClipY2||y2<ClipY1) return;
    if(x1<ClipX1)x1=ClipX1;
    if(x2>ClipX2)x2=ClipX2;
    if(y1<ClipY1)y1=ClipY1;
    if(y2>ClipY2)y2=ClipY2;
    if(x1>x2||y1>y2)return;
    if(x1>x2||y1>y2) return;
    
    length=x2-x1;
    for(int j=y1;j<y2;j++)
    {
        memset((void*)&VirtualScreen[x1+j*lineLength],color,length);
    }
}

//--------------------------------------------------------
//            InitText13h
//--------------------------------------------------------
int InitText13h()
{
unsigned int size;
int i,x1,x2;
char * literki;

literki=LoadImage13h("font.dat"); //??????????
if (literki==NULL) return 1;
PutImage13h(0,0,literki,0);
free(literki);

x1 = 7; x2 = 11;
for(i=0;i<33;i++)
 {x2 = x1+length[i]-1;
  size = GetImageSize13h(x1,8,x2,20);
  index[i] = (char*)malloc(size);
  if(index[i]!=NULL)  GetImage13h(x1,8,x2,20,index[i]);
     else return 1;
  x1 = x2;}

x1 = 8; x2 = 16;
for(i=33;i<64;i++)
 {x2 = x1+length[i]-1;
  size = GetImageSize13h(x1,32,x2,44);
  index[i] =(char*) malloc(size);
  if(index[i]!=NULL)  GetImage13h(x1,32,x2,44,index[i]);
     else return 1;
  x1 = x2;}

x1 = 7; x2 = 11;
for(i=64;i<91;i++)
 {x2 = x1+length[i]-1;
  size = GetImageSize13h(x1,56,x2,78);
  index[i] =(char*) malloc(size);
  if(index[i]!=NULL)  GetImage13h(x1,56,x2,78,index[i]);
     else return 1;
  x1 = x2;}

 return 0;
}

//------------------------------------------------------
//               Transform
//------------------------------------------------------
char Transform13h(unsigned char znak)
{
switch(znak)
     {case 134 : return '#';  //a
      case 145 : return '$'; //e
      case 162 : return '%';  //o'
      case 166 : return '&'; //z'
      case 167 : return 39 ; //z
      case 158 : return '*'; //s
      case 141 : return '+'; //c
      case 164 : return 47;  //n
      case 143 : return '<';  //A
      case 144 : return '=';  //E
      case 163 : return '>';  //O
      case 160 : return '@';  //Z'
      case 161 : return '['; //Z
      case 149 : return '\\';  //C
      case 152 : return ']';   //S
      case 156 : return '^';    //L
      case 165 : return '_';   //N
      case 146 : return '`';   //l
      }
  return znak;
}

//--------------------------------------------------------
//             ClearText
//-------------------------------------------------------
void ClearText13h()
{
int i;

for(i=0;i<92;i++) if(index[i]!=NULL) free(index[i]);
}
//--------------------------------------------------------
//             OutTextDelay13h                wypisywanie liter
//-------------------------------------------------------
void OutTextDelay13h(int x,int y,char *text,int colour1,int colour2,int del)
{
char * letter;
unsigned char znak;

while(*text!=NULL)
     {znak=Transform13h(*text);
     if(znak<32||znak>32+91)return;
     letter=(char*)index[znak-32];
     if(letter!=NULL)
     PutImageChange13h(x-1,y,letter,1,255,colour2);
     PutImageChange13h(x,y+1,letter,1,255,colour2);
     PutImageChange13h(x,y,letter,1,255,colour1);
     x=x+length[znak-32]-1;
     if(x>320)return;
     delay(del);
     text++;}
}
//--------------------------------------------------------
//             CenterText13h                wypisywanie liter
//-------------------------------------------------------
void CenterText13h(int xl,int yg,int xp,int yd,char *text,int colour)
{
int x,y,dl=0,wsk;
int i,ile=0;


wsk=strlen(text);
while(*text!=NULL)
{ile=ile+length[*text-32]-1;
if(ile>(xp-xl-12))*text=NULL;
text++; }
for(i=0;i<wsk;i++) text--;

wsk=strlen(text);
while(*text!=NULL)
{dl=dl+length[*text-32]-1;
text++;}
if(xp-xl<dl)return;
if(yd-yg<13)return;
y=(yd-yg+1)>>1;
y=yg+y-7;
x=(xp-xl-dl)>>1;
x=xl+x;
for(i=0;i<wsk;i++) text--;
OutText13h(x,y,text,colour);
}

//--------------------------------------------------------
//             OutText13h                wypisywanie liter
//-------------------------------------------------------
void OutText13h(int x,int y,char *text,int colour)
{
char * letter;
char znak;

while(*text!=NULL)
     {znak=Transform13h(*text);
     letter=index[znak-32];
     PutImageChange13h(x,y,letter,1,255,colour);
     x=x+length[znak-32]-1;
     text++;}
}
//--------------------------------------------------------
//             Write13h                wypisywanie liter
//-------------------------------------------------------

Write13h(int x,int y, int maxx, int maxdl, char *txt, int tcolour,int bcolour)
{
  int cx=0,a,ll,xp,wsk=0,ile=0;
  char k,l;
  char str[2]={0,0};
wsk=strlen(txt);
while(*txt!=NULL)
{ile=ile+length[*txt-32]-1;
if(ile>maxx-12)*txt=NULL;
txt++; }
for(int i=0;i<wsk;i++) txt--;
cx=strlen(txt);
  do
  {
  if(maxx==0)
     Bar13h(x,y,x+(maxdl+1)*11-1,y+14,bcolour);
     else Bar13h(x,y,x+maxx,y+14,bcolour);
    xp=x;
    for(a=0;a<strlen(txt);a++)
       {str[0]=txt[a];
       if(a==cx)
         {Bar13h(xp,y+11,xp+length[txt[a]-32],y+12,tcolour);
          OutText13h(xp,y,str,tcolour);}
       else
          {OutText13h(xp,y,str,tcolour);}
       xp+=length[txt[a]-32]-1;}

    if(cx==strlen(txt))
      {Bar13h(xp,y+11,xp+8,y+12,tcolour);}
    ll=strlen(txt);
    k=getch();
    if(!k)l=getch();
    if(!k)
      switch(l)
           {case  75:if(cx>0)cx--;break;
            case  77:if(cx<ll)cx++;break;
        case  71:cx=0;break;
        case  79:cx=ll;break;
        case  83:if(cx<ll)
           for(a=cx;a<ll;a++)txt[a]=txt[a+1];break;}
    else
      switch(k)
           {case   8:if(cx>0)
                {cx--;
             for(a=cx;a<ll;a++)txt[a]=txt[a+1];}break;
    default:if((k>31)&&(ll<maxdl)&&(xp<maxx+x-18))
{if((k==' ')||(k>='0')&&(k<='z')&&((k<';')||(k>'@'))&&((k<'[')||(k>='a')))
  {for(a=ll+1;a>cx;a--)txt[a]=txt[a-1];
      txt[cx]=k;
      cx++;}break;}
      }
  }
  while((k!=13)&&(k!=27));
  return(k);
}
/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void OpenGraphicFile()
{
    graphicfile=fopen("graf.dat","rb");
}
/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void CloseGraphicFile()
{
    fclose(graphicfile);
}

/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void OpenPaletteFile()
{
    palettefile=fopen("pal.dat","rb");
}
/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void ClosePaletteFile()
{
    fclose(palettefile);
}


/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void LoadExtendedPalette(int pal)
{
        long whence=pal*768;
        if(rgb==NULL)return;
        if(palettefile==NULL)return;
        fseek(palettefile,whence,0);
        fread(rgb,768,1,palettefile);
}

void LoadPalette13h(char* name)
{
        FILE *f=fopen(name,"rb");
        if(f==NULL)return;
        if(rgb==NULL)return;
        if(palettefile==NULL)return;
        fread(rgb,768,1,f);
        fclose(f);
}

//////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////
void SetExtendedPalette()
{
if(rgb==NULL)return;
char* Palette=(char*)rgb;
/* This function sets the palette (will work on any VGA or SVGA card) by programming the card
        directly.  There IS a BIOS function to do it, but doing it directly is both quicker and - for protected
        mode programs - more straightforward   */

short int i;
char *p;//far

for (i=0; i<768; i++)
    Palette[i] = Palette[i] >> 2;

p = Palette;

outp(0x3c6, 0xff);
for (i=0; i<=255; i++)
    {
    outp(0x3c8, i);
    outp(0x3c9, *p++);
    outp(0x3c9, *p++);
    outp(0x3c9, *p++);
    }

return;

}
/////////////////////////////////////////////////////////////////
void DownPalette(int speed)
{

/*
for(int i=1;i<32;i++)
{
   SetExtendedPalette();
}*/
BlackPalette();

}
//////////////////////////////////////////////////////////////////////
void BlackPalette(void)
{
    if(rgb==NULL)return;
    memset(rgb,0,768);
    SetExtendedPalette();
}
/////////////////////////////////////////////////////////////////
void RisePalette(int speed)
{

if(rgb==NULL){return;}
if(Rgb==NULL){SetExtendedPalette();return;}
// przepisanie rgb->Rgb i wyzerowanie rgb
/*
//  ---- rozjasnienie palety
for(int i=1;i<32;i++)
 {

 //SetExtendedPalette();
 
 }
*/


SetExtendedPalette();
}
//////////////////////////////////////////////////////////////////////
