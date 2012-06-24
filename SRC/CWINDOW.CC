/*

CWINDOW.C

(c) by Stefan Schimanski '1997

*/

#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <driver.h>
#include <cwindow.h>



char stdBorder[]="³ÄÚ¿ÀÙ";
char dblBorder[]="ºÍÉ»È¼";


//=================================================


char stdColor[]={0x71,0x17,0x17,                // Desktop
		 0x17, 0x1f, 0x1f,              // Window
		 0x17, 0x1f, 0x1f,              // Message
		 0x17, 0x1f, 0x1f, 0x71,        // Menu
		 0x17, 0x1f, 0x1f,              // List
		 0x17, 0x1f, 0x1f, 0x31,        // Edit
		 0x17, 0x1f, 0x1f, 0x1f, 0x1f,  // Bar
                 0x07, 0x1f, 0x1f,              // CrunchWindow
                };

char (*Colors)=stdColor;
int ColorNum=strlen(stdColor);


inline char Color(char Num)
{
  if (Num>=ColorNum) return 0xf4; else return *(Colors+Num);
}


void SetColors(char* Palette)
{
  if (Palette!=NULL) Colors=Palette; else
    Colors=stdColor;
  ColorNum=strlen(Colors);
}


//=================================================


CView::CView(int apx, int apy, int asx, int asy)
{
  px=apx;
  py=apy;
  sx=asx;
  sy=asy;
  Background=NULL;
  GetCursor(OldCursorX, OldCursorY);
}


CView::~CView()
{
  if (Background!=NULL) free(Background);
  PutCursor(OldCursorX, OldCursorY);
}


void CView::Draw()
{
  UpdateCursor();
}


void CView::Redraw()
{
  Draw();
}


void CView::Run()
{
  Show();

  Action();

  Hide();
}


void CView::Show()
{
  SaveBackground();
  Draw();
}


void CView::Hide()
{
  RestoreBackground();
  PutCursor(OldCursorX, OldCursorY);
}


void CView::SaveBackground()
{
  if (Background!=NULL) free(Background);
  Background=GetRect(px, py, sx, sy);
}


void CView::RestoreBackground()
{
  if (Background!=NULL) PutRect(Background, px, py, sx, sy);
}


void CView::Action()
{
}


void CView::UpdateCursor()
{
  PutCursor(0, GetMaxY()-1);
}


char CView::Color(char Num)
{
  return ::Color(ColorIndex(Num));
}


char CView::ColorIndex(char Num)
{
  return ColView+Num;
}


//================================================


CDesktop::CDesktop(char* Top, char* Bottom)
 : CView(0,0,GetMaxX(), GetMaxY())
{
  if (Top!=NULL) TopMsg=strdup(Top); else TopMsg=strdup("");
  if (Bottom!=NULL) BottomMsg=strdup(Bottom); else BottomMsg=strdup("");
}


CDesktop::~CDesktop()
{
  if (TopMsg!=NULL) free(TopMsg);
  if (BottomMsg!=NULL) free(BottomMsg);
}


void CDesktop::Draw()
{
  UpdateCursor();
  DrawBackground();
  DrawTop();
  DrawBottom();
}


void CDesktop::DrawBackground()
{
  char Col=Color(0);

  for (int y=1; y<sy-1; y++)
    for (int x=0; x<sx; x++) PutChar(px+x,py+y,'°', Col);
}


void CDesktop::DrawTop()
{
  char Col=Color(1);

  char* Msg=TopMsg;
  int x;
  for (x = 0; x<sx; x++) PutChar(px+x, py, ' ', Col);
  for (x=40-strlen(Msg)/2;
       x<sx && *Msg; x++, Msg++) PutChar(px+x, py, *Msg, Col);
}


void CDesktop::DrawBottom()
{
  char Col=Color(2);

  char* Msg=BottomMsg;
  int x;
  for (x = 0; x<sx; x++) PutChar(px+x, py+sy-1, ' ', Col);
  for (x=40-strlen(Msg)/2;
       x<sx && *Msg; x++, Msg++) PutChar(px+x, py+sy-1, *Msg, Col);
}


char CDesktop::ColorIndex(char Num)
{
  return Num+ColDesktop;
}



//================================================


CWindow::CWindow(int apx, int apy, int asx, int asy)
 : CView(apx, apy, asx, asy)
{
  Title=NULL;
  Border=strdup(dblBorder);
}


