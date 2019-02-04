#include <iostream.h>
#include <stdio.h>
//#include <dos.h>      //delay()
#include <conio.h>    //cprintf();
#include "menegdma.h"

extern MENEGERDMA SND;
extern licznik;
extern int JestSB; //Definicje tych zmiennych sa w battle
extern int irq;
extern int port;
extern int kanal;
int instal=0;

void Inst(char *nazwa,MENEGERDMA &SND)
{

 int rez = SND.InstalujWAV(nazwa);
 if(rez)
 {
  cprintf("-");
  //cout<<nazwa<<" - blad instalacji "<<rez<<" plik " << nazwa <<endl;
 }
 else
 {
     instal++;
     cprintf(".");
   //cout<<endl<<nazwa<<" zainstalowany"<<endl;
 }
}

extern char drive[4];
int ladowanie(void)
{
  int Adlib = 0;
  int jest_SB = 0;

  FILE *plik;
  if ((plik = fopen("SETUP.INI", "rb")) == NULL)
     return 1;

  fread(&Adlib,1,1,plik);
  fread(&jest_SB,1,1,plik);
  fread(&irq,1,1,plik);
  fread(&port,2,1,plik);
  fread(&kanal,2,1,plik);
  fclose(plik);

  //UstawAdlibPresent(Adlib);
  SND.Init(jest_SB,irq,port,kanal);
//  SND.Osiagniecia();
 

 /*
 cprintf("Inicjalizacja dzwieku - PREMIERA ");
 getch();
 SND.LoadGlobalData("klej.dat", 0, 183, 186);
 cprintf("Inicjalizacja dzwieku - KONIEC PREMIERA ");*/

 if(jest_SB)
 {

     /* SND.PlayWav("data\\i001.dat");
     while(!kbhit())
     {                          TO BYLA PROBA - UDANA PROBA
         cprintf("$");
     }
     SND.EndPlayWav();*/ 
     
   cprintf("Inicjalizacja dzwieku ");
   char ss[50];
   sprintf(ss,"%sdata\\sound.dat",drive);
   SND.LoadGlobalData(ss,183);
   /*
   for(int i=1;i<10;i++)
   {
       sprintf(ss,"%sdata\\w00%d.dat",drive,i);
//       printf(ss);
//       printf("     ");
//       if(i==20)getch();
       Inst(ss,SND);
   }
   for( i=10;i<100;i++)
   {
       sprintf(ss,"%sdata\\w0%d.dat",drive,i);
//       printf(ss);
//       printf("     ");
//       if(i==20)getch();
       Inst(ss,SND);
   }
   for( i=100;i<184;i++)
   {
       sprintf(ss,"%sdata\\w%d.dat",drive,i);
//       printf(ss);
//       printf("     ");
       Inst(ss,SND);
   } */

#ifdef SPYWAV
   FILE *f = fopen("raport.txt","at");
   if(f)
   {
       fprintf(f,"ladowani instal = %d",instal);
       fclose(f);
   }
#endif
   
 }
/* if(jest_SB)
 {
     cout<<"\n       Gram... zas instal = "<<dec<<instal<<endl;
     getch();
     int m = 0;
     int znak = 1;
     int result;

     do
     {
         cprintf("licznik %d\r",licznik/18);
         znak = kbhit();
         if(znak)
         {
             znak = getch(); m = daj_m(znak);
         }
       // SND.Przerwij_odtwarzanie_DMA();// getch();
       // delay(1);
         if(m<instal)
         {//if(!SND.Odtwarzaj(m))

             result = SND(m);
             if(result)
             {
                 cout << endl << " Blad operatora " << result ;
                 getch();
             }
             if(result)
             {
                 cout<<"\nOdtwarzam muzyke TU BYL DAWNIEJ TYTUL ...sluchaj i delektuj sie...";
             }
             m = 10000;
         }
      //}
     }while(znak!=27);
     */

     /*if(SND.Przerwano_DMA())
      {
       SND.Przerwij_odtwarzanie_DMA();
       cout<<"\nOdtwarzanie zostlo przerwane";
      }
     else*//*
      cout<<"\nOdtwarzanie zakonczone";
  }
*/
 return 0;
}
