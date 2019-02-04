/*  VESA32.C  -  Example 32-bit program to directly update VESA-compliant video systems. */

/* Requires Watcom C ver. 10 and DOS/4GW  */

#include <stdlib.h>
#include <dos.h>
#include <i86.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

int VESA_SetMode(int Mode);

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

int DWidth, DHeight;

main(int argc, char *argv[])
{

union REGS r;
FILE *fh;
int Width, Height, ScreenMode = 0;
char *Buffer, res;


if (argc != 2)
	{
	printf("\nUsage:  VESA32 {PCX file name}\n\n");
	exit(0);
	}

if ( (fh = fopen(argv[1], "rb")) == NULL)
	{
	printf("\nFile %s not found\n\n", argv[1]);
	exit(0);
	}

printf("Enter screen mode: (1=640x480, 2=800x600, 3=1024x768, 4=Exit)\n");
res = 0;
while (res < '1' || res >'4')
   res = getch();

switch (res)
   {
   case '1':
   ScreenMode = 0x0101;
   DHeight = 480;
   DWidth = 640;
   break;

   case '2':
   ScreenMode = 0x0103;
   DHeight = 600;
   DWidth = 800;
   break;

   case '3':
   ScreenMode = 0x0105;
   DHeight = 768;
   DWidth = 1024;
   break;

   case '4':
   exit(0);
   }

fread(&Header, sizeof(PCXHEADER), 1, fh);

Width = (Header.x2 - Header.x1) + 1;
Height = (Header.y2 - Header.y1) + 1;

/* OK, let's see if the card is capable of supporting this resolution */

if (VESA_SetMode(ScreenMode) != 0)
	{
	printf("\nYour display card is not VESA-compliant, or its VESA implementation\n");
	printf("does not support a resolution of %4dx%3dx256 colors - try a lower resolution\n\n",
				DWidth, DHeight);
	exit(0);
	}

/*  OK, everything is in order - unpack the PCX image  */

Buffer = malloc(DWidth * DHeight);
if (Buffer != NULL)
   {
   memset(Buffer, 0, DWidth * DHeight);
   UnpackPCX(fh, Buffer, Width, Height);

   /*  Display the image  */

   VESA_UpdateScreen( ScreenMode, Buffer, DWidth, DHeight);

   getch();
   }

RestoreTextMode();

return(0);
}


UnpackPCX(FILE *fh, char *Buffer, int Width, int Height)
{

/*  Note that this is NOT a properly robust PCX reader - it's for the purposes of
	this example only, and it makes a lot of assumptions, so it's easy to fool  */

char palette[768];
char *wbuf, *wptr, c, l;
int Offset, j, k, WHeight;

/* Read and set the palette */
fseek(fh, -768L, SEEK_END);
fread(palette, 768, 1, fh);
SetVGAPalette(palette);

/* Reposition to the start of the image data, and continue */
fseek(fh, sizeof(PCXHEADER), SEEK_SET);
wptr=wbuf=malloc(Width);

if (DHeight > Height)
   WHeight = Height;
else
   WHeight = DHeight;

for (j=0; j<WHeight; j++)
   {
   for (k=0; k<Width;)
      {
      c = fgetc(fh);
      if (c > 191)
         {
         l = c - 192;
         c = fgetc(fh);
         memset(wptr, c, l);
         wptr += l;
         k += l;
         }
      else
         {
         *wptr = c;
         wptr++;   k++;
         }
      }
   if (DWidth >= Width)
      memcpy(Buffer+(j*DWidth), wbuf, Width);
   else
      memcpy(Buffer+(j*DWidth), wbuf, DWidth);

   wptr = wbuf;
   }

return;
}

VESA_UpdateScreen(int ScreenMode, char *Buffer, int Width, int Height)
{

int j;
unsigned int NoOfPages, LastPage;

/* Before we start, work out how many pages are required, and how many bytes
	are needed in the last page.  */

NoOfPages = (Width * Height) / 65536;

LastPage = (Width * Height) - (NoOfPages * 65536);

/*  OK, now just update the screen  */
for (j = 0; j < NoOfPages; j++)
	{
	VESA_SetPage(j);
	memcpy(0xA0000, (const void *)Buffer, 65536);
	Buffer += 65536;
	}

if (LastPage != 0)
	{
	VESA_SetPage(j);
	memcpy(0xA0000, (const void *)Buffer, LastPage);
	}

return;
}

int VESA_SetMode(int Mode)
{

union REGS r;

r.h.ah = 0x4F;
r.h.al = 2;
r.w.bx = Mode;

int386(0x10, &r, &r);


/*  Note - it's not possible to simply return AX (or AH), because AL will contain 4Fh after a
    successful call... */

if (r.h.ah == 0)
	return(0);
else
	return(1);

}



void VESA_SetPage(short int Page)
{


union REGS r;

r.h.ah = 0x4F;
r.h.al = 5;
r.w.bx = 0;
r.w.dx = Page;

int386(0x10, &r, &r);

}


void SetVGAPalette(char *Palette)
{

/* This function sets the palette (will work on any VGA or SVGA card) by programming the card
	directly.  There IS a BIOS function to do it, but doing it directly is both quicker and - for protected
	mode programs - more straightforward   */

short int i;
char far *p;

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


void RestoreTextMode()
{
    union REGS r;
    r.h.al = 0x03;                    /* 16-colour VGA text mode */
    r.h.ah = 0x00;                    /* Set graphics mode */
    int386(0x10, &r, &r);

}

