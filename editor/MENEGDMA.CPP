/**************************************************************************
*** Plik  : MENEGWAV.CPP                                                ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klase OPREWAV operujaca na strukturze WAV  ***
**************************************************************************/

#include <string.h>             //strncpy()
#include <malloc.h>      //NULL
#include <iostream.h>   //ifstream
#include "menegdma.h"
#include "sb.h"

#include <iostream.h>
#include <conio.h>         //getch();

//void UstawAdlibPresent(byte);

//byte     MENEGERDMA::Droga = 0;
///word     MENEGERDMA::pisz = 0;

//word     MENEGERDMA::dopuszczalny_kanal[4] = {0x0087,0x0083,0x0081,0x0082};


word     MENEGERDMA::Rejest_strony_kanalu = 0x0083;
word     MENEGERDMA::Rejest_adresowy_kanalu;
word     MENEGERDMA::Rejest_licznika_kanalu;

word     MENEGERDMA::odtwarzaj_kazdy = 1;
word     MENEGERDMA::pamiec = 0;
word     MENEGERDMA::PSP = 0;
word     MENEGERDMA::koniecPSP = 0;
word     MENEGERDMA::max_ilosc_melodii_DMA = 0;
word     MENEGERDMA::ilosc_melodii_DMA = 0;
word     MENEGERDMA::wolny_uchwyt = BRAK_WOLNEGO_UCHWYTU;
word     MENEGERDMA::ostatnio_wczytany = 0;
word     MENEGERDMA::Segment_Okna_EMS = 0;
word     MENEGERDMA::jest_w_oknie = MAXYMALNA_ILOSC_STRON_FIZYCZNYCH;

word MENEGERDMA::odtwarzany_uchwyt = 0;  //Uchwyt obecnie odtwarzanego kawalka
word MENEGERDMA::odtwarzana_strona_fizyczna = MAXYMALNA_ILOSC_STRON_FIZYCZNYCH;
word MENEGERDMA::odtwarzana_strona_logiczna = 0;

word MENEGERDMA::nastepna_czesc_uchwyt;
word MENEGERDMA::przygotowany_do_odtwarzania_uchwyt;

byte MENEGERDMA::wolna_str[MAXYMALNA_ILOSC_STRON_FIZYCZNYCH] = {0,0,0,0};

byte MENEGERDMA::nastepna_czesc_strona = 0;
byte MENEGERDMA::przygotowany_do_odtwarzania_strona = 0;
byte MENEGERDMA::FMy;

//byte MENEGERDMA::nie_ma_cz2 = 1;  //No bo jej nie ma na poczatku;
byte MENEGERDMA::jest_odtwarzany = 0;    //jesli jest odtwarzany == 1;
byte MENEGERDMA::przerwano_DMA = 0;

opisWAVa *MENEGERDMA::WAVy;

byte MENEGERDMA::IRQ = 0xFF;
byte MENEGERDMA::Jest_SB = 0;
word MENEGERDMA::KanalDMA = 0;
word MENEGERDMA::WersjaSB = 0;

word MENEGERDMA::zawartosc_portow_DMA[5];
byte MENEGERDMA::MaskaIRQ;
byte MENEGERDMA::NowaMaskaIRQ;

byte MENEGERDMA::nastepna_strona_gotowa;
byte MENEGERDMA::nr_gotowej_strony;
/*byte MENEGERDMA::P;
byte MENEGERDMA::YY;
byte MENEGERDMA::XX;
byte MENEGERDMA::ZZ;
byte MENEGERDMA::VV;*/
//word MENEGERDMA::Rejest_strony_kanalu = 0x0000;

