/**************************************************************************
*** Plik  : MENEGWAV.CPP                                                ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny klase OPREWAV operujaca na strukturze WAV  ***
**************************************************************************/
#include <stdlib.h>       //exit()
#include <stdio.h>       //exit()
#include <string.h>       //strncpy()
#include <malloc.h>       //NULL
#include <iostream.h>     //
#include <conio.h>
#include <dos.h>          //_dos_setvect()
#include "define.h"
#include "dpmi.h"
#include "menegdma.h"
//::~

#include <iostream.h>
#include <conio.h>         //cprintf()

int MENEGERDMA::MultiWav=0;
int MENEGERDMA::DO_zagrania=0;
int MENEGERDMA::ZAgrano=0;
void *MENEGERDMA::MaxSample=0;
void *MENEGERDMA::SamplesForSB=0;
void *MENEGERDMA::NewSamplesForSB=0;
void *MENEGERDMA::GlobalDataWAV=0;
int MENEGERDMA::HiInterrupt=0;  // Jeden, gdy obslugiwane jest wyzsze przerwanie
int MENEGERDMA::loaded=0;
int MENEGERDMA::dopuszczalny_kanal[4] = {0x0087,0x0083,0x0081,0x0082};
int licznik;

int MENEGERDMA::wartosc_maski_kanalu=0;
int MENEGERDMA::wartosc_rejestru_trybu=0;

int             MENEGERDMA::how_paragraphs=0;
unsigned short MENEGERDMA::Selector=0;
int             MENEGERDMA::SegmentForSB=0;

int     MENEGERDMA::Rejest_strony_kanalu = 0x0083;
int     MENEGERDMA::Rejest_adresowy_kanalu;
int     MENEGERDMA::Rejest_licznika_kanalu;

int     MENEGERDMA::odtwarzaj_kazdy = 1;
int     MENEGERDMA::pamiec = 0;
int     MENEGERDMA::PSP = 0;
int     MENEGERDMA::koniecPSP = 0;
int     MENEGERDMA::max_ilosc_melodii_DMA = 0;
int     MENEGERDMA::ilosc_melodii_DMA = 0;
int     MENEGERDMA::wolny_uchwyt = BRAK_WOLNEGO_UCHWYTU;
int     MENEGERDMA::ostatnio_wczytany = 0;

int MENEGERDMA::odtwarzany_uchwyt = 0;  //Uchwyt obecnie odtwarzanego kawalka

int MENEGERDMA::jest_odtwarzany = 0;    //jesli jest odtwarzany == 1;
int MENEGERDMA::przerwano_DMA = 0;

opisWAVa *MENEGERDMA::WAVy;

int MENEGERDMA::IRQ = 0xFF;
int MENEGERDMA::Jest_SB = 0;
int MENEGERDMA::KanalDMA = 0;

int MENEGERDMA::zawartosc_portow_DMA[5];
int MENEGERDMA::LMaskaIRQ=0;
int MENEGERDMA::HMaskaIRQ=0;
int MENEGERDMA::NowaMaskaIRQ;

