#include "adlib.h"
#include <conio.h>
#include <dos.h>
// początkowa warość rejestru BDh
char reg0xBD = 0x00; // 0xC0;

int _cx3_3, _cx23_3, RythmMode;

void _setCXvalues()
/* ustawia _cx3_3 i _cx23_3 na liczbę
   obiegów pętli opóźniającej dla
   odpowiednio 3.3 us i 23.3 us */
{
  // Artur tu popracuj
  /*
   int t1,t2;
   asm cli
   t1 = peek(0x40, 0x6C);
   asm sti
   asm mov ax,550
   loop1:
   asm mov cx,10000
   loop2:
   asm {
    nop; dec cx; or cx,cx;
    jnz loop2
   }
   asm dec ax
   asm jnz loop1
   asm cli
   t2 = peek(0x40, 0x6C);
   asm sti
   t2 -= t1;
   _cx3_3 = 330 / t2;
   
   if (_cx3_3 == 0)
    _cx3_3 = 1;
   _cx23_3 = 2330 / t2;
   if (_cx23_3 == 0)
    _cx23_3 = 1;
   */
}

#pragma startup _setCXvalues

void WriteALReg(char reg, char value)
// wpisuje wartość value do rejestru reg
// układu FM
{
  // Artur tu popracuj

  /*
  outportb(0x388, reg );    //Wpisuje reg
  asm mov cx,[_cx3_3]
  loop1:
  asm {
   nop; dec cx; or cx,cx     //A tu czeka 3.3usek.
   jne loop1
  }
  outportb(0x389, value);    //Wpisuje value
  asm mov cx,[_cx23_3]
  loop2:
  asm {
   nop; dec cx; or cx,cx     //A tu czeka 23.3usek.
   jne loop2
  }
  */
}

char ReadALByte()
// odczytuje port statusu
{
  return (char)inp(0x388); // inportb
}

int IsALPresent()
/* wykrywa obecność karty Ad Lib
   przy pomocy liczników.
   Zwraca 0 lub 1=znalezionp */
{
  // Artur tu popracuj
  /*
char s1, s2, t;
WriteALReg(1,0);
WriteALReg(4,0x60);
WriteALReg(4,0x80);
s1 = ReadALByte();
WriteALReg(2,0xFF);
WriteALReg(4,0x21);
for (t = 0; t<=50; t++)
 s2 = ReadALByte();
WriteALReg(4,0x60);
WriteALReg(4,0x80);
return (s1 & 0xE0)==0 && (s2 & 0xE0)==0xC0;
*/
  return 0; // to wytnij
}

// głośności dla posczególnych instrumentów kanalow
char _volumeReg[11];
char _volumeRegM[11];
void SetALMode(int mode)
/* przełącza Ad Lib na tryb melodyczny
   dla mode==0 lub tryb rytmiczny
   dla mode!=0 */
{
  reg0xBD &= 0xDF;
  WriteALReg(0xBD, reg0xBD);
  if (RythmMode == mode) {
    reg0xBD |= 0x20;
    for (int i = 6; i < 11; _volumeReg[i++] = 0)
      ;
  } else
    reg0xBD &= 0xDF;
  WriteALReg(0xBD, reg0xBD);
  WriteALReg(8, 0);
}

int _carrOfs[9] // ofsety nośników dla poszcz. instrum
    = {3, 4, 5, 11, 12, 13, 19, 20, 21};
char synth[9] = {0, 0, 0, 0, 0,
                 0, 0, 0, 0}; // ustalenie rodzaju syntezy dla kazdego z kanalow
// ofsety modulatorów sa o 3 mniejsze

/*
  W poniższych funkcjach parametr voice może przyjmować
  wartości od 0 do 8 w trybie melodycznym
  i od 0 do 10 w trybie rytmicznym. W tym drugim przypadku
  mamy następujące przyporządkowania:
  6 - bęben basowy; 7 - werbel; 8 - bębenek;
  9 - talerz; 10 - high hat
*/

