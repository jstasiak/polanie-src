/////////////////////////////////////////////////////////////////////
//   image13h.cpp
//    kompilowac w modelu Large
////////////////////////////////////////////////////////////////////
#include "lida13h.h"
#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <stdio.h>
#include <string.h>
//////////////////////////////////////////////////////////////////
// Zmienne srodowiskowe
//////////////////////////////////////////////////////////////////
unsigned char far *VirtualScreenPtr;
unsigned char far *ActualScreenPtr;
int WriteMode13h = 0; // 0-copy put 1-xor put
unsigned char *rgb = NULL;
int length[91] = {5, 3, 5, 7, 7, 7, 7, 7, 5, 5, 7, 7,  3, 5, 3, 7, 7, 6,  7,
                  7, 7, 7, 7, 7, 7, 7, 3, 3, 9, 7, 9,  7, 8, 9, 8, 8, 8,  7,
                  7, 9, 8, 3, 6, 8, 7, 9, 8, 9, 7, 9,  8, 8, 7, 8, 9, 12, 8,
                  9, 8, 8, 8, 8, 7, 8, 5, 7, 7, 7, 7,  7, 5, 7, 7, 3, 3,  6,
                  3, 9, 7, 7, 7, 7, 5, 7, 4, 7, 7, 11, 7, 7, 7};
void far *index[91];
///////////////////////////////////////////////////////////////////////
// inicjowanie ekranu virtualnego
///////////////////////////////////////////////////////////////////////
int InitVirtualScreen(void) {
  VirtualScreenPtr =
      (unsigned char far *)farcalloc(64000L, sizeof(unsigned char));
  if (VirtualScreenPtr == NULL)
    return 0;
  else
    return 1;
}
//////////////////////////////////////////////////////////////////////
// kopiowanie virtualnego obrazu na ekran
//////////////////////////////////////////////////////////////////////
void ShowVirtualScreen(void) {
  if (VirtualScreenPtr == NULL)
    return;
  movedata(FP_SEG(VirtualScreenPtr), FP_OFF(VirtualScreenPtr) - 4, 0xA000, 0,
           64000L);
}
///////////////////////////////////////////////////////////////////////////
// Zwalnianie pamieci zajmowanej przez ekran
//////////////////////////////////////////////////////////////////////
void FreeVirtualScreen(void) {
  if (VirtualScreenPtr == NULL)
    return;
  farfree(VirtualScreenPtr);
  VirtualScreenPtr = NULL;
}
////////////////////////////////////////////////////////////////////////////
// ustawienie aktualnego ekranu dla funkcji graficznych
//   0-rzeczywisty       1-wirtualny
//////////////////////////////////////////////////////////////////////////
void SetScreen(int Screen) {
  if ((Screen) && (VirtualScreenPtr != NULL))
    ActualScreenPtr = VirtualScreenPtr;
  else
    ActualScreenPtr = (unsigned char far *)MK_FP(0xA000, 0);
}
///////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void Init13h(void) {
  asm {
   mov al,13h;
   mov ah,00h;
   int 10h;
  }
  SetScreen(0);
  rgb = (unsigned char *)farmalloc(768);
}
/////////////////////////////////////////////////////////////////
// END13H
///////////////////////////////////////////////////////////////
void Close13h(void) {
  asm {
    mov al,03h;
    mov ah,00h;
    int 10H;
  }
  if (rgb != NULL) farfree(rgb);
}
///////////////////////////////////////////////////////////////////////
//  rysowanie punktu
///////////////////////////////////////////////////////////////////////
void PutPixel13h(int x, int y, int color) {
  asm {
 push ds
 lds si,ActualScreenPtr; // ds:si poczatek ekranu

 mov AX,y
 mov BX,x
 xchg AH,AL
 add BX,AX
 shr AX,1
 shr AX,1
 add BX,AX
 MOV SI,Bx // w si offset punktu
 mov Ax,color
 MOV ds:[SI],AL
 pop ds
  }
}
unsigned int GetImageSize13h(int x1, int y1, int x2, int y2) {
  unsigned int b;
  int a;
  if (x1 > x2) {
    a = x1;
    x1 = x2;
    x2 = a;
  }
  if (y1 > y2) {
    a = y1;
    y1 = y2;
    y2 = a;
  }
  return (x2 - x1 + 1) * (y2 - y1 + 1) + 6;
}
/*
{--------------------------------------------------------------------------}
PROCEDURE RAMCOPY(SOURCE,DEST:POINTER;SIZE:WORD);assembler;
asm
 push ds
 PUSH ES
 lds SI,SOURCE
 les DI,DEST
 mov CX,SIZE
 rep movsb
 POP ES
 pop ds
end;
{----------------------------------------------------------}
{===========================================================}
{------------------------------------------------------------}
*/
//////////////////////////////////////////////////////////////////
// Load image from file "name" to picture
//////////////////////////////////////////////////////////////////
unsigned char far *LoadImage13h(char *name)

