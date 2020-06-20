#include <iostream>
#include <ncurses.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include<locale.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
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
int mission_body_len=5;

int growthX, growthY;
int poisonX, poisonY;
int gate1, gate2;
int gate1_x,gate1_y;
int gate2_x,gate2_y;

int Growth_Item = 2;
int Poison_Item = 8;
int numberOfgate = 3;
int timeseconds = 10;

int current_body_len = 3;
int current_Growth_Item = 0;
int current_Poison_Item = 0;
int current_numberOfgate = 0;
double current_timeseconds = 0.0;

int direction[5][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 0} };


int map[4][32][32] =
{
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
},
{
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
   {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1},
   {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
}
}; // [stage][행][열]) 일단은 스테이지 1만
char opposition_key = ' ' ;
char key_input = ' ';
char key_input2 = ' ';   // 시작화면에서 q 버튼 누르는 key 받는 입력
char select_key = ' ';   // pause화면에서 enterkey 받는 입력


WINDOW *win2;
WINDOW *win3;
WINDOW *win4;
WINDOW *win5;
WINDOW *NextStage_window;

void startScreen();          // 맨 처음 시작 화면
void GameScreen(int stage_number);

void stage(int stage_num);
void EatG(int stage_num);
void PoisonP(int stage_num);
void GameOver();              // GameOver 화면 윈도우에 띄우기
void keyinput(int stage_num, char key);
void GrowthItem(int stage_num);
void PoisonItem(int stage_num);
void MakeGate(int stage_number);
int HeadPosition();
void Gate(int stage_num, int direct);
void NextStage();
void Success(double current_timeseconds);

int main()
{
  int stage_number = 0;

  tick = 0;
  std::srand(static_cast<int>(std::time(0))); // 현재시간을랜덤해서 seed 로 준다.
  int whileTimes = 0;
  // body 길이를 5까지로 늘림
  // 겹치는걸 테스트하기 위해
  bodyX[1] = xo+1;
  bodyY[1] = yo;
  bodyX[2] = xo+2;
  bodyY[2] = yo;
  //printPause();
  setlocale(LC_ALL,"");
  //NextStage();
  startScreen();


  for(int stage_number=0 ; stage_number<4 ; stage_number++ ){
    gameOver=false;
    if(key_input == 'd'){
      opposition_key = 'a';
      body_len=3;
      xo = 15;
      yo = 15;
      bodyX[1] = xo-1;
      bodyY[1] = yo;
      bodyX[2] = xo-2;
      bodyY[2] = yo;
    }
    else{
      body_len=3;
      xo = 15;
      yo = 15;
      bodyX[1] = xo+1;
      bodyY[1] = yo;
      bodyX[2] = xo+2;
      bodyY[2] = yo;
    }

    GameScreen(stage_number);
    GrowthItem(stage_number);
    PoisonItem(stage_number);
    MakeGate(stage_number);
    if(stage_number != 0){

      current_Growth_Item=0;
      current_body_len=3;
      current_numberOfgate = 0;
      mission_body_len+=2;
      Growth_Item+=2;
      //Poison_Item+=2;
      numberOfgate+=1;
      timeseconds+=20;
    }
    usleep(1000000);

    while(gameOver==false){

        current_timeseconds = ((double)whileTimes/1000000.0000);
        if(kbhit()){
          key_input = getch();
        }

        keyinput(stage_number, key_input);  //xo, yo 값 바꿔줌, body위치 재설정
        EatG(stage_number); //뱀의 머리가 닿으면 G가 다른위치로 바뀌고 길이 늘려주기
        PoisonP(stage_number);
        Gate(stage_number, HeadPosition());

        if(stage_number==0 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds) gameOver=true;
        if(stage_number==1 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds) gameOver=true;
        if(stage_number==2 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds) gameOver=true;
        if(stage_number==3 && current_body_len >= mission_body_len && current_Growth_Item>=2 && current_timeseconds>timeseconds) gameOver=true;
        stage(stage_number);


        GameScreen(stage_number);
        usleep(tick);
        whileTimes += tick;
        if(current_timeseconds>=7.0 && whileTimes % 15000000 == 0 ) MakeGate(stage_number);
        if(whileTimes % 3000000 == 0){ // 와일문 20번 돌때마다 틱바꿔주기
          GrowthItem(stage_number);
          PoisonItem(stage_number);
        }

    }
    if (stage_number==3){
      Success(current_timeseconds);
    }
    else{
      NextStage();
    }

    clear();

  }
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
  //  delwin();
    endwin(); // 얘도 그냥 안먹힘
    exit(0);
  }  // q 버튼 누르면 터미널로 돌아감.
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

