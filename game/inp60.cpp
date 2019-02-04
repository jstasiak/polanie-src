#include <conio.h>
#include <dos.h>
#include <graph.h>
#include <iostream.h>

struct J4 {
  unsigned int edi;
  unsigned int esi;
  unsigned int ebp;
  unsigned int zarezerwowane;
  unsigned int ebx;
  unsigned int edx;
  unsigned int ecx;
  unsigned int eax;
  unsigned short flags;
  unsigned short es;
  unsigned short ds;
  unsigned short fs;
  unsigned short gs;
  unsigned short ip;
  unsigned short cs;
  unsigned short sp;
  unsigned short ss;
} t;

rccoord TextPos;
REGS r;
SREGS s;

void(__far __interrupt *OrginalKeyboardHnd)();
int StateBreak = 1;
int loop = 1;
int counter;

inline void buf_wyj(void) {
  unsigned char rej_stanu;
  do
    rej_stanu = (unsigned char)inp(0x64);
  while ((rej_stanu & 0x21) != 0x01);
}

void BlockKeyboard(void) {
  unsigned char scan_code = 0x01;
  unsigned char intrmask;

  cout << "\n Teraz klawiatura jest wylaczona, testowane sa jedynie porty "
          "klawiatury\n ESC - odblokowuje klawiature"
       << endl;

  intrmask = (unsigned char)inp(0x21);
  outp(0x21, intrmask | 0x02);

  do {
    buf_wyj();
    scan_code = (unsigned char)inp(0x60);
    cout << "\t " << dec << (int)scan_code << "\t";
    if (scan_code & 0x80) {
      cout << " Puszczony\t" << hex << (int)scan_code << endl << endl;
    } else {
      cout << " Wcisniety\t" << hex << (int)scan_code << endl;
    }
  } while (scan_code != 0x01);

  outp(0x21, intrmask);

  cout << " Klawiatura zostala odblokowana" << endl;
}

void ClearScreen(void) {
  char *token = (char *)0xB8000;
  // memset((void*)0xB8000,0,4000);
  for (int i = 0; i < 2000; i++) {
    *(token + (i << 1)) = 0;
  }
}

void __far __interrupt KeyboardHnd() {
  static int prefix = 0, pressedLCTR = 0, pressedRCTR = 0;
  static int semaphore = 1, ofseet = 0;
  static char Token = '0';
  char *token = (char *)0xB8000;
  int Old, old, key, forport, right;

  // int key,forport;
  Old = ofseet + 160;
  *(token + ofseet) = Token;
  *(token + Old) = ' ';

  if (Token == '9')
    Token = '0';
  else
    Token++;
  if (ofseet < (4000 - 2))
    ofseet += 2;
  else {
    ofseet = 0;
    ClearScreen();
  }

  if ((ofseet % 320) == 160) {
    ofseet += 160;
    if (ofseet >= 4000) {
      ofseet = 0;
      ClearScreen();
    }
  }

  if (semaphore) {
    semaphore = 0;

    key = inp(0x60);
    forport = inp(0x61);
    outp(0x61, forport | 0x80);
    outp(0x20, 0x20);
    outp(0x61, forport);

    old = 1;
    right = 0;
    switch (key) {
    case 0xE1:
      old = 0;
    case 0xE0:
      prefix = key;
      break;
    default:
      switch (prefix)
      /*{
          case 0xE0: if(key!=29) old=1;
          case 0xE2: prefix=0; break;
          case 0xE1: if((key==29)||(key==157)) prefix++; break;
          default:  if(key!=29) old=1;
                    break;
      }*/
      {
      case 0xE1:
        old = 0;
        if ((key == 29) || (key == 157))
          prefix++;
        break;
      case 0xE2:
        old = 0;
        prefix = 0;
        break;
      case 0xE0:
        right = 1;
      default:
        switch (key) {
        case 29:
          if (right)
            pressedRCTR = 1; // Wcisniecie CTR
          else
            pressedLCTR = 1;
          break;
        case 157:
          if (right)
            pressedRCTR = 0; // Puszczenie CTR
          else
            pressedLCTR = 0;
          break;
        case 70:
        case 44:
        case 46:
          if (pressedLCTR || pressedRCTR)
            old = 0; // Wcisnieto 'z' lub 'c' gdy wcisniety jest lewy lub
                     // prawy CTR, to nie wprowadzaj tego znaku ('z' lub
                     // 'c') do bufora
          break;
        }
        prefix = 0;
        break;
      }
    }

    if (old) {
      *(token + Old) = '!';
      (*OrginalKeyboardHnd)();
    }
    semaphore = 1;
  }
}

void TestKEYBOARD(void) {
  counter = 0;
  cout << endl << " TEST KLAWIATURY" << endl;
  OrginalKeyboardHnd = _dos_getvect(0x09);
  _dos_setvect(0x09, KeyboardHnd);

  cout << " Klawiatura przejeta. Wyjscie ESC" << endl;

  TextPos = _gettextposition();
  ClearScreen();
  do {
    if (kbhit()) {
      int token = getch();
      switch (token) {
      case 27:
        loop = 0;
        break;
      }
    } else {
      _settextposition(TextPos.row, TextPos.col);
      cout << counter++;
    }
  } while (loop);

  _dos_setvect(0x09, OrginalKeyboardHnd);
  cout << endl << "Klawiatura orginalna" << endl;
}

void main(void) { TestKEYBOARD(); }

/*{
    case 0xE0: if(key!=29) old=1;
    case 0xE2: prefix=0; break;
    case 0xE1: if((key==29)||(key==157)) prefix++; break;
    default:  if(key!=29) old=1;
              break;
}*/