CWindow::~CWindow()
{
  if (Title!=NULL) free(Title);
  if (Border!=NULL) free(Border);
}


void CWindow::SetBorder(char* aBorder)
{
  if (Border!=NULL) free(Border);
  if (aBorder!=NULL) Border=strdup(aBorder); else Border=strdup(dblBorder);
}

void CWindow::SetTitle(char* aTitle)
{
  if (Title!=NULL) free(Title);

  if (aTitle!=NULL) Title=strdup(aTitle); else Title=NULL;
}


void CWindow::Draw()
{
  UpdateCursor();
  DrawBorder();
  DrawContent();
}


void CWindow::DrawBorder()
{
  if (Title!=NULL)
  {
    int Len=strlen(Title);

    char Col=Color(0);
    int x;
    for (x=1; x<sx-2 && x<(sx-Len-2)/2; x++) PutChar(px+x, py, *(Border+1), Col);
    PutChar(px, py, *(Border+2), Col);
    PutChar(px+sx-1, py, *(Border+3), Col);

    Col=Color(1);
    PutChar(px+x, py, ' ', Col);
    x++;

    char* Msg=Title;
    for (; x<sx-1 && *Msg; x++,Msg++) PutChar(px+x,py,*Msg, Col);

    PutChar(px+x, py, ' ', Col);
    x++;

    Col=Color(0);
    for (; x<sx-1; x++) PutChar(px+x,py,*(Border+1), Col);
  } else
  {
    char Col=Color(0);
    for (int x=1; x<sx-1; x++) PutChar(px+x, py, *(Border+1), Col);
    PutChar(px, py, *(Border+2), Col);
    PutChar(px+sx-1, py, *(Border+3), Col);
  }

  char Col=Color(0);
  for (int x=1; x<sx-1; x++)
  {
    PutChar(px+x, py+sy-1, *(Border+1), Col);
  }

  for (int y=1; y<sy-1; y++)
  {
    PutChar(px, py+y, *Border, Col);
    PutChar(px+sx-1, py+y, *Border, Col);
  }

  PutChar(px, py+sy-1, *(Border+4), Col);
  PutChar(px+sx-1, py+sy-1, *(Border+5), Col);
}


void CWindow::DrawContent()
{
  char Col=Color(2);

  for (int y=1; y<sy-1; y++)
    for (int x=1; x<sx-1; x++)
      PutChar(px+x,py+y,' ',Col);
}


char CWindow::ColorIndex(char Num)
{
  return ColWindow+Num;
}


//===================================================


CMessage::CMessage(char* Msg, ...)
 : CWindow(0,0,1,1)
{
  if (Msg!=NULL)
  {
    va_list arg;
    va_start(arg, Msg);

    Message=(char*)malloc(1024);

    vsprintf(Message, Msg, arg);

    va_end(arg);
  } else Message=NULL;


  CheckMessage();
  SizeTo(Cols+4, Lines+2);
  MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
}


CMessage::CMessage(char* Msg, va_list arg)
 : CWindow(0,0,1,1)
{
  if (Msg!=NULL)
  {
    Message=(char*)malloc(1024);

    vsprintf(Message, Msg, arg);
  } else Message=NULL;


  CheckMessage();
  SizeTo(Cols+4, Lines+2);
  MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
}


CMessage::CMessage(int apx, int apy, int asx, int asy, char* Msg, ...)
 : CWindow(apx, apy, asx, asy)
{
  if (Msg!=NULL)
  {
    va_list arg;
    va_start(arg, Msg);

    Message=(char*)malloc(512);

    vsprintf(Message, Msg, arg);

    va_end(arg);

    CheckMessage();
  } else Message=NULL;
}


CMessage::CMessage(int asx, int asy, char* Msg, ...)
 : CWindow(0, 0, asx, asy)
{
  if (Msg!=NULL)
  {
    va_list arg;
    va_start(arg, Msg);

    Message=(char*)malloc(256);

    vsprintf(Message, Msg, arg);

    va_end(arg);
  } else Message=NULL;

  CheckMessage();
  MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
}


CMessage::~CMessage()
{
  if (Message!=NULL) free(Message);
}


