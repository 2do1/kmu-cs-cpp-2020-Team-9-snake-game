#include <iostream>
#include <ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include<locale.h>
#include <stdio.h>

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
int tick; // 스테이지마다 속도 다르게 표현
int xo=15;
int yo=15;
bool gameOver=false;
int bodyX[30] = {0} , bodyY[30] = {0};
int body_len = 3;

int growthX, growthY;
int poisonX, poisonY;

int Growth_Item = 5;
int Poison_Item = 8;
int numberOfgate = 3;
int timeseconds = 30;

char opposition_key = ' ' ;
char key_input = ' ';
char key_input2 = ' ';   // 시작화면에서 q 버튼 누르는 key 받는 입력
char select_key = ' ';   // pause화면에서 enterkey 받는 입력

WINDOW *backwin;
WINDOW *win1;
WINDOW *win2;
WINDOW *win3;
WINDOW *win4;
WINDOW *win5;

void startScreen();          // 맨 처음 시작 화면
void GameScreen();
void printPause();        // 게임화면

//void score_board();

void reset();               // stage1
void pausebutton();         // 게임 중 p 버튼 누르면 함수 실행
void EatG();
void PoisonP();
void GameOver();              // GameOver 화면 윈도우에 띄우기
void printmap(int x,int y);
void keyinput(char key);
void GrowthItem();
void PoisonItem();

