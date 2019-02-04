#include "adlib.h"
#include "instrum.h"
#include <conio.h>
#include <dos.h>
#include <iostream.h>
#include <malloc.h>
#include <stdio.h>

struct S3MStruct {
  struct instrum instT[40];
  unsigned char kolejnosc[100];
  char *paternT[25], *wskP, *paterns;
  char linia, speed, czekaj, tempo;
  int rozkaz;
  char Mvol, Gvol;
  char AdlibPresent;
  char graj;
  char insN;
  unsigned char vol[40];
  unsigned char dsk[40];
  int Hz[40];
} S3M;

unsigned char MVol = 8;
////////////////////////////////////////////////////////////////////////
int GetPaternMemory(void);
void SetTimerProc(int freq, void (*proc)());
void ResetTimerProc(void);
void WyciszAdliba(void);
void MusicOn(void);
void MusicOff(void);
char MusikStatus(void);
//////////////////////////////////////////////////////////////////////////
int GetPaternMemory() {
  int size = 13000; //<<<<<<<<<<<<<<<<<<rozmiar pamieci na paterny
  S3M.paterns = NULL;
  if (!S3M.AdlibPresent)
    return 0;
  if ((S3M.paterns = (char *)malloc(size)) == NULL)
    return 1;
  return 0;
}
////////////////////////////////////////////////////////////////////////////
int FreePaternMemory() {
  if (!S3M.AdlibPresent)
    return 0;
  if (S3M.paterns != NULL)
    free(S3M.paterns);
  return 0;
}

