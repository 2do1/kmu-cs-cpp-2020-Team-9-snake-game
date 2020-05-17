#include <ncurses.h>
#include <iostream>
using namespace std;

int xo,yo;
WINDOW *win1;
WINDOW *win2;

void setup();
void printmap();
void keyinput();


int main(){

  setup();
    while(1){
      printmap();
      keyinput();
    }

  getch();
  delwin(win1);
  delwin(win2);
  endwin();

  return 0;

}

void setup(){


  initscr();
  resize_term(50,50);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);

  border('|','|','-','-','+','+','+','+');
  mvprintw(2,15,"snake game");
  refresh();
  getch();

  win1 = newwin(32,32,3,3);
  wbkgd(win1, COLOR_PAIR(1));
  wattron(win1, COLOR_PAIR(1));
  mvwprintw(win1,1,1,"a");
  wborder(win1,'|','|','-','-','+','+','+','+');
  wrefresh(win1);

  win2 = newwin(5,10,3,35);
  wbkgd(win2, COLOR_PAIR(1));
  wattron(win2, COLOR_PAIR(1));
  mvwprintw(win2,1,1,"a");
  wborder(win2,'|','|','-','-','+','+','+','+');
  wrefresh(win2);


    keypad(stdscr,TRUE);
    noecho();
    curs_set(0);

}

void printmap(){
  int x = 30;
  int y = 30;
  xo = 15;
  yo = 15;
  int map[x][y]={0,};   // 우선 map 의 모든 값을 0으로 설정한다.


  for(int i=0;i<y;i++){
    for(int j=0;j<x;j++){

      if (i==yo && j==xo){
        mvwprintw(win1,yo,xo,"O"); //snake!
      }

}
}
}

void keyinput(){

  int key = getch();
  switch(key){
    case 72://up
      yo--;
      break;

    case 75://left
      xo--;
      break;

    case 77://right
      xo++;
      break;

    case 80: //down
      yo++;
      break;

    default:
      break;
  }
}