MENEGERDMA::MENEGERDMA(int ile_MAX_chcesz_miec_melodii_WAV, int paragraphs)
{
 unsigned short Segment_Address;
 OrginalTimer = OrginalnaKlawiatura = 0;
 Jest_SB = 0;
 if(alloc_DOS_memory((unsigned short)(paragraphs<<1), Segment_Address, Selector))
 {
     cprintf("\nBrak pamieci DPMI");
     exit(0);
 }
 how_paragraphs = (int)paragraphs;
 SegmentForSB   = (int)Segment_Address;

// int FS, NS, Mem;
// FS = SegmentForSB<<4;
// Mem = how_paragraphs<<4;
// NS = FS + Mem - 1;

 /*if((FS&0xFFFF0000) == (NS&0xFFFF0000))
 {
     SegmentForSB = FS;
 }else
 {
     SegmentForSB = NS+1;
 }*/
 SegmentForSB=SegmentForSB<<4; 
 SegmentForSB=SegmentForSB&0xffff0000;
 SegmentForSB+=0x10000;

 
 how_paragraphs = how_paragraphs<<4;

 WAVy = NULL;

 if(ile_MAX_chcesz_miec_melodii_WAV)
 if(!Rezerwuj_Pamiec(ile_MAX_chcesz_miec_melodii_WAV)) //Zwraca ilosc obiektow OPERWAV
  {
      free_DOS_memory(Selector);
      cprintf("\nBrak pamieci na podstawowe struktury danych");
      exit(0);
  }
    ///Przejecie klawiatury
    OrginalnaKlawiatura = _dos_getvect(0x09);
    _dos_setvect(0x09,NowaKlawiatura);
    ///Przejecie zegara
    OrginalTimer=_dos_getvect(0x08);
    _dos_setvect(0x08,NewTime);
}
/////////////////////////////////////////////////////////////////////////////
void MENEGERDMA::Init(int jest_SB, int irq, int port, int kanal)
{                                  // 0x220     0x0083

 if(!WAVy) return;
 if(jest_SB)
 {
  Jest_SB = 1;
  IRQ = irq;
  KanalDMA = port;  //0x220
  Rejest_strony_kanalu = kanal;    //0x0083
  daj_kanal();  //Ustawia Rejest_adresowy_kanalu i Rejest_licznika_kanalu
                //na podstawie Rejest_strony_kanalu
 }

 if(Jest_SB)
 {
    wartosc_maski_kanalu = 0x04;
    wartosc_rejestru_trybu = 0x48;
    for(int i = 0; i<4; i++)
    {
        if(dopuszczalny_kanal[i]==kanal)
        {
            wartosc_maski_kanalu |= i;
            wartosc_rejestru_trybu |= i;
        }
    }

  zawartosc_portow_DMA[0] = inp(0x0002);
  zawartosc_portow_DMA[1] = inp(0x0002);
  zawartosc_portow_DMA[2] = inp(0x0083);
  zawartosc_portow_DMA[3] = inp(0x0003);
  zawartosc_portow_DMA[4] = inp(0x0003);

  LMaskaIRQ=inp(0x0021);
  HMaskaIRQ=inp(0x00A1);

  if(IRQ<16) //  Niskie przerwanie
  {
      irq = 0x01<<(IRQ-8);
  }else
  {
      int k = IRQ & 0xFFFFFFF8;
      irq = 0x01<<(IRQ-k);    // To nie powinno byc 8, qni 70h
  }

  irq = 0xFF ^ irq;
  if(IRQ<16) //  Niskie przerwanie
  {
      HiInterrupt=0;
      NowaMaskaIRQ = LMaskaIRQ & irq;
      outp(0x0021,NowaMaskaIRQ);    //Zezwolenie na przerwanie nr IRQ
  }else
  {
      HiInterrupt=0xFFFF;
      NowaMaskaIRQ = HMaskaIRQ & irq;
      outp(0x00A1,NowaMaskaIRQ);    //Zezwolenie na przerwanie nr IRQ
  }

  OrginalneIRQ = _dos_getvect(IRQ);
  _dos_setvect(IRQ,NoweIRQ);

  Rozkaz_dla_SB(0xD1);  //Odblokuj glosnik

  Rozkaz_dla_SB(0x40);    //Ustawienie czestotliwosci probkowania
  Rozkaz_dla_SB(211);       //Rozkaz_dla_SB(211); -22050Hz
 }
}

MENEGERDMA::~MENEGERDMA(void)
{
    deInit();          //Upewnij sie co robi deInit()
    if(OrginalnaKlawiatura)  _dos_setvect(0x09,OrginalnaKlawiatura);
    if(OrginalTimer)             _dos_setvect(0x08,OrginalTimer);

    //cprintf("\n\n\r Wykonuje sie MENEGERDMA::~MENEGERDMA(void)");
    if(Selector)
    {
        free_DOS_memory(Selector);
  /*      if(free_DOS_memory(Selector))
          {
            //  cprintf("\n\r Niepoprawne zwolnienie pamioeci");
          }else
          {
            //  cprintf("\n\r POPRAWNE zwolnienie pamieci");
          }*/
    }

    if(GlobalDataWAV)
    {
        free(GlobalDataWAV);
        GlobalDataWAV=0;
    }

    for(int i = 0; i<max_ilosc_melodii_DMA; i++)
    {
        WAVy[i].ZwolnijUchwyt();
    }
}

