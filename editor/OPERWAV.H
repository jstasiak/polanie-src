/**************************************************************************
*** Plik  : OPERWAV.H                                                   ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klase operujaca na strukturze WAV          ***
**************************************************************************/

#ifndef DEFINICJA_KLASY_OPERWAV
#define DEFINICJA_KLASY_OPERWAV
#include "const.h"
#include "typepas.h"
#include "wav.h"
#include "ems.h"
#include "opiswava.h"

class OPERWAV
{
 //static word Segment_Okna_EMS;
 //void NAJBARDZIEJ_KRETYNSKA_FUNKCJA_ZAMIAST_KONSTRUKTORA(void);
public:

 //OPERWAV(void);
 //~OPERWAV(void);

private:
 WAV   naglowek;
 //char  nazwa[DLUGOSC_NAZWY_PLIKU];

 void TworzStructWAV(WAV*);
 word ZapiszWAVnaDysk(WAV*,char*,char*,long);

 word Czytaj_naglowek_WAV(ifstream*);
 word Naglowek_WAV_poprawny(void);
 word Zwolnij_pamiec_EMS(word);
 word Rezerwuj_pamiec_EMS_na_probki(opisWAVa&);

 word Czytaj_probki_do_bufora(ifstream*, word, word);

// void Inicjuj_daneDMA(void);
// word Podaj_Okno_EMS(void);

public:
 word ZaladujWAV_z_dysku(char*,opisWAVa&, word);
// word Zwolnij_uchwyt(opisWAVa&);
// word OdtwarzajWAV(byte,byte);
// word Czy_mozna_odtwarzac_WAV(void);
// char *tytul(void);

 //friend class MENEGERDMA;
};
#endif
