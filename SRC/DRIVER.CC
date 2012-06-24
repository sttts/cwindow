/*

DRIVER.C - Borland version

(c) by Stefan Schimanski '1997

    ported to DJGPP by Janko Heilgeist '97

*/


#include <stdlib.h>
#include <driver.h>

#ifndef __WATCOMC__
  #include <conio.h>
#endif


//===========================================

#ifdef __WATCOMC__
char* VRAM=(char*)0xb8000;
#elif defined(__DJGPP__)
  #include <sys/farptr.h>
  #include <go32.h>
#else
char far* VRAM=(char far*)0xb8000000;
#endif


#if defined(__DJGPP__)

void* GetRect(int px, int py, int asx, int asy)
{
  if (asx*asy>0)
  {
    char* Buf=(char*)malloc(asx*asy*2);

    for (int y=0; y<asy; y++)
      for (int x=0; x<asx; x++)
      {
	*(Buf+(y*asx+x)*2)= _farpeekb(_dos_ds,
	                              0xb8000+(y+py)*160+(x+px)*2);
	*(Buf+(y*asx+x)*2+1)= _farpeekb(_dos_ds,
	                                0xb8000+(y+py)*160+(x+px)*2+1);
      }

    return (void*)Buf;
  } else return NULL;
}


void PutRect(void* Rect, int px, int py, int asx, int asy)
{
  if (Rect!=NULL && asx*asy>0)
  {
    char* Buf=(char*)Rect;

    for (int y=0; y<asy; y++)
      for (int x=0; x<asx; x++)
      {
        _farpokeb(_dos_ds,
                  0xb8000+(y+py)*160+(x+px)*2,
                  *(Buf+(y*asx+x)*2));

	_farpokeb(_dos_ds,
	          0xb8000+(y+py)*160+(x+px)*2+1,
	          *(Buf+(y*asx+x)*2+1));
      }
  }
}


void PutChar(int x, int y, char c, char Col)
{
  if (x<GetMaxX() && y<GetMaxY() && x>=0 && y>=0)
  {
    _farpokeb(_dos_ds, 0xb8000+y*160+x*2, c);
    _farpokeb(_dos_ds, 0xb8000+y*160+x*2+1, Col);
  }
}


#else

void* GetRect(int px, int py, int asx, int asy)
{
  if (asx*asy>0)
  {
    char* Buf=(char*)malloc(asx*asy*2);

    for (int y=0; y<asy; y++)
      for (int x=0; x<asx; x++)
      {
	*(Buf+(y*asx+x)*2)=*(VRAM+(y+py)*160+(x+px)*2);
	*(Buf+(y*asx+x)*2+1)=*(VRAM+(y+py)*160+(x+px)*2+1);
      }

    return (void*)Buf;
  } else return NULL;
}


void PutRect(void* Rect, int px, int py, int asx, int asy)
{
  if (Rect!=NULL && asx*asy>0)
  {
    char* Buf=(char*)Rect;

    for (int y=0; y<asy; y++)
      for (int x=0; x<asx; x++)
      {
	*(VRAM+(y+py)*160+(x+px)*2)=*(Buf+(y*asx+x)*2);
	*(VRAM+(y+py)*160+(x+px)*2+1)=*(Buf+(y*asx+x)*2+1);
      }
  }
}


void PutChar(int x, int y, char c, char Col)
{
  if (x<GetMaxX() && y<GetMaxY() && x>=0 && y>=0)
  {
    *(VRAM+y*160+x*2)=c;
    *(VRAM+y*160+x*2+1)=Col;
  }
}

#endif

void PutString(int x, int y, char* s, char Col)
{
  while (x>=0 && x<GetMaxX() && y>=0 && y<GetMaxY() && *s)
  {
    PutChar(x,y,*s,Col);
    x++;
    s++;
  }
}

#ifdef __WATCOMC__

extern void gotoxy(char x, char y);
#pragma aux gotoxy=		\
	"dec dl"		\
	"dec dh"		\
	"mov ah, 2"		\
	"mov bh, 0"		\
	"int 0x10"		\
	parm [dl][dh]		\
	modify [bh]

extern int wherex();
#pragma aux wherex=		\
	"mov ah, 3"		\
	"mov bh, 0"		\
	"int 0x10"		\
	"mov al, dl"		\
	"and eax, 0xff"		\
	"inc eax"		\
	modify [dx bh]		\
	value [eax]

extern int wherey();
#pragma aux wherey=		\
	"mov ah, 3"		\
	"mov bh, 0"		\
	"int 0x10"		\
	"mov al, dh"		\
	"and eax, 0xff"		\
	"inc eax"		\
	modify [dx bh]		\
	value [eax]


#endif

void PutCursor(int x, int y)
{
  gotoxy(x+1, y+1);
}


void GetCursor(int &x, int &y)
{
  x=wherex()-1;
  y=wherey()-1;
}

int GetMaxX()
{
  return 80;
}


int GetMaxY()
{
  return 25;
}



