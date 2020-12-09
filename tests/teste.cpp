#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#define LAR 30
#define ALT 15
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define PLAYER_COLOR 2
#define WALL_COLOR 1
// global
int ch, op = 0;
int x = 15, y = 7;
int directions[4] = {0,1,2,3};


void rand_dir(){
  int values[4] = {0};
  int aux = rand()%4;
  //the fist rand
  values[aux] = 1;
  directions[0] = aux;
  //the two next
  for(int i=1; i<3; i++){
    while(values[aux] == 1){
      aux = rand()%4;
    }
    values[aux] = 1;
    directions[i] = aux;
  }
  //the last one
  for(int i=0; i<4; i++){
    if(values[i] == 0){
      values[i] = 1;
      directions[3] = i;
    }
  }
  return;
}

void print_arrow(int x, int y, int dir){
  if(dir == UP){
    mvaddstr(x, y, "↑");
  }else if(dir == DOWN){
    mvaddstr(x, y, "↓");
  }else if(dir == LEFT){
    mvaddstr(x, y, "←");
  }else{
    mvaddstr(x, y, "→");
  }
}

void print_dir(int x, int y){
  //up side
  if((x-1) > 0){
    print_arrow((x-1), y, directions[0]);
  }
  //down side
  if((x+1) <= ALT){
    print_arrow((x+1), y, directions[1]);
  }
  //up left
  if((y-1) > 0){
    print_arrow(x, (y-1), directions[2]);
  }
  //down side
  if((y+1) <= LAR){
    print_arrow(x, (y+1), directions[3]);
  }
}

void delete_dir(int x, int y){
  //up side
  if((x-1) > 0){
    mvaddch((x-1), y, ' ');
  }
  //down side
  if((x+1) <= ALT){
    mvaddch((x+1), y, ' ');
  }
  //up left
  if((y-1) > 0){
    mvaddch(x, (y-1), ' ');
  }
  //down side
  if((y+1) <= LAR){
    mvaddch(x, (y+1), ' ');
  }
}

void print_map(){
  clear();
  move(0,0);
  attron(COLOR_PAIR(WALL_COLOR));
  for(int i = 0; i < LAR+2; i++) addch('#');

  for(int i = 1; i <= ALT; i++){
    mvaddch(i, 0, '#');
    mvaddch(i, LAR+1, '#');
  }

  move(ALT+1, 0);

  for(int i = 0; i < LAR+2; i++) addch('#');
  attroff(COLOR_PAIR(1));
  refresh();
  return;
}

void mov_player (int dir) {
  delete_dir(y, x);
  mvaddch(y, x, ' '); //erase last position;

  if(dir == directions[0]){
    if (y > 1)
      y--;
  }else if(dir == directions[1]){
    if(y < ALT)
      y++;
  }else if(dir == directions[2]){
    if(x > 1)
      x--;
  }else if(dir == directions[3]){
    if(x < LAR)
      x++;
  }
//☬◈☫
  attron(COLOR_PAIR(2));
  mvaddstr(y, x, "◈");
  attroff(COLOR_PAIR(2));
  rand_dir();
  print_dir(y, x);
  refresh();
  return;
}

int main (void) {
  //setlocale(LC_CTYPE,"C-UTF-8");
  setlocale (LC_ALL, "");
  //RANDOM SEED
  srand(time(NULL));

  initscr();   /* Start curses mode     */
  //COLORS
  start_color();
  //        id            font color   bg color
  init_pair(PLAYER_COLOR, COLOR_GREEN, COLOR_BLACK);//player
  init_pair(WALL_COLOR, COLOR_YELLOW, COLOR_YELLOW);//wall

  //stops buffer
  cbreak();
  printw("Press any key to start\n"); /* Print init message   */
  refresh();
  getch();
  curs_set(0);
  noecho();
  print_map();
  mov_player(' ');

  //activate key code
  keypad(stdscr, TRUE);



  while(op == 0) {
    ch = getch();

    switch(ch) {
      case KEY_UP:
        mov_player(UP);
        break;

      case KEY_DOWN:
        mov_player(DOWN);
        break;

      case KEY_RIGHT:
        mov_player(RIGHT);
        break;

      case KEY_LEFT:
        mov_player(LEFT);
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
