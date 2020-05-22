# kmu-cs-cpp-2020-Team-9-snake-game
C++을 이용한 Snake game 제작

## pseudo-code
```
int main(void){
  while(게임 종료 안되면){
      키보드 입력 감지(); 
      뱀 위치 업데이트(); 
      Growth 아이템 업데이트(); 
      Position 아이템 업데이트(); // Growth, Position 아이템 펄스 정해야함
      게이트 업데이트(); //Immune wall은 게이트로 변할 수 없음, 게이트 출현 방법 정하기
      스코어 업데이트(); 
      뱀이 움직일 수 있는 지 체크(); // 벽 만남, 키보드 방향(꼬리 방향으로 키보드 입력), 자기 몸 통과->게임종료조건 체크
      뱀 이동();
      뱀이 Growth 아이템 먹음(); // 꼬리 부분 위치 유지 Growth 아이템 놓인 부분이 뱀 머리?됨
      뱀이 Position 아이템 먹음(); // 꼬리 감소
      뱀이 게이트 진입함();
      스코어 계산 및 점수판 출력(); // 움직일 때마다 스코어 업데이트한 거 + 게임시간으로 계산
      }
```


## conference 01
```
- 다음 주 일요일까지 2단계 까지 구현목표
- 맵 제작, 키보드 입력 - 백소현, 김정엽 (수요일 까지 마무리해서 전달)
- 뱀 HEAD  틱, 키보드 입력에 따른 움직임 제어 - 이도원, 김채환 

- 맵 4 단계 구현
  1. 30 x 30
  2. 25 x 25
  3. 23 x 23
  4. 21 x 21 ( Wall 구현)

```

## conference 02
```
채환,도원 -> 틱 및 수정사항 구현
소현,정엽 -> growth, position, gate 발생 구현
채환, 도원 -> gate 발생 시킨 코드 받아서 뱀 움직임 구현하기
소현, 정엽 -> 이후로는 ncurse, growth,position  자유롭게 구현하고 있기

그러다가 어느정도 되면 다시 회의하고 서로 분량 조절!
```
