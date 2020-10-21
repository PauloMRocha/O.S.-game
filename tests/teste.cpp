#include <ncurses.h>
#include <locale.h>
#define LAR 30
#define ALT 15

void print_map(){
  clear();
  move(0,0);
  for(int i=0; i<LAR+2; i++){
    addch('#');
  }
  for(int i=1; i<=ALT; i++){
    mvaddch(i, 0, '#');
    mvaddch(i, LAR+1, '#');
  }
  move(ALT+1,0);
  for(int i=0; i<LAR+2; i++){
    addch('#');
  }
  refresh();
}

void mov_player(int &x, int &y, char dir){
  mvaddch(y, x, ' '); //erase last position;
  switch (dir) {
    case 'u':
      if(y<=1) break;
      y--;
      break;
    case 'd':
      if(y>=ALT) break;
      y++;
      break;
    case 'l':
      if(x<=1) break;
      x--;
      break;
    case 'r':
      if(x>=LAR) break;
      x++;
      break;
    default:
      break;
  }
  mvaddstr(y, x, "\u2620");
  refresh();
}

int main()
{
  int ch, op=0;
  int x=15, y=7;
  setlocale(LC_ALL, "");
  initscr();                 /* Start curses mode     */
  cbreak();
  printw("Hello World !!!\n"); /* Print Hello World    */
  refresh();                 /* Print it on to the real screen */
  getch();
  curs_set(0);
  noecho();
  print_map();
  mov_player(x, y, ' ');
  keypad(stdscr, TRUE);
  while(op==0){
    ch = getch();
    switch(ch){
      case KEY_UP:
        mov_player(x, y, 'u');
        break;
      case KEY_DOWN:
        mov_player(x, y, 'd');
        break;
      case KEY_RIGHT:
        mov_player(x, y, 'r');
        break;
      case KEY_LEFT:
        mov_player(x, y, 'l');
        break;
      case 27:
        op = 1;
        break;
      default:
      break;
    }
  }
  endwin();                  /* End curses mode    */
  return 0;
}
