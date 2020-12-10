#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <pthread.h>
#include <semaphore.h>
#include <bits/stdc++.h>
#define LAR 30
#define ALT 15
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define WALL_COLOR 1
#define ENEMY_COLOR 2
#define PLAYER_COLOR 3
#define TIMER_COLOR 4

using namespace std;

// global
int ch, op = 0;
int directions[4] = {0,1,2,3};
int stop = 1;
pair<int,int> p_coord;
vector<pair<int,int>> e_coord;

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
  attroff(COLOR_PAIR(WALL_COLOR));
  refresh();
  return;
}

void mov_player(int dir) {
  delete_dir(p_coord.second, p_coord.first);
  mvaddch(p_coord.second, p_coord.first, ' '); //erase last position;

  if(dir == directions[0]){//DOWN
    if (p_coord.second > 1)
      p_coord.second--;
  }else if(dir == directions[1]){//UP
    if(p_coord.second < ALT)
      p_coord.second++;
  }else if(dir == directions[2]){//LEFT
    if(p_coord.first > 1)
      p_coord.first--;
  }else if(dir == directions[3]){//RIGHT
    if(p_coord.first < LAR)
      p_coord.first++;
  }
//☬◈☫
  attron(COLOR_PAIR(PLAYER_COLOR));
  mvaddstr(p_coord.second, p_coord.first, "◈");
  attroff(COLOR_PAIR(PLAYER_COLOR));
  rand_dir();
  print_dir(p_coord.second, p_coord.first);
  refresh();
  return;
}

void mov_enemy(){
  //int dir_e = rand()%4;
  //mvaddch(e_coord[0].second, e_coord[0].first, ' '); //erase last position;
  e_coord[0].second ++;
  // if(dir_e == 0){//DOWN
  //   if (e_coord[0].second > 1)
  //     e_coord[0].second--;
  // }else if(dir_e == 1){//UP
  //   if(e_coord[0].second < ALT)
  //     e_coord[0].second++;
  // }else if(dir_e == 2){//LEFT
  //   if(e_coord[0].first > 1)
  //     e_coord[0].first--;
  // }else if(dir_e == 3){//RIGHT
  //   if(e_coord[0].first < LAR)
  //     e_coord[0].first++;
  // }

  attron(COLOR_PAIR(ENEMY_COLOR));
  mvaddstr(e_coord[0].second, e_coord[0].first, "☫");
  attroff(COLOR_PAIR(ENEMY_COLOR));
  refresh();
}

void* player_thread(void* var){
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
  stop = 0;
  pthread_exit(NULL);
}

void* timer_thread(void* var){
  int sec=0, min=0;
  while(stop){
    attron(COLOR_PAIR(TIMER_COLOR));
    move(0, 3);
    printw("TIME - %02d:%02d", min, sec);
    refresh();
    attroff(COLOR_PAIR(TIMER_COLOR));
    sec++;
    if(sec == 60){
      sec = 0;
      min++;
    }
    sleep(1);
  }
  pthread_exit(NULL);
}

void* enemy_thread(void* var){
  pair<int,int> ep;
  ep.first = 5; ep.second = 8;
  e_coord.push_back(ep);
  while(stop){
    mov_enemy();
    sleep(2);
  }
  pthread_exit(NULL);
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
  init_pair(WALL_COLOR, COLOR_YELLOW, COLOR_YELLOW);//wall
  init_pair(ENEMY_COLOR, COLOR_RED, COLOR_BLACK);
  init_pair(PLAYER_COLOR, COLOR_GREEN, COLOR_BLACK);//player
  init_pair(TIMER_COLOR, COLOR_WHITE, COLOR_BLACK);//timer
  //stops buffer
  cbreak();
  printw("Press any key to start\n"); /* Print init message   */
  refresh();
  getch();
  curs_set(0);
  noecho();

  p_coord.first = 15; p_coord.second = 7;
  print_map();
  mov_player(-1);

  //activate key code
  keypad(stdscr, TRUE);

  pthread_t timer;
  pthread_t player_t;
  pthread_t enemy_t;
  void *status_timer;
  void *status_player;
  void *status_enemy;

  pthread_create(&timer, NULL, timer_thread, NULL);
  pthread_create(&player_t, NULL, player_thread, NULL);
  pthread_create(&enemy_t, NULL, enemy_thread, NULL);
  pthread_join(timer, &status_timer);
  pthread_join(enemy_t, &status_enemy);
  pthread_join(player_t, &status_player);

  //pthread_exit(NULL);
  endwin();                  /* End curses mode    */
  return 0;
}
