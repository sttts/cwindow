/*

DEMO.CC

(c) by Stefan Schimanski '1997

*/


#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <cwindow.h>


int langidx=0;

//===========================================

char lngstr[2][10][300] =
     { { "Welcome to this beautiful demo...",
         "Please choose a menu entry.",
         "This is a message box, which can handle parameters:\n"
         "%u %s"
         "\nIt can handle linefeeds, too.",
         "Please enter your name: ",
         "How old are you? ",
         "%s is %u years old.",
         "List item 1",
         "List item 2",
         "This is a very long title",
         "Byebye..."
       },
       { "Willkommen zu dieser wunderschînen Demonstration...",
         "Bitte wÑhlen Sie einen MenÅ-Eintrag aus.",
         "Dies ist ein Nachrichten-Fenster, das Parameter unterstÅtzt:\n"
         "%u %s"
         "\nEs kînnen sogar neue Zeilen beschrieben werden.",
         "Bitte geben Sie Ihren Namen ein: ",
         "Wie alt sind Sie? ",
         "%s ist %u Jahre alt.",
         "Listen-Eintrag 1",
         "Listen-Eintrag 2",
         "Dies ist ein sehr langer Titel",
         "TschÅss..."
     } };


char *GetString(int Num)
{
  return (lngstr[langidx][Num]);
}


//===========================================


void Window()
{
  CWindow Win(5, 5, 70, 18);
  Win.Show();
  getch();
  Win.Hide();
}


void Edit()
{
  CEdit EditName(GetString(3), "Stefan", 20);
  EditName.Run();
  char* Name=EditName.GetInput();

  CEdit EditAlter(GetString(4), "999", 3, "0123456789");
  EditAlter.Run();
  int Alter=atoi(EditAlter.GetInput());

  Message(GetString(5), Name, Alter);
}


#include "copy.scr"

void Ansi()
{
  CCrunchWindow Win(0,5,80,14,COPYRIGHT,COPYRIGHT_LENGTH);
  Win.Show();
  getch();
  Win.Hide();
}


void Bar()
{
  CBar Bar(15, 11, 50, 0, 100, 0);
  Bar.Show();

  for(int x=0; x<=100; x+=10)
  {
    Bar.Update(x);
    getch();
  }

  Bar.Hide();
}


void List()
{
  CList List;

  List.InsertItem(GetString(6));
  List.InsertItem(GetString(7));
  List.SetTitle(GetString(8));

  List.Show();
  getch();
  List.Hide();
}


void LongMenu()
{
  CMenu Menu(0, 10);

  char s[3]=" ";
  for (char c='A'; c<='Z'; c++)
  {
    s[0]=c;
    Menu.InsertItem(s);
  }

  Menu.Show();
  Menu.Action();
  Menu.Hide();
}


void AskLanguage()
{
  CMenu *Language=new CMenu;
  Language->InsertItem("Englisch/English");
  Language->InsertItem("Deutsch/German");
  Language->Show();
  Language->Action();
  langidx = Language->GetSelected();
  if (langidx==99) langidx=0;
  Language->Hide();
  delete Language;
}


main()
{
  CDesktop Back("CWindow-Test v" CWINDOW_VERSIONSTR,
                "(c) 1997 by S. Schimanski & J. Heilgeist");
  Back.Show();

  AskLanguage();

  Message(GetString(0));

  CMessage* Info=new CMessage(14,3,51,3,
                              lngstr[langidx][1]);
  Info->Show();

  CMenu Menu;
  Menu.InsertItem("CWindow");
  Menu.InsertItem("CMessage");
  Menu.InsertItem("CEdit");
  Menu.InsertItem("CCrunchWindow");
  Menu.InsertItem("CBar");
  Menu.InsertItem("CList");
  Menu.InsertItem("Long CMenu");
  Menu.InsertLine();
  Menu.InsertItem("Close");
  Menu.Show();

  int Quit=0;
  while(!Quit)
  {
    Menu.Action();

    switch(Menu.GetSelected())
    {
     case 0: Window(); break;
     case 1: Message(GetString(2), 12345, "abcde"); break;
     case 2: Edit(); break;
     case 3: Ansi(); break;
     case 4: Bar(); break;
     case 5: List(); break;
     case 6: LongMenu(); break;
     case 8: Quit=1; break;
    }
  }

  Menu.Hide();

  Info->Hide();
  delete Info;

  Message(lngstr[langidx][9]);

  Back.Hide();
}
