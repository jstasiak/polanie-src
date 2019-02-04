/*
 Plik : CD.CPP
 Autor: Artur Bidziñski
 Data : 18 maja 1996r.
*/

//    #define  POMOC
#undef POMOC
#ifdef POMOC
#include <conio.h>
#include <iostream.h>
#endif
#include "define.h"
#include "dpmi.h"
#include <i86.h>
#include <string.h>

#ifdef POMOC
extern void gotoxy(int, int);
#endif

struct J4 {
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
};

struct volumeinfo {
  unsigned char mode;
  unsigned char input0;
  unsigned char volume0;
  unsigned char input1;
  unsigned char volume1;
  unsigned char input2;
  unsigned char volume2;
  unsigned char input3;
  unsigned char volume3;
};

struct {
  unsigned short drive;
  unsigned short version;
  unsigned short error;
  unsigned char first_track;
  unsigned char last_track;
  unsigned int total_time;
  struct {
    unsigned int start;
    unsigned char control;
  } track[99 + 1 + 1];
  int command;
  int busy;
  int play_from;
} cd;

struct // structura sprawdzona  (co do dlugosci)
{
  unsigned char length;
  unsigned char subunit;
  unsigned char command;
  unsigned short status;
  unsigned char reserved[8];
  unsigned char media_dsc;
  unsigned int buffer;
  unsigned short buffer_size;
  unsigned short sector;
  unsigned int reserved2;
} ioctli_request = {
    sizeof(ioctli_request), 0, 3, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0};

struct // structura sprawdzona  (co do dlugosci)
{
  unsigned char length;
  unsigned char subunit;
  unsigned char command;
  unsigned short status;
  unsigned char reserved[8];
  unsigned char media_dsc;
  unsigned int buffer;
  unsigned short buffer_size;
  unsigned short sector;
  unsigned int reserved2;
} ioctli_request_vol = {
    sizeof(ioctli_request), 0, 3, 0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0, 0};

struct {
  unsigned char command;
  unsigned int status;
} device_status = {6, 0};

const unsigned short ERROR = 0x8000;
const unsigned short BUSY = 0x0200;
const unsigned short DONE = 0x0100;

REGS re;
SREGS se;
J4 t;

// volumeinfo my_vol, volume_for_cd;
volumeinfo my_new_volume;
int track = 0;
int memisalloc = 0;
unsigned short Selector = 0;
unsigned short Segment = 0;
unsigned int LineAddress = 0;
unsigned int LineAddressTwo = 0;
int NrOfTracks = 0;
int FirstTrack = 0;
int LastTrack = 0;

unsigned int BufferSizeForCD = 512;
int BigActiveCDAudio = 1;
int ActiveCDAudio = 1;

/*24.X.int MinPlay(void)
{
    return FirstTrack;
}
int MaxPlay(void)
{
    return LastTrack;
}*/

void BigOffCDAudio(void) { BigActiveCDAudio = 0; }

void BigOnCDAudio(void) { BigActiveCDAudio = 1; }

void OffCDAudio(void) { ActiveCDAudio = 0; }

void OnCDAudio(void) { ActiveCDAudio = 1; }

void SetMaxTrack(int maxtrack) { LastTrack = maxtrack; }

// int GetNrOfTracks(void)
//{
//  return NrOfTracks;
//}

unsigned int red2hsg(unsigned int red) {
  return ((red >> 16) & 0xFF) * 60 * 75 + ((red >> 8) & 0xFF) * 75 +
         (red & 0xFF) - 150;
}

void AllocCDBuffer(void) {
  int result;
  if (memisalloc)
    return;
  result = alloc_DOS_memory((unsigned short)((BufferSizeForCD + 15) >> 4),
                            Segment, Selector);
  if (result)
    return;

  memisalloc = 1;
  LineAddress = (unsigned int)Segment << 4;
  LineAddressTwo = LineAddress + 256; // Pracujemy na tym samym buforze pamieci
}

void FreeCDBuffer(void) {
  if (memisalloc) {
    free_DOS_memory(Selector);
    memisalloc = 0;
  }
}

