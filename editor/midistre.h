typedef unsigned char BYTE;
typedef unsigned int WORD;

class MIDIstream {
public:
  void SetData(BYTE *data, WORD len);
  long GetTime() { return ReadVarLen(); }
  void DoMessage();
  void SeekTo0() { datapos = 0; }
  virtual void TheEnd() { ; }
  virtual void Msg8(BYTE chan, BYTE b1, BYTE b2) { ; }
  virtual void Msg9(BYTE chan, BYTE b1, BYTE b2) { ; }
  virtual void MsgA(BYTE chan, BYTE b1, BYTE b2) { ; }
  virtual void MsgB(BYTE chan, BYTE b1, BYTE b2) { ; }
  virtual void MsgC(BYTE chan, BYTE b1) { ; }
  virtual void MsgD(BYTE chan, BYTE b1) { ; }
  virtual void MsgE(BYTE chan, BYTE b1, BYTE b2) { ; }
  virtual void RealTimeMsg(BYTE msg) { ; }
  virtual void Common0Bytes(BYTE msg) { ; }
  virtual void Common1Byte(BYTE msg, BYTE b1) { ; }
  virtual void Common2Bytes(BYTE msg, BYTE b1, BYTE b2) { ; }
  virtual void SysExF0(BYTE *data, WORD len) { ; }
  virtual void SysExF7(BYTE *data, WORD len) { ; }
  virtual void MetaEvent(BYTE type, BYTE *data, WORD len);

protected:
  BYTE *databuf, lastmsg;
  WORD datalen, datapos;
  long ReadVarLen();
  BYTE GetAndSkip();
};
