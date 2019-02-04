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
int InitBuffers13h(void);
void FreeBuffers13h(void);
void ClearScreen13h();
void SetScreen(int Screen);
void PutPixel13h(int x, int y, int color);
void Rectangle13h(int x1, int y1, int x2, int y2, int color);
void Bar13h(int x1, int y1, int x2, int y2, int color);
int GetImageSize13h(int x1, int y1, int x2, int y2);
void GetImage13h(int x1, int y1, int x2, int y2, char *map);
void PutImage13h(int x1, int y1, char *map, int typ);
void PutImageChange13h(int x1, int y1, char *map, int typ, char c1, char c2);
void PutImageChangeInverse13h(int x1, int y1, char *map, int typ, char c1,
                              char c2);
int SaveImage13h(char *name, void *picture);
char *LoadImage13h(char *name);
int LoadToScreen13h(int offset, int line);
void OpenGraphicFile(void);
void CloseGraphicFile(void);
void LoadExtendedPalette(int);
void LoadPalette13h(char *name);
void SetClippingArea13h(int x1, int y1, int x2, int y2);
void OpenPaletteFile(void);
void ClosePaletteFile(void);

void SetExtendedPalette(void);
void DownPalette(int);
void RisePalette(int);
void BlackPalette(void);
void ClearText13h();
void OutText13h(int x, int y, char *text, int colour);
void OutTextDelay13h(int x, int y, char *text, int colour1, int colour2,
                     int del);
int InitText13h(); //    il.liter | tekst | kolor liter  i  tla
int Write13h(int x, int y, int maxx, int maxdl, char *, int tcolour,
             int bcolour);
void CenterText13h(int xl, int yg, int xp, int yd, char *text, int colour);
//               lewy gorny     |  prawy dolny
#endif