void GameScreen(int x){

  initscr();

  stage(x);
                 // 게임 화면
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
  mvwprintw(win2,3,20,"%d",current_body_len);
  //wrefresh(win2);
  //refresh();
  mvwprintw(win2,6,10,"Growth-Item:     ");
  mvwprintw(win2,6,25,"%d",current_Growth_Item);          // stage1 에서 growth item 초기값

  //mvwprintw(win2,,10,"Poison-Item:     ");
  //mvwprintw(win2,7,25,"%d",current_Poison_Item);        // stage1 에서 posion item 초기값

  mvwprintw(win2,9,10,"# of Gate:     ");
  mvwprintw(win2,9,23,"%d",current_numberOfgate);

  mvwprintw(win2,11,10,"seconds:     ");
  mvwprintw(win2,11,20,"%f",current_timeseconds);
  mvwprintw(win2,11,24,"초 가 지나는 중!");


  wrefresh(win2);

  win3 = newwin(13,40,29,60);
  wbkgd(win3,COLOR_PAIR(2));// mission board color
  wattron(win3,COLOR_PAIR(2));// mission board color

  //미션은 stage 마다 고정되게 줄건지 특정 범위에 따라 랜덤에서 줄건지 결정해보자.
  mvwprintw(win3,1,1,"*-*-*-*-*-*-*-MISSION-*-*-*-*-*-*-*-*-*-");
  mvwprintw(win3,3,10,"Snake_length:     ");
  mvwprintw(win3,3,25,"%d",mission_body_len);      // 몸 길이

  mvwprintw(win3,6,10,"+(growth):     ");
  mvwprintw(win3,6,22,"%d",Growth_Item);    // 획득 아이템

  //mvwprintw(win3,7,10,"-(loss):     ");
  //mvwprintw(win3,7,20,"%d",Poison_Item);    // 손실 아이템

  mvwprintw(win3,9,10,"Gate:     ");
  mvwprintw(win3,9,18,"%d",numberOfgate);      // 통과할 게이트 수 : stage1 에서는 3개

  mvwprintw(win3,11,10,"seconds:     ");       // 게임시작 후 몇초만 지나면 통과 : stage1 에서는 30초
  mvwprintw(win3,11,18," 합산");
  mvwprintw(win3,11,24,"%d",timeseconds);
  mvwprintw(win3,11,26,"초는 버티세요!");
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
  mvwprintw(win5,1,15,"STAGE ");
  mvwprintw(win5,1,22,"%d",x+1);

  wrefresh(win5);


  //getch();
  curs_set(0);

  delwin(win2);
  delwin(win3);
  delwin(win4);

  //endwin();
}



void stage(int stage_num){ //1단계


  WINDOW *win1;

  tick = 150000;
  initscr();
  win1 = newwin(32,32,10,24);
  wrefresh(win1);
  wbkgd(win1,COLOR_PAIR(1));
  wattron(win1,COLOR_PAIR(2)); // game ttle, snake color
  mvwprintw(win1, yo, xo, "0");
  for(int i=0;i<32;i++){
    for(int j=0;j<32;j++){
      if(map[stage_num][i][j] == 2)mvwprintw(win1,i, j, "*" );
      else if ((map[stage_num][i][j] == 1) || (map[stage_num][i][j] == 3 )){

        mvwprintw(win1,i, j, "+" );

      }
      if(map[stage_num][i][j] == 3 && current_timeseconds>=7.0){mvwprintw(win1, i,j, "X");}

      for(int k = 1; k<body_len; k++){

        if(growthX == j && growthY == i){
          mvwprintw(win1, i, j, "G");
        }
        else if(poisonX == j && poisonY == i)
        {
          mvwprintw(win1, i, j, "P");
        }
        else if(bodyX[k] == j && bodyY[k] == i){
            mvwprintw(win1, i, j, "0");
        }

      }
    }
  }


  wrefresh(win1);
  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);
  delwin(win1);
  endwin();

}