void SetInstrum(int voice, void *instr)
// przeprogramowuje głos voice
// wg danych wskazywanych przez instr
{
  char *instrByte = (char *)instr;
  int baseReg = 0x20, ofs;
  int rythmOfs[] = {0x14, 0x12, 0x15, 0x11};
  if (RythmMode && voice > 6) {
    for (ofs = 0; ofs <= 8; ofs += 2) {
      WriteALReg(baseReg + rythmOfs[voice - 7], instrByte[ofs]);
      if ((baseReg += 0x20) == 0xA0)
        baseReg = 0xE0;
    }
    WriteALReg(0xC7 + (voice == 8 || voice == 9), instrByte[10]);
    _volumeReg[voice] = instrByte[2];
  } else {
    int carr = _carrOfs[voice], modul = carr - 3;
    for (ofs = 0; ofs < 10; ofs++) {
      WriteALReg(baseReg + ((ofs & 1) ? carr : modul), instrByte[ofs]);
      if ((ofs & 1) == 0)
        continue;
      if ((baseReg += 0x20) == 0xA0)
        baseReg = 0xE0;
    }
    WriteALReg(0xC0 + voice, instrByte[10]);
    _volumeRegM[voice] = instrByte[2];
    _volumeReg[voice] = instrByte[3];   // cKSL+cTL
    synth[voice] = (instrByte[10] & 1); // 2    6
  }
}

int _voiceLastFreq[11];

void PlayNoteFreq(int voice, int octave, int f_num)
// włącza głos voice ustawiając numer oktawy na octave
// i wartość F_NUMBER na f_num
{
  if (RythmMode && voice > 5) {
    /*  switch (voice)
       {
         case 6: //bassdrum
          WriteALReg(0xA6, f_num & 255);
          WriteALReg(0xB6, (f_num >> 8) + (octave << 2) + 32);
          break;
         case 8: //tomtom
          WriteALReg(0xA8, f_num & 255);
          WriteALReg(0xB8, (f_num >> 8) + (octave << 2));
       }
      reg0xBD |= 1 << 10 - voice;
      WriteALReg(0xBD, reg0xBD);
    */
  } else {
    WriteALReg(0xA0 + voice, f_num & 255);
    WriteALReg(0xB0 + voice, (f_num >> 8) + (octave << 2) + 32);
  }
  _voiceLastFreq[voice] = (octave << 10) + f_num;
}

void PlayNote(int voice, int octave, int note)
/* włącza głos voice z częstotliwością odpowiadającą
   nucie note z oktawy octave, gdzie note może być:
   0=C; 1=C#; 2=D; 3=D#; 4=E; 5=F;
   6=F#; 7=G; 8=G#; 9=A; 10=A#; 11=B              */
{
  int _noteFreqs[12] = {343, 363, 385, 408, 432, 458,
                        485, 514, 544, 577, 611, 647};
  PlayNoteFreq(voice, octave, _noteFreqs[note]);
}

///////////////////////////////////////////////////////////////////
void StopNote(int voice)
// wyłącza nutę na głosie voice powoli
{
  WriteALReg(0xA0 + voice, _voiceLastFreq[voice] & 255);
  WriteALReg(0xB0 + voice, _voiceLastFreq[voice] >> 8);
}
///////////////////////////////////////////////////////////////////
void KillNote(int voice)
// wyłącza nutę na głosie voice szybko
{
  WriteALReg(0xA0 + voice, 0);
  WriteALReg(0xB0 + voice, 0);
}

//////////////////////////////////////////////////////////////////////////////////

void SetVolume(int voice, unsigned char volume) // volume 0-min 63-max
// ustawia głośność dla głosu voice
// na wartość volume (0=min, 127=max)
{
  int RythmVolumeOfs[] = {19, 20, 18, 21, 17};
  // volume = (64 - (volume >> 1)) >> 1;//oryginal
  volume = (64 - volume);
  _volumeRegM[voice] = (_volumeRegM[voice] & 192) + volume;
  _volumeReg[voice] = (_volumeReg[voice] & 192) + volume;
  //                           ^cKSL                 ^cTL
  if (RythmMode && voice > 5)
    WriteALReg(0x40 + RythmVolumeOfs[voice - 6], _volumeReg[voice]);
  else {
    WriteALReg(0x40 + _carrOfs[voice], _volumeReg[voice]);
    if (synth[voice])
      WriteALReg(0x40 + _carrOfs[voice] - 3, _volumeRegM[voice]);
  }
}