{
  FILE *file;
  unsigned int size;
  unsigned char far *picture;

  file = fopen(name, "rb");
  if (file == NULL)
    return NULL;
  fread(&size, 1, 2, file);
  picture = (unsigned char *)farmalloc(size);
  if (picture == NULL) {
    fclose(file);
    return (NULL);
  }
  asm {
            push ds
	    lds si,picture
	    mov ax,size
	    mov ds:[si],ax
            pop ds
  }
  picture = (unsigned char *)MK_FP(FP_SEG(picture), FP_OFF(picture) + 2);
  size -= 2;
  fread(picture, size, 1, file);
  fclose(file);
  picture = (unsigned char *)MK_FP(FP_SEG(picture), FP_OFF(picture) - 2);
  return (picture);
}
////////////////////////////////////////////////////////////////////////////
// Save picture image to file "name"
//////////////////////////////////////////////////////////////////////////
int SaveImage13h(char *name, void far *picture)

{
  FILE *file;
  unsigned int size, s;

  size = *(unsigned int *)picture;
  s = size;
  file = fopen(name, "wb");
  if (file == NULL) {
    return (1);
  }
  fwrite(picture, s, 1, file);
  fclose(file);
  return (0);
}

//////////////////////////////////////////////////////////////////
//  laduje bitmape pod adres map
//////////////////////////////////////////////////////////////////
void GetImage13h(int x1, int y1, int x2, int y2, void far *map) {
  unsigned int a, size, height, length;

  if (map == NULL)
    return;
  if (x1 > x2) {
    a = x1;
    x1 = x2;
    x2 = a;
  }
  if (y1 > y2) {
    a = y1;
    y1 = y2;
    y2 = a;
  }
  size = ((x2 - x1) + 1) * ((y2 - y1) + 1) + 6;
  height = (y2 - y1);
  length = (x2 - x1);

  asm {
push es
push ds
push di
        // do es:di zaladowac adres map
les di,map
mov ax,size // wpisz size
mov es:[di],ax
inc di
inc di
mov ax,length // wpisz length
mov es:[di],ax
inc di
inc di
mov ax,height // wpisz height
mov es:[di],ax
inc di
inc di

        // do ds:si adres poczatku ekranu
lds si,ActualScreenPtr
        // do dx zaladowac adres X,Y w pamieci ekranu
mov ax,y1
mov bx,x1
xchg ah,al
add bx,ax
shr ax,1
shr ax,1 // ax*4
add bx,ax // w bx offset w pamieci ekranu

mov ax,height
  }
petla:
  asm {
 dec ax
 mov cx,length
  }
linia1:
  asm {
  mov dl,ds:[bx] // wez z ekranu ds:bx
  mov es:[di],dl // wloz do map es:di
  inc bx
  inc di
 loop linia1
 add bx,320
 sub bx,length //       zwieksz bx o 320-length
 cmp ax,0 // czy ax=0? jesli tak to koniec
jne petla
jmp koniec
  }
koniec:
  asm {
pop di
pop ds
pop es
  }
}
//////////////////////////////////////////////////////////////////
//  wyswietla bitmape
//////////////////////////////////////////////////////////////////
void PutImage13h(int x1, int y1, void far *map, int typ) {
  unsigned int size, height, length;

  if (map == NULL)
    return;

  asm {
push es
push ds
push di
        // do ds:si zaladowac adres poczatku ekranu
lds si,ActualScreenPtr;
    // do dx zaladowac adres X,Y w pamieci ekranu
mov ax,y1
mov bx,x1
xchg ah,al
add bx,ax
shr ax,1
shr ax,1 // ax*4
add bx,ax // w bx offset w pamieci ekranu
les di,map // es:di adres map

mov ax,word ptr es:[di]
mov size,ax

inc di // opusc size
inc di
mov ax,word ptr es:[di] // zaladuj dlugosc
mov length,ax
inc di
inc di
mov ax,word ptr es:[di] // zaladuj wysokosc
mov height,ax
inc di
inc di
mov dx,typ
cmp dx,0
jne petla // skok do petli gdy nie zero
  }
petla1:
  asm {
 dec ax
 mov cx,length
  }
linia1:
  asm {
  mov dl,es:[di]
  mov ds:[bx],dl
  inc bx
  inc di
 loop linia1
 add bx,320
 sub bx,length //          {??? o ile ???}
 cmp ax,0
jne petla1
jmp koniec
  }
petla:
  asm {
 dec ax
 mov cx,length
  }
linia:
  asm {
  mov dl,es:[di]
  cmp dl,0
  je nic
  mov ds:[bx],dl
  }
nic:
  asm {
  inc bx
  inc di
 loop linia
 add bx,320
 sub bx,length //
 cmp ax,0
jne petla
  }
koniec:
  asm {
pop di
pop ds
pop es
  }
}
//////////////////////////////////////////////////////////////////
//  wyswietla bitmape i zamienia kolor c1 na c2
//////////////////////////////////////////////////////////////////
void PutImageChange13h(int x1, int y1, void far *map, int typ, char c1,
                       char c2) {
  unsigned int size, height, length;

  if (map == NULL)
    return;

  asm {
push es
push ds
push di
push si
push dx
        // do ds:si zaladowac adres poczatku ekranu
lds si,ActualScreenPtr;
    // do dx zaladowac adres X,Y w pamieci ekranu
mov ax,y1
mov bx,x1
xchg ah,al
add bx,ax
shr ax,1
shr ax,1 // ax*4
add bx,ax // w bx offset w pamieci ekranu
les di,map // es:di adres map

mov ax,word ptr es:[di]
mov size,ax

inc di // opusc size
inc di
mov ax,word ptr es:[di] // zaladuj dlugosc
mov length,ax
inc di
inc di
mov ax,word ptr es:[di] // zaladuj wysokosc
mov height,ax
push ax // odloz na stos
inc di
inc di
mov dx,typ
cmp dx,0
jne petla // skok do petli gdy nie zero
  }
petla1:
  asm {
 pop ax
 dec ax
 push ax
 mov cx,length
  }
linia1:
  asm {
  mov dl,es:[di]
    // porownac z c1 i zamienic na c2
  mov al,c1
  cmp al,dl
  jne bezzmian1
  mov dl,c2
    //------------------------
  }
bezzmian1:
  asm {
  mov ds:[bx],dl
  inc bx
  inc di
 loop linia1
 add bx,320
 sub bx,length //          {??? o ile ???}
 pop ax
 cmp ax,0
 push ax
jne petla1
jmp koniec
  }
petla:
  asm {
 pop ax // pop'y i push'e
 dec ax
 push ax
 mov cx,length
  }
linia:
  asm {
  mov dl,es:[di]
  mov al,c1
  cmp al,dl
  jne bezzmian
  mov dl,c2
    //------------------------
  }
bezzmian:
  asm {
  cmp dl,0
  je nic
  mov ds:[bx],dl
  }
nic:
  asm {
  inc bx
  inc di
 loop linia
 add bx,320
 sub bx,length //
 pop ax
 cmp ax,0
 push ax
jne petla
  }
koniec:
  asm {
pop ax

pop dx
pop si
pop di
pop ds
pop es
  }
}