int main()
{
  tick = 0;
  int whileTimes = 0;
  // body 길이를 5까지로 늘림
  // 겹치는걸 테스트하기 위해
  bodyX[1] = xo+1;
  bodyY[1] = yo;
  bodyX[2] = xo+2;
  bodyY[2] = yo;
  //printPause();
  setlocale(LC_ALL,"");
  startScreen();
  GameScreen();

  printmap(30,30);

  GrowthItem();
  PoisonItem();
  while(!gameOver){
      if(kbhit()){
        key_input = getch();
      }
      keyinput(key_input);  //xo, yo 값 바꿔줌, body위치 재설정
      EatG(); //뱀의 머리가 닿으면 G가 다른위치로 바뀌고 길이 늘려주기
      PoisonP();
      reset();

      GameScreen();
      usleep(tick);
      whileTimes += tick;
      if(whileTimes % 3000000 == 0){ // 와일문 20번 돌때마다 틱바꿔주기
        GrowthItem();
        PoisonItem();
      }

  }

  ///mvprintw(15, 11, "Game Over");
  //getch();
  endwin();
  //GameOver();

  return 0;
}
void startScreen(){     // 시작화면
  initscr();        // Curses 모드 시작
  start_color();    // Color 사용 선언
  attrset(A_BOLD);
  init_pair(1, COLOR_RED,COLOR_WHITE);
  init_pair(2, COLOR_WHITE,COLOR_BLACK);
  attron(COLOR_PAIR(2));
  border('|','|','-','-','+','+','+','+');


  mvprintw(6,23,"____________   ______________ ___________   _____________________  ____________");
  mvprintw(7,23,"__  ___/__  | / /__    |__  //_/__  ____/   __  ____/__    |__   |/  /__  ____/");
  mvprintw(8,23,"_____ \\__   |/ /__  /| |_  ,<  __  __/      _  / __ __  /| |_  /|_/ /__  __/ ");
  mvprintw(9,23,"____/ /_  /|  / _  ___ |  /| | _  /___      / /_/ / _  ___ |  /  / / _  /___  ");
  mvprintw(10,23,"/____/ /_/ |_/  /_/  |_/_/ |_| /_____/      \\____/  /_/  |_/_/  /_/  /_____/ ");

  mvprintw(19,18,"                 up");
  mvprintw(20,18,"               ______ ");
  mvprintw(21,18,"              ||    ||");
  mvprintw(22,18,"              || w  ||");
  mvprintw(23,18,"              ||____||");
  mvprintw(24,18,"              |/____\\|");
  mvprintw(25,18,"      ______   ______   ______");
  mvprintw(26,18,"     ||    || ||    || ||    ||");
  mvprintw(27,18,"left || a  || || s  || || d  || right ");
  mvprintw(28,18,"     ||____|| ||____|| ||____|| ");
  mvprintw(29,18,"     |/____\\| |/____\\| |/____\\| ");
  mvprintw(30,18,"                                  ");
  mvprintw(31,18,"          down");

  mvprintw(14,50,"           /^\\/^\\ ");
  mvprintw(15,50,"         _|__|  O|");
  mvprintw(16,50,"\\/     /~     \\_/ \\ ");
  mvprintw(17,50," \\____|__________/  \\ ");
  mvprintw(18,50,"        \\_______      \\ ");
  mvprintw(19,50,"                `\\     \\                 \\ ");
  mvprintw(20,50,"                  |     |                  \\ ");
  mvprintw(21,50,"                 /      /                    \\ ");
  mvprintw(22,50,"                /     /                       \\\\");
  mvprintw(23,50,"              /      /                         \\ \\ ");
  mvprintw(24,50,"             /     /                            \\  \\ ");
  mvprintw(25,50,"           /     /             _----_            \\   \\ ");
  mvprintw(26,51,"         /     /           _-~      ~-_         |   |");
  mvprintw(27,58," (      (        _-~    _--_    ~-_     _/   |");
  mvprintw(28,51,"         \\      ~-____-~    _-~    ~-_    ~-_-~    /");
  mvprintw(29,51,"           ~-_           _-~          ~-_       _-~");
  mvprintw(30,50,"               ~--______-~                ~-___-~");

  mvprintw(32,10,"      ____                      _____ _   _ _____ _____ ____    _              _             _   _ ");
  mvprintw(33,10,"     |  _ \\ _ __ ___  ___ ___  | ____| \\ | |_   _| ____|  _ \\  | |_ ___    ___| |_ __ _ _ __| |_| |");
  mvprintw(34,10,"     | |_) | '__/ _ \\/ __/ __| |  _| |  \\| | | | |  _| | |_) | | __/ _ \\  / __| __/ _` | '__| __| |");
  mvprintw(35,10,"     |  __/| | |  __/\\__ \\__ \\ | |___| |\\  | | | | |___|  _ <  | || (_) | \\__ \\ || (_| | |  | |_|_|");
  mvprintw(36,10,"     |_|   |_|  \\___||___/___/ |_____|_| \\_| |_| |_____|_| \\_\\  \\__\\___/  |___/\\__\\__,_|_|   \\__(_)");
  key_input2 = getch();
  clear();
  //reset();
  if(key_input2=='q'){//clear();
    endwin(); exit(0);}  // q 버튼 누르면 터미널로 돌아감.
  else if(key_input2=='\n'){
    //if(key_input2=!'q'){   // q 이외의 버튼을 누르면 게임시작.

      //getch();
      clear();
      refresh();

      //reset();
    }
    else{   // 'q' 키와 '엔터키 ' 이외에는 그냥 계속 시작화면
      startScreen();
    }
}

