/**************************************************************************
*** Plik  : DEFINE.H                                                    ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 3 listopada 1995r.                                          ***
***************************************************************************
*** Opis  : Plik definicyjny stale uzywane przez klasy MENEGERWAV       ***
***         i OPERWAV                                                   ***
**************************************************************************/

#ifndef DEFINICJE_STALYCH_DLA_KLAS_MENEGERWAV_I_OPERWAV
#define DEFINICJE_STALYCH_DLA_KLAS_MENEGERWAV_I_OPERWAV

#define BLAD_DLA_KONSTRUKTORA 0xFF
#define BRAK_BLADU_PRZYDZIALU 0x00

#define JEST_GRANY 1
#define NIE_JEST_GRANY 0

//#define WIELKOSC_PAMIECI  0x7080 //450kB
#define NULLL 0
#define WAV_ZALADOWANY 0
#define BRAK_PLIKU_WAV 2
#define ZA_KROTKI_NAGLOWEK_WAV 3
#define BLEDNY_NAGLOWEK_WAV 4
#define BRAK_PAMIECI_NA_PROBKI 5
#define NIEODCZYTANE_PROBKI 6
#define NIEPOPRAWNE_PROBKI 7

#define PROBKI_DMA_ZAINSTALOWANE 0
#define BRAK_WOLNEGO_UCHWYTU_DMA 1
#define BRAK_WOLNEGO_UCHWYTU 0xFFFF

#define PROBA_ZWOLNIENIA_ZBYT_DUZEGO_UCHWYTU 8
#define PROBA_ZWOLNIENIA_UCHWYTU_NULL 9

#define PROBA_REZERWACJI_PAMIECI_POD_NIENULLOWY_WSKAZNIK 10
#define ZA_DLUGIE_PROBKI 11
#define PROBA_ZWOLNIENIA_UCHWYTU_OBECNIE_ODTWARZANEGO 12
#define PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU 13
#define PROBA_ODTWORZENIA_UCHWYTU_NULL 14
#define PROBA_ODTWORZENIA_UCHWYTU_OBECNIE_ODTWARZANEGO 15
#define PROBA_ODTWORZENIA_PRZED_ZAKONCZENIEM_ODTWARZANIA 16
#define ROZKAZ_PRZERWANIA_ODTWARZANIA 17
#define BRAK_SOUND_BLASTERA 18

#define MAXYMALNA_ILOSC_STRON_FIZYCZNYCH 4
#define BRAK_PAMIECI_EMS 19
#define BLAD_EMSa_NIEUDANA_PROBA_UMIESZCZENIA_STRONY_W_OKNIE 20
#define BLAD_PRZY_ZWALNIANIU_PAMIECI_EMS 21
#define BRAK_EMSa_W_PAMIECI 22
#define EMS_JEST_W_PAMIECI 0
#define BLAD_PRZY_POBRANIU_SEGMENTOW_OKIEN_EMS 24
#define PROBA_PRZYDZIALU_ZBYT_DUZEGO_BLOKU_EMS 25
#define BLAD_PRZYDZIALU_PAMIECI_EMS 26
#define BLAD_PRZY_POBRANIU_WERSJI_EMS 27
#define WERSJA_EMS_WCZESNIEJSZA_NIZ_40 28
#define BLAD_PRZY_ODCZYCIE_LICZBY_STRON_FIZYCZNYCH 29
#define BRAK_PAMIECI_NA_PODSTAWONE_STRUKTURY_DANYCH 30
#define BLAD_PRZY_POBRANIU_SEGMENTU_RAMKI_STRON 31
#define BRAK_WOLNEJ_STRONY_W_RAMCE 32
#define PROBA_PRZYDZIALU_BLOKU_O_ZERO_STRONACH 33
#define BLAD_PRZY_POBRANIU_MAX_LICZBY_UCHWYTOW 34
#define PROBA_ODTWORZENIA_UCHWYTU_O_MNIEJSZYM_PRIORYTECIE 35
#define BLAD_PRZYDZIALU_PAMIECI_DOS_PRZEZ_DPMI 36

////////////////////////////////////////////////////////
// Rzeczy zwracane przez cdrom
////////////////////////////////////////////////////////

#define ERROR_INT_2FH 37
#define NO_MEMORY_FOR_CD 38
#define NO_ENDING_PLAYWAV 39

#define ZWYKLE_ZALADOWANIE_WAVA 1
#define GLOBALNE_ZALADOWANIE_WAVOW 2

#define BLAD_OTWARCIA_GLOBALNEGO_PLIKU_Z_WAVAMI 40
#define BRAK_PAMIECI_NA_WSZYSTKIE_DANE_WAV 41

#endif