MENEGERDMA::MENEGERDMA(word ile_MAX_chcesz_miec_melodii_WAV)
{
// word Kanal;
// byte irq;
// word Wersja;

 WAVy = NULL;

 if(!(EMS::Czy_jest_EMS()))
 if(EMS::SegmentRamki())
 if(ile_MAX_chcesz_miec_melodii_WAV)
 if(!Rezerwuj_Pamiec(ile_MAX_chcesz_miec_melodii_WAV)) //Zwraca ilosc obiektow OPERWAV
  {
   cprintf("\nBrak pamieci na podstawowe struktury danych");
   //asm mov ax,0x4C00
   //asm int 0x21
   
  }
//Artur tu pracuj
/* OrginalnaKlawiatura = getvect(0x23);

 if(OrginalnaKlawiatura)
  setvect(0x23,NowaKlawiatura);
 asm mov ax,0x3301  //Zapisanie znacznika BREAK
 asm mov dl,0       //Znacznik ma byc wylaczony
 asm int 0x21
*/
}
/////////////////////////////////////////////////////////////////////////////
//MENEGERDMA::MENEGERDMA(word ile_MAX_chcesz_miec_melodii_WAV,word Kanal,byte irq)
/*MENEGERDMA::MENEGERDMA(word ile_MAX_chcesz_miec_melodii_WAV,byte Adlib,byte jest_SB, byte irq, word port, word kanal)
{
   Init(ile_MAX_chcesz_miec_melodii_WAV, Adlib, jest_SB, irq, port, kanal);
} */
/////////////////////////////////////////////////////////////////////////////////
MENEGERDMA::MENEGERDMA(void)
{
}
/////////////////////////////////////////////////////////////////////////////////
void MENEGERDMA::Init(/*byte Adlib,*/byte jest_SB, byte irq, word port, word kanal)
{                                                                                              // 0x220     0x0083
 //WAVy = NULL;
 Jest_SB = 0;
 return;
/* if(!WAVy) return;
// UstawAdlibPresent(Adlib); Przeniesione do funkcji faduj
 if(!(EMS::Czy_jest_EMS()))
 if(EMS::SegmentRamki())
// if(ile_MAX_chcesz_miec_melodii_WAV)
// if(Rezerwuj_Pamiec(ile_MAX_chcesz_miec_melodii_WAV)) //Zwraca ilosc obiektow OPERWAV
 if(jest_SB)
 {//TUUUUUUU
  IRQ = irq;
  KanalDMA = port;  //0x220
  Rejest_strony_kanalu = kanal;    //0x0083
  daj_kanal();  //Ustawia Rejest_adresowy_kanalu i Rejest_licznika_kanalu
  Jest_SB = 1;  //na podstawie Rejest_strony_kanalu

 }//TUUUUUUU

 if(Jest_SB)
 {
  zawartosc_portow_DMA[0] = inportb(0x0002);
  zawartosc_portow_DMA[1] = inportb(0x0002);
  zawartosc_portow_DMA[2] = inportb(0x0083);
  zawartosc_portow_DMA[3] = inportb(0x0003);
  zawartosc_portow_DMA[4] = inportb(0x0003);

  MaskaIRQ = inportb(0x0021);
  irq = 0x01<<(IRQ-8);
  irq = 0xFF ^ irq;
  NowaMaskaIRQ = MaskaIRQ & irq;

  OrginalneIRQ = getvect(IRQ);
  setvect(IRQ,NoweIRQ);
  outportb(0x0021,NowaMaskaIRQ);    //Zezwolenie na przerwanie nr IRQ
  Rozkaz_dla_SB(0xD1);  //Odblokuj glosnik

  Rozkaz_dla_SB(0x40);    //Ustawienie czestotliwosci probkowania
  Rozkaz_dla_SB(211);       //Rozkaz_dla_SB(211); -22050Hz

 }*/
}

MENEGERDMA::~MENEGERDMA(void)
{
 deInit();
// Artur tu pracuj
/*
 for(word i = 0; i<max_ilosc_melodii_DMA; i++)
   if(WAVy[i].zajety)
     WAVy[i].Zwolnij_Uchwyt();

 if(OrginalnaKlawiatura)
  setvect(0x23,OrginalnaKlawiatura);
 */ 
}