void CMessage::DrawContent()
{
  int x=2;
  int y=1;
  char* Msg=Message;

  char Col=Color(2);
  while (y<sy-1 && *Msg)
  {
    if (*Msg>31 || *Msg<0)
    {
      PutChar(px+x,py+y,*Msg, Col);
      x++;
    } else
    {
      switch(*Msg)
      {
	case '\n': for (; x<sx-2; x++) PutChar(px+x,py+y,' ', Col);
		   x=2; y++; break;
  	case '\r': x=2; break;
      }
    }

    Msg++;

    if (x>=sx-2 && *Msg!='\r' && *Msg!='\n')
    {
      x=2;
      y++;
    }
  }

  if (y<sy-1)
  {
    for (; x<sx-2; x++) PutChar(px+x,py+y,' ', Col);
    y++;

    for (; y<sy-1; y++)
      for (x=2; x<sx-2; x++)
	PutChar(px+x,py+y,' ',Col);
  }

  for (y=1; y<sy-1; y++)
  {
    PutChar(px+1, py+y, ' ', Col);
    PutChar(px+sx-2, py+y, ' ', Col);
  }
}


void CMessage::CheckMessage()
{

  if (Message==NULL) return;

  Lines=1;
  Cols=0;
  char* Msg=Message;
  int ActCol=0;

  while (*Msg)
  {
    if (*Msg=='\n' || ActCol>=GetMaxX()-4)
    {
      Lines++;
      ActCol=0;
    } else ActCol++;
    Msg++;

    if (ActCol>Cols) Cols=ActCol;
  }
}


void CMessage::Action()
{
  getch();
}


char CMessage::ColorIndex(char Num)
{
  return ColMessage+Num;
}


//===============================================


CList::CList(int aMaxHeight)
 : CWindow(0,0,10,2)
{
  MaxLen=0;
  Num=0;
  Pos=0;
  Center=0;
  MaxHeight=aMaxHeight;
}


CList::~CList()
{
  for (int n=0; n<Num; n++) if (Items[n]!=NULL) free(Items[n]);
}


void CList::InsertItem(char* Msg, ...)
{
  if (Msg!=NULL)
  {
    va_list arg;
    va_start(arg, Msg);

    Items[Num]=(char*)malloc(256);
    vsprintf(Items[Num], Msg, arg);
    if (strlen(Items[Num])>MaxLen) MaxLen=strlen(Items[Num]);

    va_end(arg);

    Num++;

    if (Num+2<=MaxHeight) SizeTo(MaxLen+6, Num+2); else SizeTo(MaxLen+6, sy);
    MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
  }
}


void CList::InsertLine()
{
  Items[Num]=NULL;
  Num++;
  if (Num+2<=MaxHeight) SizeTo(MaxLen+6, Num+2); else SizeTo(MaxLen+6, sy);
  MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
}


void CList::ScrollTo(int aPos)
{
  Pos = aPos;
  if (Pos+sy-3>=Num) Pos=Num-sy+2;
  if (Pos<0) Pos=0;
  Draw();
}


void CList::Scroll(int aPos)
{
  ScrollTo(Pos+aPos);
}


void CList::DrawContent()
{
  char Col=Color(2);
  int Line;
  for (Line=0; Line<sy-2 && Line+Pos<Num; Line++)
  {
    PutChar(px+1, py+Line+1, ' ', Col);
    PutChar(px+2, py+Line+1, ' ', Col);
    PutChar(px+sx-2, py+Line+1, ' ', Col);
    PutChar(px+sx-3, py+Line+1, ' ', Col);

    char* Msg=Items[Line+Pos];

    int x=3;
    if (Msg!=NULL)
    {
      int Len=strlen(Msg);
      if (Center==1) for (; x<(sx-Len)/2; x++) PutChar(px+x,py+Line+1,' ', Col);
      for (; x<sx-3 && *Msg && *Msg>=32; x++, Msg++)
	      PutChar(px+x,py+Line+1,*Msg,Col);
    }
    for (; x<sx-3; x++) PutChar(px+x,py+Line+1,' ', Col);
  }

  for (; Line<sy-2; Line++)
  {
    for (int x=1; x<sx-2; x++) PutChar(px+x,py+Line+1,' ', Col);
  }
}


char CList::ColorIndex(char Num)
{
  return ColList+Num;
}


void CList::SetTitle(char* aTitle)
{
  CWindow::SetTitle(aTitle);

  if (Title!=NULL)
  {
    if (MaxLen<strlen(Title))
    {
      MaxLen=strlen(Title);
      SizeTo(MaxLen+6, Num+2);
      MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
    }
  }
}


void CList::SetCenter(int On)
{
  Center=On;
}


//===============================================


CMenu::CMenu(int aWarping, int aMaxHeight)
 : CList(aMaxHeight)
{
  Selected=0;
  Escaped=0;
  Warping=aWarping;
}


