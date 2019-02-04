/*ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป
  บ Plik  : ZABEZP.CPP                                                     บ
  บ Autor : Artur Bidzinski                                                บ
  บ Data  : 26 luty 1996r.                                                 บ
  วฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤฤถ
  บ Opis  : To co utrudni - mam nadzieje - zycie tym co lamia              บ
  ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ*/

#include "zabezset.h"

const long punkt_wejscia = 5392;
const char *koncowki[] = {"","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19"};
const char nazwa_[] = {".\\graf\\graf.0"};
char  Bufor[DL_BUFORA];
char nazwa_graf[30];

//*******************************************************************
//generuje liczbe od 0 do (num-1)
unsigned int generuj_graf(unsigned int num)
{
 static unsigned int licznik;
 static int pierwszy = 1;
 static unsigned long s;

 if(pierwszy)
  {
   unsigned int sss = peek(0x40,0x6E);
   licznik = peek(0x40,0x6C);
   s = s + licznik + 65536*sss;
   pierwszy = 0;
   licznik = licznik & 0x000F;
   if(!licznik) licznik = 7;
  }

 licznik--;
 if(!licznik)  pierwszy = 1;

 s = ((unsigned long)31413*(unsigned long)s + (unsigned long)13849) % (unsigned long)65536;

 return (unsigned int) (unsigned long)(((unsigned long)num*(unsigned long)s) / (unsigned long)65536);
}

//*******************************************************************
void tworz_nazwe_graf(int ktory_plik)
{
 strcpy(nazwa_graf,nazwa_);
 strcat(nazwa_graf,koncowki[ktory_plik]);
}