void MENEGERDMA::deInit(void)
{
    // Artur tu pracuj
    /*
 if(Jest_SB)
 {
  outportb(0x0021,MaskaIRQ);
  setvect(IRQ,OrginalneIRQ);
  outportb(0x0002,zawartosc_portow_DMA[0]);
  outportb(0x0002,zawartosc_portow_DMA[1]);
  outportb(0x0083,zawartosc_portow_DMA[2]);
  outportb(0x0003,zawartosc_portow_DMA[3]);
  outportb(0x0003,zawartosc_portow_DMA[4]);

  Rozkaz_dla_SB(0xD3); //Wylaczenie glosnika
  Jest_SB = 0;
 }
 */
}
//rem by Artur luty 96
/*void TEST(byte znak)
{
   static word przesun = 0;

   asm push si
   asm push ds

   asm mov ax,0xB800
   asm mov si,przesun
   asm mov ds,ax
   asm mov al,znak
   asm mov ds:[si],al

   asm pop ds
   asm add si,2
   asm mov przesun,si
   asm pop si
} */


void interrupt (*MENEGERDMA::OrginalneIRQ)(...);

void interrupt MENEGERDMA::NoweIRQ(...)
{
 // Artur tu pracuj
 /*   
 static byte semafor = 0;

 asm cli
 inportb(KanalDMA + 0x000E);
 outportb(0x0020,0x20);  //Przerwanie zostalo obsluzone

// TEST('$');

 if(!semafor)
  {
   semafor = 1;
   asm sti

  // TEST('X');
   (WAVy[odtwarzany_uchwyt].grana_strona)++;
   if((WAVy[odtwarzany_uchwyt].grana_strona) < (WAVy[odtwarzany_uchwyt].dlug))
     {
    //  TEST('Y');
     // pisz = 0;
      Odtwarzaj_DMA();
   //   WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
   //   jest_odtwarzany = NIE_JEST_GRANY;
   //   wolna_str[odtwarzana_strona_fizyczna] = 0;
   //   przerwano_DMA = 0;
     }
   else
     {
  //    TEST('Z');
      WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
      jest_odtwarzany = NIE_JEST_GRANY;
      odtwarzany_uchwyt = NIE_JEST_GRANY;
      odtwarzaj_kazdy  = 1;
      wolna_str[odtwarzana_strona_fizyczna] = 0;
      przerwano_DMA = 0;
     }
   semafor = 0;
  }
  */
}

/* WAVy[Uchwyt].jest_grany = JEST_GRANY;
 jest_odtwarzany = JEST_GRANY;
 odtwarzany_uchwyt = Uchwyt;
 WAVy[odtwarzany_uchwyt].grana_strona = 0;

 return Odtwarzaj_DMA();*/
/* if(1)
  {
   Przerwij_odtwarzanie_DMA(); //Przenioslem text wprosc
   Rozkaz_dla_SB(0xD0); //Przerwij transmisje DMA

  }
 else
  {
   //Rozkazss_dla_SB(0xD0); //Przerwij transmisje DMA
  // Odtwarzaj_DMA(&(pierwszy_OPERWAV[odtwarzany_uchwyt].cz2));
  }*/
void interrupt (*MENEGERDMA::OrginalnaKlawiatura)(...) = 0;

void interrupt MENEGERDMA::NowaKlawiatura(...)
{
/* asm pushf
 asm push ax
 asm in al,0x60
 asm cmp al,29  //Wcisnieto CTR
 asm jz koniec
 asm cmp al,69  //Wcisnieto PAUSE
 asm jz koniec
 asm cmp al,29+128  //Puszczono CTR
 asm jz koniec
 asm cmp al,69+128  //Puszczono PAUSE
 asm jz koniec

 asm pop ax
 asm popf
 (*OrginalnaKlawiatura)();
 asm jmp drugi_koniec

 koniec:
 asm mov al,0x20
 asm out 0x0020,al
 asm pop ax
 asm popf
 drugi_koniec:*/
}

/*void MENEGERDMA::daj_kanal(void)
{
 for(word i=0; i<4; i++)
  {
   if(Rejest_strony_kanalu == dopuszczalny_kanal[i])
    break;
  }

 if(i>3)
  {
   Rejest_strony_kanalu = 0x0083;
   Rejest_adresowy_kanalu = 2;
   Rejest_licznika_kanalu = 3;
   return;
  }

 Rejest_adresowy_kanalu = 2*i;
 Rejest_licznika_kanalu = Rejest_adresowy_kanalu + 1;
}     */