///////////////////////////////////////////////////////////////////////////
int LoadPaterns(char *filename) {
  int i, index, kk = 0;
  FILE *f;
  int OrdNum, InsNum, PatNum;

  if (!S3M.AdlibPresent)
    return 0;
  if (S3M.graj) {
    MusicOff();
    kk = 1;
  }
  for (i = 0; i < 9; i++)
    KillNote(i);

  if (NULL == (f = fopen(filename, "rb")))
    return 1;

  fseek(f, 0x30, 0); // is   it
  fread(&S3M.Gvol, 1, 1, f);
  fread(&S3M.speed, 1, 1, f);
  fread(&S3M.tempo, 1, 1, f);
  fread(&S3M.Mvol, 1, 1, f);

  fseek(f, 0x20, 0);
  fread(&OrdNum, 2, 1, f);
  fread(&InsNum, 2, 1, f);
  fread(&PatNum, 2, 1, f);

  for (i = 0; i < 100; i++)
    S3M.kolejnosc[i] = 0;
  fseek(f, 0x60, 0);
  for (i = 0; i < OrdNum; i++)
    fread(&S3M.kolejnosc[i], 1, 1, f);

  S3M.insN = InsNum;
  for (i = 0; i < InsNum; i++) {
    fseek(f, 0x60 + OrdNum + i * 2, 0); // ustaw sie na i-ty wskaznik
    fread(&index, 2, 1, f);             // wczytaj wskaznik
    index *= 16;
    index += 0x10;
    fseek(f, index, 0); // ustaw sie tam gdzie wskazuje wskaznik
    fread((void *)&S3M.instT[i], 11, 1, f); // wczytaj instrument
    fread(&S3M.vol[i], 1, 1, f);
    fread(&S3M.vol[i], 1, 1, f);
    fread(&S3M.dsk[i], 1, 1, f);
    fread(&S3M.Hz[i], 2, 1, f);
    fread(&S3M.Hz[i], 2, 1, f);
  }
  for (i = 0; i < InsNum; i++) {
    unsigned char *tab = (unsigned char *)&S3M.instT[i];
    if (tab[10] & 1)
      tab[2] = (tab[2] & 192);
    tab[3] = (tab[3] & 192);
  }
  S3M.wskP = S3M.paterns;
  for (i = 0; i < PatNum; i++) {
    S3M.paternT[i] = S3M.wskP;
    fseek(f, 0x60 + OrdNum + InsNum * 2 + i * 2,
          0);               // ustaw sie na poczatek tablicy adresow paternow
    fread(&index, 2, 1, f); // wczytaj adres i-tego paternu
    index *= 16;            // przesun o 4
    fseek(f, index, 0);     // skok na poczatek paternu
    int Length = 0;
    fread(&Length, 2, 1, f);                // wczytanie dlugosci paternu
    fread(S3M.wskP, Length, 1, f);          // wczytanie itego paternu;
    S3M.wskP = (char *)(S3M.wskP + Length); // zwiekszenie wskP.
    S3M.wskP = 0; // 0 na koncu paternu(tak na wszelki wypadek)
    S3M.wskP += 10;
  }

  fclose(f);
  S3M.czekaj = 0;
  S3M.linia = 0;
  S3M.rozkaz = 0;
  S3M.wskP = S3M.paternT[0];
  if (kk)
    MusicOn();
  return 0;
}
//////////////////////////////////////////////////////////////////////////
void PlayOneLine() {
  unsigned char vvv;
  static unsigned char note, inst, vol, ef, par, chanel;
  static int freq[12] = {343, 363, 385, 408, 432, 458,
                         485, 514, 544, 577, 611, 647};
  static int kon = 0, nrRozkazu = 255;
  static unsigned char Vol[9] = {63, 63, 63, 63, 63, 63, 63, 63, 63};
  static char endLine;

  if (!S3M.graj)
    return;
  if (!S3M.AdlibPresent)
    return;
  if (S3M.czekaj) {
    S3M.czekaj--;
    return;
  }

  endLine = 0;
  do {
    note = 255;
    vol = 255;
    ef = 0;

    chanel = *(char *)S3M.wskP;
    (char *)S3M.wskP++;
    if (!chanel) {
      endLine = 1;
    } else // grajmy
    {
      if (chanel & 32) {
        note = *(char *)S3M.wskP;
        (char *)S3M.wskP++;
        inst = *(char *)S3M.wskP;
        (char *)S3M.wskP++;
      }
      if (chanel & 64) {
        vol = *(char *)S3M.wskP;
        (char *)S3M.wskP++;
      }
      if (chanel & 128) {
        ef = *(char *)S3M.wskP;
        (char *)S3M.wskP++;
        par = *(char *)S3M.wskP;
        (char *)S3M.wskP++;
      }
      chanel = (chanel & 31);
      //----graj-------
      if (ef == 1) {
        if (par > 0)
          S3M.speed = par;
      } //??? A
      if (note == 254)
        StopNote(chanel);
      if (note < 254 && MVol < 64) {
        StopNote(chanel);
        Vol[chanel] = S3M.vol[inst - 1];
        if (vol < 255) {
          Vol[chanel] = vol;
        }
        if (inst && inst <= S3M.insN)
          SetInstrum(chanel, &S3M.instT[inst - 1]);
        if (Vol[chanel] > MVol)
          vvv = Vol[chanel] - MVol;
        else
          vvv = 1;
        SetVolume(chanel, vvv);
        PlayNoteFreq(chanel, (note >> 4), freq[note & 15]);
      } else if (vol < 255) {
        Vol[chanel] = vol;
        if (Vol[chanel] > MVol)
          vvv = Vol[chanel] - MVol;
        else
          vvv = 1;
        SetVolume(chanel, vvv);
      }

      if (!vol || MVol == 64)
        KillNote(chanel);

      if (ef == 4) {
        Vol[chanel] -= (par << 3);
        if (Vol[chanel] > MVol)
          vvv = Vol[chanel] - MVol;
        else
          vvv = 1;
        SetVolume(chanel, vvv);
      }
      if (ef == 3) {
        kon = 1;
        nrRozkazu = 255;
      } // koniec  ???
      if (ef == 2) {
        kon = 1;
        nrRozkazu = (int)par;
      }
    }
  } while (!endLine);
  // S3M.rozkaz to jest rozkaz
  if (kon) // skok do paternu
  {
    S3M.linia = 63;
    if (nrRozkazu < 255)
      S3M.rozkaz = (int)nrRozkazu - 1;
    nrRozkazu = 255;
    kon = 0;
  }
  S3M.linia++;
  if (S3M.linia == 64) {
    S3M.linia = 0;
    S3M.rozkaz++;
    if (S3M.kolejnosc[S3M.rozkaz] == 0xff)
      S3M.rozkaz = 0; // skok na poczatek
    S3M.wskP = (char *)S3M.paternT[S3M.kolejnosc[S3M.rozkaz]];
    S3M.linia = 0;
  }
  S3M.czekaj = S3M.speed - 1;
}
//////////////////////////////////////////////////////////////////////////
void Play() {}
/*
do{
cout<<"Patern "<<(int)S3M.rozkaz<<","<<(int)S3M.kolejnosc[S3M.rozkaz]<<" linia
"<<(int)S3M.linia<<"("; PlayOneLine(); cout<<endl;

delay(160-S3M.tempo);

}while(!kbhit());

for(int chanel=0;chanel<9;chanel++)
    {
     StopNote(chanel);
    }
}
  */
//////////////////////////////////////////////////////////////////////////
void UstawAdlibPresent(unsigned char Wartosc) { S3M.AdlibPresent = Wartosc; }

//////////////////////////////////////////////////////////////////////////
void UstawAdliba(void) {
  // S3M.AdlibPresent = (char)IsALPresent();
  if (S3M.AdlibPresent) {
    SetALMode(1);
  }
  S3M.graj = 0;
  SetTimerProc(0x32, PlayOneLine); //
}
//////////////////////////////////////////////////////////////////////////
void WylaczAdliba(void) {
  ResetTimerProc();
  if (S3M.AdlibPresent) {
    WyciszAdliba();
  }
}
/////////////////////////////////////////////
void WyciszAdliba() {
  for (int chanel = 0; chanel < 9; chanel++) {
    KillNote(chanel);
  }
}
/////////////////////////////////////////////
void MusicOn() { S3M.graj = 1; }
/////////////////////////
void MusicOff() {
  S3M.graj = 0;
  WyciszAdliba();
}

char MusikStatus() { return S3M.graj; }
