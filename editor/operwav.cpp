/**************************************************************************
*** Plik  : OPERWAV.CPP                                                 ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klase operujaca na strukturze WAV          ***
**************************************************************************/
#include "operwav.h"
#include <dos.h>     //FP_SEG, FP_OFF
#include <fstream.h> //ifstream, ofstream
#include <string.h>  //strcpy()

// word OPERWAV::Segment_Okna_EMS = 0;

/*void OPERWAV::NAJBARDZIEJ_KRETYNSKA_FUNKCJA_ZAMIAST_KONSTRUKTORA(void)
{
// strncpy(nazwa,"NONAME.",DLUGOSC_NAZWY_PLIKU);
// jest_grany = 0;
/// UchEMS     = 0;
// dlug       = 0;
// dlug_ostatniej_strony = 0;
// zajety     = 0;
} */

/*OPERWAV::OPERWAV(void)
{
 NAJBARDZIEJ_KRETYNSKA_FUNKCJA_ZAMIAST_KONSTRUKTORA();
}

OPERWAV::~OPERWAV(void)
{
 word identyfikator;
 if(zajety)
  Zwolnij_pamiec_EMS();
} */

void OPERWAV::TworzStructWAV(WAV *wav) {
  strcpy(wav->RIFF, "RIFF");
  wav->ile_po_8_bajcie = 0;
  strcpy(wav->WAVE, "WAVE");
  strcpy(wav->fmt, "fmt ");
  wav->wFormatTag = 1;
  wav->nChannels = 0;
  wav->nSamplesPerSec = 0;
  wav->nAvgBytesPerSec = 0;
  wav->nBlockAlign = 0;
  wav->nBitsPerSample = 0;
  strcpy(wav->data, "data");
  wav->ile_po_data = 0;
}

word OPERWAV::ZapiszWAVnaDysk(WAV *, char *, char *, long) { return 0; }

word OPERWAV::ZaladujWAV_z_dysku(char *nazwa_pliku, opisWAVa &WAVek,
                                 word wolna_strona_fizyczna) {
  ifstream plikWAV;
  // byte plik_otwarty = 0;
  word rez;

  plikWAV.open(nazwa_pliku, ios::binary);
  if (!(plikWAV.good()))
    //   plik_otwarty = 1;
    // else
    return BRAK_PLIKU_WAV; // Brak wskazanego pliku;

  rez = Czytaj_naglowek_WAV(&plikWAV);

  if (!rez)
    rez = Naglowek_WAV_poprawny();

  if (!rez)
    rez = Rezerwuj_pamiec_EMS_na_probki(WAVek);

  if (rez) // Gdy przydzial sie nie powiodl
  {
    // if(plik_otwarty)
    plikWAV.close();
    return rez;
  }

  rez = Czytaj_probki_do_bufora(&plikWAV, wolna_strona_fizyczna, WAVek.UchEMS);

  // if(plik_otwarty)
  plikWAV.close();

  if (rez) {
    Zwolnij_pamiec_EMS(WAVek.UchEMS);
    WAVek.Init_opisWAVa();
    return rez;
  }
  return WAV_ZALADOWANY;
}

word OPERWAV::Czytaj_naglowek_WAV(ifstream *plikWAV) {
  word rezultat_czytania;
  byte bajt;
  byte *wskaznik_na_glowka_WAV;

  wskaznik_na_glowka_WAV = (byte *)&naglowek;

  for (word i = 0; i < sizeof(WAV); i++) {
    plikWAV->get(bajt);
    rezultat_czytania = plikWAV->good();
    if (rezultat_czytania) {
      *wskaznik_na_glowka_WAV = bajt;
      wskaznik_na_glowka_WAV++;
    } else
      break;
  }

  if (rezultat_czytania)
    return 0;
  return ZA_KROTKI_NAGLOWEK_WAV;
}

