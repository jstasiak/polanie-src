//////////////////////////////////////////////////////////////
// image13h.h
///////////////////////////////////////////////////////////////
#ifndef __lida13h_H
#define __lida13h_H

// SIZE-ROZMIAR MAPY BITOWEJ
// MAP:[size,length,height,....data.....]
//     [  2 ,   2  ,   2  ,   size-6    ]
void Init13h(void);
void Close13h(void);
int InitVirtualScreen(void);
void ShowVirtualScreen(void);
void FreeVirtualScreen(void);
void SetScreen(int Screen);
void PutPixel13h(int x, int y, int color);
void Bar13h(int x1, int y1, int x2, int y2, int color);
unsigned int GetImageSize13h(int x1, int y1, int x2, int y2);
void GetImage13h(int x1, int y1, int x2, int y2, void far *map);
void PutImage13h(int x1, int y1, void far *map, int typ);
void PutImageChange13h(int x1, int y1, void far *map, int typ, char c1,
                       char c2);
int SaveImage13h(char *name, void far *picture);
unsigned char far *LoadImage13h(char *name);
void LoadExtendedPalette(char *name);
void SetExtendedPalette(void);
void DownPalette(int);
void RisePalette(int);
void BlackPalette(void);
void ClearText13h();
void OutText13h(int x, int y, unsigned char *text, int colour);
int InitText13h(); //    il.liter | tekst | kolor liter  i  tla
int Write13h(int x, int y, int maxx, int maxdl, char *, int tcolour,
             int bcolour);
void CenterText13h(int xl, int yg, int xp, int yd, char *text, int colour);
//             lewy gorny rog   | prawy dolny |
#endif