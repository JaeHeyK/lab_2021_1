/*
  이 코드에서는 F가 '모든 선택이 가능할 때(s3, s2, s1, PASS 모두 선택할 수 있다고 할 때)'
  승리할 수 있는 경우를 4자리 이진수로 표기하고, 이를 winState라고 합니다.
  예를 들어 F가 현재 잔액 n에서 s1, s3를 가져갈 경우 이긴다고 하면,
  'n에 대한 winState는 1010'이라고 할 수 있습니다.(MSB->LSB 순서대로 s3,s2,s1,PASS)
  이 때 winState가 0000일 경우,
  '잔액 n일 때는 F가 어떤 선택을 하던 이길 수 없다'는 뜻이므로,승자는 S임을 나타냅니다.
  반대로 winState가 0000이 아닐경우,
  '잔액 n일 때 F가 승리할 방법이 존재한다'는 의미이므로, 승자는 F로 간주됩니다.

  또한 문제를 분석한 결과, 승부가 결정되는 조건은 'n < s1 일 때 PASS를 낼 수 있는가'임을 알 수 있습니다.
  (이것은 모의 게임을 진행하여 게임트리 맨 아래의 승부가 결정되는 순간을 관찰함으로써 알아낼 수 있습니다.)
  즉, base 조건인 (n < s1)일 때 PASS를 할 수 없으면 지게 되므로,
  다시 말해 (n >= s1)일 때 현재 상태에서 PASS를 선택하면 항상 질 수 밖에 없으므로,
  base 조건이 아닐 때 현재 상태에서 PASS를 고르는 경우는 고려하지 않았습니다.
  또한 위에서 언급한 바와 같이 'n < s1 일 때 먼저 PASS를 낼 수 있다면 승자'이므로,
  '(s1보다 작은) n에 대한 winState는 0001'이라 할 수 있습니다.

  한편 현재의 winState는 내가 (s3,s2,s1)를 선택한 각각의 경우 상대방의 winState들에 의해 결정됩니다.
  자세히 설명하자면, 현재 금액이 n일 때, 나의 winState는
    1) 사전에 계산한 '잔액 (n - s3)에 대한 winState' 중, s3를 제외한 winState (상대방은 s3를 선택할 수 없으므로)
    2) 사전에 계산한 '잔액 (n - s2)에 대한 winState' 중, s2를 제외한 winState (상대방은 s2를 선택할 수 없으므로)
    3) 사전에 계산한 '잔액 (n - s1)에 대한 winState' 중, s1을 제외한 winState (상대방은 s1를 선택할 수 없으므로)
  에서,
    1)이 0000이라면 현재 winState에 1000 포함, 0000이 아닐 경우 winState에 포함하지 않음
      (왜냐하면 상대방의 winState가  0000 == 상대방은 해당 경우에 이길 수 없음 == 해당 경우는 내가 이기는 경우,
      반대로 상대방의 winState가 0000이 아님 == 상대방은 해당 경우에 이길 수 있는 방법이 있음 == 해당 경우는 내가 이길 수 없음)
    2)가 0000이라면 현재 winState에 0100 포함, 0000이 아닐 경우 winState에 포함하지 않음 (위와 동일한 이유)
    3)이 0000이라면 현재 winState에 0010 포함, 0000이 아닐 경우 winState에 포함하지 않음 (위와 동일한 이유)
  와 같은 방식으로 결정됩니다.

    예를 들어 현재 금액이 7이고, (s3,s2,s1)이 각각 (6, 3, 2)일 때,
      1) 사전에 계산한 잔액 1(= 7-6)에 대한 winState는 0001인데, 상대방은 s3(=6)를 선택하지 못하므로 1000을 제외하면, 이 경우의 winState는 0001
      2) 사전에 계산한 잔액 4(= 7-3)에 대한 winState는 0010인데, 상대방은 s2(=3)를 선택하지 못하므로 0100을 제외하면, 이 경우의 winState는 0010
      3) 사전에 계산한 잔액 5(= 7-2)에 대한 winState는 0110인데, 상대방은 s1(=2)을 선택하지 못하므로 0010을 제외하면, 이 경우의 winState는 0100
    에서,
      1)이 0001이므로, 현재의 winState에 1000을 포함하지 않음.
      2)이 0010이므로, 현재의 winState에 0100을 포함하지 않음.
      3)이 0100이므로, 현재의 winState에 0010을 포함하지 않음.
    이므로, '7에 대한 winState는 0000'이고 승자는 S임을 알 수 있습니다.
  
  따라서 base 조건의 winState들을 미리 배열에 저장하고, 이를 이용하여 위의 문단과 같이 계산을 수행하면
  원하는 경우의 winState와 승자를 얻을 수 있습니다.
*/

#include <fstream>
#include <iostream>

using namespace std;

const int take = 3;   // 가져갈 수 있는 돈의 가지수; 문제에서는 3가지
const int MAX = 100;  // 항아리에 든 돈의 최댓값; 문제에서는 100

int takes[take+1];    // PASS를 포함한 가져갈 수 있는 액수를 저장할 배열
int winStates[MAX+1];  // 0의 경우를 포함한 winState를 저장할 배열

int getWinState(int currentJug, int prevTake); // 현재 항아리 내 잔액과 상대방이 이전에 가져간 금액을 이용하여 새로운 winState를 얻어내는 함수; prevTake는 S(i)에서 i에 해당하는 값

char printWinner(int winState); // winState를 읽고 해당하는 승리자를 반환하는 함수

int main() {
  ifstream in("jug.inp");
  ofstream out("jug.out");

  takes[0] = 0; // takes 중 PASS에 해당하는 금액은 0으로 간주한다.
  in >> takes[1] >> takes[2] >> takes[3];

  int jug;
  while(in>>jug) {
    fill_n(winStates, MAX+1, -1); // winStates를 -1로 초기화
    out << printWinner(getWinState(jug, 4)) << "\n"; // 게임을 시작할 때는 이전에 아무 금액도 가져가지 않으므로, prevTake에 (3,2,1,0이 아닌) 4를 전달
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