int CallMSCDEX(void *requestHeader, int size) {
  memcpy((void *)LineAddress, requestHeader, size);
  t.eax = 0x1510;
  t.ecx = (unsigned int)cd.drive;
  t.ebx = 0;
  t.edi = 0; // AB
  t.esi = 0; // AB
  t.es = Segment;

  re.x.eax = 0x0300;
  re.x.ebx = 0x002F;
  re.x.ecx = 0x0000;
  re.x.edi = FP_OFF(&t);
  se.es = FP_SEG(&t);
  int386x(0x31, &re, &re, &se);
  if (re.x.cflag)
    return ERROR_INT_2FH;

  memcpy(requestHeader, (void *)LineAddress, size);
  return 0;
}

int CheckError(unsigned short status) {
  cd.busy = status & BUSY ? 1 : 0;
  if (status & ERROR) {
    cd.error = (status & 0xF);
    return -1;
  }
  return 0;
}

int MakeIOCtliCall(void *buffer, int size) {
  // unsigned short selector=0;
  // unsigned short segment;
  // unsigned int   lineaddress = LineAddress;

  // alloc_DOS_memory( (unsigned short)((size+15)>>4) , segment, selector);
  // lineaddress=((int)segment)<<4;
  memcpy((void *)LineAddressTwo, buffer, size);

  ioctli_request.buffer = LineAddressTwo << 12;
  ioctli_request.buffer_size = (unsigned short)size;

  CallMSCDEX((void *)&ioctli_request, sizeof(ioctli_request));

  memcpy(buffer, (void *)LineAddressTwo, size);

  // free_DOS_memory(selector);
  return 0;
}

int InitCD(void) {
  //  if(!BigActiveCDAudio) return 0;
  AllocCDBuffer();
  if (!memisalloc)
    return NO_MEMORY_FOR_CD;

  //    cout <<"> Getting drive letter"<<endl;

  t.eax = 0x1500;
  t.ecx = 0xFFFF;

  re.x.eax = 0x0300;
  re.x.ebx = 0x002F;
  re.x.ecx = 0x0000;
  re.x.edi = FP_OFF(&t);
  se.es = FP_SEG(&t);
  int386x(0x31, &re, &re, &se);
  if (re.x.cflag)
    return ERROR_INT_2FH;

  cd.drive = (unsigned short)t.ecx;

  //    cout <<"> Drive letter is "<<char(65+cd.drive)<<endl;

  //    cout<<"> Getting driver version"<<endl;

  t.eax = 0x150C;

  re.x.eax = 0x0300;
  re.x.ebx = 0x002F;
  re.x.ecx = 0x0000;
  re.x.edi = FP_OFF(&t);
  se.es = FP_SEG(&t);
  int386x(0x31, &re, &re, &se);
  if (re.x.cflag)
    return ERROR_INT_2FH;

  cd.version = (unsigned short)t.ebx;

  //    cout<<"> Driver version is
  //    "<<(cd.version>>8)<<"."<<(cd.version&255)<<endl;

  if (cd.version == 65535)
    return (-1);
  MakeIOCtliCall(&device_status, sizeof(device_status));

  if (CheckError(ioctli_request.status))
    return -2;
  if ((device_status.status & (1 << 4)) == 0)
    return -2;
  if ((device_status.status & (1 << 9)) == 0)
    return -2;

  return 0;
}

unsigned int smallVolume = 5;
unsigned char Volume = 255;

/*void cd_set_volume(volumeinfo *vol)
{
   vol->mode = 3;
   ioctli_request.length = sizeof(ioctli_request);
   ioctli_request.subunit   = 0;
   ioctli_request.command   = 12;
   ioctli_request.media_dsc = 0;

   MakeIOCtliCall((void*)vol, sizeof(volumeinfo));

   cd.error = ioctli_request.status;
}                 */

