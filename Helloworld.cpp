#include <iostream>
#include <ncurses.h>
#include <time.h>
int xo=15;
int yo=15;
WINDOW *win1;
void startScreen();          // 맨 처음 시작 화면
void GameScreen();          // 게임화면
void reset();               // stage1
void printmap(int x,int y);
void keyinput();
int main()
{
  startScreen();
  reset();
  GameScreen();
  printmap(30,30);

  while(true){

    keyinput();
    reset();
    GameScreen();


  }

  return 0;
}
void startScreen(){
  initscr();        // Curses 모드 시작
  start_color();    // Color 사용 선언
  init_pair(1, COLOR_GREEN,COLOR_WHITE);
  init_pair(2, COLOR_GREEN,COLOR_WHITE);
  border('|','|','-','-','X','X','X','X');
  mvprintw(8,20,"If you want to start the game, press any button!");
  mvprintw(10,30,"(Press any KEY to start the game)");
  mvprintw(12,31," left,right,up,down : Move    ");
  mvprintw(14,32,"  P : Pause         ");
  mvprintw(16,33,"  ESC : Quit        ");
  refresh();
  getch();
  clear();
  endwin();
}

void GameScreen(){
  initscr();
  reset();                 // 게임 화면
  WINDOW *win2;
  WINDOW *win3;

  start_color();
  init_pair(1, COLOR_GREEN,COLOR_WHITE);
  init_pair(2, COLOR_GREEN,COLOR_WHITE);

  win2 = newwin(13,40,5,40);
  wbkgd(win2,COLOR_PAIR(1));
  wattron(win2,COLOR_PAIR(1));
  mvwprintw(win2,1,1,"SCORE-BOARD");
  wborder(win2,'|','|','-','-','X','X','X','X');
  mvwprintw(win2,3,1,"Length :  ");
  mvwprintw(win2,5,1,"Growth-Item:     ");
  mvwprintw(win2,7,1,"Poison-Item:     ");
  mvwprintw(win2,9,1,"# of Gate:     ");
  mvwprintw(win2,11,1,"seconds:     ");


  wrefresh(win2);

  win3 = newwin(13,40,20,40);
  wbkgd(win3,COLOR_PAIR(1));
  wattron(win3,COLOR_PAIR(1));
  mvwprintw(win3,1,1,"MISSION");
  mvwprintw(win3,3,1,"B:     ");
  mvwprintw(win3,5,1,"+:     ");
  mvwprintw(win3,7,1,"-:     ");
  mvwprintw(win3,9,1,"G:     ");
  mvwprintw(win3,11,1,"seconds:     ");


  wborder(win3,'|','|','-','-','X','X','X','X');
  wrefresh(win3);

  //getch();
  curs_set(0);

  delwin(win2);
  delwin(win3);
  //endwin();
}
void reset(){        // 스테이지마다 다르게 설정하면 된다. 앞으롷

  initscr();
  //printmap(32,32);
  win1 = newwin(32,32,5,5);
  //mvwprintw(win1,yo,xo,"O");
  wbkgd(win1,COLOR_PAIR(1));
  wattron(win1,COLOR_PAIR(1));
  //mvwprintw(win1,15,12,"STAGE 1");

  for(int i=0;i<30;i++){
    for(int j=0;j<30;j++){

      if ((i==yo)&&(j==xo)){
        mvwprintw(win1,yo,xo,"O"); //snake!
      }
    }
  }


  wborder(win1,'W','W','W','W','X','X','X','X');
  wrefresh(win1);

  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);
  //getch();
  delwin(win1);
}

void printmap(int x, int y){

  mvwprintw(win1,yo,xo,"O");

  int map[x][y]={0,};   // 우선 map 의 모든 값을 0으로 설정한다.

  map[0][0]=2;     // immune wall 의 값을 2로 지정
  map[0][y-1]=2;
  map[x-1][0]=2;
  map[x-1][y-1]=2;

  map[10][10]=3;      // 뱀 머리는 3이라는 값으로 설정
                        // 뱀 몸, 뱀 꼬리도 4,5 라는 값으로 설정해주면 된다.

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

      if ((i==yo)&&(j==xo)){
        mvwprintw(win1,yo,xo,"O"); //snake!
      }
    }
  }
}

void keyinput(){

  int key = getch();
  switch(key){
    case 'a'://up
      yo--;
      break;

    case 'w'://left
      xo--;
      break;

    case 'd'://right
      xo++;
      break;

    case 's': //down
      yo++;
      break;

    default:
      break;
  }
}