word OPERWAV::Naglowek_WAV_poprawny(void) {
  word rez = 0;
  byte *bajt = (byte *)&naglowek;

  rez = strncmp((char *)bajt, "RIFF", 4);
  if (!rez)
    rez = strncmp((char *)bajt + 0x08, "WAVE", 4);
  if (!rez)
    rez = strncmp((char *)bajt + 0x0C, "fmt ", 4);
  if (!rez)
    rez = strncmp((char *)bajt + 0x24, "data", 4);
  if (!rez)
    rez = (word)!naglowek.nSamplesPerSec; // Czy czestotliwosc <> zera

  if (rez)
    return BLEDNY_NAGLOWEK_WAV;
  return rez; // Czyli zero - NAGLOWEK POPRAWNY;
}

word OPERWAV::Zwolnij_pamiec_EMS(word identyfikator) {
  // Artur tu pracuj
  /* asm mov ax,0x4500    //Zwolnij strony
   asm mov dx,identyfikator;
   asm int 0x67;
   asm or ah,ah
   asm jz Zwolniono_pamiec_EMS*/
  return BLAD_PRZY_ZWALNIANIU_PAMIECI_EMS;
Zwolniono_pamiec_EMS:

  // opis.Init_opisWAVa();
  return 0;
}

word OPERWAV::Rezerwuj_pamiec_EMS_na_probki(opisWAVa &opis) {
  word identyfikator;
  word dlugosc;
  // word bladd;
  long long_dlugosc;
  // if(zajety) return PROBA_REZERWACJI_PAMIECI_POD_NIENULLOWY_WSKAZNIK;
  // if((naglowek.ile_po_data) > (long)0x0000FFFF ) return ZA_DLUGIE_PROBKI;

  long_dlugosc = naglowek.ile_po_data + 0x00003FFF;
  long_dlugosc = long_dlugosc >> 14; // w stronach 16kB
  dlugosc = (word)long_dlugosc;

  if (!dlugosc)
    return PROBA_PRZYDZIALU_BLOKU_O_ZERO_STRONACH;
  // Artur tu pracuj
  /*
     asm mov bx,dlugosc
     asm mov ax,0x4300    //Przydziel BX stron 16kB
     asm int 0x67

     asm or ah,ah
     asm jz EMS_przydzielil
  */
  return BRAK_PAMIECI_EMS;

EMS_przydzielil:
  // Artur tu pracuj
  // asm mov identyfikator,dx
  // dalej juz nie pracuj

  // zajety = 1;    //Nie wolno o tym zapomniec !

  opis.UchEMS = identyfikator;
  opis.dlug = (byte)dlugosc;
  dlugosc = (word)(naglowek.ile_po_data & 0x00003FFF);
  if (!dlugosc)
    dlugosc = 0x4000;
  opis.dlug_ostatniej_strony = dlugosc - 1;
  opis.zajety = 1;
  opis.czestotliwosc = 211;

  return 0; // Pamiec zarezerwowana;    //probki = //(char*)
            // farmalloc(naglowek.ile_po_data); //new char
            // [naglowek.ile_po_data];
}

