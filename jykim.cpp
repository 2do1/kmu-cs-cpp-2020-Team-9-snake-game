int HeadPosition{
  if( xo == gate1_x && yo == gate1_y){
    if( bodyX[1]==gate1_x && bodyY[1]=gate1_y-1) return 0;  // 진행방향 오른쪽
    else if( bodyX[1]==gate1_x-1 && bodyY[1]==gate1_y) return 1;   // 진행 방향 아래
    else if( bodyX[1]==gate1_x && bodyY[1]==gate1_y+1) return 2;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate1_x+1 && bodY[1]==gate1_y) return 3;   // 진행 방향 위쪽
  }
  else if( xo == gate2_x && yo == gate2_y){
    if( bodyX[1]==gate2_x && bodyY[1]=gate2_y-1) return 4;  // 진행방향 오른쪽
    else if( bodyX[1]==gate2_x-1 && bodyY[1]==gate2_y) return 5;   // 진행 방향 아래
    else if( bodyX[1]==gate2_x && bodyY[1]==gate2_y+1) return 6;   // 진행 방향 왼쪽
    else if( bodyX[1]==gate2_x+1 && bodyY[1]==gate2_y) return 7;   // 진행 방향 위쪽
  }
  else retrun -1;
}