void GameScreen(){

  initscr();
  reset();                 // 게임 화면
  //WINDOW *win2;   // 스코어 보드 윈도우
  //WINDOW *win3;   //  미션 윈도우
  //WINDOW *win4;   //  몇번 째 STAGE 인지 알려주는 윈도우(stage 2,3,4,5 때 숫자만 바꿔주면 된다.)

  start_color();
  init_pair(1, COLOR_WHITE,COLOR_BLACK); // ? , game chang
  init_pair(2, COLOR_WHITE,COLOR_BLACK); //fontcolor & game, score window , snake background color
  init_pair(3, COLOR_BLACK,COLOR_WHITE);
  init_pair(4, COLOR_WHITE,COLOR_GREEN);
  init_pair(5, COLOR_WHITE,COLOR_BLACK);
  win2 = newwin(13,40,15,60);
  //box(win2,0,0);
  wbkgd(win2,COLOR_PAIR(2)); //score_board color
  wattron(win2,COLOR_PAIR(2)); // score board color
  mvwprintw(win2,1,1,"*-*-*-*-*-*-*-SCORE-BOARD-*-*-*-*-*-*-");
  wborder(win2,'|','|','-','-','X','X','X','X');
  mvwprintw(win2,3,10,"Length :  ");
  //std::cout << body_len << std :: endl;

  // 길이 출력
  mvwprintw(win2,3,20,"%d",body_len);
  //wrefresh(win2);
  //refresh();
  mvwprintw(win2,5,10,"Growth-Item:     ");
  mvwprintw(win2,5,25,"%d",Growth_Item);          // stage1 에서 growth item 초기값

  mvwprintw(win2,7,10,"Poison-Item:     ");
  mvwprintw(win2,7,25,"%d",Poison_Item);        // stage1 에서 posion item 초기값

  mvwprintw(win2,9,10,"# of Gate:     ");
  mvwprintw(win2,9,23,"%d",numberOfgate);

  mvwprintw(win2,11,10,"seconds:     ");
  mvwprintw(win2,11,20,"%d",timeseconds);


  wrefresh(win2);

  win3 = newwin(13,40,29,60);
  wbkgd(win3,COLOR_PAIR(2));// mission board color
  wattron(win3,COLOR_PAIR(2));// mission board color

  //미션은 stage 마다 고정되게 줄건지 특정 범위에 따라 랜덤에서 줄건지 결정해보자.
  mvwprintw(win3,1,1,"*-*-*-*-*-*-*-MISSION-*-*-*-*-*-*-*-*-*-");
  mvwprintw(win3,3,10,"Snake_length:     ");
  mvwprintw(win3,3,25,"%d",body_len);      // 몸 길이

  mvwprintw(win3,5,10,"+(growth):     ");
  mvwprintw(win3,5,22,"%d",Growth_Item);    // 획득 아이템

  mvwprintw(win3,7,10,"-(loss):     ");
  mvwprintw(win3,7,20,"%d",Poison_Item);    // 손실 아이템

  mvwprintw(win3,9,10,"Gate:     ");
  mvwprintw(win3,9,18,"%d",numberOfgate);      // 통과할 게이트 수 : stage1 에서는 3개

  mvwprintw(win3,11,10,"seconds:     ");       // 게임시작 후 몇초만 지나면 통과 : stage1 에서는 30초
  mvwprintw(win3,11,22,"%d",timeseconds);
  wborder(win3,'|','|','-','-','X','X','X','X');
  wrefresh(win3);

  win4 = newwin(8,80,0,25);
  attrset(A_BOLD);
  wbkgd(win4,COLOR_PAIR(1)); //stitle
  wattron(win4,COLOR_PAIR(1)); // title
  mvwprintw(win4,0,2,"                 _                                                  ____");
  mvwprintw(win4,1,2,"                | |                                                / . .\\");
  mvwprintw(win4,2,2," ___ _ __   __ _| | _____    __ _  __ _ _ __ ___   ___             \\  ---<");
  mvwprintw(win4,3,2,"/ __| '_ \\ / _` | |/ / _ \\  / _` |/ _` | '_ ` _ \\ / _ \\             \\  /");// stage 마다 출력 내용 바꾸면 된다.
  mvwprintw(win4,4,2,"\\__ \\ | | | (_| |   <  __/ | (_| | (_| | | | | | |  __/   __________/ /");
  mvwprintw(win4,5,2,"|___/_| |_|\\__,_|_|\\_\\___|  \\__, |\\__,_|_| |_| |_|\\___|-=:___________/ ");
  mvwprintw(win4,6,2,"                             __/ |                     ");
  mvwprintw(win4,7,2,"                            |___/ ");
  wrefresh(win4);

  win5 = newwin(3,40,11,60);
  attrset(A_BOLD);
  wbkgd(win5,COLOR_PAIR(3)); //stitle
  wattron(win5,COLOR_PAIR(3)); // title
  mvwprintw(win5,1,15,"STAGE 1");
  wrefresh(win5);



  //getch();
  curs_set(0);

  delwin(win2);
  delwin(win3);
  delwin(win4);

  //endwin();
}
void reset(){ //1단계
      // 스테이지마다 다르게 설정하면 된다. 앞으로
  tick = 150000;

  initscr();
  win1 = newwin(32,32,10,24);

  //backwin2 = newwin(36,36,8,22);

  //refresh();
  //wrefresh(backwin2);

  wrefresh(win1); /////
  //mvwprintw(win1,yo,xo,"O");
  //wbkgd(backwin2,COLOR_PAIR(5));
  //wborder(win1,'|','|','-','-','X','X','X','X');
  wbkgd(win1,COLOR_PAIR(1));// game board color
  wattron(win1,COLOR_PAIR(2)); // game ttle, snake color




  //wattron(backwin2,COLOR_PAIR(5));
  //mvwprintw(win1,15,12,"STAGE 1");
  mvwprintw(win1, yo, xo, "\u2B1C");
  for(int i=0;i<31;i++){
    for(int j=0;j<31;j++){

      for(int k = 1; k<body_len; k++){

        if(growthX == j && growthY == i){
          mvwprintw(win1, i, j, "\u2B50");
        }
        else if(poisonX == j && poisonY == i)
        {
          mvwprintw(win1, i, j, "\u2620");
        }
        else if(bodyX[k] == j && bodyY[k] == i){
            mvwprintw(win1, i, j, "\u2B1C");
        }


      }

    }
  }

  //box(backwin2,0,0);
  wborder(win1,'|','|','-','-','X','X','X','X');
  //wrefresh(backwin2);

  wrefresh(win1);


  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);
  //getch();
  delwin(win1);
}

