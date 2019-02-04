/**************************************************************************
*** Plik  : OPISWAWA.H                                                  ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 11 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik deklarujacy klase opisWAVa                             ***
**************************************************************************/

#ifndef DEFINICJA_KLASY_opisWAVa
#define DEFINICJA_KLASY_opisWAVa
#include "const.h"
#include "typepas.h"
#include "wav.h"

class opisWAVa
{
public:
 opisWAVa(void);
private:
 word  UchEMS;  	//Uchwyt dla EMS'a
 word  dlug_ostatniej_strony;
 byte  jest_grany;
 byte  dlug;		//w stronach 16kB
 byte  grana_strona;
 byte  zajety;
 byte  czestotliwosc;

//private:

public:
 void Init_opisWAVa(void);
 word Zwolnij_Uchwyt(void);
 word Przydziel_pamiec_EMS(long);

 friend class OPERWAV;
 friend class MENEGERDMA;
};

#endif
