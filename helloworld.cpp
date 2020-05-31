#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

// 키보드 kbhit 구현 9~39 틱구현 
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
// 

int xo=15;
int yo=15;
bool gameOver=false;
int bodyX[30] = {0} , bodyY[30] = {0};
int body_len = 5; // 길이 5로 늘려줌
char opposition_key = ' ' ;
char key_input = ' ';
WINDOW *win1;
void startScreen();          // 맨 처음 시작 화면
void GameScreen();          // 게임화면
void reset();               // stage1
void printmap(int x,int y);
void keyinput(char key);
int main()
{
  startScreen();
  reset();
  GameScreen();
  printmap(30,30);

  // body 길이를 5까지로 늘림
  // 겹치는걸 테스트하기 위해
  bodyX[1] = xo+1;
  bodyY[1] = yo;
  bodyX[2] = xo+2;
  bodyY[2] = yo;
  bodyX[3] = xo+3;
  bodyY[3] = yo;
  bodyX[4] = xo+4;
  bodyY[4] = yo;

  while(!gameOver){
      if(kbhit()){ // 키보드가 입력받을때 실행
        key_input = getch(); 
      }
      keyinput(key_input);  //xo, yo 값 바꿔줌, body위치 재설정
      reset();
      GameScreen();
      usleep(200000);
  }
  mvprintw(15, 11, "Game Over");
  getch();
  endwin();

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
  win1 = newwin(32,32,5,5);
  //mvwprintw(win1,yo,xo,"O");
  wbkgd(win1,COLOR_PAIR(1));
  wattron(win1,COLOR_PAIR(1));
  //mvwprintw(win1,15,12,"STAGE 1");

  for(int i=0;i<31;i++){
    for(int j=0;j<31;j++){

      if ((i==yo)&&(j==xo)){
        mvwprintw(win1,i,j,"O"); //snake!
      }
      else{
        for(int k = 0; k<body_len; k++){

          if(bodyX[k] == j && bodyY[k] == i){
            if(k == 1){
              mvwprintw(win1, i, j, "1");
            }
            if(k >= 2){
              mvwprintw(win1, i, j, "2");
            }
          }
        }
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

  map[xo][yo]=3;      // 뱀 머리는 3이라는 값으로 설정
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


}

void keyinput(char key){

  int tmpX = bodyX[0];
  int tmpY = bodyY[0];
  int tmp2X, tmp2Y;
  bodyX[0] = xo;
  bodyY[0] = yo;


  if(key =='w' || key =='a' || key =='s'||key =='d'){
    for(int i = 0; i<body_len; i++){
      tmp2X = bodyX[i];
      tmp2Y = bodyY[i];
      bodyX[i] = tmpX;
      bodyY[i] = tmpY;
      tmpX = tmp2X;
      tmpY = tmp2Y;
      }


    if (opposition_key == key){  // 현재 입력 받은 키와 전에 입력받은 키의 반대키가 같으면 게임종료.
      gameOver = true;
      std:: cout << "Game Over"; // 이거를 윈도우에 띄우면 좋을

    }
    else{
      switch(key){

        case 'w'://up
          yo--;
          opposition_key = 's';  //opposition_key 재설정
          break;

        case 'a'://left
          xo--;
          opposition_key = 'd';
          break;

        case 'd'://right
          xo++;
          opposition_key = 'a';
          break;

        case 's': //down
          yo++;
          opposition_key = 'w';
          break;

        default:
          break;

      }
    }
    if (xo >= 31 || xo <= 0 || yo >= 31 || yo <= 0 ){ // 벽 닿으면 종료
      gameOver = true;
    }

    for(int i = 1; i < sizeof(bodyX) / sizeof(int); i++) // 머리와 몸이 닿으면 종료되는 코드
    {
      if(xo == bodyX[i] && yo == bodyY[i])
      {
        gameOver = true;
      }
    }
  }
}