//*********************************************************************
int odswiez_Polan(int ktory)
{
 int otw_polanie = 0;
 int otw_plik    = 0;
 int otw_zbior   = 0;
 int petluj = 1;
 int polanie;
 int plik;
 int zbior;
 int liczba_bajtow;
 int index = 0;
 int licznik_bufora;
 int rezultat = 0;

 opis_klucza opis;
 ftime dataPolan;

 polanie = open("polanie.exe",O_BINARY|O_WRONLY);
 if(polanie == (-1))
  {
   rezultat = BLAD_OTWARCIA_POLAN;
   goto koniec_odswiezania;
  }

 otw_polanie = 1;
 getftime(polanie,&dataPolan);  //Odczytanie daty Polan
 lseek(polanie,0,SEEK_SET);

 tworz_nazwe_graf(12);
 plik = open(nazwa_graf,O_BINARY|O_RDONLY);

 if(plik == (-1))
  {
   //close(polanie);
   //return BLAD_ORWARCIA_PLIKU_GRAF012;
   rezultat = BLAD_ORWARCIA_PLIKU_GRAF012;
   goto koniec_odswiezania;
  }

 otw_plik = 1;

 if(ktory == 12)
  {
   zbior = plik;
  }
 else
  {
   tworz_nazwe_graf(ktory);
   zbior = open(nazwa_graf,O_BINARY|O_RDONLY);
   if(zbior == (-1))
    {
     rezultat = BLAD_OTWARCIA_ZBIORU;
     goto koniec_odswiezania;
    }
   otw_zbior = 1;
  }

 //------------ Sprobujemy odczytac teraz klucz z pliku - oby sie udalo...
 lseek(plik,punkt_wejscia + (long)(ktory-1)*sizeof(opis_klucza),SEEK_SET);
 read(plik,(void*)(&opis),sizeof(opis_klucza));  //Oczytanie klucza i miejsca odczytu w pliku zbior

 lseek(zbior,opis.poz_graf,SEEK_SET);  //Zbior przygotowany do zapisu

 index = 0;
 do
  {
   liczba_bajtow = read(zbior,(void*)(&Bufor),DL_BUFORA);

   for(licznik_bufora = 0; licznik_bufora<liczba_bajtow; licznik_bufora++)
    {
     Bufor[licznik_bufora] = Bufor[licznik_bufora] + opis.klucz[index];
     index++;
     if(index == DL_KLUCZA)
      index = 0;
    }

   licznik_bufora = write(polanie,(void*)(&Bufor),liczba_bajtow);
   if(licznik_bufora != liczba_bajtow)
    {
      petluj = 0;
      rezultat = BRAK_MIEJSCA_NA_DYSKU;
    }

   if(liczba_bajtow != DL_BUFORA)
     petluj = 0;
  }while(petluj);

 koniec_odswiezania:

 if(otw_polanie)
   {
    setftime(polanie,&dataPolan);
    close(polanie);
   }
 if(otw_plik)
   close(plik);
 if(otw_zbior)
  close(zbior);

 return rezultat;
}
int odswiez_Polan1(int ktory)
{
 int otw_polanie = 0;
 int otw_plik    = 0;
 int otw_zbior   = 0;
 int petluj = 1;
 int polanie;
 int plik;
 int zbior;
 int liczba_bajtow;
 int index = 0;
 int licznik_bufora;
 int rezultat = 0;

 opis_klucza opis;
 ftime dataPolan;

 polanie = open("polanie.exe",O_BINARY|O_WRONLY);
 if(polanie == (-1))
  {
   rezultat = BLAD_OTWARCIA_POLAN;
   goto koniec_odswiezania;
  }

 otw_polanie = 1;
 getftime(polanie,&dataPolan);  //Odczytanie daty Polan
 lseek(polanie,0,SEEK_SET);

 tworz_nazwe_graf(12);
 plik = open(nazwa_graf,O_BINARY|O_RDONLY);

 if(plik == (-1))
  {
   //close(polanie);
   //return BLAD_ORWARCIA_PLIKU_GRAF012;
   rezultat = BLAD_ORWARCIA_PLIKU_GRAF012;
   goto koniec_odswiezania;
  }

 otw_plik = 1;

 if(ktory == 12)
  {
   zbior = plik;
  }
 else
  {
   tworz_nazwe_graf(ktory);
   zbior = open(nazwa_graf,O_BINARY|O_RDONLY);
   if(zbior == (-1))
    {
     rezultat = BLAD_OTWARCIA_ZBIORU;
     goto koniec_odswiezania;
    }
   otw_zbior = 1;
  }

 //------------ Sprobujemy odczytac teraz klucz z pliku - oby sie udalo...
 lseek(plik,punkt_wejscia + (long)(ktory-1)*sizeof(opis_klucza),SEEK_SET);
 read(plik,(void*)(&opis),sizeof(opis_klucza));  //Oczytanie klucza i miejsca odczytu w pliku zbior

 lseek(zbior,opis.poz_graf,SEEK_SET);  //Zbior przygotowany do zapisu

 index = 0;
 do
  {
   liczba_bajtow = read(zbior,(void*)(&Bufor),DL_BUFORA);

   for(licznik_bufora = 0; licznik_bufora<liczba_bajtow; licznik_bufora++)
    {
     Bufor[licznik_bufora] = Bufor[licznik_bufora] + opis.klucz[index];
     index++;
     if(index == DL_KLUCZA)
      index = 0;
    }

   licznik_bufora = write(polanie,(void*)(&Bufor),liczba_bajtow);
   if(licznik_bufora != liczba_bajtow)
    {
      petluj = 0;
      rezultat = BRAK_MIEJSCA_NA_DYSKU;
    }

   if(liczba_bajtow != DL_BUFORA)
     petluj = 0;
  }while(petluj);

 koniec_odswiezania:

 if(otw_polanie)
   {
    setftime(polanie,&dataPolan);
    close(polanie);
   }
 if(otw_plik)
   close(plik);
 if(otw_zbior)
  close(zbior);

 return rezultat;
}
int odswiez_Polan2(int ktory)
{
 int otw_polanie = 0;
 int otw_plik    = 0;
 int otw_zbior   = 0;
 int petluj = 1;
 int polanie;
 int plik;
 int zbior;
 int liczba_bajtow;
 int index = 0;
 int licznik_bufora;
 int rezultat = 0;

 opis_klucza opis;
 ftime dataPolan;

 polanie = open("polanie.exe",O_BINARY|O_WRONLY);
 if(polanie == (-1))
  {
   rezultat = BLAD_OTWARCIA_POLAN;
   goto koniec_odswiezania;
  }

 otw_polanie = 1;
 getftime(polanie,&dataPolan);  //Odczytanie daty Polan
 lseek(polanie,0,SEEK_SET);

 tworz_nazwe_graf(12);
 plik = open(nazwa_graf,O_BINARY|O_RDONLY);

 if(plik == (-1))
  {
   //close(polanie);
   //return BLAD_ORWARCIA_PLIKU_GRAF012;
   rezultat = BLAD_ORWARCIA_PLIKU_GRAF012;
   goto koniec_odswiezania;
  }

 otw_plik = 1;

 if(ktory == 12)
  {
   zbior = plik;
  }
 else
  {
   tworz_nazwe_graf(ktory);
   zbior = open(nazwa_graf,O_BINARY|O_RDONLY);
   if(zbior == (-1))
    {
     rezultat = BLAD_OTWARCIA_ZBIORU;
     goto koniec_odswiezania;
    }
   otw_zbior = 1;
  }

 //------------ Sprobujemy odczytac teraz klucz z pliku - oby sie udalo...
 lseek(plik,punkt_wejscia + (long)(ktory-1)*sizeof(opis_klucza),SEEK_SET);
 read(plik,(void*)(&opis),sizeof(opis_klucza));  //Oczytanie klucza i miejsca odczytu w pliku zbior

 lseek(zbior,opis.poz_graf,SEEK_SET);  //Zbior przygotowany do zapisu

 index = 0;
 do
  {
   liczba_bajtow = read(zbior,(void*)(&Bufor),DL_BUFORA);

   for(licznik_bufora = 0; licznik_bufora<liczba_bajtow; licznik_bufora++)
    {
     Bufor[licznik_bufora] = Bufor[licznik_bufora] + opis.klucz[index];
     index++;
     if(index == DL_KLUCZA)
      index = 0;
    }

   licznik_bufora = write(polanie,(void*)(&Bufor),liczba_bajtow);
   if(licznik_bufora != liczba_bajtow)
    {
      petluj = 0;
      rezultat = BRAK_MIEJSCA_NA_DYSKU;
    }

   if(liczba_bajtow != DL_BUFORA)
     petluj = 0;
  }while(petluj);

 koniec_odswiezania:

 if(otw_polanie)
   {
    setftime(polanie,&dataPolan);
    close(polanie);
   }
 if(otw_plik)
   close(plik);
 if(otw_zbior)
  close(zbior);

 return rezultat;
}