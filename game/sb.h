/*
  Plik : SB.H
  Autor: Artur Bidzinski
  Opis : Poprawione 10 wrzesnia 1996r.
*/
#ifndef KLASA_PODSTAWOWYCH_OPERACJI_NA_KARCIE_MUZYCZNEJ
#define KLASA_PODSTAWOWYCH_OPERACJI_NA_KARCIE_MUZYCZNEJ

//#include <dos.h>   //outportb itd.

//typedef unsigned char byte;
//typedef unsigned int  word;

class sb
{
    protected:

    static int Odczytano_maske_przerwan;
    static int Maska_przerwan;
    static int Byla_detekcja_DMA;
    static int Byla_detekcja_IRQ;
    static int Adres_DMA;
    static int Nr_IRQ;
    static int WersjaSB;

    static void (__interrupt __far *org_IRQ)();
    static void (__interrupt __far *fun)();
    static void __interrupt __far Przerwanie_IRQ();
    static void Zezwol_na_przerwanie(int);
    static int  Oczekuj_na_Przerwanie(int);

    public:
    static int  Detekcja_portu_DMA(void);
    static int  Detekcja_IRQ_DMA(void);
    static int  Pobierz_wersjeSB(void);
    static void WyslijBajt(int rozkaz);

    static int  Podaj_port_DMA(void);
    static int  Podaj_nr_IRQ(void);
    static int  Podaj_Maske_przerwan(void);
    static int  Podaj_WersjaSB(void);

    sb(void);
    ~sb(void);
};

#endif
