#include "instrum.h"
#include <stdio.h>

#define ID_SBI 0x1A494253
#define ID_IBK 0x1A4B4249

instrum *ReadInstrumIndex(char *filename, int index)
/* odczytuje z pliku filename (mo¿e to byæ plik
   SBI lub IBK) instrument o numerze index
   (od 0 do 127 dla IBK i 0 dla SBI) */
{
  FILE *f;
  long fileid;
  if (NULL == (f = fopen(filename, "rb")))
    return NULL;
  fread(&fileid, 1, 4, f);
  switch (fileid) {
  case ID_SBI:
    fseek(f, 0x24, SEEK_SET);
    break;
  case ID_IBK:
    fseek(f, 16 * index, SEEK_CUR);
    break;
  default:
    fclose(f);
    return NULL;
  }
  instrum *ins = new instrum;
  fread(ins, 1, sizeof(instrum), f);
  fclose(f);
  return ins;
}