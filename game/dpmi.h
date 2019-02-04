/*
 Plik : DPMI.H
 Autor: Artur Bidziński
 Data : 18 maja 1996r.
*/

#ifndef MODUL_DO_IPX_Z_WYKORZYSTANIEM_SPECYFIKACJI_DPMI
#define MODUL_DO_IPX_Z_WYKORZYSTANIEM_SPECYFIKACJI_DPMI

/*struct J4
{
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
};*/

int Presence_DPMI(void);
int Presence_DPMI_in_real_mode(void);
int alloc_DOS_memory(unsigned short, unsigned short &, unsigned short &);
int free_DOS_memory(unsigned short);

#endif // MODUL_DO_IPX_Z_WYKORZYSTANIEM_SPECYFIKACJI_DPMI
