#include <ncurses.h>
#include <iostream>
using namespace std;

void printmap(int x, int y);
int main()
{
printmap(32,32);             // 벽 제외 순수 map 만 (30 x 30) 이므로
return 0;
}
void printmap(int x, int y){
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
      if(map[i][j]==2){              // immune wall은 'X'
        cout<<"X";
      }
      else if(map[i][j]==1){        // WALL은 'W'
        cout << "W";
      }
      else if(map[i][j]==0){        // 빈 공간
        cout << " ";
      }
    }
    cout << endl;
  }
}
