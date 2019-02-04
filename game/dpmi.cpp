/*
 Plik : DPMI.CPP
 Autor: Artur Bidziñski
 Data : 18 maja 1996r.
*/

#include <i86.h>

/*struct J4
{
    unsigned int edi;
    unsigned int esi;
    unsigned int ebp;
    unsigned int zarezerwowane;
    unsigned int ebx;
    unsigned int edx;
    unsigned int ecx;
    unsigned int eax;
    unsigned short flags;
    unsigned short es;
    unsigned short ds;
    unsigned short fs;
    unsigned short gs;
    unsigned short ip;
    unsigned short cs;
    unsigned short sp;
    unsigned short ss;
};*/

REGS r;
// J4    t;

int nie_ma_DPMI = 1;
int nie_ma_DPMI_w_trybie_rzeczywistym = 1;
int nie_ma_IPX = 1;

/////////////////////////////////////////////////////////////////
// Zwraca 0 gdy komputer pracuje w trybie wirtualnym pod kontrola DPMI
// Zwraca 1 gdy komputer pracuje w trybie rzeczywistym lub wirtualnym 8086
// lub nie sa dostepne funkcje DPMI (przerwanie 31H)
/////////////////////////////////////////////////////////////////
int Presence_DPMI(void) {
  r.x.eax = 0x1686;
  int386(0x2F, &r, &r);
  if (!r.w.ax)
    nie_ma_DPMI = 0;
  return nie_ma_DPMI;
}

/////////////////////////////////////////////////////////////////
// Zwraca 0 gdy przerwanie 2Fh wykryje obecnosc DPMI
// Zwraca 1 gdy DPMI nie jest zainstalowany
/////////////////////////////////////////////////////////////////
int Presence_DPMI_in_real_mode(void) {
  r.x.eax = 0x1687;
  int386(0x2F, &r, &r);
  if (!r.w.ax)
    nie_ma_DPMI_w_trybie_rzeczywistym = 0;
  return nie_ma_DPMI_w_trybie_rzeczywistym;
}

/*
        UWAGA: PONI¯SZE FUNKCJE WOLNO U¯YWAC TYLKO WÓWCZAS,
               GDY Obecnosc_DPMI() ZWRÓCI£A ZERO.
*/
/////////////////////////////////////////////////////////////////
// Zwraca 0 gdy przydzielenie sie powiod³o
// Zwraca 1 gdy kleska i wtedy zmienia sie znaczenie parametrów:
// Adres_segmentowy = kod b³êdu systemu DOS
// Selektor         = rozmiar najwiekszsego dostepnego bloku w paragrafach
/////////////////////////////////////////////////////////////////
int alloc_DOS_memory(unsigned short how_paragraphs,
                     unsigned short &Segment_Address,
                     unsigned short &Selector) {
  r.x.eax = 0x0100;
  r.x.ebx = how_paragraphs;
  int386(0x31, &r, &r);

  Segment_Address = r.w.ax;

  if (r.x.cflag) {
    Selector = r.w.bx;
    return 1;
  }

  Selector = r.w.dx;
  return 0;
}
/////////////////////////////////////////////////////////////////
int free_DOS_memory(unsigned short Selector) {
  r.x.eax = 0x0101;
  r.w.dx = Selector;

  //            cout << "\n Jestem przed zwolnieniem pamieci";
  int386(0x31, &r, &r);
  //            cout << ". Jestem po zwolnieniu pamieci ";

  if (r.x.cflag)
    return r.x.eax & 0xFFFF;
  return 0;
}