CMenu::~CMenu()
{
}


int CMenu::GetSelected()
{
  if (Escaped) return 99; else
    return Selected;
}


void CMenu::DrawContent()
{
  char Col=Color(2);
  int Line;
  for (Line=0; Line+Pos<Selected && Line<sy-2; Line++)
  {
    PutChar(px+1, py+Line+1, ' ', Col);
    PutChar(px+2, py+Line+1, ' ', Col);
    PutChar(px+sx-2, py+Line+1, ' ', Col);
    PutChar(px+sx-3, py+Line+1, ' ', Col);

    char* Msg=Items[Line+Pos];
    int x=3;
    if (Msg!=NULL)
    {
      int Len=strlen(Msg);
      if (Center==1) for (; x<(sx-Len)/2; x++) PutChar(px+x,py+Line+1,' ', Col);
      for (; x<sx-3 && *Msg && *Msg>=32; x++, Msg++)
      	PutChar(px+x,py+Line+1,*Msg,Col);
    }
    for (; x<sx-3; x++) PutChar(px+x,py+Line+1,' ', Col);
  }

  if (Line+Pos<Num && Line<sy-2)
  {
    PutChar(px+1, py+Line+1, ' ', Col);
    PutChar(px+sx-2, py+Line+1, ' ', Col);
    PutChar(px+2, py+Line+1, ' ', Color(3));
    PutChar(px+sx-3, py+Line+1, ' ', Color(3));

    char* Msg=Items[Line+Pos];
    int x=3;
    if (Msg!=NULL)
    {
      int Len=strlen(Msg);
      if (Center==1) for (; x<(sx-Len)/2; x++) PutChar(px+x,py+Line+1,' ', Color(3));

      for (; x<sx-3 && *Msg && *Msg>=32; x++, Msg++)
      	PutChar(px+x,py+Line+1,*Msg,Color(3));
    }
    for (; x<sx-3; x++) PutChar(px+x,py+Line+1,' ', Color(3));

    Line++;
  }

  for (; Line+Pos<Num && Line<sy-2; Line++)
  {
    PutChar(px+1, py+Line+1, ' ', Col);
    PutChar(px+2, py+Line+1, ' ', Col);
    PutChar(px+sx-2, py+Line+1, ' ', Col);
    PutChar(px+sx-3, py+Line+1, ' ', Col);

    char* Msg=Items[Line+Pos];
    int x=3;
    if (Msg!=NULL)
    {
      int Len=strlen(Msg);
      if (Center==1) for (; x<(sx-Len)/2; x++) PutChar(px+x,py+Line+1,' ', Col);
      for (; x<sx-3 && *Msg && *Msg>=32; x++, Msg++)
      	PutChar(px+x,py+Line+1,*Msg,Col);
    }
    for (; x<sx-3; x++) PutChar(px+x,py+Line+1,' ', Col);
  }

  for (; Line<sy-2; Line++)
  {
    for (int x=1; x<sx-2; x++) PutChar(px+x,py+Line+1,' ', Col);
  }

  if (Pos>0) PutChar(px+sx-1, py+1, 30, Col);
  if (Pos+sy-2<Num) PutChar(px+sx-1, py+sy-2, 31, Col);
}


void CMenu::SetSelected(int New)
{
  if (New<Num && Items[New]!=NULL)
  {
    Selected=New;

    if (Selected<Pos) ScrollTo(Selected); else
    if (Selected>=Pos+sy-3) ScrollTo(Selected-sy+3); else Draw();
  }
}


void CMenu::Up()
{
  int New=Selected;
  if (Warping && New==0) New = Num;
  while (New>0)
  {
    New--;
    if (Items[New]!=NULL) break;
  }

  SetSelected(New);
}


void CMenu::Down()
{
  int New=Selected;
  if(Warping && New == Num-1) New = -1;
  while (New<Num-1)
  {
    New++;
    if (Items[New]!=NULL) break;
  }

  if (Items[New]!=NULL) Selected=New;

  if (Selected<Pos) ScrollTo(Selected); else
  if (Selected>=Pos+sy-3) ScrollTo(Selected-sy+3); else Draw();
}


void CMenu::Action()
{
  Escaped=0;
  int Quit=0;

  while (!Quit)
  {
    char c=getch();

    if (c==0)
    {
      c=getch();
      switch(c)
      {
       case 72: Up(); break;
       case 80: Down(); break;
      }
    } else
    {
      switch(c)
      {
       case 27: Escaped=1;
		Quit=1;
		break;
       case 13: Quit=1;
		break;
      }
    }
  }
}


