/**************************************************************************
*** Plik  : OPISWAWA.CPP                                                ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 11 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klasy opisWAVa                             ***
**************************************************************************/

#include "opiswava.h"
#include "define.h"
#include <malloc.h>

opisWAVa::opisWAVa(void) { Init_opisWAVa(); }

void opisWAVa::Init_opisWAVa(void) {
  samples = 0;
  jest_grany = 0;
  size = 0; // w stronach 16kB
  //    dlug_ostatniej_strony = 0;
  zajety = 0;
  zagrano = 0;
  do_zagrania = 0;
}

void opisWAVa::ZwolnijUchwyt(void) {
  if (samples && (zajety == ZWYKLE_ZALADOWANIE_WAVA)) {
    free(samples);
  }
  Init_opisWAVa();
}

/*int opisWAVa::Przydziel_pamiec(size_t wielkosc)
{
    pamiec=malloc(wielkosc);
    if(pamiec)
    {
        dlug=(int)wielkosc;
        zajety=1;
        return 0;
    }
    return BLAD_PRZYDZIALU_PAMIECI_EMS;
}*/

/*

size_t
malloc
alloc

*/