void cd_set_volume(volumeinfo *vol) {
  // unsigned short selector=0;
  // unsigned short segment;
  // unsigned int   lineaddress;
  int Size = sizeof(volumeinfo);

  vol->mode = 3;
  ioctli_request_vol.length = sizeof(ioctli_request_vol);
  ioctli_request_vol.subunit = 0;
  ioctli_request_vol.command = 12;
  ioctli_request_vol.media_dsc = 0;

  // alloc_DOS_memory( (unsigned short)((Size+15)>>4) , segment, selector);
  // lineaddress=((int)segment)<<4;
  memcpy((void *)LineAddressTwo, (void *)vol, Size);

  ioctli_request_vol.buffer = LineAddressTwo << 12;
  ioctli_request_vol.buffer_size = (unsigned short)Size;

  CallMSCDEX((void *)&ioctli_request_vol, sizeof(ioctli_request_vol));

  memcpy((void *)vol, (void *)LineAddressTwo, Size);

  // free_DOS_memory(selector);
  //  cd.error = ioctli_request.status;
}

/*void upvolume(void)
{
#ifdef POMOC
   gotoxy(20,1);
   cout << "UPvolume()       ";

#endif
    if(Volume>200) return;
    if(Volume<200)
   {
       Volume+=50;
    }else
    {
        Volume = 255;
    }
#ifdef POMOC
   gotoxy(20,2);
   cout << "UPvolume() my_new_volume.volume0>0)" << (int)Volume <<" - powinien
glos sie poglosnic     "; #endif my_new_volume.volume3=Volume;
       my_new_volume.volume2=Volume;
       my_new_volume.volume1=Volume;
       my_new_volume.volume0=Volume;

       my_new_volume.input0 = 0;
       my_new_volume.input1 = 1;
       my_new_volume.input2 = 2;
       my_new_volume.input3 = 3;

       memcpy((void*)&volume_for_cd,(void*)&my_new_volume,sizeof(volumeinfo));
       cd_set_volume(&volume_for_cd);
}*/

/*void downvolume(void)
{
#ifdef POMOC
   gotoxy(20,1);
   cout << "downvolume()     ";
#endif
    if(!Volume) return;
   if(Volume>200)
   {
       Volume = 200;
    }else
    {
        Volume -= 50;
    }
#ifdef POMOC
   gotoxy(20,2);
   cout << "downvolume() my_new_volume.volume0>0) " << (int)Volume <<" -
powinien glos sie sciszyc"; #endif my_new_volume.volume3=Volume;
       my_new_volume.volume2=Volume;
       my_new_volume.volume1=Volume;
       my_new_volume.volume0=Volume;

       my_new_volume.input0 = 0;
       my_new_volume.input1 = 1;
       my_new_volume.input2 = 2;
       my_new_volume.input3 = 3;

       memcpy((void*)&volume_for_cd,(void*)&my_new_volume,sizeof(volumeinfo));
       cd_set_volume(&volume_for_cd);
}*/

void ReadNrOfTracks(void) {
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return;
  struct {
    unsigned char command;
    unsigned char first;
    unsigned char last;
    unsigned int lead_out;
  } ibuffer = {10, 0, 0, 0};

  MakeIOCtliCall(&device_status, sizeof(device_status));
  CheckError(ioctli_request.status);

  for (int i = 0; i < 200000; i++) {
    MakeIOCtliCall(&ibuffer, sizeof(ibuffer));
    if ((ioctli_request.status & ERROR) == 0)
      break;
    if ((ioctli_request.status & 0xFF) != 0xF)
      break;
  }
  CheckError(ioctli_request.status);
  cd.first_track = ibuffer.first;
  cd.last_track = ibuffer.last;
  cd.total_time = ibuffer.lead_out;
  struct {
    unsigned char command;
    unsigned char track;
    unsigned int start;
    unsigned char control;
  } tbuffer = {11, 0, 0, 0};
  for (i = cd.first_track; i <= cd.last_track; i++) {
    tbuffer.track = (unsigned char)i;
    MakeIOCtliCall(&tbuffer, sizeof(tbuffer));
    CheckError(ioctli_request.status);
    cd.track[i].start = tbuffer.start;
    cd.track[i].control = tbuffer.control & 0xF0;
  }
  cd.track[cd.last_track + 1].start = cd.total_time;
  i = cd.first_track;
  int min = ((cd.total_time >> 16) & 0xFF) - ((cd.track[i].start >> 16) & 0xFF);
  int sec = ((cd.total_time >> 8) & 0xFF) - ((cd.track[i].start >> 8) & 0xFF);
  int frm = (cd.total_time & 0xFF) - (cd.track[i].start & 0xFF);
  if (sec < 0) {
    min -= 1;
    sec = 60 + sec;
  }
  if (frm < 0) {
    sec -= 1;
    frm = 75 + frm;
  }

  FirstTrack = (int)cd.first_track;
  LastTrack = (int)cd.last_track;
  NrOfTracks = LastTrack - FirstTrack + 1;

  //         cout<<"> Tracks: "<<cd.last_track-cd.first_track+1;
  //         cout<<", total time: "<<min<<":"<<sec<<":"<<frm<<endl;
}