char* CMenu::GetText()
{
  if (Escaped) return ""; else
    return Items[Selected];
}


char CMenu::ColorIndex(char Num)
{
  return ColMenu+Num;
}


//================================================


CEdit::CEdit(char* Msg, char* Default, int aMaxLen, char* aKeys, int aSecret)
 : CWindow(0,0,10,10)
{
  MaxLen=aMaxLen;
  Input=(char*)malloc(MaxLen+1);
  Secret=aSecret;

  if (Msg!=NULL) Message=strdup(Msg); else Message=strdup("");
  if (Default!=NULL)
  {
    strncpy(Input, Default, MaxLen);
    *(Input+MaxLen)=0;
  } else strcpy(Input,"");
  if (aKeys!=NULL) Keys=strdup(aKeys); else Keys=NULL;

  Pos=0;

  MsgLen=strlen(Message);
  SizeTo(MsgLen+MaxLen+4, 3);
  MoveTo((GetMaxX()-sx)/2, (GetMaxY()-sy)/2);
}


CEdit::~CEdit()
{
  if (Message!=NULL) free(Message);
  if (Input!=NULL) free(Input);
  if (Keys!=NULL) free(Keys);
}


char* CEdit::GetInput()
{
  return Input;
}


void CEdit::DrawContent()
{
  char Col=Color(2);

  PutChar(px+1, py+1, ' ', Col);
  PutChar(px+sx-2, py+1, ' ', Col);
  PutString(px+2, py+1, Message, Col);

  Col=Color(3);
  char* Msg=Input;
  int x;
  if (Secret)
  {
    for (x=0; x<MaxLen && *Msg; x++, Msg++)
      PutChar(px+MsgLen+2+x,py+1,'*',Col);
  } else
  {
    for (x=0; x<MaxLen && *Msg; x++, Msg++)
      PutChar(px+MsgLen+2+x,py+1,*Msg,Col);
  }

  for (;x<MaxLen;x++) PutChar(px+MsgLen+2+x,py+1,' ',Col);
}


void CEdit::Action()
{
  int Quit=0;
  while (!Quit)
  {
    char c=getch();
    if (c==0)
    {
      c=getch();
      switch(c)
      {
	case 75: if (Pos>0) Pos--; Draw(); break;
	case 77: if (Pos<MaxLen-1) Pos++; Draw(); break;
	case 83: Delete(); Draw(); break;
	case 71: Pos=0; Draw(); break;
	case 79: Pos=strlen(Input); Draw(); break;
      }
    } else
    {
      switch(c)
      {
	case 13: Quit=1; break;
	case 8:  BackSpace(); Draw(); break;
	case 27: strcpy(Input, ""); Quit=1; break;
	default: if (c>31)
		 {
		   Insert(c); Draw();
		 }
		 break;
      }
    }
  }
}


void CEdit::Insert(char c)
{
  if (Keys!=NULL)
  {
    if (strchr(Keys, c)==NULL) return;
  }

  int Len=strlen(Input);
  if (Len<MaxLen)
  {
    if (Pos>=Len)
    {
      for (; Len<Pos; Len++) *(Input+Len)=' ';
      *(Input+Pos+1)=0;
      *(Input+Pos)=c;
    } else
    {
      for (char* t=Input+Len; t>Input+Pos; t--) *t=*(t-1);
      *(Input+Pos)=c;
    }

    if (Pos<MaxLen-1) Pos++;
  }
}


void CEdit::BackSpace()
{
  if (Pos>0)
  {
    if (Pos-1<strlen(Input)) strcpy(Input+Pos-1, Input+Pos);
    Pos--;
  }
}


void CEdit::Delete()
{
  if (Pos<strlen(Input))
  {
    if (Pos==strlen(Input)-1) *(Input+Pos)=0; else
      strcpy(Input+Pos, Input+Pos+1);
  }
}


void CEdit::UpdateCursor()
{
  PutCursor(px+MsgLen+2+Pos, py+1);
}


char CEdit::ColorIndex(char Num)
{
  return ColEdit+Num;
}


//==================================================


char Message(char* Msg, ...)
{
  va_list arg;
  va_start(arg, Msg);

  CMessage* Box=new CMessage(Msg, arg);
  Box->Show();
  char c=getch();
  Box->Hide();
  delete Box;

  va_end(arg);

  return c;
}