//////////////////////////////////////////////////////////////////
//  wyswietla bar'a
//////////////////////////////////////////////////////////////////
void Bar13h(int x1, int y1, int x2, int y2, int color) {
  int a;
  unsigned int height, length;
  if ((x1 == x2) || (y1 == y2))
    return;
  if (x1 > x2) {
    a = x1;
    x1 = x2;
    x2 = a;
  }
  if (y1 > y2) {
    a = y1;
    y1 = y2;
    y2 = a;
  }
  height = y2 - y1;
  length = x2 - x1;

  asm {
push es
push ds
push di
        // do ds:si zaladowac adres poczatku ekranu
lds si,ActualScreenPtr;
    // do dx zaladowac adres X,Y w pamieci ekranu
mov ax,y1
mov bx,x1
xchg ah,al
add bx,ax
shr ax,1
shr ax,1 // ax*4
add bx,ax // w bx offset w pamieci ekranu

mov ax,height
mov dx,ax

mov ax,color
  }
petla:
  asm {
dec dx
mov cx,length
  }
linia1:
  asm {
mov ds:[bx],al
inc bx
loop linia1
add bx,320
sub bx,length //          {??? o ile ???}
cmp dx,0
jne petla

pop di
pop ds
pop es
  }
}
//--------------------------------------------------------
//            InitText13h
//--------------------------------------------------------
int InitText13h() {
  unsigned int size;
  int i, x1, x2;
  void far *literki;

  literki = LoadImage13h("fonts1.13h"); //??????????
  if (literki == NULL)
    printf("%c", 7);
  PutImage13h(0, 0, literki, 0);
  farfree(literki);

  x1 = 7;
  x2 = 11;
  for (i = 0; i < 33; i++) {
    x2 = x1 + length[i] - 1;
    size = GetImageSize13h(x1, 8, x2, 20);
    index[i] = farmalloc(size);
    if (index[i] != NULL)
      GetImage13h(x1, 8, x2, 20, index[i]);
    else
      return 1;
    x1 = x2;
  }

  x1 = 8;
  x2 = 16;
  for (i = 33; i < 64; i++) {
    x2 = x1 + length[i] - 1;
    size = GetImageSize13h(x1, 32, x2, 44);
    index[i] = farmalloc(size);
    if (index[i] != NULL)
      GetImage13h(x1, 32, x2, 44, index[i]);
    else
      return 1;
    x1 = x2;
  }

  x1 = 7;
  x2 = 11;
  for (i = 64; i < 91; i++) {
    x2 = x1 + length[i] - 1;
    size = GetImageSize13h(x1, 56, x2, 78);
    index[i] = farmalloc(size);
    if (index[i] != NULL)
      GetImage13h(x1, 56, x2, 78, index[i]);
    else
      return 1;
    x1 = x2;
  }

  return 0;
}