void MENEGERDMA::daj_kanal(void)
{
 switch(Rejest_strony_kanalu)
  {
   case 0x0087 : Rejest_adresowy_kanalu = 0; break;
   case 0x0083 : Rejest_adresowy_kanalu = 2; break;
   case 0x0081 : Rejest_adresowy_kanalu = 4; break;
   case 0x0082 : Rejest_adresowy_kanalu = 6; break;
   default : Rejest_strony_kanalu = 0x0083; Rejest_adresowy_kanalu = 2; break;
  }
  Rejest_licznika_kanalu = Rejest_adresowy_kanalu + 1;
}

word MENEGERDMA::Rezerwuj_Pamiec(word ile_chcesz_miec_melodii_DMA)
{
 static byte pamiec_rezerwowano = 0;

 if(pamiec_rezerwowano) return max_ilosc_melodii_DMA;

 ile_chcesz_miec_melodii_DMA++;

 do
 {
  ile_chcesz_miec_melodii_DMA--;
  WAVy = new opisWAVa[ile_chcesz_miec_melodii_DMA];
 } while((!WAVy) && (ile_chcesz_miec_melodii_DMA));

 max_ilosc_melodii_DMA = ile_chcesz_miec_melodii_DMA;;
 if(max_ilosc_melodii_DMA) pamiec_rezerwowano = 1;
 return max_ilosc_melodii_DMA; //Boze! dzieki ze pamietales za mnie o tym +i...
}

word MENEGERDMA::ile_mozesz_miec_DMA(void) { return max_ilosc_melodii_DMA; }

word MENEGERDMA::ile_masz_DMA(void) { return ilosc_melodii_DMA; }

/*void MENEGERDMA::Osiagniecia(void)
{
 byte irq = IRQ;
 if(IRQ) irq -= 8;
 cout<<"\nRAPORT DETEKCJI KARTY";
 cout<<"\nKanal DMA : "<<hex<<KanalDMA<<'h';
 cout<<"\nNumer IRQ : "<<(word)irq<<'h';
 cout<<"\nWersja Sound Blastera "<<dec<<(WersjaSB>>8)<<'.'<<(WersjaSB & 0x00FF)<<endl;
}
  */
//*****************************************
//**** OBLGUGA ZWIEZANA Z ODTWARZANIEM ****
//*****************************************

//*****************************************************************
//**** Zwraca nr bledu, gdy operacja wczytania pliku sie       ****
//****     nie powiodla                                        ****
//**** Zwraca 0, gdy operacja wczytania pliku sie powiodla     ****
//****     wtedy aktywowane sa obiekty OPERWAV wraz z naglow-  ****
//****     kami WAV, rezerwowana jest pamiec dla danych, wczy- ****
//****     tywane sa do niej probki. Na koniec inicjowane sa   ****
//****     struktury daneDMA oraz zmienna ostatnio_wczytany    ****
//****     zawierajaca indeks tablicy pierwszy_OPERWAV, gdzie  ****
//****     wczytane sa probki                                  ****
//*****************************************************************
word MENEGERDMA::InstalujWAV(char *nazwa)
{
 OPERWAV NowyWAV;
 word wolna_strona_fizyczna = Daj_wolna_strone();

 if(Czy_jest_wolny_uchwyt())
   return BRAK_WOLNEGO_UCHWYTU_DMA;

 if(wolna_strona_fizyczna>=MAXYMALNA_ILOSC_STRON_FIZYCZNYCH)
   return BRAK_WOLNEJ_STRONY_W_RAMCE;

 wolna_str[wolna_strona_fizyczna] = 1;  //Rezewrujemy stone na potrzeby ladowania WAVA

 word rezultat_czytania = NowyWAV.ZaladujWAV_z_dysku(nazwa,WAVy[wolny_uchwyt],wolna_strona_fizyczna);

 wolna_str[wolna_strona_fizyczna] = 0;  //Zwracamy rezerwacje - strona wracado puli wolnych

 if(rezultat_czytania) return rezultat_czytania; //Jesli czytanie sie nie powiodlo zwroc przyczyne bledu

 ilosc_melodii_DMA++;   //jesli sie powiodlo zwieksz licznik wczytanych melodii
 ostatnio_wczytany = wolny_uchwyt;
 wolny_uchwyt = BRAK_WOLNEGO_UCHWYTU;
 return PROBKI_DMA_ZAINSTALOWANE; //i poinformuj o tym radosnym wydarzeniu
}

