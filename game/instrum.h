struct instrum {
  /* ustawienia zgodne ze strukturą
     pliku SBI od bajtu 24h do 2Eh */
  unsigned mMULTI : 4;
  unsigned mKSR : 1;
  unsigned mEGTYP : 1;
  unsigned mAM : 1;
  unsigned mVIB : 1;

  unsigned cMULTI : 4;
  unsigned cKSR : 1;
  unsigned cEGTYP : 1;
  unsigned cAM : 1;
  unsigned cVIB : 1;

  unsigned mTL : 6;
  unsigned mKSL : 2;

  unsigned cTL : 6;
  unsigned cKSL : 2;

  unsigned mDR : 4;
  unsigned mAR : 4;

  unsigned cDR : 4;
  unsigned cAR : 4;

  unsigned mRR : 4;
  unsigned mSL : 4;

  unsigned cRR : 4;
  unsigned cSL : 4;

  unsigned mWS : 2;
  unsigned : 6;

  unsigned cWS : 2;
  unsigned : 6;

  unsigned synth : 1;
  unsigned FB : 3;
  unsigned : 4;
};

instrum *ReadInstrum(char *filename, char *name);
instrum *ReadInstrumIndex(char *filename, int index);