//------------------------------------------------------
//               Transform
//------------------------------------------------------
char Transform13h(char znak) {
  switch (znak) {
  case 134:
    return '#';
  case 145:
    return '$';
  case 162:
    return '%';
  case 166:
    return '&';
  case 167:
    return 41;
  case 158:
    return '*';
  case 141:
    return '+';
  case 164:
    return '/';
  case 143:
    return '<';
  case 144:
    return '=';
  case 163:
    return '>';
  case 160:
    return '@';
  case 161:
    return '[';
  case 149:
    return '\\';
  case 152:
    return ']';
  case 156:
    return '^';
  case 165:
    return '_';
  case 146:
    return '`';
  }
  return znak;
}

//--------------------------------------------------------
//             ClearText
//-------------------------------------------------------
void ClearText13h() {
  int i;

  for (i = 0; i < 92; i++)
    if (index[i] != NULL)
      farfree(index[i]);
}
//--------------------------------------------------------
//             CenterText13h                wypisywanie liter
//-------------------------------------------------------
void CenterText13h(int xl, int yg, int xp, int yd, char *text, int colour) {
  int x, y, dl = 0, wsk;
  wsk = strlen(text);
  // strcpy(napis, text);
  while (*text != NULL) {
    dl = dl + length[*text - 32] - 1;
    text++;
  }
  if (xp - xl < dl)
    return;
  if (yd - yg < 13)
    return;
  y = (yd - yg + 1) >> 1;
  y = yg + y - 7;
  x = (xp - xl - dl) >> 1;
  x = xl + x;
  for (int i = 0; i < wsk; i++)
    text--;
  OutText13h(x, y, text, colour);
}
//--------------------------------------------------------
//             OutText13h                wypisywanie liter
//-------------------------------------------------------
void OutText13h(int x, int y, char *text, int colour) {
  void far *letter;
  char znak;

  while (*text != NULL) {
    znak = Transform13h(*text);
    letter = index[znak - 32];
    PutImageChange13h(x, y, letter, 1, 255, colour);
    x = x + length[znak - 32] - 1;
    text++;
  }
}
//--------------------------------------------------------
//             Write13h                wypisywanie liter
//-------------------------------------------------------

