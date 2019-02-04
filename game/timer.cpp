#include <dos.h>

/* UWAGA: ten modu³ nale¿y kompilowaæ przy wy³¹czonej
   kontroli przepe³nienia stosu
   (OPTIONS|COMPILER|ENTRY/EXIT CODE/Standard stack frame) */

// int licznik = 0;

// Artur tutaj musisz popracowac

void (*_NewTickProc)();
void interrupt (*_OldTickProc)(...);
int _TickValue;

void interrupt _TickProc(...) {
  return;
  // Artur tu pracuj
  /*
static cnt = 0,okey = 1;
if (++cnt >= _TickValue)
 { cnt = 0;
 licznik++;
 _OldTickProc();
 }
else
 {
 outportb(0x20,0x20);
 }
if(okey)
    {
    okey=0;
    _NewTickProc();
    okey=1;
    }
*/
}

void SetTimerProc(int freq, void (*proc)()) {
  // Artur tu pracuj
  /*
_TickValue = 1193180 / freq;
_OldTickProc = _dos_getvect(8);
_NewTickProc = proc;
asm cli
outportb(0x43,0x36);
outportb(0x40,_TickValue & 255);
outportb(0x40,_TickValue >> 8);
_dos_setvect(8,_TickProc);
_TickValue = 65536 / _TickValue;
asm sti
*/
}

void ResetTimerProc() {
  /*
asm cli
outportb(0x43,0x36);
outportb(0x40,0);
outportb(0x40,0);
_dos_setvect(8,_OldTickProc);
asm sti
*/
}
