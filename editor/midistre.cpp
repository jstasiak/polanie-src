#include "midistre.h"

void MIDIstream::SetData(BYTE *data, WORD len)
// ustawia adres i długość bufora z danymi MIDI
{
  databuf = data;
  datalen = len;
  datapos = 0;
}

long MIDIstream::ReadVarLen()
// odczytuje liczbę w postaci o zmiennej długości
{
  long value;
  BYTE c;
  if ((value = databuf[datapos++]) & 0x80) {
    value &= 0x7F;
    do {
      value = (value << 7) + ((c = databuf[datapos++]) & 0x7F);
    } while (c & 0x80);
  }
  return value;
}

BYTE MIDIstream::GetAndSkip()
// pobiera kolejny bajt danych omijając
// ew. komunikaty czasowe
{
  while (databuf[datapos++] >= 0xF8)
    ;
  return databuf[datapos - 1];
}

void MIDIstream::DoMessage()
// pobiera kolejny komunikat i wywołuje
// odpowiednią dla niego funkcję
{
  BYTE B, C, D;
  WORD len;
  if (datapos >= datalen) {
    TheEnd();
    return;
  }
  B = databuf[datapos++];
  if (B < 0x80) // running status byte; B>=0 zawsze bo jest unsigned
  {
    B = lastmsg;
    datapos--;
  }
  lastmsg = B;
  switch (B >> 4) {
  case 0x8: // wyłącz nutę
    C = GetAndSkip();
    D = GetAndSkip();
    Msg8(B & 15, C, D);
    break;
  case 0x9: // włącz nutę
    C = GetAndSkip();
    D = GetAndSkip();
    Msg9(B & 15, C, D);
    break;
  case 0xA: // nacisk klawisza
    C = GetAndSkip();
    D = GetAndSkip();
    MsgA(B & 15, C, D);
    break;
  case 0xB: // ustawienie kontrolera
    C = GetAndSkip();
    D = GetAndSkip();
    MsgB(B & 15, C, D);
    break;
  case 0xE: // zmiana wys. dźwięku
    C = GetAndSkip();
    D = GetAndSkip();
    MsgE(B & 15, C, D);
    break;
  case 0xC: // zmiana programu
    MsgC(B & 15, GetAndSkip());
    break;
  case 0xD: // głośność
    MsgD(B & 15, GetAndSkip());
    break;
  default:
    switch (B) {
    case 0xF0: // system exclusive F0
      len = ReadVarLen();
      SysExF0(databuf + datapos, len);
      datapos += len;
      break;
    case 0xF1:
    case 0xF3: // komunikaty F1 i F3
      Common1Byte(B, GetAndSkip());
      break;
    case 0xF2: // pozycja melodii
      C = GetAndSkip();
      D = GetAndSkip();
      Common2Bytes(B, C, D);
      break;
    case 0xF4:
    case 0xF5:
    case 0xF6:
      Common0Bytes(B);
      break;
    case 0xF7: // system exclusive F7
      len = ReadVarLen();
      SysExF7(databuf + datapos, len);
      datapos += len;
      break;
    case 0xFF: // metazdarzenie
      C = databuf[datapos++];
      len = ReadVarLen();
      MetaEvent(C, databuf + datapos, len);
      datapos += len;
      break;
    default: // komunikaty czasowe
      RealTimeMsg(B);
    }
  }
}

void MIDIstream::MetaEvent(BYTE type, BYTE *data, WORD len) {
  if (type == 0x2F)
    TheEnd();
}
