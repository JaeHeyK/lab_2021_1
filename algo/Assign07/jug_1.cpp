/*
  이 코드에서는 승리할 수 있는 경우를 winState라 하고, 4자리 이진수로 표기합니다.
  예를 들어 현재 잔액에서 s1, s3를 가져가면 이길 수 있을 때,
  winState는 1010으로 표기됩니다.(MSB->LSB 순서로 s3,s2,s1,PASS)
  따라서 winState가 0000(==0)일 경우, 현재 상태에서는 F가 어떤 선택을 하던
  이길 수 없다는 뜻이므로, 승자가 S임을 나타냅니다.
  반대로 winState가 0000이 아닐경우, F가 승리할 방법이 있다는 의미이므로
  승자는 F로 간주됩니다.

  만약 선택 가능한 경우의 winState가 모두 0이 아닐 경우,
  다음 차례에서 상대방이 항상 이길 방법이 있다는 의미이므로 현재의 winState는 0이 됩니다.
  반대로 선택 가능한 경우 중 winState가 하나라도 0이라면,
  그 경우가 바로 상대방이 이길 수 없는 경우이므로 현재의 winState에 포함됩니다.
  
  또한 문제를 분석한 결과,
  base 조건인 (n < s1)일 때 PASS를 할 수 없으면 지게 되므로,
  즉 (n >= s1)일 때 현재 상태에서 0을 선택하면 항상 질 수 밖에 없으므로,
  base 조건이 아닐 때 현재 상태에서 0을 고르는 경우는 고려하지 않았습니다.
*/

#include <fstream>
#include <iostream>

using namespace std;

const int take = 3;   // 가져갈 수 있는 돈의 가지수; 문제에서는 3가지
const int MAX = 100;  // 항아리에 든 돈의 최댓값; 문제에서는 100

int takes[take+1];    // PASS를 포함한 가져갈 수 있는 액수를 저장할 배열; 0으로 초기화
int winStates[MAX+1];  // 0의 경우를 포함한 winState를 저장할 배열; -1로 초기화

int getWinState(int currentJug, int prevTake); // 현재 항아리 내 잔액과 이전에 가져간 금액을 이용하여 새로운 winState를 얻어내는 함수; prevTake는 takes의 인덱스 값

char printWinner(int winState); // winState를 읽고 해당하는 승리자를 반환하는 함수

int main() {
  ifstream in("jug.inp");
  ofstream out("jug.out");

  takes[0] = 0;
  in >> takes[1] >> takes[2] >> takes[3];

  int jug;
  while(in>>jug) {
    fill_n(winStates, MAX+1, -1);
    out << printWinner(getWinState(jug, 4)) << "\n";
  }

  in.close();
  out.close();
  return 0;
}

int getWinState(int currentJug, int prevTake) {
  if(currentJug < 0) { // currentJug가 음수인 경우는 계산에서 제외해야하므로, (0이 아닌) 0b0001을 반환
    return 0b0001;
  }

  if(winStates[currentJug] == -1) { //현재 항아리 잔액에 대한 winState 결과가 없을 때, 해당 winState 계산 필요
    if(currentJug < takes[1]) { // 현재 잔액이 s1보다 작을 경우, PASS만 가능하므로 winState에 0b0001을 저장
      winStates[currentJug] = 0b0001;
    } else { // 아닐 경우, s1, s2, s3를 빼고 난 각각의 경우에 대해 상대가 승리 못하는지 여부를 winState에 저장
      winStates[currentJug] = (( !getWinState(currentJug-takes[1], 1) ) << 1) + (( !getWinState(currentJug-takes[2], 2) ) << 2) + (( !getWinState(currentJug-takes[3], 3) ) << 3); 
    }
  }

  return winStates[currentJug] & ( (1<<prevTake) ^ (0b1111) ); // 현재 상태의 winState에서 prevTake의 경우를 제외하고도 승리할 수 있는지를 반환
}

char printWinner(int winState) {
  if(!winState == 0) {
    return 'F';
  } else {
    return 'S';
  }
}