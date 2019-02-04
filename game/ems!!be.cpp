/**************************************************************************
*** Plik  : EMS.CPP                                                     ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 9 listopada 1995r.                                          ***
***************************************************************************
*** Opis  : Plik definicyjny klase EMS                                  ***
**************************************************************************/

#include <dos.h>
#include <conio.h>
#include "ems.h"
#include <iostream.h>


word EMS::Sprawdzono_czy_jest_EMS = 0;
word EMS::JestEMS = BRAK_EMSa_W_PAMIECI;
word EMS::wersja = 0;
word EMS::blad  = 0;
word EMS::segment_okna_EMS = 0;
word EMS::max_licz_uchwytow = 0;

/*word           EMS::liczba_stron_systemu = 0;
stronyEMS far *EMS::wskaznik_stronEMS = 0;
word      far *EMS::wskaznik_wolnych_stron = 0;*/

word EMS::Pobierz_segmenty_okien_EMS(void)
{
 word ilStr;
 word segmOkna;
 word ilosc_uchwytow;
 byte err;
 byte wer;

 JestEMS = BRAK_EMSa_W_PAMIECI;
//Artur tu pracuj
/*
 asm mov ax,0x4600      //Podaj wersje EMSa
 asm int 0x67
 asm or ah,ah
 asm jz Pytaj_o_ilosc_stron
 asm mov err,ah
 blad = err;
 
 return BLAD_PRZY_POBRANIU_WERSJI_EMS;

 Pytaj_o_ilosc_stron:
 asm mov wer,al;
 wersja = wer;
 if(wersja<0x40)
   return WERSJA_EMS_WCZESNIEJSZA_NIZ_40;
*/

 ilosc_uchwytow = Pobierz_liczbe_uchwytow();
 if(ilosc_uchwytow)
  return ilosc_uchwytow;


  //to juz bylo wyremowane
 /*asm mov ax,0x5801    //Podaj liczbe stron fizycznych
 asm int 0x67
 asm or ah,ah
 asm jz Rezerwuj_miejsce_pod_tablice_stron
 asm mov err,ah
 blad = err;
 return BLAD_PRZY_ODCZYCIE_LICZBY_STRON_FIZYCZNYCH;

 Rezerwuj_miejsce_pod_tablice_stron:
 asm mov ilStr,cx
 liczba_stron_systemu = ilStr;

 wskaznik_stronEMS = new stronyEMS[liczba_stron_systemu];
 wskaznik_wolnych_stron = new word[liczba_stron_systemu];

 if((!wskaznik_stronEMS) || (!wskaznik_wolnych_stron))
  return BRAK_PAMIECI_NA_PODSTAWONE_STRUKTURY_DANYCH;

 for(ilStr = 0; ilStr < liczba_stron_systemu; ilStr++)
   wskaznik_wolnych_stron[ilStr] = 0;

 word sges = FP_SEG(wskaznik_stronEMS);
 word ofdi = FP_OFF(wskaznik_stronEMS);
 asm push es
 asm push di

 asm mov ax,sges
 asm mov di,ofdi
 asm mov es,ax
 asm mov ax,0x5800  //Podaj tablice stron
 asm int 0x67

 asm pop di
 asm pop es

 asm or ah,ah
 asm jz Moge_uznac_ze_EMS_jest_taki_jaki_mi_odpowiada
 asm mov err,ah
 blad = err;
 return BLAD_PRZY_POBRANIU_SEGMENTOW_OKIEN_EMS; Poprawne_okno_EMS:*/


//Artur tu pracuj
/*
 Odczyt_segmentu_okna_EMS:

 
 asm mov ax,0x4100
 asm int 0x67
 asm or ah,ah
 asm jz Moge_uznac_ze_EMS_jest_taki_jaki_mi_odpowiada
 asm mov err,ah

 blad = err;
 return BLAD_PRZY_POBRANIU_SEGMENTU_RAMKI_STRON;

 Moge_uznac_ze_EMS_jest_taki_jaki_mi_odpowiada:

 asm mov segmOkna,bx
 segment_okna_EMS = segmOkna;
 JestEMS = EMS_JEST_W_PAMIECI;
 */
 return 0;
}

word EMS::Czy_jest_EMS(void)
{
 if(Sprawdzono_czy_jest_EMS) return JestEMS;
 if(Sprawdz_czy_jest_EMS() == EMS_JEST_W_PAMIECI)
   Pobierz_segmenty_okien_EMS();

 return JestEMS;
}

