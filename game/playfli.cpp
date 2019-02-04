
// DOLACZENIE POTRZEBNYCH BIBLIOTEK ------------------------------------------
#include <conio.h>
//#include <graph.h>
#include <dos.h>
#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------------------------------------
void fli_color();
void fli_black();
void fli_brun();
void fli_copy();
void fli_lc();
void wait_sync();
void move_palette();
void move_screen();
int check_escape();
// DEFINICJA TYPOW -----------------------------------------------------------
// UWAGA: typ 'byte' musi zajmowac w pamieci 8-bitow (bez znaku)
//        typ 'word' musi zajmowac w pamieci 16-bitow (bez znaku)

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned char boolean;

// ZMIENNE GLOBALNE ----------------------------------------------------------
// extern char* RealVirtualScreen;
byte *screen = (unsigned char *)0xA0000; //(byte*)RealVirtualScreen; //
                                         // wirtualny ekran
byte *source;                            // bufor zrodlowy
byte pal[0x300];                         // paleta kolorow
byte *source_ptr;                        // znacznik bufora zrodlowego
long int timer_delay, timer_stop;
long int startTime = 0;

void init_timer() {
  outp(0x43, 0x34);
  outp(0x40, 0);
  outp(0x40, 0);
}

void deinit_timer() {
  outp(0x43, 0x36);
  outp(0x40, 0);
  outp(0x40, 0);
}
long int read_timer() {
  word w;
  byte lsb, msb;
  memcpy((void *)&w, (const void *)0x46C, 2);
  outp(0x43, 0);
  lsb = inp(0x40);
  msb = inp(0x40);
  return (((w << 16) & (0x7FFFFFFF)) + (0xFFFF - ((msb << 8) + lsb)));
}

void start_timer() {
  long int t;
  t = read_timer();
  if (t >= 0x80000000)
    startTime = t - 0x80000000;
  else
    startTime = t;
}

void wait_timer(int cframe) {
  long int t;
  long int te = startTime + cframe * timer_delay;
  if (startTime + cframe * timer_delay >= 0x80000000)
    te = startTime + cframe * timer_delay - 0x80000000;
  else
    te = startTime + cframe * timer_delay;
  do {
    t = read_timer();
  } while (t < te);
}
/////////////////////// play /////////////////////////
int play(char *s) {
  FILE fh;
  byte *chunk_start;
  word *_word;
  word frames, delay_value, frame_size, chunks, chunk_size, chunk_type;
  word current_frame = 0, current_chunk;
  boolean escape = 0;
  boolean pal_changed;
  int err = 1;
  size_t status;

  source = (byte *)malloc(0xffff);
  fh = *fopen(s, "rb");
  if (ferror(&fh)) {
    err = 0;
  } else {
    init_timer();
    status = fread(source, 0x80, 1, &fh);
    if (status != 1) {
      err = 0;

    } else {
      source_ptr = source;
      _word = (word *)(source_ptr + 6);
      frames = *_word;
      _word = (word *)(source_ptr + 16);
      delay_value = *_word;
      timer_delay = (delay_value / 70.0) * 1193180.0;
      memset(screen, 0, 64000);
      memset(pal, 0, 0x300);

      /////////-------------
      //        frames-=10;
      ////------------------
      start_timer();
      do {
        pal_changed = 0;
        status = fread(source, 0x10, 1, &fh);
        if (status != 1) {
          err = 0;

        } else {
          source_ptr = source;
          _word = (word *)source_ptr;
          frame_size = *_word;
          _word = (word *)(source_ptr + 6);
          chunks = *_word;
          if (chunks > 0) {
            frame_size -= 0x10;
            status = fread(source, frame_size, 1, &fh);
            if (status != 1) {
              err = 0;

            } else {
              current_chunk = 0;
              chunk_start = source_ptr;
              do {
                source_ptr = chunk_start;
                _word = (word *)source_ptr;
                chunk_size = *_word;
                chunk_start += chunk_size;
                _word = (word *)(source_ptr + 4);
                chunk_type = *_word;
                source_ptr += 6;
                switch (chunk_type) {
                case 11: {
                  pal_changed = 1;
                  fli_color();
                  break;
                }
                case 12: {
                  fli_lc();
                  break;
                }
                case 13: {
                  fli_black();
                  break;
                }
                case 15: {
                  fli_brun();
                  break;
                }
                case 16: {
                  fli_copy();
                  break;
                }
                }
                current_chunk++;
              } while (current_chunk != chunks);
            }
          }
          if (err) {
            // wait_sync();
            if (pal_changed)
              move_palette();
            wait_timer(current_frame);
            // move_screen();
            escape = check_escape();
            current_frame++;
          }
        }
      } while ((!escape) && (current_frame < frames) && (err));
    }

    fclose(&fh);
    deinit_timer();
  }
  free(source);
  return (err);
}