char TitleMessage(char* Title, char* Msg, ...)
{
  va_list arg;
  va_start(arg, Msg);

  CMessage* Box=new CMessage(Msg, arg);
  Box->SetTitle(Title);
  Box->Show();
  char c=getch();
  Box->Hide();
  delete Box;

  va_end(arg);

  return c;
}


//==================================================


CCrunchWindow::CCrunchWindow(int apx, int apy, int asx, int asy,
                             unsigned char* aCrunch, int aLen)
 : CWindow(apx, apy, asx, asy)
{
  Crunch=(char*)aCrunch;
  Length=aLen;
}


CCrunchWindow::~CCrunchWindow()
{
}


void CCrunchWindow::DrawContent()
{
int y;
  for (y=1; y<sy-1; y++)
    for (int x=1; x<sx-1; x++)
      PutChar(px+x,py+y,' ',0x07);

  /*
   Data Structure:  <current byte>[<x>[<y>]]

    0..15 = New Foreground Color
   16..23 = New Background Color
       24 = Go down to next line, return to same horizontal position as when
            routine was started (akin to a c/r).
       25 = Displays <x> number of spaces.
       26 = Displays <x> number of <y>.  Also used to display ANY characters
            below #32.  This function is the only way to do this although it
            uses three bytes.  Otherwise the code would be interpreted as
            another command.
       27 = Toggles on/off the foreground attribute blink flag.
   28..31 = reserved
  */

  char f=7; // Foreground
  char b=0; // Background
  int z=0; // Temporary (for do)
  int x=0;
  y=0;

  for (int p=0; p<Length; p++)
  {
    unsigned char c=*(Crunch+p);

    if (c>=32)
    {
      if (x<sx-2 && y<sy-2) PutChar(px+1+x, py+1+y, c, f+b*16);
      x++;
      if (x>=80) x=0;
    } else
    if (c<16)
    {
      f=c;
    } else
    if (c<24)
    {
      b=(b & 0x08)+c-16;
    } else
    switch (c)
    {
     case 24: x=0; y++; break;
     case 25: p++;
              for (z=*(Crunch+p)+1; z>0; z--)
              {
                if (x<sx-2 && y<sy-2) PutChar(px+1+x, py+1+y, ' ', f+b*16);
                x++;
                if (x>=80) x=0;
              }
              break;
     case 26: p++;
              z=*(Crunch+p)+1;
              p++;
              c=*(Crunch+p);
              for (; z>0; z--)
              {
                if (x<sx-2 && y<sy-2) PutChar(px+1+x, py+1+y, c, f+b*16);
                x++;
                if (x>=80) x=0;
              }
              break;
     case 27: b^=0x08; break;
     default: p++;
    }
  }
}


char CCrunchWindow::ColorIndex(char Num)
{
  return ColCrunchWin+Num;
}


//======================================================


CBar::CBar(int apx, int apy, int asx, int aMin, int aMax, int aValue)
 : CWindow(apx, apy, asx, 3)
{
  Min=aMin;
  Max=aMax;
  Value=aValue;

  if (Value>Max) Value=Max;
  if (Value<Min) Value=Min;
}


CBar::~CBar()
{
}


void CBar::Update(int aValue)
{
  Value=aValue;
  if (Value>Max) Value=Max;
  if (Value<Min) Value=Min;

  DrawContent();
}


void CBar::Update()
{
  Update(Value);
}


void CBar::DrawContent()
{
  int Len=0;
  if (Max-Min>0) Len=(int)((float)(sx-4)*(Value-Min)/(Max-Min)+0.5);

  char Col=Color(4);
  int x;
  for (x=0; x<Len; x++)
  {
    PutChar(px+2+x, py+1, 'Û', Col);
  }

  if (x-Len>0.5)
  {
    PutChar(px+2+x, py+1, 'Ý', Col);
    x++;
  }

  Col=Color(3);
  for (; x<sx-4; x++)
  {
    PutChar(px+2+x, py+1, ' ', Col);
  }

  PutChar(px+1, py+1, ' ', Color(2));
  PutChar(px+sx-2, py+1, ' ', Color(2));
}


char CBar::ColorIndex(char Num)
{
  return ColBar+Num;
}

//============================================================


CSave::CSave()
 : CView(0,0,GetMaxX(),GetMaxY())
{
  Show();
}


CSave::~CSave()
{
  Hide();
}