Write13h(int x, int y, int maxx, int maxdl, char *txt, int tcolour,
         int bcolour) {
  /* width=maxdl+1 */
  int cx = strlen(txt), a, ll, xp;
  char k, l;
  char str[2] = {0, 0};
  do {
    if (maxx == 0)
      Bar13h(x, y, x + (maxdl + 1) * 11 - 1, y + 13, bcolour);
    else
      Bar13h(x, y, x + maxx, y + 13, bcolour);
    xp = x;
    for (a = 0; a < strlen(txt); a++) {
      str[0] = txt[a];
      if (a == cx) {
        Bar13h(xp, y + 11, xp + length[txt[a] - 32], y + 12, tcolour);
        OutText13h(xp, y, str, tcolour);
      } else {
        OutText13h(xp, y, str, tcolour);
      }
      xp += length[txt[a] - 32] - 1;
    }
    if (cx == strlen(txt)) {
      Bar13h(xp, y + 11, xp + 8, y + 12, tcolour);
    }
    ll = strlen(txt);
    k = getch();
    if (!k)
      l = getch();
    if (!k)
      switch (l) {
      case 75:
        if (cx > 0)
          cx--;
        break;
      case 77:
        if (cx < ll)
          cx++;
        break;
      case 71:
        cx = 0;
        break;
      case 79:
        cx = ll;
        break;
      case 83:
        if (cx < ll)
          for (a = cx; a < ll; a++)
            txt[a] = txt[a + 1];
        break;
      }
    else
      switch (k) {
      case 8:
        if (cx > 0) {
          cx--;
          for (a = cx; a < ll; a++)
            txt[a] = txt[a + 1];
        }
        break;
      default:
        if ((k > 31) && (ll < maxdl)) {
          if ((k == ' ') || (k >= '0') && (k <= 'z') &&
                                ((k < ';') || (k > '@')) &&
                                ((k < '[') || (k >= 'a'))) {
            for (a = ll + 1; a > cx; a--)
              txt[a] = txt[a - 1];
            txt[cx] = k;
            cx++;
          }
          break;
        }
      }
  } while ((k != 13) && (k != 27));
  return (k);
}

