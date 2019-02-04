/**************************************************************************
*** Plik  : EMS.H                                                       ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 9 listopada 1995r.                                          ***
***************************************************************************
*** Opis  : Plik deklarujacy klase EMS                                  ***
**************************************************************************/
#ifndef DEFINICJA_KLASY_EMS_DLA_MIRKOWYCH_KROW
#define DEFINICJA_KLASY_EMS_DLA_MIRKOWYCH_KROW

#include "const.h"
#include "typepas.h"

struct stronyEMS {
  word page_segment;
  word nr_page;
};

class EMS {
  static word Sprawdzono_czy_jest_EMS;
  static word JestEMS;
  static word wersja;
  static word blad;
  static word segment_okna_EMS;
  static word max_licz_uchwytow;

  /* static word liczba_stron_systemu;
   static stronyEMS far *wskaznik_stronEMS;
   static word far *wskaznik_wolnych_stron;*/

  static word Pobierz_segmenty_okien_EMS(void);
  static word Sprawdz_czy_jest_EMS(void);
  static word Pobierz_liczbe_uchwytow(void);

public:
  static word Czy_jest_EMS(void);
  static word SegmentRamki(void);
  static word PodajBlad(void);
  static word Liczba_uchwytow(void);
  static word Sprawdz_Liczba_uchwytow(void);

  /* static void Niszcz(void);

   EMS(void);
   ~EMS(void);

   static void Pisz_strony(void);
   static void Czystka(word);*/
};
#endif
