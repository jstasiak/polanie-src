/**************************************************************************
*** Plik  : MENEGWAV.H                                                  ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik deklaracyjny klasy MENEGERWAV                          ***
**************************************************************************/

#ifndef DEKLARACJA_KLASY_MENEGERWAV
#define DEKLARACJA_KLASY_MENEGERWAV

#include "opiswava.h"
class MENEGERDMA
{
 public:
 static int MultiWav;
 static int DO_zagrania;
 static int ZAgrano;
 static void *MaxSample;
 static void *SamplesForSB;
 static void *NewSamplesForSB;

 static void *GlobalDataWAV; //Pokazuje na skomasowane dane typu WAV
 static int HiInterrupt;
 static int loaded;       // 1 - jest zaladowany wav 0-nie
 static int wartosc_maski_kanalu;
 static int wartosc_rejestru_trybu;
 static int jest_odtwarzany;    //jesli jest odtwarzany == 1;
 private:

 static int dopuszczalny_kanal[4];
 static int             how_paragraphs;
 static unsigned short Selector;
 static int             SegmentForSB;

 static int Rejest_strony_kanalu;
 static int Rejest_adresowy_kanalu;
 static int Rejest_licznika_kanalu;
 static int odtwarzaj_kazdy;
 static int pamiec;          //Wielkosc dostepnej pamieci
 static int PSP;
 static int koniecPSP;
 static int max_ilosc_melodii_DMA;
 static int ilosc_melodii_DMA;
 static int wolny_uchwyt;
 static int ostatnio_wczytany;

 static int odtwarzany_uchwyt;  //Uchwyt obecnie odtwarzanego kawalka
 static int przerwano_DMA;

 static opisWAVa *WAVy;

 static int KanalDMA;
 static int IRQ;
 static int zawartosc_portow_DMA[5];
 static int LMaskaIRQ;
 static int HMaskaIRQ;
 static int NowaMaskaIRQ;

 static void (__interrupt __far *OrginalnaKlawiatura)();
 static void (__interrupt __far *OrginalTimer)();
 static void (__interrupt __far *OrginalneIRQ)();

 static void __interrupt __far NoweIRQ();
 //static int newtimer;
 static void __interrupt __far NewTime();
 static void __interrupt __far NowaKlawiatura();

 static void Rozkaz_dla_SB(int);

 int Rezerwuj_Pamiec(int);
 int Czy_jest_wolny_uchwyt(void);
 int Daj_wolna_strone(void);
 static int Odtwarzaj_DMA(void);   //(daneDMA*);
 int Odtwarzaj_WAVa(int);
 void daj_kanal(void);

public:
  static int Jest_SB;
  MENEGERDMA(int,int);
  void Init(int, int, int, int);
 ~MENEGERDMA(void);
  void deInit(void);
  int PlayWav(char *);
  static void EndPlayWav(void);

 static void Przerwij_odtwarzanie_DMA(void);
 int ile_mozesz_miec_DMA(void);
 int ile_masz_DMA(void);

 //OBLGUGA ZWIEZANA Z ODTWARZANIEM
 int InstalujWAV(char*);
 int Wolno_grac(void);
 int Przerwano_DMA(void);

 int Odtwarzaj(int);
 int operator()(int);
 int MENEGERDMA::LoadGlobalData(char*,int);

 int ZaladujWAV_z_dysku(char *nazwa_pliku,opisWAVa *wav);
};
#endif