word OPERWAV::Czytaj_probki_do_bufora(ifstream *plikWAV,
                                      word wolna_strona_fizyczna,
                                      word identyfikator) {
  word segmWolnejStronyEMS;

  segmWolnejStronyEMS = EMS::SegmentRamki();
  if (wolna_strona_fizyczna)
    segmWolnejStronyEMS += 0x400 << (wolna_strona_fizyczna - 1);

  word rezultat_czytania;
  byte bajt;
  word ewentualny_nr_strony;
  word logiczny_nr_strony = 0xFFFF;
  char *staly_wskaznik_na_dane, *wskaznik_na_dane;

  staly_wskaznik_na_dane = (char *)MK_FP(segmWolnejStronyEMS, 0);
  wskaznik_na_dane = staly_wskaznik_na_dane;

  for (long i = 0; i < naglowek.ile_po_data; i++) {
    ewentualny_nr_strony = (word)(i >> 14);
    if (ewentualny_nr_strony != logiczny_nr_strony) {
      // Artur tu pracuj
      /*   logiczny_nr_strony = ewentualny_nr_strony;
         asm mov ax,wolna_strona_fizyczna;
         asm mov ah,0x44    //Podlacz strone
         asm mov bx,logiczny_nr_strony;
         asm mov dx,identyfikator
         asm int 0x67
         asm or ah,ah
         asm jz Strona_jest_w_oknie

        */
      return BLAD_EMSa_NIEUDANA_PROBA_UMIESZCZENIA_STRONY_W_OKNIE;

    Strona_jest_w_oknie:
      wskaznik_na_dane = staly_wskaznik_na_dane;
    } // if end

    plikWAV->get(bajt);
    rezultat_czytania = plikWAV->good();
    if (rezultat_czytania) {
      *wskaznik_na_dane = bajt;
      wskaznik_na_dane++;
    } else
      break;
  }

  if (rezultat_czytania)
    return 0;
  return NIEODCZYTANE_PROBKI;
}

/*void OPERWAV::Inicjuj_daneDMA(void)
{
 long adres_liniowy_poczatku_danych;
 long adres_liniowy_konca_danych;
 byte Ppoczatku;
 byte Pkonca;

 //adres_liniowy_poczatku_danych = (((long)FP_SEG(probki))<<4) +
(long)FP_OFF(probki); adres_liniowy_poczatku_danych = ((long)probki)<<4;
 adres_liniowy_konca_danych = adres_liniowy_poczatku_danych +
naglowek.ile_po_data - 1; Ppoczatku = (byte)
(adres_liniowy_poczatku_danych>>16); Pkonca    = (byte)
(adres_liniowy_konca_danych>>16);

 int aa234=naglowek.nSamplesPerSec;
 cz1.Czestotliwosc = (byte)(256 -(1000000/aa234));
 cz2.Czestotliwosc = cz1.Czestotliwosc;

 if(Ppoczatku == Pkonca)
 {
  cz1.ostatni = 1;
  cz1.P = Ppoczatku;
  cz1.XXYY.ofset_dma   = (word) (adres_liniowy_poczatku_danych & 0x0000FFFF);
  cz1.VVZZ.dlugosc_dma = (word) naglowek.ile_po_data;
 }
 else
 {
  cz1.ostatni = 0;
  cz2.ostatni = 1;
  cz1.P = Ppoczatku;
  cz2.P = Pkonca;

  cz1.XXYY.ofset_dma = (word) (adres_liniowy_poczatku_danych & 0x0000FFFF);
  cz2.XXYY.ofset_dma = 0;

  //Ha no wlasnie - co teraz?
  cz1.VVZZ.dlugosc_dma = (word) ((adres_liniowy_konca_danych & 0xFFFF0000) -
adres_liniowy_poczatku_danych - 1); cz2.VVZZ.dlugosc_dma = (word)
(adres_liniowy_konca_danych & 0x0000FFFF) + 1;
 }
} */

/*word OPERWAV::Zwolnij_uchwyt(void)
{
 word BLAD_ZWOLNIENIA_PAMIECI;



 BLAD_ZWOLNIENIA_PAMIECI = Zwolnij_pamiec_EMS();

 if(BLAD_ZWOLNIENIA_PAMIECI)
  return BLAD_ZWOLNIENIA_PAMIECI;

 NAJBARDZIEJ_KRETYNSKA_FUNKCJA_ZAMIAST_KONSTRUKTORA();  //Przywrocenie stanu
poczatkowego return 0;
} */

/*word OPERWAV::Czy_mozna_odtwarzac_WAV(void)
{
 if(!zajety)    return PROBA_ODTWORZENIA_UCHWYTU_NULL;
 if(jest_grany) return PROBA_ODTWORZENIA_UCHWYTU_OBECNIE_ODTWARZANEGO;
 return 0;
}

char *OPERWAV::tytul(void) { return nazwa; }*/
