#include <ncurses.h>
#include <iostream>
using namespace std;

int xo,yo;
void printmap(int x, int y);
void keyInput();

int main()
{
  initscr();
  start_color();
  init_pair(1,COLOR_WHITE,COLOR_BLACK);
  attron(COLOR_PAIR(1));

  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);

  while(1){
    printmap(30,30);
    keyInput();
    clear();
  }

  refresh();
  attroff(COLOR_PAIR(1));

  endwin();
return 0;
}
void printmap(int x, int y){

  xo = x/2;
  yo= y/2;
  int map[x][y]={0,};   // 우선 map 의 모든 값을 0으로 설정한다.

  map[0][0]=2;     // immune wall 의 값을 2로 지정
  map[0][y-1]=2;
  map[x-1][0]=2;
  map[x-1][y-1]=2;

  for(int i=1;i<y-1;i++){    // 위쪽 wall의 값을 1로 지정
    map[0][i]=1;
  }
  for(int i=1;i<x-1;i++){    // 왼쪽 wall의 값을 1로 지정
    map[i][0]=1;
  }
  for(int i=1;i<x-1;i++){    // 오른쪽 wall의 값을 1로 지정
    map[i][y-1]=1;
  }
  for(int i=1;i<y-1;i++){   // 아래쪽 wall의 값을 1로 지정
    map[x-1][i]=1;
  }

  for(int i=0;i<y;i++){
    for(int j=0;j<x;j++){
      if(map[i][j]==2){
                    // immune wall은 'X'
        printw("X");
      }
      if (i==yo && j==xo){
        printw("O"); //snake!
      }
      else if(map[i][j]==1){        // WALL은 'W'
        printw("W");
      }
      else if(map[i][j]==0){        // 빈 공간
        printw(" ");
      }
    }
  printw("\n");
  }
}

void keyInput(){

  switch(getch()){
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