void fli_color() {
  word *packets, d, col_start = 0, col_length;

  packets = (word *)source_ptr; // odczytanie ilosci pakietow
  source_ptr += 2;
  for (d = 1; d <= *packets; d++) // przetwarzanie kolejnych pakietow
  {
    col_start += *source_ptr * 3;       // *source_ptr - ile kolorow ominac
    col_length = *(source_ptr + 1) * 3; // *source_ptr - ile kolorow zdefiniowac
    source_ptr += 2;
    if (col_length == 0)
      col_length = 0x300; // 0 oznacza 0x100
    memcpy(&pal[col_start], source_ptr,
           col_length);       // skopiowanie wzorcow do tablicy pal
    source_ptr += col_length; // uaktualnienie pozycji wskaznika
    col_start += col_length;  // uaktualnienie aktualnego wzorca kolorow
  }
}

// OBSLUGA FLI_LC ------------------------------------------------------------
/* FLI_LC zawiera roznice pomiedzy klatka poprzednia a klatka aktualna.
   Format bloku FLI_LC:
     word line_skip (ilosc linii ekranu do ominiecia; pierwsza modyfik. linia)
     word lines_to_change (ilosc linii ekranu do modyfikacji)
     kazda linia:
        byte packets (ilosc pakietow)
        kazdy pakiet:
          byte skip_count (ile punktow w linii ominac)
          byte size_count (ile punktow w linii zmodyfikowac)
          jesli bit 7 size_count = 0 to
             ? screen_data (dane do przeslania na ekran; rozmiar: size_count)
          jesli bit 7 size_count = 1 to
             byte fill_value (wartosc, ktora wypelnia sie nastepne punkty
                              ekranu; ilosc punktow oblicza sie na podstawie
                              wzoru: size_count=(size_count^0xFF)+1 lub
                              traktujac size_count jak bajt ze znakiem
                              ilosc punktow = abs(size_count)
*/

void fli_lc() {
  word *start_line, *lines_to_change, line_address, d, p;
  byte packets, c, skip_count, size_count, fill_value;

  start_line = (word *)source_ptr;            // pierwsza linia do modyfikacji
  lines_to_change = (word *)(source_ptr + 2); // ilosc linii do modyfikacji
  source_ptr += 4;                            // uaktualnienie wskaznika
  line_address = *start_line * 320;           // adres linii w buforze
  for (d = 1; d <= *lines_to_change; d++)     // petla: kolejne linie
  {
    packets = *source_ptr++;       // ilosc pakietow
    p = line_address;              // p - pomocniczy wskaznik
    for (c = 1; c <= packets; c++) // petla: kolejne pakiety
    {
      skip_count = *source_ptr++; // ile bajtow ominac
      size_count = *source_ptr++; // ile bajtow sklopiowac/wypelnic
      p += skip_count;            // ominiecie bajtow
      if (!(size_count & 0x80))   // kopiowanie czy wypelnianie?
      {
        memcpy(&screen[p], source_ptr, size_count); // kopiowanie
        source_ptr += size_count;                   // uaktualnienie wskaznika
      } else {
        size_count =
            (size_count ^ 0xFF) + 1; // wypelnianie; size_count=abs(size_count)
        fill_value = *source_ptr++;  // wartosc wypelniajaca
        memset(&screen[p], fill_value, size_count); // wypelnianie
      }
      p += size_count; // uaktualnienie wskaznika p
    }
    line_address += 320; // nastepna linia
  }
}