word EMS::Sprawdz_czy_jest_EMS(void)
{
 Sprawdzono_czy_jest_EMS = 1;
 //Artur tu pracuj
 /*
 JestEMS = BRAK_EMSa_W_PAMIECI;
 static char nazwa_EMM[8] = {"EMMXXXX0"};
 asm push ds
 asm push es
 asm mov ax,0x3567
 asm int 0x21
 asm mov di,0x000A
 asm lea si,nazwa_EMM
 asm mov cx,8
 asm cld
 asm repz cmpsb
 asm pop es
 asm pop ds
 asm jnz BrakEMSa
 */
 return EMS_JEST_W_PAMIECI;
 
 BrakEMSa:
 return BRAK_EMSa_W_PAMIECI;
}

word EMS::SegmentRamki(void)
{
 return segment_okna_EMS;
}

word EMS::PodajBlad(void)
{
 return blad;
}

word EMS::Pobierz_liczbe_uchwytow(void)
{
 word liczba_uchwytow = 0;
 word err;
//Artur tu pracuj
/*
 asm mov ax,0x5402
 asm int 0x67
 asm or ah,ah
 asm jz MamyLiczbe
 asm mov al,0
 asm mov err,ax
 blad = err;
 return BLAD_PRZY_POBRANIU_MAX_LICZBY_UCHWYTOW;

 MamyLiczbe:
 asm mov liczba_uchwytow,bx
 max_licz_uchwytow = liczba_uchwytow;
 */
 return 0;
}

word EMS::Liczba_uchwytow(void)
{
 return max_licz_uchwytow;
}

word EMS::Sprawdz_Liczba_uchwytow(void)
{
 Pobierz_liczbe_uchwytow();
 return max_licz_uchwytow;
}
/*void EMS::Niszcz(void)
{
 if(wskaznik_stronEMS)
   delete [] wskaznik_stronEMS;
 if(wskaznik_wolnych_stron)
   delete [] wskaznik_wolnych_stron;
}


EMS::EMS(void)  { Czy_jest_EMS();}
EMS::~EMS(void) { Niszcz();}

void EMS::Pisz_strony(void)
{
 word strona_fizyczna;
 word semm;
 word Uchwyt;
 if(!liczba_stron_systemu)
  {
   cout << "\nLiczba stron fizycznych wynosi o zgrozo "<<liczba_stron_systemu;
   return;
  }else cout << "\nLiczba stron fizycznych wynosi "<<liczba_stron_systemu;

 asm mov ax,0x4200
 asm int 0x67



 for(byte i = 0; i<liczba_stron_systemu; i++)
  {
   asm mov ax,0x4300
   asm mov bx,1
   asm int 0x67
   asm or ah,ah

   asm jz Dalejze:
   cout<<"\nNieudana alokacja jednej glupiej strony";
   return;

   Dalejze:
   asm mov Uchwyt,dx;

   semm = wskaznik_stronEMS[i].page_segment;
   strona_fizyczna = wskaznik_stronEMS[i].nr_page;
   cout<<dec<<(int)i<<". Segment "<<hex<<semm<<"h czyli "<<dec<<(semm>>6)<<"kB. Jest to nr strony "<<hex<<strona_fizyczna<<'h'<<endl;

   if(!kbhit()) getch();

   if(strona_fizyczna<4)
   {
    asm mov ax,strona_fizyczna
    asm mov ah,0x44
    asm mov bx,0
    asm mov dx,Uchwyt
    asm int 0x67
    asm or ah,ah
    asm jnz ZnowuDalej

    cout<<"\nZrobic czystke (T)"; char znak = getch();
    if((znak=='T') || (znak=='t'))
       Czystka(semm);
   }

   asm mov dx,Uchwyt;
   asm mov ax,0x4500
   asm int 0x67
   asm or ah,ah
   asm jz OK

   cout<<"\nPamiec NIE zwolniona!!!\n";
   return;
   OK:
   cout<<"\nPamiec zwolniona\n";

   asm jmp OPodpieta

   ZnowuDalej:
   cout<<"\nNiepodpieta strona ...";

   OPodpieta:
   if(kbhit()) break;

   //delay(500);
  }
}

void EMS::Czystka(word Strona)
{
 asm mov ax,Strona
 asm push ds
 asm push si
 asm mov ds,ax
 asm mov ax,0
 asm mov si,ax
 asm mov cx,16000

 Znowu_zero:
 asm mov ds:[si],al
 asm inc si
 asm loop Znowu_zero

 asm pop si
 asm pop ds
} */
