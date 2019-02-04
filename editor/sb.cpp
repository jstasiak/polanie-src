#include "sb.h"
const word CZAS_NA_IRQ = 100;
byte sb::Odczytano_maske_przerwan = 0;
byte sb::Maska_przerwan;
byte sb::Byla_detekcja_DMA = 0;
byte sb::Byla_detekcja_IRQ = 0;
word sb::Adres_DMA = 0;
byte sb::Nr_IRQ    = 0;
word sb::WersjaSB  = 0;
void interrupt (*sb::org_IRQ)(...) = 0;

word sb::Detekcja_portu_DMA(void)
{
 if(Byla_detekcja_DMA) return Adres_DMA;

 const byte WARTOSC     = 0xAA;
 const word LICZNIK_100 = 100;
 const word LICZNIK_10  = 10;

 byte port, znaleziono = 0;
 word Domniemany_adres,licznik_100,licznik_10;

 for(Domniemany_adres = 0x210; Domniemany_adres<=0x260; Domniemany_adres = Domniemany_adres + 0x0010)
  {
   for(licznik_10 = 0; licznik_10<LICZNIK_10; licznik_10++)
    {
     outportb(Domniemany_adres + 0x0006,1);    //Tu nalezy odczekac 3usekundy
     delay(1);  //czekaj 1000usek.
     outportb(Domniemany_adres + 0x0006,0);

     for(licznik_100 = 0 ; licznik_100<LICZNIK_100; licznik_100++)
      {
       port = inportb(Domniemany_adres + 0x000E) & 0x80;
       if(port) break;
      }
    port = inportb(Domniemany_adres + 0x000A);
    if(port == WARTOSC)
       { znaleziono = 1; break; }
    }
    if(znaleziono) break;
  }

 if(znaleziono)
  {
   Adres_DMA = Domniemany_adres;
   Byla_detekcja_DMA = 1;
  } else Adres_DMA = 0;
 return Adres_DMA;
}

void interrupt sb::Przerwanie_IRQ(...)
{
 Byla_detekcja_IRQ = 1;
 inportb(Adres_DMA + 0x000E);
 outport(0x0020,0x20);  //Przerwanie zostalo obsluzone
}

void sb::Zezwol_na_przerwanie(byte nr)
{
 byte Maska = 0x01,NowaMaska;
 Maska = Maska<<nr;
 Maska = 0xFF^Maska;
 NowaMaska = sb::Maska_przerwan & Maska;

 outport(0x0021,NowaMaska);
}

int sb::Oczekuj_na_Przerwanie(byte nr)
{
 int wartosc;
 Zezwol_na_przerwanie(nr);
 void interrupt (*fun)(...);
 fun = getvect(8+nr);
 setvect(8+nr,Przerwanie_IRQ);
 WyslijBajt(0x40);
 WyslijBajt(156);
 WyslijBajt(0x80);
 WyslijBajt(1);
 WyslijBajt(0);
 delay(CZAS_NA_IRQ);
 outportb(0x0021,sb::Maska_przerwan); //Przywroc dawna maske przerwan
 if(Byla_detekcja_IRQ)
  {
   Nr_IRQ  = 8+nr;
   Byla_detekcja_IRQ = 1;
   wartosc = 0;
  } else
  {
   Przerwanie_IRQ();
   Byla_detekcja_IRQ = 0;
   wartosc = 1;
  }
 setvect(8+nr,fun);
 return wartosc;
}

byte sb::Detekcja_IRQ_DMA(void)
{
 if(Byla_detekcja_DMA)
  {
   if(!Odczytano_maske_przerwan)
     {
      Odczytano_maske_przerwan = 1;
      Maska_przerwan = inportb(0x0021);
     }
   if(!Byla_detekcja_IRQ)
    if(Oczekuj_na_Przerwanie(2))
      if(Oczekuj_na_Przerwanie(3))
       if(Oczekuj_na_Przerwanie(5))
         if(Oczekuj_na_Przerwanie(7));
  }
 return sb::Nr_IRQ;
}

void sb::WyslijBajt(byte rozkaz)
{
 while(inportb(Adres_DMA + 0x000C) & 0x0080);
 outportb(Adres_DMA + 0x000C, rozkaz);
}

word sb::Pobierz_wersjeSB(void)
{
 word wer = 0;
 byte port;
 WyslijBajt(0xE1);
 do
 {
  port = inportb(Adres_DMA + 0x000E);
  port = port & 0x80;
 } while(!port);
 wer = ((word)inportb(Adres_DMA + 0x000A))<<8;
 do
 {
  port = inportb(Adres_DMA + 0x000E);
  port = port & 0x80;
 } while(!port);
 port = inportb(Adres_DMA + 0x000A);
 wer  = wer + (word)port;
 if(wer)
    WersjaSB = wer;
 return WersjaSB;
}

word sb::Podaj_port_DMA(void) { return sb::Adres_DMA; }
byte sb::Podaj_nr_IRQ(void) { return sb::Nr_IRQ; }
byte sb::Podaj_Maske_przerwan(void) { return sb::Maska_przerwan; }
word sb::Podaj_WersjaSB(void) { return sb::WersjaSB; }

sb::sb(void)
{
 if(!Odczytano_maske_przerwan)
  {
   Odczytano_maske_przerwan = 1;
   sb::Maska_przerwan = inportb(0x0021);
  }
}

sb::~sb(void)
{
 if(Odczytano_maske_przerwan)
  {
   Odczytano_maske_przerwan = 0;
   outportb(0x0021,sb::Maska_przerwan);
  }
 if(org_IRQ!=0) setvect(sb::Nr_IRQ,sb::org_IRQ);
}