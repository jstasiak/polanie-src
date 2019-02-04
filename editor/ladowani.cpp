//#include <iostream.h>
#include "EMS.h" //WYCIAC
#include "menegdma.h"
#include <conio.h> //getch()
#include <dos.h>   //delay()
#include <stdio.h>

extern MENEGERDMA SND;
extern byte JestSB;
extern byte irq;
extern word port;
extern word kanal;
/*word instal = 0;

word daj_m(word znak)
{
 switch(znak)
  {
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9': return (znak - 48);
   case '-': return 10;
   case '=': return 11;
   case 'q': return 12;
   case 'w': return 13;
   case 'e': return 14;
   case 'r': return 15;

   case 't': return 16;
   case 'y': return 17;
   case 'u': return 18;
   case 'i': return 19;
   case 'o': return 20;
   case 'p': return 21;
   case '[': return 22;
   case ']': return 23;
   case 'a': return 24;
   case 's': return 25;
   case 'd': return 26;
   case 'f': return 27;
   case 'g': return 28;
   case 'h': return 29;
   case 'j': return 30;
   case 'k': return 31;

   case 'l': return 32;
   case ';': return 33;
   case '\'': return 34;
   case 'z': return 35;
   case 'x': return 36;
   case 'c': return 37;
   case 'v': return 38;
   case 'b': return 39;
   case 'n': return 40;
   case 'm': return 41;
   case ',': return 42;
   case '.': return 43;
   case '/': return 44;
   case '`': return 45;
   case '~': return 46;
   case '\\': return 47;

  }//switch()
 return 0xFFFF;
} */

// void UstawAdliba(void);
extern void UstawAdlibPresent(unsigned char Wartosc);

void Inst(char *nazwa, MENEGERDMA &SND) {
  word rez;
  rez = SND.InstalujWAV(nazwa);
  // cout<<'\b';
  if (rez)
    cprintf("-");
  // cout<<nazwa<<" - blad instalacji "<<(word)rez<<endl;
  else {
    //   instal++;
    cprintf(".");
    // cout<<nazwa<<" zainstalowany"<<endl;
  }
}

int ladowanie(void) {
  byte Adlib = 0;
  byte jest_SB = 0;

  FILE *plik;
  if ((plik = fopen("SETUP.INI", "rb")) == NULL)
    return 1;

  fread(&Adlib, 1, 1, plik);
  fread(&jest_SB, 1, 1, plik);
  fread(&irq, 1, 1, plik);
  fread(&port, 2, 1, plik);
  fread(&kanal, 2, 1, plik);
  fclose(plik);

  JestSB = jest_SB;

  UstawAdlibPresent(Adlib);
  SND.Init(jest_SB, irq, port, kanal);
  // SND.Osiagniecia();
  if (jest_SB) {
    cout << "Initialisiere Sounds... ";

    Inst("data\\w001.dat", SND);
    Inst("data\\w002.dat", SND);
    Inst("data\\w003.dat", SND);
    Inst("data\\w004.dat", SND);
    Inst("data\\w005.dat", SND);
    Inst("data\\w006.dat", SND);
    Inst("data\\w007.dat", SND);
    Inst("data\\w008.dat", SND);
    Inst("data\\w009.dat", SND);
    Inst("data\\w010.dat", SND);

    Inst("data\\w011.dat", SND);
    Inst("data\\w012.dat", SND);
    Inst("data\\w013.dat", SND);
    Inst("data\\w014.dat", SND);
    Inst("data\\w015.dat", SND);
    Inst("data\\w016.dat", SND);
    Inst("data\\w017.dat", SND);
    Inst("data\\w018.dat", SND);
    Inst("data\\w019.dat", SND);
    Inst("data\\w020.dat", SND);

    Inst("data\\w021.dat", SND);
    Inst("data\\w022.dat", SND);
    Inst("data\\w023.dat", SND);
    Inst("data\\w024.dat", SND);
    Inst("data\\w025.dat", SND);
    Inst("data\\w026.dat", SND);
    Inst("data\\w027.dat", SND);
    Inst("data\\w028.dat", SND);
    Inst("data\\w029.dat", SND);
    Inst("data\\w030.dat", SND);

    Inst("data\\w031.dat", SND);
    Inst("data\\w032.dat", SND);
    Inst("data\\w033.dat", SND);
    Inst("data\\w034.dat", SND);
    Inst("data\\w035.dat", SND);
    Inst("data\\w036.dat", SND);
    Inst("data\\w037.dat", SND);
    Inst("data\\w038.dat", SND);
    Inst("data\\w039.dat", SND);
    Inst("data\\w040.dat", SND);

    Inst("data\\w041.dat", SND);
    Inst("data\\w042.dat", SND);
    Inst("data\\w043.dat", SND);
    Inst("data\\w044.dat", SND);
    Inst("data\\w045.dat", SND);
    Inst("data\\w046.dat", SND);
    Inst("data\\w047.dat", SND);
    Inst("data\\w048.dat", SND);
    Inst("data\\w049.dat", SND);
    Inst("data\\w050.dat", SND);

    Inst("data\\w051.dat", SND);
    Inst("data\\w052.dat", SND);
    Inst("data\\w053.dat", SND);
    Inst("data\\w054.dat", SND);
    Inst("data\\w055.dat", SND);
  }
  ///
  /*cout<<"\nGram..."<<EMS::Sprawdz_Liczba_uchwytow()<<endl;
  m = 0;
  znak = 1;

  do
  {
   //znak = kbhit();
   //if(znak)
   //{
    znak = getch(); m = daj_m(znak);
    // SND.Przerwij_odtwarzanie_DMA();// getch();
    // delay(1);
    if(m<instal)
     {
      //if(!SND.Odtwarzaj(m))
      if(!SND(m))
        cout<<"\nOdtwarzam muzyke TU BYL DAWNIEJ TYTUL ...sluchaj i delektuj
  sie...";
     }
   //}
  }while(znak!=27);

  if(SND.Przerwano_DMA())
   {
    SND.Przerwij_odtwarzanie_DMA();
    cout<<"\nOdtwarzanie zostlo przerwane";
   }
  else
   cout<<"\nOdtwarzanie zakonczone";
   */
  return 0;
}