void GrowthItem()
{
  int crush = 0;
  while(1)
  {
    growthX = (rand()%28) + 1;
    growthY = (rand()%28) + 1;
    for(int i = 0; i < body_len; i++)
    {
      if(growthX == bodyX[i] && growthY == bodyY[i])
      {
        crush++;
        break;
      }
    }

    if(crush == 1)
    {
      continue;
    }

    else
    {

      break;
    }
  }
}

void PoisonItem()
{
  int crush = 0;
  while(1)
  {
    poisonX = (rand()%28) + 1;
    poisonY = (rand()%28) + 1;
    for(int i = 0; i < body_len; i++)
    {
      if((poisonX == bodyX[i] && poisonY == bodyY[i]) || (poisonX == growthX && poisonY == growthY))
      {
        crush++;
        break;
      }
    }

    if(crush == 1)
    {
      continue;
    }

    else
    {
      break;
    }
  }
}

void EatG(){
  if(xo == growthX && yo == growthY)
  {
    GrowthItem();
    body_len++;
    bodyX[body_len-1] = bodyX[body_len-2];
    bodyY[body_len-1] = bodyY[body_len-2];
  }
}

void PoisonP(){
  if(xo == poisonX && yo == poisonY)
  {
    if(body_len ==3){
      gameOver = TRUE;

    }
    PoisonItem();
    body_len--;
    bodyX[body_len-1] = 0;
    bodyY[body_len-1] = 0;
  }
}

void printmap(int x, int y){

  mvwprintw(win1,yo,xo,"o");

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
    GameOver();
    clear();
    endwin();
    exit(0);
  }

  else if(key =='p'){
    printPause();
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



    if (xo >= 32 || xo < 0 || yo >= 32 || yo < 0 ){ // 벽 닿으면 종료
      gameOver = true;
      GameOver();
    }

    for(int i = 1; i < sizeof(bodyX) / sizeof(int); i++) // 머리와 몸이 닿으면 종료되는 코드
    {
      if(xo == bodyX[i] && yo == bodyY[i])
      {
        gameOver = true;
        GameOver();
      }
    }
  }
}

// 수정해야 할 부분?:p 를 누르면 단순 내용 출력이 아닌 게임이 stop 되어야 함
void pausebutton(){
  char key_input3=' ';       // 키 입력
  do{
  attrset(A_BOLD);
  mvprintw(22,20,"<Pause : Press ANY button TO RESUME>");
}while(key_input3==getch());
  //clear();  // 특정 줄을 지우는 법을 몰라서 이렇게 했는데, 한템포 락 걸린다..--보완할 점
  //reset();
  return;
}