//*********************************************
//**** Zwraca 1, gdy break wolnego uchwytu ****
//**** Zwraca 0, jest wolny uchwyt, wtedy  ****
//****     do wolny_uchwyt wpisywany jest  ****
//****     wlasciwy numer                  ****
//*********************************************
word MENEGERDMA::Czy_jest_wolny_uchwyt(void)
{
 if(ilosc_melodii_DMA<max_ilosc_melodii_DMA)
 {
  for(word i = 0; i<max_ilosc_melodii_DMA; i++)
  {
   if(!(WAVy[i].zajety)) //Jesli pierwszy_OPERWAV[i] != NULL
   {
    wolny_uchwyt = i;
    return 0;
   }
  }
 }
 return BRAK_WOLNEGO_UCHWYTU_DMA;
}

word MENEGERDMA::Zwolnij_uchwyt(word numer)
{
 if(numer>=max_ilosc_melodii_DMA) return PROBA_ZWOLNIENIA_ZBYT_DUZEGO_UCHWYTU;
 if(WAVy[numer].jest_grany) return PROBA_ZWOLNIENIA_UCHWYTU_OBECNIE_ODTWARZANEGO;
 if(!(WAVy[numer].zajety)) return PROBA_ZWOLNIENIA_UCHWYTU_NULL;

 return WAVy[numer].Zwolnij_Uchwyt();
}

byte MENEGERDMA::Wolno_grac(void) { return !jest_odtwarzany; }
byte MENEGERDMA::Przerwano_DMA(void) { return przerwano_DMA; }

word MENEGERDMA::Odtwarzaj(word Uchwyt)
{
 word wolna;
 if(!Jest_SB) return BRAK_SOUND_BLASTERA;
 if(Uchwyt >= max_ilosc_melodii_DMA) return PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU;
 if(jest_odtwarzany) return PROBA_ODTWORZENIA_PRZED_ZAKONCZENIEM_ODTWARZANIA;

 if(!(WAVy[Uchwyt].zajety)) return PROBA_ODTWORZENIA_UCHWYTU_NULL;
 if(WAVy[Uchwyt].jest_grany) return PROBA_ODTWORZENIA_UCHWYTU_OBECNIE_ODTWARZANEGO;

 if(jest_w_oknie>=MAXYMALNA_ILOSC_STRON_FIZYCZNYCH)         //Czy muzyka jest juz w oknie
  {
   wolna = Daj_wolna_strone();
   if(wolna>=MAXYMALNA_ILOSC_STRON_FIZYCZNYCH)
      return BRAK_WOLNEJ_STRONY_W_RAMCE;

   odtwarzana_strona_fizyczna = wolna;
   wolna_str[odtwarzana_strona_fizyczna] = 1;
  }

 return Odtwarzaj_WAVa(Uchwyt);
}

word MENEGERDMA::operator()(word Uchwyt)
{
 if(Uchwyt >= max_ilosc_melodii_DMA) return PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU;
 if(!odtwarzaj_kazdy)
   if(Uchwyt<odtwarzany_uchwyt+1)
      return PROBA_ODTWORZENIA_UCHWYTU_O_MNIEJSZYM_PRIORYTECIE;

 if(jest_odtwarzany)
  Przerwij_odtwarzanie_DMA();

 return Odtwarzaj(Uchwyt);
}

/*void MENEGERDMA::wolne(void)
{
 for(word i = 0; i<4; i++)
  cout<<"\nStrona "<<i<<". - "<<(word)wolna_str[i];
} */

