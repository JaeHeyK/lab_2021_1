#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  ofstream out("5.inp");

  int goal = 100000;

  vector<int> total;
  for (int i=0; i<goal; i++) {
    total.push_back(i+1);
  }

  // 시드값을 얻기 위한 random_device 생성.
  random_device rd;

  // random_device 를 통해 난수 생성 엔진을 초기화 한다.
  default_random_engine rng(rd());

  out << goal << "\n";
  shuffle(total.begin(), total.end(), rng);
  for (vector<int>::iterator it = total.begin(); it != total.end(); ++it) {
    out << *it << endl;
  }

  out.close();
  return 0;
}