void printPause(){

  WINDOW *w;
   char list[3][20] = { "    RESUME    ", "    RESTART   ", "     Quit     "};
   char item[20];
   int ch, i =30, width = 60;

   initscr();
   w = newwin( 11, 33, 18, 45 );
   init_pair(6, COLOR_BLACK, COLOR_WHITE);
   wbkgd(w,COLOR_PAIR(6));
   wattron(w,COLOR_PAIR(6));
   wrefresh(w);

   box( w, 0, 0 );


   for( i=0; i<3; i++ ) {
       if( i == 0 )
           wattron( w, A_STANDOUT ); // highlights the first item.
       else
           wattroff( w, A_STANDOUT );
       sprintf(item, "%-7s",  list[i]);
       mvwprintw( w, i+6, 9, "%s", item );
   }

   mvwprintw( w,1,3,"   ___   _   _ __  ___  ___" );
   mvwprintw( w,2,3,"  / o |.' \\ /// /,' _/ / _/" );
   mvwprintw( w,3,3," / _,'/ o // U /_\\ `. / _/ " );
   mvwprintw( w,4,3,"/_/  /_n_/ \\_,'/___,'/___/ " );

   wrefresh( w );

   i = 0;
   noecho();
   keypad( w, TRUE );
   curs_set( 0 );


   while(( ch = wgetch(w)) != 'q'){


           sprintf(item, "%-7s",  list[i]);
           mvwprintw( w, i+6, 9, "%s", item );

           switch( ch ) {
               case 'w':
                           i--;
                           i = ( i<0 ) ? 2 : i;
                           break;
               case 's':
                           i++;
                           i = ( i>2 ) ? 0 : i;
                           break;
           }
           // now highlight the next item in the list.
           wattron( w, A_STANDOUT );

           sprintf(item, "%-7s",  list[i]);
           mvwprintw( w, i+6, 9, "%s", item);
           wattroff( w, A_STANDOUT );
   }

   delwin( w );
   endwin();

}


void GameOver(){
  clear();
  initscr();
  WINDOW *gameover_window;   // 게임오버  윈도우

  start_color();
  init_pair(1, COLOR_GREEN,COLOR_BLACK);
  init_pair(2, COLOR_RED,COLOR_WHITE);
  wborder(gameover_window,'|','|','-','-','+','+','+','+');


  gameover_window = newwin(25,43,5,5);
  wbkgd(gameover_window,COLOR_PAIR(2));
  wattron(gameover_window,COLOR_PAIR(2));
  attrset(A_BOLD);
  mvprintw(12,25,"  ,--,     .--.           ,---.    .---..-.   .-.,---.  ,---.   ");
  mvprintw(13,25,".' .'     / /\\ \\ |\\    /| | .-'   / .-. )\\ \\ / / | .-'  | .-.\\  ");
  mvprintw(14,25,"|  |  __ / /__\\ \\|(\\  / | | `-.   | | |(_)\\ V /  | `-.  | `-'/ ");
  mvprintw(15,25,"\\  \\ ( _)|  __  |(_)\\/  | | .-'   | | | |  ) /   | .-'  |   (  ");
  mvprintw(16,25," \\  `-) )| |  |)|| \\  / | |  `--. \\ `-' / (_)    |  `--.| |\\ \\ ");
  mvprintw(17,25," )\\____/ |_|  (_)| |\\/| | /( __.'  )---'         /( __.'|_| \\)\\ ");
  mvprintw(18,25,"(__)             '-'  '-'(__)     (_)           (__)        (__)");
  mvprintw(19,59,"                      __    __    __    __");
  mvprintw(20,59,"                     /  \\  /  \\  /  \\  /  \\");
  mvprintw(21,5,"__________________________________________________________________________/  __\\/  __\\/  __\\/  __\\___________________");
  mvprintw(22,5,"_________________________________________________________________________/  /__/  /__/  /__/  /______________________");
  mvprintw(23,59,"                   | / \\   / \\   / \\   / \\  \\____");
  mvprintw(24,59,"                   |/   \\_/   \\_/   \\_/   \\    o \\");
  mvprintw(25,59,"                                           \\_____/--<");


  wborder(gameover_window,'|','|','-','-','X','X','X','X');
  wrefresh(gameover_window);
  //std:: cout << "Game Over" << std::endl; // 이거를 윈도우에 띄우면 좋을

  getch();
  clear();
  endwin();
  exit(0);
}