word MENEGERDMA::Odtwarzaj_DMA()
{
 static word segmOknaEMS = EMS::SegmentRamki();
 static word segm;
 static byte Po;
// static byte Cz;
 static byte XX;
 static byte YY;
 static byte VV;
 static byte ZZ;
 static word identyfikator;
 static word str_fizyczna;
 static word str_logiczna;

 przerwano_DMA = 1;

// Cz            = WAVy[odtwarzany_uchwyt].czestotliwosc;

 identyfikator = WAVy[odtwarzany_uchwyt].UchEMS;
 str_logiczna  = WAVy[odtwarzany_uchwyt].grana_strona;
 str_fizyczna  = odtwarzana_strona_fizyczna;

// if(pisz) { wolne(); getch(); }
// Artur tu pracuj
 /*  asm mov dx,identyfikator
   asm mov ax,str_fizyczna
   asm mov ah,0x44
   asm mov bx,str_logiczna
   asm int 0x67
   asm or ah,ah
   asm jz Udalo_sie_wpiac_strone
*/
   ///TU KOD PRZERYWAJACY
   return BLAD_EMSa_NIEUDANA_PROBA_UMIESZCZENIA_STRONY_W_OKNIE;

   Udalo_sie_wpiac_strone:
/* if(pisz)
  {
  cout<<"\nJestem - chociaz to slowo przynosi mi pecha...";
  getch(); }*/
//   odtwarzana_strona_logiczna = 0;
// WAVy[odtwarzany_uchwyt].grana_strona = 1;

 segm = segmOknaEMS;
 if(str_fizyczna)
   segm += 0x400<<(str_fizyczna-1);
// Artur tu pracuj
/* asm mov ax,segm
 asm push ax
 asm mov cl,12
 asm shr ax,12
 asm mov Po,al

 asm pop ax
 asm mov cl,4
 asm shl ax,cl  //AX = offset
 //asm add ax,10        //
 asm mov YY,al
 asm mov XX,ah
*/
 if((WAVy[odtwarzany_uchwyt].dlug) == (WAVy[odtwarzany_uchwyt].grana_strona + 1))
  {
   segm = WAVy[odtwarzany_uchwyt].dlug_ostatniej_strony;
 // Artur tu pracuj
 //  asm mov ax,segm
  }
 else
  //asm mov ax,0x4000
 // Artur tu pracuj
 //    asm mov ax,0x3FFF  //<<to popraw

  //asm sub ax,10     //<to juz bylo zaremowane 
// Artur tu pracuj
 /* asm mov ZZ,al      
  asm mov VV,ah

 outportb(0x000A,0x05);
 outportb(0x000C,0x00);
 outportb(0x000B,0x49);

 outportb(Rejest_adresowy_kanalu,YY);
 outportb(Rejest_adresowy_kanalu,XX);  // outportb(0x0003,ZZ);
 outportb(Rejest_strony_kanalu,Po); // outportb(0x0003,VV); Jak milo, ze nie trzeba tego pchac

 outportb(Rejest_licznika_kanalu,ZZ);
 outportb(Rejest_licznika_kanalu,VV);

 outportb(0x000A,0x01);
*/
// Rozkaz_dla_SB(0x40);    //Ustawienie czestotliwosci probkowania
// Rozkaz_dla_SB(Cz);       //Rozkaz_dla_SB(0); - Niskie Basy

 Rozkaz_dla_SB(0x14);
 Rozkaz_dla_SB(ZZ);
 Rozkaz_dla_SB(VV);    //Rozkaz_dla_SB(0x91); - Udalo sie bez tego
 return 0;
}

word MENEGERDMA::Daj_wolna_strone(void)
{
 word i, rez = MAXYMALNA_ILOSC_STRON_FIZYCZNYCH;
 for(i = 0; i<MAXYMALNA_ILOSC_STRON_FIZYCZNYCH; i++)
  if(wolna_str[i] == 0) { rez = i; break; }
 return rez;
}

word MENEGERDMA::Odtwarzaj_WAVa(word Uchwyt)
{
 WAVy[Uchwyt].jest_grany = JEST_GRANY;
 jest_odtwarzany = JEST_GRANY;
 odtwarzany_uchwyt = Uchwyt;
 WAVy[odtwarzany_uchwyt].grana_strona = 0;

// pisz = 1;

 odtwarzaj_kazdy  = 0;
 return Odtwarzaj_DMA();
}

void MENEGERDMA::Przerwij_odtwarzanie_DMA(void)
{
 //TEST('R');
 Rozkaz_dla_SB(0xD0);
 //inportb(KanalDMA + 0x000E);
 delay(2);
 WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
 jest_odtwarzany   = NIE_JEST_GRANY;
 odtwarzany_uchwyt = NIE_JEST_GRANY;
 odtwarzaj_kazdy  = 1;
 wolna_str[odtwarzana_strona_fizyczna] = 0; //Czyli wolna strona
}

