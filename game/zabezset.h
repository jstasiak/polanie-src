/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Plik  : ZABEZP.H                                                       บ
  บ Autor : Artur Bidzinski                                                บ
  บ Data  : 26 luty 1996r.                                                 บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ Opis  : Definicje struktur do zabezpieczen w Polanach                  บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#ifndef DEFINICJE_STRUCTUR_DOZABEZPIECZENIA_W_POLANACH
#define DEFINICJE_STRUCTUR_DOZABEZPIECZENIA_W_POLANACH

#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

#define IL_GRAF 18
#define DL_NAZ_PLIKU 20
#define DL_KLUCZA 20
#define DL_BUFORA 4096
#define BLAD_ORWARCIA_PLIKU_GRAF012 1
#define BLAD_ODCZYTU_DATY 2
#define BLAD_OTWARCIA_POLAN 3
#define BLAD_OTWARCIA_ZBIORU 4
#define BRAK_MIEJSCA_NA_DYSKU 5

struct opis_klucza {
  long dl_polan;
  long poz_graf;
  char klucz[DL_KLUCZA];
};

unsigned int generuj_graf(unsigned int);
int odswiez_Polan(int);
int odswiez_Polan1(int);
int odswiez_Polan2(int);

#endif