// OBSLUGA FLI_BLACK ---------------------------------------------------------
/* FLI_BLACK czysci ekran; nie zawiera zadnych danych
 */

void fli_black() { memset(screen, 0, 64000); }

// OBSLUGA FLI_BRUN ----------------------------------------------------------
/* FLI_BRUN powinien wystepowac tylko w 1-szej klatce animacji .FLI. Podobny
   w strukturze do FLI_LC zawiera informacje tworzace obraz pierwotny; czesto
   wystepuje w jednej klatce z FLI_COLOR;
   Format FLI_BRUN:
     (ilosc modyfikowanych linii const=200; pierwsza modyfikowana linia const=0)
     kazda linia:
        byte packets (ilosc pakietow)
        kazdy pakiet:
           byte size_count (ile punktow w linii zmodyfikowac)
           jesli bit 7 size_count = 1 to
              ? screen_data (bajty do przeslania na ekran; ilosc obliczana t.j.
                             w przypadku FLI_LC gdy bit 7 size_count = 0)
*/

void fli_brun() {
  word line_address = 0, d, p;
  byte packets, c, size_count, fill_value;

  for (d = 1; d <= 200; d++) // 200 kolejnych linii
  {
    packets = *source_ptr++;       // ilosc pakietow
    p = line_address;              // p - pomocniczy wskaznik
    for (c = 1; c <= packets; c++) // petla: kolejna pakiety
    {
      size_count = *source_ptr++; // ile bajtow kopiowac/wypelniac
      if (size_count & 0x80)      // kopiowanie czy wypelnianie?
      {
        size_count =
            (size_count ^ 0xFF) + 1; // kopiowanie; size_count=abs(size_count)
        memcpy(&screen[p], source_ptr, size_count); // kopiowanie
        source_ptr += size_count;                   // uaktualnienie size_count
      } else {
        fill_value = *source_ptr++; // wypelnianie; bajt wypelniajacy
        memset(&screen[p], fill_value, size_count); // wypelnianie
      }
      p += size_count; // uaktualnienie wskaznika p
    }
    line_address += 320; // nastepna linia
  }
}

// OBSLUGA FLI_COPY ----------------------------------------------------------
/*
    FLI_COPY zawiera 64000 bajtow do skopiowania na ekran
*/

void fli_copy() { memcpy(screen, source_ptr, 64000); }

// OCZEKIWANIE NA SYNCHRONIZACJE PIONOWA -------------------------------------

void wait_sync() {
  while (inp(0x3DA) & 8)
    ; // oczekiwanie na poczatek synchr.
  while (!(inp(0x3DA) & 8))
    ; // oczekiwanie na koniec synchr.
}

// AKTUALIZACJA PALETY -------------------------------------------------------

void move_palette() {
  word d;

  for (d = 0; d < 0x100; d++) {
    outp(0x3C8, d);              // przeslanie nr rejestru
    outp(0x3C9, pal[d * 3]);     // przeslanie R
    outp(0x3C9, pal[d * 3 + 1]); // przeslanie G
    outp(0x3C9, pal[d * 3 + 2]); // przeslanie B
  }
}

// AKTUALIZACJA EKRANU -------------------------------------------------------

void move_screen() {
  memcpy((void *)0xA0000, (void *)screen,
         64000); // przeslanie bufora do pamieci ekranu
}

// SPRAWDZENIE NACISNIECIA ESCAPE'A ------------------------------------------

int check_escape() {
  if (inp(0x60) == 1)
    return (1);
  else
    return (0);
  /*if(kbhit())
  {
      int key=getch();
      if(key==27)return 1;
  }
  return 0;*/
}

// KONIEC PLIKU
