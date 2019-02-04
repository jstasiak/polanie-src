
#include "image13h.h"
#include <conio.h>
#include <dos.h>
#include <i86.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char manufacturer;
  char version;
  char encoding;
  char BitsPerPixel;
  short int x1, y1;
  short int x2, y2;
  short int hres;
  short int vres;
  char palette[48];
  char reserved;
  char colour_planes;
  short int BytesPerLine;
  short int PaletteType;
  char dummy[58];
} PCXHEADER;

PCXHEADER Header;

int UnpackPCX(FILE *, char *, int, int);
extern char *VirtualScreen;
char *picture;

main() {
  FILE *f;
  char palette[768];
  int Width, Height, dx = 0;

  Init13h();
  FILE *o = fopen("pic.dat", "wb");
  if (!o)
    return 0;
  int j = 0;
  int k = 0;
  char path = 1;
  for (int i = 1; i < 40; i++) {
    char name[20];
    if (i < 10)
      sprintf(name, "tlo0%d.pcx", i);
    else
      sprintf(name, "tlo%d.pcx", i);
    /*f=fopen(name,"ab+");
    if(f!=NULL)
    {
      fseek(f,0x044,0);
      path=1;
      fwrite(&path,1,1,f);
      fseek(f, -768L, SEEK_END);
      path=230;
      fwrite(&path,1,1,f);
      path=200;
      fwrite(&path,1,1,f);
      path=150;
      fwrite(&path,1,1,f);
      fclose(f);
    }
    */
    f = fopen(name, "rb");
    if (f != NULL) {
      fread(&Header, sizeof(PCXHEADER), 1, f);
      Width = (Header.x2 - Header.x1) + 1;
      Height = (Header.y2 - Header.y1) + 1;
      UnpackPCX(f, VirtualScreen, Width, Height);
      ShowVirtualScreen();
      fseek(f, -768L, SEEK_END);
      fread(palette, 1, 768, f);
      fclose(f);
      fwrite(palette, 1, 768, o);
      short k[3] = {320, 200, 0x0101};
      // fwrite(k,6,1,o);
      fwrite(VirtualScreen, 1, 64000, o);
      j++;
    } else
      k++;
  }
  fclose(o);
  Close13h();
  printf("Zapisalem %d z 40 obrazkow.\n Nie znalazlem %d obrazkow.\n", j, k);
  return (0);
}

///////////////////////////////////////////////////////////////////////////////
//
//      UnpackPCX
//
///////////////////////////////////////////////////////////////////////////////
UnpackPCX(FILE *fh, char *Buffer, int Width, int Height) {
  /*  Note that this is NOT a properly robust PCX reader - it's for the purposes
     of this example only, and it makes a lot of assumptions, so it's easy to
     fool  */

  char palette[768];
  char *wbuf, *wptr, c, l;
  int j, k, WHeight;

  /* Read and set the palette */
  fseek(fh, -768L, SEEK_END);
  fread(palette, 768, 1, fh);

  // SetPalette256(palette);

  /* Reposition to the start of the image data, and continue */
  fseek(fh, sizeof(PCXHEADER), SEEK_SET);
  wptr = wbuf = (char *)malloc(Width);

  if (200 > Height)
    WHeight = Height;
  else
    WHeight = 200;

  for (j = 0; j < WHeight; j++) {
    for (k = 0; k < Width;) {
      c = fgetc(fh);
      if (c > 191) {
        l = c - 192;
        c = fgetc(fh);
        memset(wptr, c, l);
        wptr += l;
        k += l;
      } else {
        *wptr = c;
        wptr++;
        k++;
      }
    }
    if (320 >= Width)
      memcpy(Buffer + (j * 320), wbuf, Width);
    else
      memcpy(Buffer + (j * 320), wbuf, 320);

    wptr = wbuf;
  }

  return 0;
}
