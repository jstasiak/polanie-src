#ifndef KLASA_PODSTAWOWYCH_OPERACJI_NA_KARCIE_MUZYCZNEJ
#define KLASA_PODSTAWOWYCH_OPERACJI_NA_KARCIE_MUZYCZNEJ

#include <dos.h> //outportb itd.

typedef unsigned char byte;
typedef unsigned int word;

class sb {
protected:
  static byte Odczytano_maske_przerwan;
  static byte Maska_przerwan;
  static byte Byla_detekcja_DMA;
  static byte Byla_detekcja_IRQ;
  static word Adres_DMA;
  static byte Nr_IRQ;
  static word WersjaSB;

  static void interrupt (*org_IRQ)(...);
  static void interrupt Przerwanie_IRQ(...);
  static void Zezwol_na_przerwanie(byte);
  static int Oczekuj_na_Przerwanie(byte);

public:
  static word Detekcja_portu_DMA(void);
  static byte Detekcja_IRQ_DMA(void);
  static word Pobierz_wersjeSB(void);
  static void WyslijBajt(byte rozkaz);

  static word Podaj_port_DMA(void);
  static byte Podaj_nr_IRQ(void);
  static byte Podaj_Maske_przerwan(void);
  static word Podaj_WersjaSB(void);

  sb(void);
  ~sb(void);
};

#endif