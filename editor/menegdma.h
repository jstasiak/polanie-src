/**************************************************************************
*** Plik  : MENEGWAV.H                                                  ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik deklaracyjny klasy MENEGERWAV                          ***
**************************************************************************/

#ifndef DEKLARACJA_KLASY_MENEGERWAV
#define DEKLARACJA_KLASY_MENEGERWAV
#include <fstream.h>
#include "const.h"
#include "typepas.h"
#include "wav.h"
#include "operwav.h"
#include "ems.h"

class MENEGERDMA
{
//public:
// static byte Droga;
public:
 static byte jest_odtwarzany;    //jesli jest odtwarzany == 1;
private:

// static word pisz;
// static word dopuszczalny_kanal[4];
 static word Rejest_strony_kanalu;
 static word Rejest_adresowy_kanalu;
 static word Rejest_licznika_kanalu;
 static word odtwarzaj_kazdy;
 static word pamiec;          //Wielkosc dostepnej pamieci
 static word PSP;
 static word koniecPSP;
 static word max_ilosc_melodii_DMA;
 static word ilosc_melodii_DMA;
 static word wolny_uchwyt;
 static word ostatnio_wczytany;

 static word Segment_Okna_EMS;
 static word jest_w_oknie;

 static word odtwarzany_uchwyt;  //Uchwyt obecnie odtwarzanego kawalka
 static word odtwarzana_strona_fizyczna;
 static word odtwarzana_strona_logiczna;
 static word nastepna_czesc_uchwyt;
 static word przygotowany_do_odtwarzania_uchwyt;

 static byte wolna_str[4];

 static byte nastepna_czesc_strona;
 static byte przygotowany_do_odtwarzania_strona;
 static byte FMy;

// static byte nie_ma_cz2;
 static byte przerwano_DMA;

 static opisWAVa *WAVy;

 static word KanalDMA;
 static byte IRQ;
 static word WersjaSB;
 static word zawartosc_portow_DMA[5];
 static byte MaskaIRQ;
 static byte NowaMaskaIRQ;

 static byte nastepna_strona_gotowa;
 static byte nr_gotowej_strony;
 /* static byte P;
 static byte YY;
 static byte XX;
 static byte ZZ;
 static byte VV;*/
// static word Kanal;
 static void interrupt (*OrginalnaKlawiatura)(...);
 static void interrupt (*OrginalneIRQ)(...);

 static void interrupt NoweIRQ(...);
 static void interrupt NowaKlawiatura(...);

/* static void interrupt (*OrginalneIRQ)(...);

 static void interrupt NoweIRQ(...);*/

 static void Rozkaz_dla_SB(byte);

 word Rezerwuj_Pamiec(word);
 word Czy_jest_wolny_uchwyt(void);
 word Daj_wolna_strone(void);
 static word Odtwarzaj_DMA(void);	//(daneDMA*);
 word Odtwarzaj_WAVa(word);
 void daj_kanal(void);

public:
  static byte Jest_SB;
  MENEGERDMA(word);
  MENEGERDMA(word, byte, byte, byte, word, word); //MENEGERDMA(word,word,byte);
  MENEGERDMA(void);
//  void Init(/*byte,*/ byte, byte, word, word); //void Init(word,word,byte);
  void Init(byte, byte, word , word);
 ~MENEGERDMA(void);
  void deInit(void);

 static void Przerwij_odtwarzanie_DMA(void);
 word ile_mozesz_miec_DMA(void);
 word ile_masz_DMA(void);

 void Osiagniecia(void);

 //OBLGUGA ZWIEZANA Z ODTWARZANIEM
 word InstalujWAV(char*);
 word Zwolnij_uchwyt(word);

 byte Wolno_grac(void);
 byte Przerwano_DMA(void);

 word Odtwarzaj(word);
 word operator()(word);
// char *tytul(word Uchwyt);
/* word Detektuj_pamiec(void);
 void InfoPamiec(void);*/
/* static void wolne(void);*/
};
#endif