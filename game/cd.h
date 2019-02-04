/*
 Plik : CD.H
 Autor: Artur Bidziñski
 Data : 18 maja 1996r.
*/

#ifndef MODUL_DO_CZEGO_TO_SIE_JESZCZE_PRZEKONAMY
#define MODUL_DO_CZEGO_TO_SIE_JESZCZE_PRZEKONAMY

int InitCD(void);
void setVolume(int); // ustawia od 0 do 5        //!
int getVolume(void); // zwraca od zero do 5          //!
void CheckCD(
    void); // sprawdza czy jest grany odpowiedni track i ewentualnie nawraca
           //!
int PlayNext(void);
int PlayPrevious(void);
int PlayTrack(int);
void DeInitCD(void); //!
int StopPlaying(void);
void SetMaxTrack(int);

void FreeCDBuffer(void);
void ReadNrOfTracks(void);
int PlayTrack(int);
int GetCurrentTrack(void);

void BigOffCDAudio(void);
void BigOnCDAudio(void);

void OffCDAudio(void);
void OnCDAudio(void);

// 1.XII. int  GetNrOfTracks(void);

// 24.X.int  MinPlay(void);
// 24.X.int  MaxPlay(void);

// 24.X.void upvolume(void);   TE TO NA PEWNO NIE SA POTRZEBNE
// 24.X.void downvolume(void);

#endif
