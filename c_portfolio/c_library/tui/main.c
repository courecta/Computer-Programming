#include <stdio.h>
#include <ncurses.h>

int main(){

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("I'm gonna touch you lil bro\n");
  // refresh();
  char ch = getch();
  
while(ch != 'q' || ch != 'Q')
  {
    printw("whyd you press ");
    attron(A_BOLD);
    printw("%c", ch);
    attroff(A_BOLD);
    ch = getch();
  }

  refresh();
  getch();
  endwin();

  return 0;
}
