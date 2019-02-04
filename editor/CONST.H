/**************************************************************************
*** Plik  : CONST.CPP                                                   ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 3 listopada 1995r.                                          ***
***************************************************************************
*** Opis  : Plik definicyjny stale uzywane przez klasy MENEGERWAV       ***
***         i OPERWAV                                                   ***
**************************************************************************/

#ifndef DEFINICJE_STALYCH_DLA_KLAS_MENEGERWAV_I_OPERWAV
#define DEFINICJE_STALYCH_DLA_KLAS_MENEGERWAV_I_OPERWAV
#include "typepas.h"

const byte BLAD_DLA_KONSTRUKTORA = 0xFF;
const byte BRAK_BLADU_PRZYDZIALU = 0x00;

const byte JEST_GRANY = 1;
const byte NIE_JEST_GRANY = 0;

//const word WIELKOSC_PAMIECI = 0x7080; //450kB
const NULLL = 0;
const word WAV_ZALADOWANY = 0;
const word BRAK_PLIKU_WAV = 2;
const word ZA_KROTKI_NAGLOWEK_WAV = 3;
const word BLEDNY_NAGLOWEK_WAV = 4;
const word BRAK_PAMIECI_NA_PROBKI = 5;
const word NIEODCZYTANE_PROBKI = 6;
const word NIEPOPRAWNE_PROBKI = 7;

const word PROBKI_DMA_ZAINSTALOWANE = 0;
const word BRAK_WOLNEGO_UCHWYTU_DMA = 1;
const word BRAK_WOLNEGO_UCHWYTU     = 0xFFFF;

const word PROBA_ZWOLNIENIA_ZBYT_DUZEGO_UCHWYTU = 8;
const word PROBA_ZWOLNIENIA_UCHWYTU_NULL = 9;

const word PROBA_REZERWACJI_PAMIECI_POD_NIENULLOWY_WSKAZNIK = 10;
const word ZA_DLUGIE_PROBKI = 11;
const word PROBA_ZWOLNIENIA_UCHWYTU_OBECNIE_ODTWARZANEGO = 12;
const word PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU = 13;
const word PROBA_ODTWORZENIA_UCHWYTU_NULL = 14;
const word PROBA_ODTWORZENIA_UCHWYTU_OBECNIE_ODTWARZANEGO = 15;
const word PROBA_ODTWORZENIA_PRZED_ZAKONCZENIEM_ODTWARZANIA = 16;
const word ROZKAZ_PRZERWANIA_ODTWARZANIA = 17;
const word BRAK_SOUND_BLASTERA = 18;

const word MAXYMALNA_ILOSC_STRON_FIZYCZNYCH = 4;
const word BRAK_PAMIECI_EMS = 19;
const word BLAD_EMSa_NIEUDANA_PROBA_UMIESZCZENIA_STRONY_W_OKNIE = 20;
const word BLAD_PRZY_ZWALNIANIU_PAMIECI_EMS = 21;
const word BRAK_EMSa_W_PAMIECI = 22;
const word EMS_JEST_W_PAMIECI = 0;
const word BLAD_PRZY_POBRANIU_SEGMENTOW_OKIEN_EMS = 24;
const word PROBA_PRZYDZIALU_ZBYT_DUZEGO_BLOKU_EMS = 25;
const word BLAD_PRZYDZIALU_PAMIECI_EMS = 26;
const word BLAD_PRZY_POBRANIU_WERSJI_EMS = 27;
const word WERSJA_EMS_WCZESNIEJSZA_NIZ_40 = 28;
const word BLAD_PRZY_ODCZYCIE_LICZBY_STRON_FIZYCZNYCH = 29;
const word BRAK_PAMIECI_NA_PODSTAWONE_STRUKTURY_DANYCH = 30;
const word BLAD_PRZY_POBRANIU_SEGMENTU_RAMKI_STRON = 31;
const word BRAK_WOLNEJ_STRONY_W_RAMCE = 32;
const word PROBA_PRZYDZIALU_BLOKU_O_ZERO_STRONACH = 33;
const word BLAD_PRZY_POBRANIU_MAX_LICZBY_UCHWYTOW = 34;
const word PROBA_ODTWORZENIA_UCHWYTU_O_MNIEJSZYM_PRIORYTECIE = 35;
#endif