/*
char* Write13h(int x,int y,int no,char *str,int tcolour,int bcolour)
{
int li=0;
char z;
char *napis,*cos;
//napis=new char[no+1];
//if(napis==NULL)return napis;
napis=str;
OutText13h(x,y,str,tcolour);
while(*str!=NULL)
     {x=x+length[*str-33];
     str++;}
z=getch();
while(!(z==13))
     {if((z=='
')||(z>='0')&&(z<='z')&&((z<';')||(z>'@'))&&((z<'[')||(z>='a'))&&(li<=no)) {
*str=z; str++; li++; *str=0; *cos=z; OutText13h(x, y, cos, tcolour);}
     z=getch();}
return napis;
}

/*
*/
/////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////
void LoadExtendedPalette(char *name) {
  FILE *file;

  if (rgb == NULL)
    return;
  file = fopen(name, "rb");
  if (file == NULL)
    return;
  fread(rgb, 768, 1, file);
  fclose(file);
}
//////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////
void SetExtendedPalette() {
  if (rgb == NULL)
    return;
  asm {
 les di,rgb
 push di
 xor cx,cx
  }
l1:
  asm { // modyfikacja kazdego bajtu tablicy}
 shr Byte Ptr[es:di],1
 shr Byte Ptr[es:di],1
 inc cx
 inc di
 cmp cx,769d
 jne l1 // tu skok do l1
 pop dx
 mov ax,1012h
 xor bx,bx
 mov cx,256d
 int 10h
  }
  asm {les di, rgb xor cx, cx} l2
      : asm { // przywrocenie wartosci kazdego bajtu tablicy}
    shl Byte Ptr [es:di], 1 shl Byte Ptr [es:di], 1 inc cx inc di cmp cx,
        769d jne l2 // tu skok do l1
  }
}
/////////////////////////////////////////////////////////////////
void DownPalette(int speed) {
  int i, j;

  for (i = 1; i < 85; i++) {
    asm {
 les di,rgb
 mov ah,0
 xor cx,cx
 dec di
 xor cx,cx // wyzeruj cx
    }
  lw:            // petla do 768
    asm {
 cmp cx,768d
 je end // tu skok do end jezeli koniec petli
 inc di
 inc cx
 mov al,[es:di] // modyfikacja kazdego bajtu tablicy}
 cmp al,ah
 je lw
 dec al
 cmp al,ah
 je lw
 dec al
 cmp al,ah
 je lw
 dec al
 mov [es:di],al
 jmp lw
    }
  end:

    SetExtendedPalette();
    delay(speed);
  }
}
//////////////////////////////////////////////////////////////////////
void BlackPalette(void) {
  asm {
 les di,rgb
 mov ax,0
 xor cx,cx
 dec di
 xor cx,cx // wyzeruj cx
  }
lw:                // petla do 768
  asm {
 cmp cx,768d
 je end // tu skok do end jezeli koniec petli
 inc di
 inc cx
 mov [es:di],al
 jmp lw
  }
end:

  SetExtendedPalette();
}
/////////////////////////////////////////////////////////////////
void RisePalette(int speed) {
  int i;
  unsigned char j;
  unsigned char *Rgb;
  if (rgb == NULL) {
    return;
  }
  Rgb = (unsigned char *)farmalloc(768);
  if (Rgb == NULL) {
    SetExtendedPalette();
    return;
  }
  // przepisanie rgb->Rgb i wyzerowanie rgb
  asm {
 push ds
 les di,rgb
 lds si,Rgb
 mov ah,0
 dec di
 dec si
 xor cx,cx // wyzeruj cx
  }
l1:            // petla do 768
  asm {
 cmp cx,768d
 je end1 // tu skok do end jezeli koniec petli
 inc di
 inc si
 inc cx
 mov al,[es:di] // modyfikacja kazdego bajtu tablicy}
 mov [ds:si],al
 mov [es:di],ah
 jmp l1
  }
end1:
  asm { pop ds }
  //  ---- rozjasnienie palety
  for (i = 1; i < 64; i++) {
    j = (63 - i) * 4;
    asm {
 push ds
 les di,rgb
 lds si,Rgb
 dec di
 dec si
 xor cx,cx // wyzeruj cx
 mov al,j
    }
  lw:           // petla do 768
    asm {
 cmp cx,768d
 je end // tu skok do end jezeli koniec petli
 inc di
 inc si
 inc cx
 mov ah,ds:[si] // Rgb
 sub ah,al
 jnc lw1
 mov ah,0
    }
  lw1:
    asm {
 mov es:[di],ah // rgb
 jmp lw
    }
  end:
    asm { pop ds}

    SetExtendedPalette();
    if (speed)
      delay(speed);
  }

  asm {
 push ds
 les di,Rgb
 lds si,rgb
 mov ah,0
 dec di
 dec si
 xor cx,cx // wyzeruj cx
  }
l2:            // petla do 768
  asm {
 cmp cx,768d
 je end2 // tu skok do end jezeli koniec petli
 inc di
 inc si
 inc cx
 mov al,[es:di] // modyfikacja kazdego bajtu tablicy}
 mov [ds:si],al
 jmp l2
  }
end2:
  asm { pop ds}

  farfree(Rgb);
  SetExtendedPalette();
}
//////////////////////////////////////////////////////////////////////
