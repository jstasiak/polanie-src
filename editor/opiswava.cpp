/**************************************************************************
*** Plik  : OPISWAWA.CPP                                                ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 11 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klasy opisWAVa                             ***
**************************************************************************/

#include "opiswava.h"

opisWAVa::opisWAVa(void) { Init_opisWAVa(); }

void opisWAVa::Init_opisWAVa(void) {
  jest_grany = 0;
  UchEMS = 0; // Uchwyt dla EMS'a
  dlug = 0;   // w stronach 16kB
  dlug_ostatniej_strony = 0;
  grana_strona = 0;
  zajety = 0;
  czestotliwosc = 0;
}

word opisWAVa::Zwolnij_Uchwyt(void) {
  word Uchwyt = UchEMS;
  // Artur tu pracuj
  /*asm mov dx,Uchwyt
  asm mov ax,0x4500
  asm int 0x67
  asm or ah,ah
  asm jz Poprawne_zwolnienie_pamieci_EMS
 */
  return BLAD_PRZY_ZWALNIANIU_PAMIECI_EMS;

Poprawne_zwolnienie_pamieci_EMS:
  Init_opisWAVa();
  return 0;
}

word opisWAVa::Przydziel_pamiec_EMS(long wielkosc) {
  word rozmiar;
  word Wielkosc;
  word Uchwyt;

  if (wielkosc > 0xFFFFC000)
    return PROBA_PRZYDZIALU_ZBYT_DUZEGO_BLOKU_EMS;
  Wielkosc = wielkosc;

  wielkosc += 0x00003FFF;
  wielkosc = wielkosc >> 14;

  rozmiar = wielkosc;
  // Artur tu pracuj
  /*
   asm mov ah,0x43    //Przydziel strony
   asm mov bx,rozmiar
   asm int 0x67
   asm or ah,ah
   asm jz Przydzielono_pamiec_EMS
  */
  return BLAD_PRZYDZIALU_PAMIECI_EMS;

Przydzielono_pamiec_EMS:
  // Artur tu pracuj
  // asm mov Uchwyt,dx;

  UchEMS = Uchwyt; // Uchwyt dla EMS'a
  dlug = rozmiar;  // w stronach 16kB
  dlug_ostatniej_strony = Wielkosc & 0x00003FFF;
  return 0;
}
