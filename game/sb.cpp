/*
  Plik : SB.CPP
  Autor: Artur Bidzinski
  Opis : Poprawione 10 wrzesnia 1996r.
*/

#include "sb.h"
#include <conio.h> //inp() outp()
#include <dos.h>   //_dos_getvect()
#include <i86.h>   //delay()
const int CZAS_NA_IRQ = 100;
int sb::Odczytano_maske_przerwan = 0;
int sb::Maska_przerwan;
int sb::Byla_detekcja_DMA = 0;
int sb::Byla_detekcja_IRQ = 0;
int sb::Adres_DMA = 0;
int sb::Nr_IRQ = 0;
int sb::WersjaSB = 0;
void(__interrupt __far *sb::org_IRQ)() = 0;
void(__interrupt __far *sb::fun)();

int sb::Detekcja_portu_DMA(void) {
  if (Byla_detekcja_DMA)
    return Adres_DMA;

  const int WARTOSC = 0xAA;
  const int LICZNIK_100 = 100;
  const int LICZNIK_10 = 10;

  int znaleziono = 0;
  int Domniemany_adres, licznik_100, licznik_10;
  int port;

  for (Domniemany_adres = 0x210; Domniemany_adres <= 0x260;
       Domniemany_adres = Domniemany_adres + 0x0010) {
    for (licznik_10 = 0; licznik_10 < LICZNIK_10; licznik_10++) {
      outp(Domniemany_adres + 0x0006, 1); // Tu nalezy odczekac 3usekundy
      delay(1);                           // czekaj 1000usek.
      outp(Domniemany_adres + 0x0006, 0);

      for (licznik_100 = 0; licznik_100 < LICZNIK_100; licznik_100++) {
        port = inp(Domniemany_adres + 0x000E) & 0x80;
        if (port)
          break;
      }
      port = inp(Domniemany_adres + 0x000A);
      if (port == WARTOSC) {
        znaleziono = 1;
        break;
      }
    }
    if (znaleziono)
      break;
  }

  if (znaleziono) {
    Adres_DMA = Domniemany_adres;
    Byla_detekcja_DMA = 1;
  } else
    Adres_DMA = 0;
  return Adres_DMA;
}

void __interrupt __far sb::Przerwanie_IRQ() {
  Byla_detekcja_IRQ = 1;
  inp(Adres_DMA + 0x000E);
  outp(0x0020, 0x20); // Przerwanie zostalo obsluzone
}

void sb::Zezwol_na_przerwanie(int nr) {
  int Maska = 0x01, NowaMaska;
  Maska = Maska << nr;
  Maska = 0xFFFFFFFF ^ Maska;
  NowaMaska = sb::Maska_przerwan & Maska;

  outp(0x0021, NowaMaska);
}

int sb::Oczekuj_na_Przerwanie(int nr) {
  int wartosc;
  Zezwol_na_przerwanie(nr);
  // void (__interrupt __far *fun)();
  fun = _dos_getvect(8 + nr);
  _dos_setvect(8 + nr, Przerwanie_IRQ);
  WyslijBajt(0x40);
  WyslijBajt(156);
  WyslijBajt(0x80);
  WyslijBajt(1);
  WyslijBajt(0);
  delay(CZAS_NA_IRQ);
  outp(0x0021, sb::Maska_przerwan); // Przywroc dawna maske przerwan
  if (Byla_detekcja_IRQ) {
    Nr_IRQ = 8 + nr;
    Byla_detekcja_IRQ = 1;
    wartosc = 0;
  } else {
    Przerwanie_IRQ();
    Byla_detekcja_IRQ = 0;
    wartosc = 1;
  }
  _dos_setvect(8 + nr, fun);
  return wartosc;
}

int sb::Detekcja_IRQ_DMA(void) {
  if (Byla_detekcja_DMA) {
    if (!Odczytano_maske_przerwan) {
      Odczytano_maske_przerwan = 1;
      Maska_przerwan = inp(0x0021);
    }
    if (!Byla_detekcja_IRQ)
      if (Oczekuj_na_Przerwanie(2))
        if (Oczekuj_na_Przerwanie(3))
          if (Oczekuj_na_Przerwanie(5))
            if (Oczekuj_na_Przerwanie(7))
              ;
  }
  return sb::Nr_IRQ;
}

void sb::WyslijBajt(int rozkaz) {
  // Poprawić absolutnie poprawic
  while (inp(Adres_DMA + 0x000C) & 0x0080)
    ;
  outp(Adres_DMA + 0x000C, rozkaz);
}

int sb::Pobierz_wersjeSB(void) {
  int wer = 0;
  int port;
  WyslijBajt(0xE1);
  do {
    port = (int)inp(Adres_DMA + 0x000E);
    port = port & 0x80;
  } while (!port);
  wer = ((int)inp(Adres_DMA + 0x000A)) << 8;
  do {
    port = inp(Adres_DMA + 0x000E);
    port = port & 0x80;
  } while (!port);

  port = (int)inp(Adres_DMA + 0x000A);
  wer = wer + port;
  if (wer)
    WersjaSB = wer;
  return WersjaSB;
}

int sb::Podaj_port_DMA(void) { return sb::Adres_DMA; }
int sb::Podaj_nr_IRQ(void) { return sb::Nr_IRQ; }
int sb::Podaj_Maske_przerwan(void) { return sb::Maska_przerwan; }
int sb::Podaj_WersjaSB(void) { return sb::WersjaSB; }

sb::sb(void) {
  if (!Odczytano_maske_przerwan) {
    Odczytano_maske_przerwan = 1;
    sb::Maska_przerwan = inp(0x0021);
  }
}

sb::~sb(void) {
  if (Odczytano_maske_przerwan) {
    Odczytano_maske_przerwan = 0;
    outp(0x0021, sb::Maska_przerwan);
  }
  if (org_IRQ != 0) {
    _dos_setvect(sb::Nr_IRQ, sb::org_IRQ);
  }
}
