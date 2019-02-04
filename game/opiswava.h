/**************************************************************************
*** Plik  : OPISWAWA.H                                                  ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 11 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik deklarujacy klase opisWAVa                             ***
**************************************************************************/

#ifndef DEFINICJA_KLASY_opisWAVa
#define DEFINICJA_KLASY_opisWAVa
//#include "wav.h"

class opisWAVa {
public:
  opisWAVa(void);

private:
  //    int  UchEMS;      //Uchwyt dla EMS'a
  void *samples;
  //    int dlug_ostatniej_strony;
  int jest_grany;
  int size; // w stronach 16kB
  int zagrano;
  int do_zagrania;

  int zajety; //    int czestotliwosc; - poniewaz czestotliwosc jest raz na
              //    zawsze ustawiana

  // private:

public:
  void Init_opisWAVa(void);
  void ZwolnijUchwyt(void);
  //  int  Przydziel_pamiec(size_t);

  friend class OPERWAV;
  friend class MENEGERDMA;
};

#endif
