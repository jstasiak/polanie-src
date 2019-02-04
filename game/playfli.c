#include <stdio.h>

void main(int argc, char *argv[])
{
  int status;

  graph_mode();
  status=play(argv[1]);		// status=1 : OK;
				// status=0 : blad przy otwieraniu/czytaniu pliku .FLI
  text_mode();
  printf("Status: ");
  (status) ? (printf("OK\n")) : (printf("I/O error\n"));
}

// ---------------------------------------------------------------------------

#include <graph.h>
#include <stdio.h>
#include <string.h>
#include <i86.h>
#include <dos.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned char boolean;

byte screen[64000];
byte source[64000];
byte pal[0x300];
byte *source_ptr;
long int timer_delay, timer_stop;

void graph_mode()
{
  union REGS regs;
  regs.w.ax=0x13;
  int386(0x10,&regs,&regs);
}

void text_mode()
{
  union REGS regs;
  regs.w.ax=0x03;
  int386(0x10,&regs,&regs);
}

int play(char *s)
{
  FILE fh;
  byte *chunk_start;
  word *_word;
  word frames, delay_value, frame_size, chunks, chunk_size, chunk_type;
  word current_frame=0, current_chunk;
  boolean escape=0;
  boolean pal_changed;
  int err=1;
  size_t status;

  fh=*fopen(s,"rb");
  if (ferror(&fh)) err=0;
  else
   {
     init_timer();
     status=fread(source,0x80,1,&fh);
     if (!status) err=0;
     else
      {
        source_ptr=source;
        _word=(word *) (source_ptr+6);
        frames=*_word;
        _word=(word *) (source_ptr+16);
        delay_value=*_word;
        timer_delay=(delay_value/70.0)*1193180.0;
        memset(screen,0,64000);
        memset(pal,0,0x300);
        do
          {
            start_timer();
            pal_changed=0;
            status=fread(source,0x10,1,&fh);
            if (!status) err=0;
            else
             {
               source_ptr=source;
               _word=(word *) source_ptr;
               frame_size=*_word;
               _word=(word *) (source_ptr+6);
               chunks=*_word;
               if (chunks>0)
                {
                  frame_size-=0x10;
                  status=fread(source,frame_size,1,&fh);
                  if (!status) err=0;
                  else
                   {
                     current_chunk=0;
                     chunk_start=source_ptr;
                      do
                        {
                          source_ptr=chunk_start;
                          _word=(word *) source_ptr;
                          chunk_size=*_word;
                          chunk_start+=chunk_size;
                          _word=(word *) (source_ptr+4);
                          chunk_type=*_word;
                          source_ptr+=6;
                          switch (chunk_type)
                           {
                             case 11: {pal_changed=1; fli_color(); break;}
                             case 12: {fli_lc(); break;}
                             case 13: {fli_black(); break;}
                             case 15: {fli_brun(); break;}
                             case 16: {fli_copy(); break;}
                           }
                          current_chunk++;
                        }
                      while (current_chunk!=chunks);
                   }
                }
               if (err)
                {
                  wait_sync();
                  if (pal_changed) move_palette();
                  move_screen();
                  wait_timer();
                  escape=check_escape();
                  current_frame++;
                }
             }
          }
        while ((!escape) && (current_frame!=frames) && (err));
      }
     fclose(&fh);
     deinit_timer();
   }
 return(err);
}

void fli_color()
{
  word *packets, d, col_start=0, col_length;

  packets=(word *) source_ptr;
  source_ptr+=2;
  for (d=1;d<=*packets;d++)
   {
     col_start+=*source_ptr*3;
     col_length=*(source_ptr+1)*3;
     source_ptr+=2;
     if (col_length==0) col_length=0x300;
     memcpy(&pal[col_start],source_ptr,col_length);
     source_ptr+=col_length;
     col_start+=col_length;
   }
}

void fli_lc()
{
  word *start_line, *lines_to_change, line_address, d, p;
  byte packets, c, skip_count, size_count, fill_value;

  start_line=(word *) source_ptr;
  lines_to_change=(word *) (source_ptr+2);
  source_ptr+=4;
  line_address=*start_line*320;
  for (d=1;d<=*lines_to_change;d++)
   {
     packets=*source_ptr++;
     p=line_address;
     for (c=1;c<=packets;c++)
      {
        skip_count=*source_ptr++;
        size_count=*source_ptr++;
        p+=skip_count;
        if (!(size_count & 0x80))
         {
           memcpy(&screen[p],source_ptr,size_count);
           source_ptr+=size_count;
         }
        else
         {
           size_count=(size_count^0xFF)+1;
           fill_value=*source_ptr++;
           memset(&screen[p],fill_value,size_count);
         }
        p+=size_count;
      }
     line_address+=320;
   }
}

void fli_black()
{
  memset(screen,0,64000);
}

void fli_brun()
{
  word line_address=0, d, p;
  byte packets, c, size_count, fill_value;

  for (d=1;d<=200;d++)
   {
     packets=*source_ptr++;
     p=line_address;
     for (c=1;c<=packets;c++)
      {
        size_count=*source_ptr++;
        if (size_count & 0x80)
         {
           size_count=(size_count^0xFF)+1;
           memcpy(&screen[p],source_ptr,size_count);
           source_ptr+=size_count;
         }
        else
         {
           fill_value=*source_ptr++;
           memset(&screen[p],fill_value,size_count);
         }
        p+=size_count;
      }
     line_address+=320;
   }
}

void fli_copy()
{
  memcpy(screen,source_ptr,64000);
}

void wait_sync()
{
  while (inp(0x3DA) & 8);
  while (!(inp(0x3DA) & 8));
}

void move_palette()
{
  word d;

  for (d=0;d<0x100;d++)
   {
     outp(0x3C8,d);
     outp(0x3C9,pal[d*3]);
     outp(0x3C9,pal[d*3+1]);
     outp(0x3C9,pal[d*3+2]);
   }
}

void move_screen()
{
  memcpy(0xA0000,screen,64000);
}

int check_escape()
{
  if (inp(0x60)==1) return(1); else return(0);
}

void init_timer()
{
  outp(0x43,0x34);
  outp(0x40,0);
  outp(0x40,0);
}

void deinit_timer()
{
  outp(0x43,0x36);
  outp(0x40,0);
  outp(0x40,0);
}

long int read_timer()
{
  word w;
  byte lsb, msb;
  memcpy(&w,0x46C,2);
  outp(0x43,0);
  lsb=inp(0x40);
  msb=inp(0x40);
  return(((w<<16)&(0x7FFFFFFF))+(0xFFFF-((msb<<8)+lsb)));
}

void start_timer()
{
  long int t;
  t=read_timer();
  if ((t+timer_delay)>=0x80000000) timer_stop=t+timer_delay-0x80000000;
  else timer_stop=t+timer_delay;
}

void wait_timer()
{
  long int t;
  do t=read_timer(); while (t<timer_stop);
}