void MENEGERDMA::Rozkaz_dla_SB(byte Rozkaz)
{
 static byte port;
 word licznik = 1;
 do
  {
      // Artur tu pracuj
  /* port = inportb(KanalDMA + 0x000C);
  */
   port = port & 0x80;
   licznik++;
   if(!licznik){WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
                jest_odtwarzany = NIE_JEST_GRANY;
                odtwarzany_uchwyt = NIE_JEST_GRANY;
                odtwarzaj_kazdy  = 1;
                wolna_str[odtwarzana_strona_fizyczna] = 0;
                przerwano_DMA = 0;
        return;}
  }while(port);
  // Artur tu pracuj
 /*outportb(KanalDMA + 0x000C,Rozkaz);*/
}

/*char *MENEGERDMA::tytul(word Uchwyt)
{
// if((Uchwyt >= max_ilosc_melodii_DMA) || (!(pierwszy_OPERWAV[Uchwyt].probki))) return "NONAME.";

 return pierwszy_OPERWAV[Uchwyt].tytul();
} */

/*word MENEGERDMA::Detektuj_pamiec(void)
{
 asm mov ah,0x62
 asm int 0x21
 asm mov ax,bx
 PSP = _AX;

 return 0;
}

void MENEGERDMA::InfoPamiec(void)
{
 char tytul[9] = {0,0,0, 0,0,0, 0,0,0};
 word psp;
 word pam;
 byte blad = 0xFF;
 byte znacznik;
 Detektuj_pamiec();

 psp = PSP;

   znacznik = *(byte*)MK_FP(psp-1,0);
   strncpy(tytul,(char*)MK_FP(psp-1,8),8);
   koniecPSP = *(word*)MK_FP(psp-1,3);
   cout<<"\nZnacznik - "<<hex<<(word)znacznik<<"   Tytul : "<<tytul;
   cout<<"\nSegment PSP : "<<hex<<*(word*)MK_FP(psp-1,1)<<"h w odroznieniu od prawdziwego PSP: "<<hex<<psp;
   cout<<"\nWielkosc pamieci : "<<hex<<koniecPSP<<'h'<<endl;

 psp = psp + koniecPSP;
   znacznik = *(byte*)MK_FP(psp,0);
   strncpy(tytul,(char*)MK_FP(psp,8),8);
   koniecPSP = *(word*)MK_FP(psp,3);
   cout<<"\nZnacznik - "<<hex<<(word)znacznik<<"   Tytul : "<<tytul;
   cout<<"\nSegment PSP : "<<hex<<*(word*)MK_FP(psp,1)<<"h w odroznieniu od prawdziwego PSP: "<<hex<<psp;
   cout<<"\nWielkosc pamieci : "<<hex<<koniecPSP<<'h'<<endl;

   asm mov ah,0x48
   asm mov bx,0xFFFF
   asm int 0x21
   asm jc blad_01
   asm mov byte ptr blad,0x00
   asm mov word ptr pam,ax
   asm jmp blad_00

  blad_01:
   asm mov byte ptr blad,0x01
   asm mov word ptr pam,bx
  blad_00:

  if(blad)
   {
    cout<<"Blad : "<<dec<<(int)blad<<"   Maxymalna wlna pamiec : "<<hex<<pam;
   }
  asm mov ah,0x48
  asm mov bx,pam
  asm int 0x21

   asm jc blad_02
   asm mov byte ptr blad,0x03
   asm mov word ptr pam,ax
   asm jmp blad_03

  blad_02:
   asm mov byte ptr blad,0x02
   asm mov word ptr pam,bx
  blad_03:

  if(blad==3)
   {
    cout<<" Wymagana pamiec zostala przydzielona, segment "<<hex<<pam<<'h';
    asm mov ax,pam
    asm push es
    asm mov es,ax
    asm mov ah,0x49
    asm int 0x21
    asm pop es
   }
  if(blad == 2) cout<<" I tym razem zabraklo pamieci...";
}
*/
