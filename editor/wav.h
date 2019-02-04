/**************************************************************************
*** Plik  : WAV.H                                                       ***
*** Autor : Artur Bidzinski                                             ***
*** Data  : 28 pazdziernika 1995r.                                      ***
***************************************************************************
*** Opis  : Plik definicyjny struktury do tworzenia pliku typu .WAV     ***
**************************************************************************/

#ifndef DEFINICJA_KLASY_WAV
#define DEFINICJA_KLASY_WAV
#include "typepas.h"

const word DLUGOSC_NAZWY_PLIKU = 13;

struct rozklad_worda_ofset
{
 byte YY;
 byte XX;
};

union OFSET_DMA
{
 word                 ofset_dma;
 rozklad_worda_ofset  rozklad_ofsetu_dma;
};

struct rozklad_worda_dlugosc
{
 byte ZZ;
 byte VV;
};

union DLUGOSC_DMA
{
 word                  dlugosc_dma;
 rozklad_worda_dlugosc rozklad_dlugosci_dma;
};

struct daneDMA
{
 byte ostatni;
 byte Czestotliwosc;	//= 256 - 1.000.000/czestotliwosc probek
 byte P;
 OFSET_DMA   XXYY;
 DLUGOSC_DMA VVZZ;
 daneDMA    *nastepny;
};

class WAV
{
 char RIFF[4];           //4  0   00H
 long ile_po_8_bajcie;   //4  4   04H
 char WAVE[4];           //4  8   08H
 char fmt[4];            //4  12  0CH
 long ile_od_teraz;      //4  16  10H
 word wFormatTag;        //2  20  14H
 word nChannels;         //2  22  16H
 long nSamplesPerSec;    //4  24  18H
 long nAvgBytesPerSec;   //4  28  1CH
 word nBlockAlign;       //2  32  20H
 word nBitsPerSample;    //2  34  22H
 char data[4];           //4  36  24H
 long ile_po_data;       //4  40  28H  44 2CH - NASTEPNY BAJT
 friend class OPERWAV;
};
#endif
