int IsALPresent();
void SetALMode(int mode);
void SetInstrum(int voice, void *instr);
void PlayNote(int voice, int octave, int note);
void StopNote(int voice);
void KillNote(int voice);
void PlayNoteFreq(int voice, int octave, int f_num);
void SetVolume(int voice, unsigned char volume);
