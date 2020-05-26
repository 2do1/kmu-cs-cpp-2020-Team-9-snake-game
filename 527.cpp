#include <iostream>
#include <ncurses.h>
#include <time.h>
#include<unistd.h>

int xo=15;
int yo=15;
bool gameOver=false;
int bodyX[30] = {0} , bodyY[30] = {0};
int body_len = 5; // 길이 5로 늘려줌
char opposition_key = ' ' ;
char key_input = ' ';
char key_input2 = ' ';
WINDOW *win1;
void startScreen();          // 맨 처음 시작 화면
void GameScreen();          // 게임화면

//void score_board();

void reset();               // stage1
void pausebutton();

void GameOver();              // GameOver 화면 윈도우에 띄우기
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
      nodelay(win1, false);
      key_input = getch();
      keyinput(key_input);  //xo, yo 값 바꿔줌, body위치 재설정
      reset();
      GameScreen();
  }
  //mvprintw(15, 11, "Game Over");
  GameOver();
  getch();
  endwin();

  return 0;
}
void startScreen(){
  initscr();        // Curses 모드 시작
  start_color();    // Color 사용 선언
  attrset(A_BOLD);
  init_pair(1, COLOR_RED,COLOR_WHITE);
  init_pair(2, COLOR_CYAN,COLOR_MAGENTA);
  attron(COLOR_PAIR(2));
  border('|','|','-','-','X','X','X','X');
  mvprintw(8,20,"+-----------------------------------------------+");
  mvprintw(9,20,"|         S       N       A       K       E     |");
  mvprintw(10,20,"+-----------------------------------------------+");
  mvprintw(12,20,"If you want to start the game, press any button!");
  mvprintw(14,20,"--------(Press any KEY to start the game)-------");
  mvprintw(18,20,"                                                ");
  mvprintw(20,20,"-----------------W,A,S,D : Move ----------------");
  mvprintw(22,20,"------------------P : Pause---------------------");
  mvprintw(24,20,"------------------ESC : Quit--------------------");
  key_input2 = getch();
  if(key_input2=='q'){clear(); endwin(); exit(0);}
  else{
    if(key_input2=!'q'){
      //refresh();
      //getch();
      clear();
      endwin();
    }
}
clear();
endwin();
}

void GameScreen(){
  initscr();
  reset();                 // 게임 화면
  WINDOW *win2;   // 스코어 보드 윈도우
  WINDOW *win3;   //  미션 윈도우
  WINDOW *win4;   //  몇번 째 STAGE 인지 알려주는 윈도우(stage 2,3,4,5 때 숫자만 바꿔주면 된다.)

  start_color();
  init_pair(1, COLOR_GREEN,COLOR_BLACK);
  init_pair(2, COLOR_RED,COLOR_WHITE);

  win2 = newwin(13,40,5,40);
  wbkgd(win2,COLOR_PAIR(2));
  wattron(win2,COLOR_PAIR(2));
  mvwprintw(win2,1,1,"SCORE-BOARD");
  wborder(win2,'|','|','-','-','X','X','X','X');
  mvwprintw(win2,3,1,"Length :  ");
  //std::cout << body_len << std :: endl;

  // 길이 출력
  mvwprintw(win2,3,10,"%d",body_len);
  //wrefresh(win2);
  //refresh();
  mvwprintw(win2,5,1,"Growth-Item:     ");
  mvwprintw(win2,5,15,"%d",body_len);

  mvwprintw(win2,7,1,"Poison-Item:     ");
  mvwprintw(win2,7,15,"%d",body_len);

  mvwprintw(win2,9,1,"# of Gate:     ");
  mvwprintw(win2,9,13,"%d",body_len);

  mvwprintw(win2,11,1,"seconds:     ");
  mvwprintw(win2,11,10,"%d",body_len);



  wrefresh(win2);

  win3 = newwin(13,40,20,40);
  wbkgd(win3,COLOR_PAIR(2));
  wattron(win3,COLOR_PAIR(2));
  mvwprintw(win3,1,1,"MISSION");
  mvwprintw(win3,3,1,"B:     ");
  mvwprintw(win3,5,1,"+:     ");
  mvwprintw(win3,7,1,"-:     ");
  mvwprintw(win3,9,1,"G:     ");
  mvwprintw(win3,11,1,"seconds:     ");
  wborder(win3,'|','|','-','-','X','X','X','X');
  wrefresh(win3);

  win4 = newwin(5,80,38,5);
  attrset(A_BOLD);
  wbkgd(win4,COLOR_PAIR(2));
  wattron(win4,COLOR_PAIR(2));
  mvwprintw(win4,2,33,"STAGE 1");  // stage 마다 바꾸면 된다.
  wrefresh(win4);

  //getch();
  curs_set(0);

  delwin(win2);
  delwin(win3);
  delwin(win4);
  //endwin();
}
void reset(){        // 스테이지마다 다르게 설정하면 된다. 앞으롷

  initscr();
  win1 = newwin(32,32,5,5);
  //mvwprintw(win1,yo,xo,"O");
  wbkgd(win1,COLOR_PAIR(1));
  wattron(win1,COLOR_PAIR(2));
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


  if(key =='q'){
    clear();
    endwin();
    exit(0);
  }

  else if(key =='p'){
    pausebutton();
  }
  else if(key =='w' || key =='a' || key =='s'||key =='d'){
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
      GameOver();
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

void pausebutton(){   // 수정해야 할 부분?
  initscr();
  char key_input3=' ';
  do{
  attrset(A_BOLD);
  mvprintw(22,20,"<Pause : Press ANY button TO RESUME>");
}while(key_input3==getch());
  reset();
  return;
}

//void score_board(){
//}

  //
  //if(key_input3==getch()){
  //  clear();
    //reset();
  //  return ;
//  }
void GameOver(){
  clear();
  initscr();
  WINDOW *gameover_window;   // 스코어 보드 윈도우

  start_color();
  init_pair(1, COLOR_GREEN,COLOR_BLACK);
  init_pair(2, COLOR_RED,COLOR_WHITE);

  gameover_window = newwin(25,43,5,5);
  wbkgd(gameover_window,COLOR_PAIR(2));
  wattron(gameover_window,COLOR_PAIR(2));
  attrset(A_BOLD);
  mvprintw(22,20,"+-----------------------------------------------+");
  mvprintw(23,20,"|  G     A     M     E     O     V     E     R  |");
  mvprintw(24,20,"+-----------------------------------------------+");
  wborder(gameover_window,'|','|','-','-','X','X','X','X');
  wrefresh(gameover_window);
  //std:: cout << "Game Over" << std::endl; // 이거를 윈도우에 띄우면 좋을
  getch();
  clear();
  endwin();
}