void GrowthItem(int stage_num)
{
  int crush = 0;
  while(1)
  {
    growthX = (rand()%30) + 1;
    growthY = (rand()%30) + 1;
    for(int i = 0; i < body_len; i++)
    {
      if(growthX == bodyX[i] && growthY == bodyY[i])
      {
        crush++;
        break;
      }
    }
    if(map[stage_num][growthY][growthX] == 1 ||map[stage_num][growthY][growthX] == 2 ||map[stage_num][growthY][growthX] == 3)crush++;

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

void PoisonItem(int stage_num)
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
      if(map[stage_num][poisonY][poisonX] == 1|| map[stage_num][poisonY][poisonX] == 2 ||map[stage_num][poisonY][poisonX] == 3){crush++;}
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

void EatG(int stage_num){
  if(xo == growthX && yo == growthY)
  {
    GrowthItem(stage_num);
    body_len++;
    current_body_len++;
    current_Growth_Item++;
    bodyX[body_len-1] = bodyX[body_len-2];
    bodyY[body_len-1] = bodyY[body_len-2];
  }
}

void PoisonP(int stage_num){
  if(xo == poisonX && yo == poisonY)
  {

    PoisonItem(stage_num);
    body_len--;
    current_body_len--;
    bodyX[body_len-1] = 0;
    bodyY[body_len-1] = 0;
  }
}


void keyinput(int stage_num, char key){

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



    if (map[stage_num][yo][xo] ==1 ){ // 벽 닿으면 종료
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

  else{}
  if (body_len == 2){  // 현재 입력 받은 키와 전에 입력받은 키의 반대키가 같으면 게임종료.
    gameOver = true;
    GameOver();

  }


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

void NextStage(){


  initscr();

  wborder(NextStage_window,'|','|','-','-','+','+','+','+');
  NextStage_window = newwin(25,43,5,5);


  attrset(A_BOLD);
  mvprintw(11,25," ____");
  mvprintw(12,25,"/\\  _`\\     ");
  mvprintw(13,25,"\\ \\,\\L\\_\\  __  __    ___    ___     __    ____    ____  ");
  mvprintw(14,25," \\/_\\__ \\ /\\ \\/\\ \\  /'___\\ /'___\\ /'__`\\ /',__\\  /',__\\ ");
  mvprintw(15,25,"   /\\ \\L\\ \\ \\ \\_\\ \\/\\ \\__//\\ \\__//\\  __//\\__, `\\/\\__, `\\");
  mvprintw(16,25,"   \\ `\\____\\ \\____/\\ \\____\\ \\____\\ \\____\\/\\____/\\/\\____/");
  mvprintw(17,25,"    \\/_____/\\/___/  \\/____/\\/____/\\/____/\\/___/  \\/___/ ");

  mvprintw(19,25," __  __                  __        ____    __");
  mvprintw(20,25,"/\\ \\/\\ \\                /\\ \\__    /\\  _`\\ /\\ \\__ ");
  mvprintw(21,25,"\\ \\ `\\\\ \\     __   __  _\\ \\ ,_\\   \\ \\,\\L\\_\\ \\ ,_\\    __       __      __  ");
  mvprintw(22,25," \\ \\ , ` \\  /'__`\\/\\ \\/'\\\\ \\ \\/    \\/_\\__ \\\\ \\ \\/  /'__`\\   /'_ `\\  /'__`\\");
  mvprintw(23,25,"  \\ \\ \\`\\ \\/\\  __/\\/>  </ \\ \\ \\_     /\\ \\L\\ \\ \\ \\_/\\ \\L\\.\\_/\\ \\L\\ \\/\\  __/ ");
  mvprintw(24,25,"   \\ \\_\\ \\_\\ \\____\\/\\_/\\_\\ \\ \\__\\    \\ `\\____\\ \\__\\ \\__/.\\_\\ \\____ \\ \\____\\");
  mvprintw(25,25,"    \\/_/\\/_/\\/____/\\//\\/_/  \\/__/     \\/_____/\\/__/\\/__/\\/_/\\/___L\\ \\/____/");
  mvprintw(26,25,"                                                              /\\____/ ");
  mvprintw(27,25,"                                                              \\_/__/ ");
  wrefresh(NextStage_window);

  getch();
  clear();
  endwin();

}

void Success(double current_timeseconds){


  initscr();
  WINDOW *success;
  success = newwin(45,85,5,5);
  wborder(success,'|','|','-','-','+','+','+','+');



  attrset(A_BOLD);
  mvwprintw(success,11,30,"success");
  mvwprintw(success,15,25,"게임 클리어 시간이 총 ");
  mvwprintw(success,17,25,"%f",current_timeseconds);      // 몸 길이
  mvwprintw(success,17,30,"초 걸렸습니다!!!!!!");      // 몸 길이

  wrefresh(success);

  getch();
  clear();
  endwin();

}

void MakeGate(int stage_number){

  vector <vector<int> > temp;

  for(int i=0; i<32;i++){
    for(int j=0; j<32; j++){
      if(map[stage_number][i][j]==3){
        map[stage_number][i][j]=1;
      }
      if(map[stage_number][i][j]==1){
        vector<int> temp1;

        temp1.push_back(j);
        temp1.push_back(i);

        temp.push_back(temp1);

      }

    }
  }


  while(true){

    gate1=rand()%temp.size();
    gate2=rand()%temp.size();

      if (gate1!=gate2) {
        break;
      }
  }

  gate1_x = temp[gate1][0];
  gate1_y = temp[gate1][1];
  map[stage_number][gate1_y][gate1_x]=3;

  gate2_x = temp[gate2][0];
  gate2_y= temp[gate2][1];
  map[stage_number][gate2_y][gate2_x]=3;
}



void Gate(int stage_num, int direct)
{ if(direct == -1){

}
  else if(direct / 4 == 0)
  {
    while(1)
    {
      direct %= 4;
      int dirX = direction[direct][0];
      int dirY = direction[direct][1];
      if(gate2_x+dirX<0 ||gate2_y+dirY <0|| gate2_x+dirX >31||gate2_y+dirY>31){
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else if(map[stage_num][gate2_x+dirX][gate2_y+dirY] == 1|| map[stage_num][gate2_x+dirX][gate2_y+dirY] == 2|| map[stage_num][gate2_x+dirX][gate2_y+dirY] == 3 )
      {
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else
      {
        xo = gate2_x + dirX;
        yo = gate2_y + dirY;
        if(direct ==0){
          key_input = 'd';
          opposition_key = 'a';
        }
        else if(direct == 1){
          key_input = 's';
          opposition_key = 'w';
        }
        else if(direct == 2){
          key_input = 'a';
          opposition_key = 'd';
        }
        else if(direct == 3){
          key_input = 'w';
          opposition_key = 's';
        }
        break;
      }
    }
  }
  else if(direct / 4 == 1)
  {
    while(1)
    {
      direct %= 4;
      int dirX = direction[direct][0];
      int dirY = direction[direct][1];
      if(gate1_x+dirX<0 ||gate1_y+dirY <0|| gate1_x+dirX >31||gate1_y+dirY>31){
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;

      }
      else if(map[stage_num][gate1_x+dirX][gate1_y+dirY] == 1|| map[stage_num][gate1_x+dirX][gate1_y+dirY] == 2||map[stage_num][gate1_x+dirX][gate1_y+dirY] == 3 )
      {
        direct++;
        if(direct == 4)
        {
          direct = 0;
        }
        continue;
      }
      else
      {
        xo = gate1_x + dirX;
        yo = gate1_y + dirY;
        if(direct ==0){
          key_input = 'd';
          opposition_key = 'a';
        }
        else if(direct == 1){
          key_input = 's';
          opposition_key = 'w';
        }
        else if(direct == 2){
          key_input = 'a';
          opposition_key = 'd';
        }
        else if(direct == 3){
          key_input = 'w';
          opposition_key = 's';
        }
        break;
      }
    }
  }
}

int HeadPosition(){
  current_numberOfgate +=1;
  if( xo == gate1_x && yo == gate1_y){
    if( bodyX[1]==gate1_x-1 && bodyY[1]==gate1_y) return 0;  // 진행방향 오른쪽
    else if( bodyX[1]==gate1_x && bodyY[1]==gate1_y-1) return 1;   // 진행 방향 아래
    else if( bodyX[1]==gate1_x+1 && bodyY[1]==gate1_y) return 2;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate1_x && bodyY[1]==gate1_y+1) return 3;   // 진행 방향 위쪽
    else{
      current_numberOfgate -= 1;
      return -1;}
  }
  else if( xo == gate2_x && yo == gate2_y){
    if( bodyX[1]==gate2_x-1 && bodyY[1]==gate2_y) return 4;  // 진행방향 오른쪽
    else if( bodyX[1]==gate2_x && bodyY[1]==gate2_y-1) return 5;   // 진행 방향 아래
    else if( bodyX[1]==gate2_x+1 && bodyY[1]==gate2_y) return 6;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate2_x && bodyY[1]==gate2_y+1) return 7;   // 진행 방향 위쪽
    else{current_numberOfgate -= 1;
      return -1;}
    }
  else{current_numberOfgate -= 1;return -1;
    }
}