void MENEGERDMA::deInit(void)
{
    // Artur tu pracuj
    if(Jest_SB)
    {
        Jest_SB = 0;
        outp(0x0021,LMaskaIRQ);
        outp(0x00A1,HMaskaIRQ);
        _dos_setvect(IRQ,OrginalneIRQ);
        outp(0x0002,zawartosc_portow_DMA[0]);
        outp(0x0002,zawartosc_portow_DMA[1]);
        outp(0x0083,zawartosc_portow_DMA[2]);
        outp(0x0003,zawartosc_portow_DMA[3]);
        outp(0x0003,zawartosc_portow_DMA[4]);

        Rozkaz_dla_SB(0xD3); //Wylaczenie glosnika
    }
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

int MENEGERDMA::PlayWav(char *name)
{
    int result;
#ifdef SPYWAV
    FILE *f = fopen("raport.txt","at");

    if(f!=NULL)
    {
        fprintf(f,"PlayWav DEBUG\n loaded = %d\n",loaded);
        fclose(f);
    }
#endif

    if(loaded) return NO_ENDING_PLAYWAV;
    result=InstalujWAV(name);

#ifdef SPYWAV
    f = fopen("raport.txt","at");
    fprintf(f," InstalujWAV result = %d\nPlayWav DEBUG - END\n",result);
    fclose(f);
#endif
    if(result)
    {
        return result;
    }

    loaded=1;
    return operator()(ostatnio_wczytany);
}

//
// Zwraca 0
//
void MENEGERDMA::EndPlayWav(void)
{
    int free=ostatnio_wczytany;
#ifdef SPYWAV
    FILE *f = fopen("raport.txt","at");
    fprintf(f,"ostatnio_wczytany = %d\n",ostatnio_wczytany);
    fclose(f);
#endif
    if(!loaded) // jesli nie wywolano funkcji PlayWav
    {
        return;
    }
    loaded=0;
    if(jest_odtwarzany)
    {
        Przerwij_odtwarzanie_DMA();

    }
    ilosc_melodii_DMA--;    //Aby wgrywac melodie od poczatku
    WAVy[free].ZwolnijUchwyt();
}

void (__interrupt __far *MENEGERDMA::OrginalneIRQ)();
//int MENEGERDMA::newtimer=0;
void (__interrupt __far *MENEGERDMA::OrginalTimer)();

void __interrupt __far MENEGERDMA::NewTime()
{
    OrginalTimer();
    licznik++;
}

void __interrupt __far MENEGERDMA::NoweIRQ()
{
 // Artur tu pracuj
 /**/
 static int semafor = 0;

 //asm cli
 outp(0x0020,0x20);  // Przerwanie zostalo obsluzone
 if(HiInterrupt)
 {
     outp(0x00A0,0x20);  // Wysokie przerwanie zostalo obsluzone
 }

 inp(KanalDMA + 0x000E);

 if(!semafor)
  {
   semafor = 1;

   WAVy[odtwarzany_uchwyt].zagrano += WAVy[odtwarzany_uchwyt].do_zagrania;
   if((WAVy[odtwarzany_uchwyt].zagrano) < (WAVy[odtwarzany_uchwyt].size))
   {       
       //WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;//MD
       //jest_odtwarzany = JEST_GRANY;
       Odtwarzaj_DMA();
       ///przerwano_DMA = 0;
       
   }else
   {       
       WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
       WAVy[odtwarzany_uchwyt].zagrano = 0;
       jest_odtwarzany = NIE_JEST_GRANY;
       odtwarzany_uchwyt = NIE_JEST_GRANY;
       odtwarzaj_kazdy  = 1;
       przerwano_DMA = 0;
       
   }
   semafor = 0;
  }
}


void (__interrupt __far *MENEGERDMA::OrginalnaKlawiatura)() = 0;

void __interrupt __far MENEGERDMA::NowaKlawiatura()
{
    static int prefix=0;
    int old,key,forport;
            
        key = inp(0x60);
        forport = inp(0x61);
        outp(0x61,forport|0x80);
        
        outp(0x61,forport);

        old = 0;
        switch(key)
        {
            case 0xE0: case 0xE1: prefix=key; break;
            default: switch(prefix)
                      {
                          case 0xE0: if(key!=29) old=1;
                          case 0xE2: prefix=0; break;
                          case 0xE1: if((key==29)||(key==157)) prefix++; break;
                          default:  if(key!=29) old=1;
                                    break;
                      }
                      /*{
                          case 0xE1: old = 0;
                                      if((key==29)||(key==157)) prefix++;
                                      break;
                          case 0xE2: old = 0;
                                      prefix=0;
                                      break;
                          case 0xE0: right=1;
                          default:  switch(key)
                                     {
                                         case 29: if(right)pressedRCTR=1;   //Wcisniecie CTR
                                                   else pressedLCTR=1;
                                                   break;
                                         case 157:if(right)pressedRCTR=0;   //Puszczenie CTR
                                                   else    pressedLCTR=0;
                                                   break;                                         
                                         case 70: case 44: case 46: if(pressedLCTR||pressedRCTR) old=0; //Wcisnieto 'z' lub 'c' gdy wcisniety jest lewy lub prawy CTR, to nie wprowadzaj tego znaku ('z' lub 'c') do bufora
                                                   break; 
                                     }
                                     prefix = 0;
                                     break;
                      }*/
        }//aswitch(key)
        
        if(old)(*OrginalnaKlawiatura)();
        else   outp(0x20,0x20);
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

int MENEGERDMA::Rezerwuj_Pamiec(int ile_chcesz_miec_melodii_DMA)
{
 static int pamiec_rezerwowano = 0;

 if(pamiec_rezerwowano) return max_ilosc_melodii_DMA;

 /*ile_chcesz_miec_melodii_DMA++;

 do
 {
  ile_chcesz_miec_melodii_DMA--;
  WAVy = new opisWAVa[ile_chcesz_miec_melodii_DMA];
 } while((!WAVy) && (ile_chcesz_miec_melodii_DMA));*/

 WAVy = new opisWAVa[ile_chcesz_miec_melodii_DMA];

 if(WAVy) max_ilosc_melodii_DMA = ile_chcesz_miec_melodii_DMA;
 else max_ilosc_melodii_DMA = 0;
 if(max_ilosc_melodii_DMA) pamiec_rezerwowano = 1;
 return max_ilosc_melodii_DMA; //Boze! dzieki ze pamietales za mnie o tym +i...
}

int MENEGERDMA::ile_mozesz_miec_DMA(void) { return max_ilosc_melodii_DMA; }

int MENEGERDMA::ile_masz_DMA(void) { return ilosc_melodii_DMA; }

/*void MENEGERDMA::Osiagniecia(void)
{
 int irq = IRQ;
 if(IRQ) irq -= 8;
 cout<<"\nRAPORT DETEKCJI KARTY";
 cout<<"\nKanal DMA : "<<hex<<KanalDMA<<'h';
 cout<<"\nNumer IRQ : "<<irq<<'h';
}*/

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
int MENEGERDMA::InstalujWAV(char *nazwa)
{
#ifdef SPYWAV
    FILE *f;
#endif
 //OPERWAV NowyWAV;
 int rezultat_czytania;
 //24.X.int wolna_strona_fizyczna = Daj_wolna_strone();
 if(loaded) return NO_ENDING_PLAYWAV;
 if(Czy_jest_wolny_uchwyt())
 {
#ifdef SPYWAV

    f = fopen("raport.txt","at");
    fprintf(f," InstalujWAV Czy_jest_wolny_uchwyt() = %d\n",Czy_jest_wolny_uchwyt());
    fclose(f);
#endif

   return BRAK_WOLNEGO_UCHWYTU_DMA;
 }

 /*24.X.if(wolna_strona_fizyczna>=MAXYMALNA_ILOSC_STRON_FIZYCZNYCH)
   return BRAK_WOLNEJ_STRONY_W_RAMCE;

 24.X.wolna_str[wolna_strona_fizyczna] = 1;*/  //Rezewrujemy stone na potrzeby ladowania WAVA

 rezultat_czytania = /*NowyWAV.*/ZaladujWAV_z_dysku(nazwa,&WAVy[wolny_uchwyt]);
#ifdef SPYWAV
    f = fopen("raport.txt","at");
    fprintf(f," InstalujWAV rezultat_czytania = %d\n",rezultat_czytania);
    fclose(f);
#endif


 //24.X.wolna_str[wolna_strona_fizyczna] = 0;  //Zwracamy rezerwacje - strona wracado puli wolnych

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
int MENEGERDMA::Czy_jest_wolny_uchwyt(void)
{
 if(ilosc_melodii_DMA<max_ilosc_melodii_DMA)
 {
  for(int i = 0; i<max_ilosc_melodii_DMA; i++)
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

/*FUNKCJE WKLADAMY DO LAMUSA, PONIEWAZ NIE JEST NAM W TRAKCIE GRY POTRZEBNA
int MENEGERDMA::Zwolnij_uchwyt(int numer)
{
 if(numer>=max_ilosc_melodii_DMA) return PROBA_ZWOLNIENIA_ZBYT_DUZEGO_UCHWYTU;
 if(WAVy[numer].jest_grany) return PROBA_ZWOLNIENIA_UCHWYTU_OBECNIE_ODTWARZANEGO;
 if(!(WAVy[numer].zajety)) return PROBA_ZWOLNIENIA_UCHWYTU_NULL;

 WAVy[numer].ZwolnijUchwyt();
 return 0;
}*/

int MENEGERDMA::Wolno_grac(void) { return !jest_odtwarzany; }
int MENEGERDMA::Przerwano_DMA(void) { return przerwano_DMA; }

int MENEGERDMA::Odtwarzaj(int Uchwyt)
{
 ///24.X.int wolna;
 if(!Jest_SB) return BRAK_SOUND_BLASTERA;
 if(Uchwyt >= max_ilosc_melodii_DMA) return PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU;
 if(jest_odtwarzany) return PROBA_ODTWORZENIA_PRZED_ZAKONCZENIEM_ODTWARZANIA;

 if(!(WAVy[Uchwyt].zajety)) return PROBA_ODTWORZENIA_UCHWYTU_NULL;
 if(WAVy[Uchwyt].jest_grany) return PROBA_ODTWORZENIA_UCHWYTU_OBECNIE_ODTWARZANEGO;

 return Odtwarzaj_WAVa(Uchwyt);
}

int MENEGERDMA::operator()(int Uchwyt)
{
 if(Uchwyt >= max_ilosc_melodii_DMA) return PROBA_ODTWORZENIA_ZBYT_DUZEGO_UCHWYTU;
 if(!odtwarzaj_kazdy)
   if(Uchwyt<odtwarzany_uchwyt+1)
      return PROBA_ODTWORZENIA_UCHWYTU_O_MNIEJSZYM_PRIORYTECIE;

 if(jest_odtwarzany)
  Przerwij_odtwarzanie_DMA();

 return Odtwarzaj(Uchwyt);
}

int MENEGERDMA::Odtwarzaj_DMA()
{
 static int Po;
 static int XX;
 static int YY;
 static int VV;
 static int ZZ;
 static int zagrac;

 //przerwano_DMA = 1;
 /*if(DO_zagrania>how_paragraphs)
 {
     MultiWav = 1;
     zagrac = how_paragraphs;
     memcpy((void*)SegmentForSB,(void*)(((char*)WAVy[odtwarzany_uchwyt].samples)+ZAgrano),how_paragraphs;
     DO_zagrania -= how_paragraphs;
     ZAgrano += how_paragraphs;
 }else
 {
     MultiWav = 0;
     zagrac = DO_zagrania;
     zagrac-=100;if(zagrac<0)zagrac=0;
     memcpy((void*)SegmentForSB,(void*)(((char*)(WAVy[odtwarzany_uchwyt].samples))+ZAgrano),zagrac);
     DO_zagrania = 0;
 } */

 if(((WAVy[odtwarzany_uchwyt].zagrano)+how_paragraphs)<(WAVy[odtwarzany_uchwyt].size))
 {
     WAVy[odtwarzany_uchwyt].do_zagrania = how_paragraphs;
 }else
 {
     WAVy[odtwarzany_uchwyt].do_zagrania = (WAVy[odtwarzany_uchwyt].size)-(WAVy[odtwarzany_uchwyt].zagrano);
 }
 zagrac=WAVy[odtwarzany_uchwyt].do_zagrania;
 if(zagrac>0)zagrac--;
 ZZ = zagrac & 0x00FF;
 VV = zagrac>>8;

 Po = (SegmentForSB>>16) & 0x00FF;
 XX = (SegmentForSB>>8) & 0x00FF;
 YY = SegmentForSB & 0x00FF;

 memcpy((void*)SegmentForSB,(void*)(((char*)(WAVy[odtwarzany_uchwyt].samples))+(WAVy[odtwarzany_uchwyt].zagrano)),WAVy[odtwarzany_uchwyt].do_zagrania);
 
 outp(0x000A,wartosc_maski_kanalu);
 outp(0x000C,0x00);
 outp(0x000B,wartosc_rejestru_trybu);

 outp(Rejest_adresowy_kanalu,YY);
 outp(Rejest_adresowy_kanalu,XX);  // outportb(0x0003,ZZ);
 outp(Rejest_strony_kanalu,Po); // outportb(0x0003,VV); Jak milo, ze nie trzeba tego pchac

 outp(Rejest_licznika_kanalu,ZZ);
 outp(Rejest_licznika_kanalu,VV);

 outp(0x000A,wartosc_rejestru_trybu&03);

 Rozkaz_dla_SB(0x14);
 Rozkaz_dla_SB(ZZ);
 Rozkaz_dla_SB(VV);    //Rozkaz_dla_SB(0x91); - Udalo sie bez tego
 return 0;
}

int MENEGERDMA::Odtwarzaj_WAVa(int Uchwyt)
{
 WAVy[Uchwyt].jest_grany = JEST_GRANY;
 jest_odtwarzany = JEST_GRANY;
 odtwarzany_uchwyt = Uchwyt;
 WAVy[odtwarzany_uchwyt].zagrano = 0;
 SamplesForSB = WAVy[odtwarzany_uchwyt].samples;
 MaxSample = (char*)WAVy[odtwarzany_uchwyt].samples + (DO_zagrania - 1);
 
 WAVy[odtwarzany_uchwyt].do_zagrania = WAVy[odtwarzany_uchwyt].size;

 odtwarzaj_kazdy  = 0;
 return Odtwarzaj_DMA();
}

void MENEGERDMA::Przerwij_odtwarzanie_DMA(void)
{
 //TEST('R');
 Rozkaz_dla_SB(0xD0);
 //inportb(KanalDMA + 0x000E);
 delay(1);
 WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
 jest_odtwarzany   = NIE_JEST_GRANY;
 odtwarzany_uchwyt = NIE_JEST_GRANY;
 odtwarzaj_kazdy  = 1;
}

void MENEGERDMA::Rozkaz_dla_SB(int Rozkaz)
{
    static int port;
    static int licznik;
    licznik = 1;
    do
    {
        port = (int)inp(KanalDMA + 0x000C);
        port = port & 0x80;
        if(!(++licznik))
        {
            WAVy[odtwarzany_uchwyt].jest_grany = NIE_JEST_GRANY;
            jest_odtwarzany = NIE_JEST_GRANY;
            odtwarzany_uchwyt = NIE_JEST_GRANY;
            odtwarzaj_kazdy  = 1;
            przerwano_DMA = 0;
            return;
        }
    }while(port);
    outp(KanalDMA + 0x000C,Rozkaz);
}

//#define SPRAWDZIAN
#ifdef SPRAWDZIAN
char spraw[150];
FILE *sprawdzian=0;
#endif

int MENEGERDMA::LoadGlobalData(char *name, int MaxWav)
{
    long int DataLength;
    long int WavLength=0;
    FILE *f = fopen(name,"rb");

    if(!f) return BLAD_OTWARCIA_GLOBALNEGO_PLIKU_Z_WAVAMI;

#ifdef SPRAWDZIAN
    sprawdzian = fopen("sprawdzi.txt","wt");
#endif

    fseek(f,0L,SEEK_END);       //Skok na koniec pliku
    DataLength = ftell(f);     //Zapamietanie taj pozycji
    fseek(f,0L,SEEK_SET);       //Skok na poczatek pliku
    DataLength -= MaxWav*sizeof(int);

    GlobalDataWAV = malloc(DataLength);
    if(!GlobalDataWAV)
    {
        fclose(f);
        return BRAK_PAMIECI_NA_WSZYSTKIE_DANE_WAV;
    }

    fread(GlobalDataWAV,DataLength,1,f);
    DataLength = 0;
    for(int i = 0; i<MaxWav; i++)
    {
        WAVy[i].samples = (void*)((char*)GlobalDataWAV + DataLength);
#ifdef SPRAWDZIAN
        if(sprawdzian)
        {
            sprintf(spraw," %d. Przesuniecie %d bajtow. ",i,DataLength);
            fwrite(spraw,strlen(spraw),1,sprawdzian);
        }
#endif

        fread((void*)&WavLength,sizeof(WavLength),1,f);
        WAVy[i].size = WavLength;
        WAVy[i].zajety = GLOBALNE_ZALADOWANIE_WAVOW;
        DataLength += WavLength;

#ifdef SPRAWDZIAN
        if(sprawdzian)
        {
            sprintf(spraw," Dlugosc wav'a %d bajtow. \n",WavLength);
            fwrite(spraw,strlen(spraw),1,sprawdzian);
        }
#endif
    }
    fclose(f);
    ilosc_melodii_DMA = MaxWav;

#ifdef SPRAWDZIAN
    if(sprawdzian) fclose(sprawdzian);
#endif
    return 0;
}

#define WYPISZNAEKRAN
#undef WYPISZNAEKRAN
#ifdef WYPISZNAEKRAN
    int Rplik = 0;
    char filename[20];
    FILE *pp;
    char ff[] = {"naekran"};
#endif


int MENEGERDMA::ZaladujWAV_z_dysku(char *nazwa_pliku,opisWAVa *wav)
{
    int LengthWav;    
    FILE*f;
    
    f=fopen(nazwa_pliku,"rb");
    if(!f)return BRAK_PLIKU_WAV;

#ifdef WYPISZNAEKRAN
    Rplik++;
    sprintf(filename,"%s%d.txt",ff,Rplik);
    pp=fopen(filename,"wt");
    fprintf(pp," Otwarlem plik %s \n Bede sie staral odtworzyc plik %s \n",filename,nazwa_pliku);
#endif
    fread((void*)&LengthWav,sizeof(LengthWav),1,f);
    wav->samples = malloc(LengthWav);
    wav->size = LengthWav;
#ifdef WYPISZNAEKRAN
    if(pp)
    {
        fprintf(pp," Dlugosc pliku %d \n",wav->size);
    }
#endif
    if(!(wav->samples))
    {
        fclose(f);
#ifdef WYPISZNAEKRAN
        if(pp)
        {
           fprintf(pp," Brak pamieci %d bajtow \n",LengthWav);
        }
#endif
        return BRAK_PAMIECI_NA_PROBKI;
    }
    fread(wav->samples,wav->size,1,f);//czytaj probki
    fclose(f);    
    wav->zajety = ZWYKLE_ZALADOWANIE_WAVA;
#ifdef WYPISZNAEKRAN
    if(pp)fclose(pp);
#endif

    return WAV_ZALADOWANY;
}