struct {
  unsigned char length;
  unsigned char subunit;
  unsigned char command;
  unsigned short status;
  unsigned char reserved[8];
  unsigned char adr_mode;
  unsigned int start;
  unsigned int sectors;
} play_request = {sizeof(play_request),     0, 132, 0,
                  {0, 0, 0, 0, 0, 0, 0, 0}, 1, 0,   0};

struct {
  unsigned char length;
  unsigned char subunit;
  unsigned char command;
  unsigned short status;
  unsigned char reserved[8];
} stop_request = {sizeof(stop_request), 0, 133, 0, {0, 0, 0, 0, 0, 0, 0, 0}};

int StopPlaying(void) {
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return 0;
  CallMSCDEX(&stop_request, sizeof(stop_request));

  return CheckError(stop_request.status);
}

void setVolume(int newVolume) // ustawia od 0 do 5        //!
{
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return;
  if ((newVolume < 0) || (newVolume > 5))
    return;
  switch (newVolume) {
  case 0:
    Volume = 0;
    break;
  case 1:
    Volume = 50;
    break;
  case 2:
    Volume = 75;
    break;
  case 3:
    Volume = 113;
    break;
  case 4:
    Volume = 170;
    break;
  case 5:
    Volume = 255;
    break;
  }

  smallVolume = newVolume;

  my_new_volume.volume3 = Volume;
  my_new_volume.volume2 = Volume;
  my_new_volume.volume1 = Volume;
  my_new_volume.volume0 = Volume;

  my_new_volume.input0 = 0;
  my_new_volume.input1 = 1;
  my_new_volume.input2 = 2;
  my_new_volume.input3 = 3;

  // 24.X.memcpy((void*)&volume_for_cd,(void*)&my_new_volume,sizeof(volumeinfo));
  // 24.X.cd_set_volume(&volume_for_cd);
  cd_set_volume(&my_new_volume);
}

int PlayTrack(int numbertrack) {
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return 0;
  MakeIOCtliCall(&device_status, sizeof(device_status));
  CheckError(ioctli_request.status);

  if (cd.busy)
    StopPlaying();
  if (numbertrack > LastTrack) {
    numbertrack = LastTrack;
  }

  for (int i = numbertrack; i <= LastTrack; i++) {
    if ((cd.track[i].control & 0x40) == 0)
      break;
  }
  if (i > LastTrack) {
    //                 cout<<"­ Seek beyond last track"<<endl;
    return -1;
  }
  play_request.start = cd.track[i].start;
  play_request.sectors = red2hsg(cd.total_time) - red2hsg(cd.track[i].start);
  CallMSCDEX((void *)&play_request, sizeof(play_request));
  CheckError(play_request.status);
  // cout<<"! Playing started from track "<<i<<endl;
  track = numbertrack;
  setVolume(smallVolume);
  return 0;
}

int GetCurrentTrack(void) {
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return 0;
  if (!cd.busy)
    return 0;
  struct {
    unsigned char command;
    unsigned char control;
    unsigned char track;
    unsigned char index;
    unsigned char min1;
    unsigned char sec1;
    unsigned char frm1;
    unsigned char zero;
    unsigned char min2;
    unsigned char sec2;
    unsigned char frm2;
  } qbuffer = {12};
  MakeIOCtliCall(&qbuffer, sizeof(qbuffer));
  if (CheckError(ioctli_request.status))
    return -1;
  return ((qbuffer.track & 0xF0) >> 4) * 10 + qbuffer.track & 0x0F;
}

int PlayNext(void) {
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return 0;
  int result = track;
#ifdef POMOC
  cout << "\n PlayNext() ";
  cout << "\n result " << result << " cd.first_track" << (int)cd.first_track
       << " cd.last_track " << (int)cd.last_track << endl;
#endif
  if (result < LastTrack)
    result++;
  else
    result = 5; // FirstTrack;

#ifdef POMOC
  cout << "\n PlayTrack(" << dec << result << ')' << endl;
#endif

  PlayTrack(result);
  return 0;
}

int PlayPrevious(void) {
  int result = track;
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return 0;
#ifdef POMOC
  cout << "\n PlayPrevious() ";
  cout << "\n result " << result << " cd.first_track" << (int)cd.first_track
       << " cd.last_track " << (int)cd.last_track << endl;
#endif
  if (result > 5)
    result--;
  else
    result = LastTrack;
#ifdef POMOC
  cout << "\n PlayTrack(" << dec << result << ')' << endl;
#endif
  PlayTrack(result);
  return 0;
}

int getVolume(void) // zwraca od zero do 5          //!
{
  return (int)smallVolume;
}

// TA FUNKCJE NALEZY POPRAWIC
void CheckCD(
    void) // sprawdza czy jest grany odpowiedni track i ewentualnie nawraca
{
  if ((!BigActiveCDAudio) || (!ActiveCDAudio))
    return;
  int result = GetCurrentTrack();
  if (result && (result != track)) {
    PlayTrack(track);
  }
}

int PlayNext(void);
int PlayPrevious(void);
int PlayTrack(int);
void DeInitCD() {
  if (BigActiveCDAudio) {
    StopPlaying();
    FreeCDBuffer();
  }
}

//*********************************************************************************
//*********************************************************************************
/* ORG KF

struct
{
  byte length;
  byte subunit;
  byte command;
  word status;
  byte reserved[8];
  byte adr_mode;
  dword start;
  dword sectors;
}
play_request={sizeof (play_request),0,132,0,{0,0,0,0,0,0,0,0},1,0,0};

struct
{
  byte length;
  byte subunit;
  byte command;
  word status;
  byte reserved[8];
}
stop_request={sizeof (stop_request),0,133,0,{0,0,0,0,0,0,0,0}};

int CDAStopPlaying(void)
{
  CallMSCDEX((void*)&stop_request);
        clogfile<<"ù Playing stopped"<<endl;
  return CheckError(stop_request.status);
}

int CDAMediaChanged(void)
{
  MakeIOCtliCall (&media_changed_status,sizeof (media_changed_status));
        return (media_changed_status.status==0xFF);
}

int CDACurrentTrack(void)
{
  if (!mscdex.busy) return 0;
  struct
  {
    byte command;
    byte control;
    byte track;
    byte index;
    byte min1;
    byte sec1;
    byte frm1;
    byte zero;
    byte min2;
    byte sec2;
    byte frm2;
  }
  qbuffer={12};
  MakeIOCtliCall(&qbuffer,sizeof (qbuffer));
  if (CheckError(ioctli_request.status)) return -1;
        return bcd2bin(qbuffer.track);
}

int CDAPlayTrack(int track)
{
        MakeIOCtliCall (&device_status,sizeof (device_status));
        CheckError (ioctli_request.status);

        if (mscdex.busy) CDAStopPlaying();
  if (track>mscdex.last_track) {
                clogfile<<"­ Seek beyond last track"<<endl;
                return -1;
        }

  for (int i=track;i<=mscdex.last_track;i++)
  {
    if ((mscdex.track[i].control&0x40)==0) break;
  }
  if (i>mscdex.last_track) {
                clogfile<<"­ Seek beyond last track"<<endl;
                return -1;
        }
  play_request.start=mscdex.track[i].start;
  play_request.sectors=red2hsg (mscdex.total_time-mscdex.track[i].start);
  CallMSCDEx(&play_request);
  CheckError(play_request.status);
  clogfile<<"ù Playing started from track "<<i<<endl;
        return 0;
}

void CDADone(void)
{
        if (CDAStopOnExit) CDAStopPlaying();
        _dos_freemem(CDABufferSelector);
}



*/
